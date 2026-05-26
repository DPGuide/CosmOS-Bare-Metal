/// ==========================================
/// COSMOS OS - INTEL HD AUDIO (HDA) DRIVER
/// ==========================================
#include "schneider_lang.h"

uint32_t hda_base_addr = 0;

/// BARE METAL FIX: RAM Adressen für HDA Ringe (33 MB Marke)
uint32_t* hda_corb = (uint32_t*)0x02100000; /// Command Outbound Ring Buffer
uint64_t* hda_rirb = (uint64_t*)0x02110000; /// Response Inbound Ring Buffer (64-bit pro Eintrag)
uint64_t* hda_bdl  = (uint64_t*)0x02120000; /// Buffer Descriptor List
uint32_t* hda_dpl  = (uint32_t*)0x02130000; /// DMA Position Lower Base

uint32_t hda_corb_wp = 0;
uint32_t hda_output_stream_offset = 0;
uint32_t hda_dac_nid = 0;
uint32_t hda_pin_nid = 0;

inline uint32_t hda_read32(uint32_t offset) { return *((volatile uint32_t*)(hda_base_addr + offset)); }
inline uint16_t hda_read16(uint32_t offset) { return *((volatile uint16_t*)(hda_base_addr + offset)); }
inline uint8_t  hda_read8 (uint32_t offset) { return *((volatile uint8_t*)(hda_base_addr + offset)); }

inline void hda_write32(uint32_t offset, uint32_t val) { *((volatile uint32_t*)(hda_base_addr + offset)) = val; }
inline void hda_write16(uint32_t offset, uint16_t val) { *((volatile uint16_t*)(hda_base_addr + offset)) = val; }
inline void hda_write8 (uint32_t offset, uint8_t  val) { *((volatile uint8_t*)(hda_base_addr + offset)) = val; }

extern char cmd_status[256];
extern void str_cpy(char* d, const char* s); 

/// ==========================================
/// 1. CORB & RIRB (Der Chat-Kanal mit dem Codec)
/// ==========================================
uint32_t hda_send_verb(uint32_t codec, uint32_t nid, uint32_t verb, uint32_t param) {
    uint32_t payload = (codec << 28) | (nid << 20) | (verb << 8) | param;
    
    hda_corb_wp++;
    if (hda_corb_wp >= 256) hda_corb_wp = 0;
    
    hda_corb[hda_corb_wp] = payload;
    hda_write16(0x48, hda_corb_wp); /// CORBWP (Write Pointer)
    
    /// Warten auf die RIRB Antwort
    uint16_t rirb_wp = hda_read16(0x58);
    int timeout = 100000;
    while (rirb_wp == hda_read16(0x58) && timeout > 0) {
        asm volatile("pause");
        timeout--;
    }
    
    if (timeout == 0) return 0xFFFFFFFF; /// Fehler
    
    rirb_wp = hda_read16(0x58);
    uint32_t response = (uint32_t)(hda_rirb[rirb_wp] & 0xFFFFFFFF);
    return response;
}

/// ==========================================
/// 2. HDA CONTROLLER RESET & INIT
/// ==========================================
extern "C" void hda_init_controller(uint32_t base) {
    hda_base_addr = base;
    
    /// 1. Reset (CRST Bit 0 in GCTL auf 0)
    hda_write32(0x08, hda_read32(0x08) & ~0x01);
    for(int i=0; i<1000000; i++) asm volatile("pause");
    
    /// 2. Aufwecken (CRST auf 1)
    hda_write32(0x08, hda_read32(0x08) | 0x01);
    for(int i=0; i<1000000; i++) {
        if (hda_read32(0x08) & 0x01) break;
        asm volatile("pause");
    }
    
    /// 3. CORB Setup
    hda_write8(0x4C, 0); /// CORB Stop
    hda_write32(0x40, (uint32_t)(uint64_t)hda_corb);
    hda_write32(0x44, 0);
    hda_write16(0x4A, 1); /// Reset Read Pointer
    for(int i=0; i<1000; i++) asm volatile("pause");
    hda_write16(0x4A, 0); 
    hda_write16(0x48, 0); /// Write Pointer auf 0
    hda_corb_wp = 0;
    hda_write8(0x4C, 2); /// CORB Start!
    
    /// 4. RIRB Setup
    hda_write8(0x5C, 0); /// RIRB Stop
    hda_write32(0x50, (uint32_t)(uint64_t)hda_rirb);
    hda_write32(0x54, 0);
    hda_write16(0x5A, hda_read16(0x5A) | 0x8000); /// Reset Read Pointer
    hda_write8(0x5C, 2); /// RIRB Start!
    
    /// Finde den Output-Stream (Offsets ab 0x80)
    uint16_t gcap = hda_read16(0x00);
    uint8_t iss = (gcap >> 8) & 0x0F;
    hda_output_stream_offset = 0x80 + (iss * 0x20);

    /// Codec wecken (State Change Status)
    hda_write16(0x0E, 0xFFFF);
    for(int i=0; i<10000; i++) asm volatile("pause");
    
    /// 6. Auto-Scan des Codecs (Suche DAC und Lautsprecher/Kopfhörer)
    uint32_t codec_id = 0; /// Meistens Codec 0
    uint32_t root_nodes = hda_send_verb(codec_id, 0, 0xF00, 0x04);
    uint32_t afg_start = (root_nodes >> 16) & 0xFF;
    
    if (afg_start == 0) {
        str_cpy(cmd_status, "HDA: NO CODEC AFG FOUND!");
        return;
    }
    
    uint32_t afg_nodes = hda_send_verb(codec_id, afg_start, 0xF00, 0x04);
    uint32_t node_start = (afg_nodes >> 16) & 0xFF;
    uint32_t node_count = afg_nodes & 0xFF;
    
    for(uint32_t i = 0; i < node_count; i++) {
        uint32_t nid = node_start + i;
        uint32_t caps = hda_send_verb(codec_id, nid, 0xF00, 0x09);
        uint32_t type = (caps >> 20) & 0x0F;
        
        if (type == 0 && hda_dac_nid == 0) hda_dac_nid = nid; /// Audio Output (DAC)
        if (type == 4 && hda_pin_nid == 0) {
            uint32_t cfg = hda_send_verb(codec_id, nid, 0xF1C, 0); /// Config Default
            if (((cfg >> 30) & 3) != 1) { /// Nicht "No Physical Connection"
                hda_pin_nid = nid; /// Pin Complex (Line Out / Speaker)
            }
        }
    }
    
    if (hda_dac_nid > 0 && hda_pin_nid > 0) {
        /// Format auf 48 kHz, 16-bit, Stereo (0x4011) setzen
        hda_send_verb(codec_id, hda_dac_nid, 0x200, 0x4011); 
        
        /// Verstärker (Amp) hochdrehen (Unmute + Max Vol)
        hda_send_verb(codec_id, hda_dac_nid, 0x300, 0xB000 | 0x7F);
        hda_send_verb(codec_id, hda_pin_nid, 0x300, 0xB000 | 0x7F);
        
        /// Pin als Output aktivieren
        hda_send_verb(codec_id, hda_pin_nid, 0x707, 0x40);
        
        /// Stream Channel auf Stream 1 (0x10) setzen
        hda_send_verb(codec_id, hda_dac_nid, 0x706, 0x10);
        
        str_cpy(cmd_status, "INTEL HD AUDIO (HDA) INITIALIZED!");
    } else {
        str_cpy(cmd_status, "HDA: NO VALID DAC/PIN FOUND!");
    }
}

/// ==========================================
/// 3. PROCEDURAL HDA SOUND (ERDBEBEN)
/// ==========================================
extern uint32_t random();

/// ==========================================
/// 4. HDA SOFTWARE SYNTHESIZER (ARCADE SOUND)
/// ==========================================
extern "C" void play_hda_freq(uint32_t freq) {
    if (hda_output_stream_offset == 0 || hda_dac_nid == 0) return;
    uint32_t stream_base = hda_output_stream_offset;
    
    if (freq == 0) {
        hda_write8(stream_base + 0x00, 0); /// RUN = 0
        return;
    }
    
    /// 1. Stream stoppen & Reset
    hda_write8(stream_base + 0x00, 0); 
    hda_write8(stream_base + 0x03, 1); 
    for(int i=0; i<1000; i++) asm volatile("pause");
    hda_write8(stream_base + 0x03, 0); 
    
    /// 2. Rechteckwelle in RAM rendern
    uint32_t pcm_addr = 0x02200000;
    int16_t* pcm_buf = (int16_t*)pcm_addr;
    
    uint32_t samples_per_cycle = 48000 / freq;
    if (samples_per_cycle == 0) samples_per_cycle = 1;
    
    uint32_t total_samples = 48000; /// 1 Sekunde Puffer reicht für Beeps locker
    
    for(uint32_t i=0; i<total_samples; i+=2) {
        uint32_t pos = i % samples_per_cycle;
        int16_t sample = (pos < (samples_per_cycle / 2)) ? 8000 : -8000;
        pcm_buf[i]   = sample; /// Links
        pcm_buf[i+1] = sample; /// Rechts
    }
    
    /// 3. BDL Setup
    hda_bdl[0] = pcm_addr;
    hda_bdl[1] = 0;
    hda_bdl[2] = total_samples * 2; /// Bytes (16-bit pro Sample)
    hda_bdl[3] = 1; /// IOC
    
    hda_write32(stream_base + 0x18, (uint32_t)(uint64_t)hda_bdl); 
    hda_write32(stream_base + 0x1C, 0);                 
    
    hda_write32(stream_base + 0x08, total_samples * 2); /// CBL
    hda_write16(stream_base + 0x0C, 0);                 /// LVI = 0
    
    hda_write16(stream_base + 0x12, 0x4011);            /// Format (48 kHz, 16-bit, Stereo)
    hda_write8(stream_base + 0x02, (1 << 4));           /// Stream ID 1
    
    /// 4. START!
    hda_write8(stream_base + 0x00, 0x06 | 0x01); 
}

extern "C" void play_hda_earthquake() {
    if (hda_output_stream_offset == 0 || hda_dac_nid == 0) return;
    
    uint32_t stream_base = hda_output_stream_offset;
    
    /// 1. Stream stoppen
    hda_write8(stream_base + 0x00, 0); /// RUN = 0
    hda_write8(stream_base + 0x03, 1); /// Reset Stream
    for(int i=0; i<1000; i++) asm volatile("pause");
    hda_write8(stream_base + 0x03, 0); 
    
    /// 2. BDL und PCM Setup
    uint32_t pcm_addr = 0x02200000;
    int16_t* pcm_buf = (int16_t*)pcm_addr;
    
    /// Erdbeben berechnen (60000 Samples)
    for(int i=0; i<60000; i+=2) {
        int rumble = (random() % 8000) - 4000; 
        pcm_buf[i]   = (int16_t)rumble; /// Linkes Ohr
        pcm_buf[i+1] = (int16_t)rumble; /// Rechtes Ohr
    }
    
    /// BDL eintragen (1 Eintrag, 120.000 Bytes = 60000 * 2)
    hda_bdl[0] = pcm_addr;
    hda_bdl[1] = 0;
    hda_bdl[2] = 120000;
    hda_bdl[3] = 1; /// IOC Bit
    
    hda_write32(stream_base + 0x18, (uint32_t)(uint64_t)hda_bdl); /// BDL Base Address
    hda_write32(stream_base + 0x1C, 0);                 /// BDL Base Upper
    
    hda_write32(stream_base + 0x08, 120000);            /// CBL (Cyclic Buffer Length)
    hda_write16(stream_base + 0x0C, 0);                 /// LVI (Last Valid Index = 0)
    
    hda_write16(stream_base + 0x12, 0x4011);            /// Format (48 kHz, 16-bit, Stereo)
    
    /// Stream ID (1) setzen
    hda_write8(stream_base + 0x02, (1 << 4));
    
    /// 3. FEUER FREI!
    hda_write8(stream_base + 0x00, 0x06 | 0x01); /// RUN = 1, Interrupts an
}
