/// ==========================================
/// COSMOS OS - INTEL HD AUDIO (HDA) DRIVER
/// ==========================================
#include "schneider_lang.h"

uint32_t hda_base_addr = 0;

/// BARE METAL FIX: Feste RAM-Adressen für HDA Ringe (34 MB Marke)
/// CRITICAL: Der DMA-Controller braucht PHYSIKALISCHE Adressen!
/// Wenn wir statische C-Arrays nehmen, kennen wir die physikalische Adresse NICHT.
/// Stattdessen: Hardcoded wie beim xHCI-Treiber (0x08100000 etc.)
/// 128-Byte-Alignment für HDA-Hardware-Anforderungen!
#define HDA_CORB_PHYS  0x02200000   /// 34 MB (128-byte aligned)
#define HDA_RIRB_PHYS  0x02201000   /// 34 MB + 4K 
#define HDA_BDL_PHYS   0x02202000   /// 34 MB + 8K
#define HDA_PCM_PHYS   0x02210000   /// 34 MB + 64K (Audio-Daten PCM)

uint32_t* hda_corb = (uint32_t*)HDA_CORB_PHYS;
uint64_t* hda_rirb = (uint64_t*)HDA_RIRB_PHYS;
uint64_t* hda_bdl  = (uint64_t*)HDA_BDL_PHYS;

uint32_t hda_corb_wp = 0;
uint32_t hda_output_stream_offset = 0;
uint32_t hda_dac_nid = 0;
uint32_t hda_pin_nid = 0;
uint32_t hda_codec_id = 0;

inline uint32_t hda_read32(uint32_t offset) { return *((volatile uint32_t*)(hda_base_addr + offset)); }
inline uint16_t hda_read16(uint32_t offset) { return *((volatile uint16_t*)(hda_base_addr + offset)); }
inline uint8_t  hda_read8 (uint32_t offset) { return *((volatile uint8_t*)(hda_base_addr + offset)); }

inline void hda_write32(uint32_t offset, uint32_t val) { *((volatile uint32_t*)(hda_base_addr + offset)) = val; }
inline void hda_write16(uint32_t offset, uint16_t val) { *((volatile uint16_t*)(hda_base_addr + offset)) = val; }
inline void hda_write8 (uint32_t offset, uint8_t  val) { *((volatile uint8_t*)(hda_base_addr + offset)) = val; }

extern char cmd_status[]; 
extern void str_cpy(char* d, const char* s); 

/// Kleine Wartefunktion (ca. 1ms bei ~1GHz)
static void hda_delay(int loops) {
    for(volatile int i=0; i<loops; i++) { asm volatile("nop"); }
}

/// ==========================================
/// 1. IMMEDIATE COMMAND INTERFACE (PIO - kein DMA nötig!)
/// ==========================================
/// Der HDA-Standard bietet ZWEI Wege um mit dem Codec zu reden:
///   A) CORB/RIRB (DMA) - kompliziert, braucht korrekte physikalische Adressen, Cache-Flush...
///   B) Immediate Command Interface (PIO) - DIREKTE Register, NULL DMA!
/// Wir benutzen jetzt Methode B, weil DMA bei uns nie funktioniert hat.
///
/// Register:
///   ICW  (0x60) = Immediate Command Write (wir schreiben das Verb hierhin)
///   IRR  (0x64) = Immediate Response Read (die Antwort steht hier)
///   ICS  (0x68) = Immediate Command Status
///                  Bit 0 = ICB (Immediate Command Busy) -> 1=senden, HW löscht wenn fertig
///                  Bit 1 = IRV (Immediate Result Valid) -> HW setzt wenn Antwort da ist

uint32_t hda_send_verb(uint32_t codec, uint32_t nid, uint32_t verb, uint32_t param) {
    uint32_t payload;
    if (verb >= 0x100) {
        payload = (codec << 28) | (nid << 20) | (verb << 8) | param;
    } else {
        payload = (codec << 28) | (nid << 20) | (verb << 16) | param;
    }
    
    /// 1. Warten bis der letzte Befehl fertig ist (ICB = 0)
    int timeout = 100000;
    while ((hda_read16(0x68) & 0x01) && timeout > 0) {
        asm volatile("nop");
        timeout--;
    }
    if (timeout == 0) return 0xFFFFFFFF;
    
    /// 2. IRV (Bit 1) löschen durch Schreiben einer 1
    hda_write16(0x68, 0x02);
    
    /// 3. Das Verb in das ICW-Register schreiben
    hda_write32(0x60, payload);
    
    /// 4. ICB setzen (Bit 0) -> Der Controller sendet das Verb jetzt!
    hda_write16(0x68, hda_read16(0x68) | 0x01);
    
    /// 5. Warten bis ICB gelöscht wird (= Befehl abgeschlossen)
    timeout = 100000;
    while ((hda_read16(0x68) & 0x01) && timeout > 0) {
        asm volatile("nop");
        timeout--;
    }
    if (timeout == 0) return 0xFFFFFFFF;
    
    /// 6. Prüfen ob IRV (Bit 1) gesetzt ist (= Antwort gültig)
    if (!(hda_read16(0x68) & 0x02)) return 0xFFFFFFFF;
    
    /// 7. Antwort auslesen
    return hda_read32(0x64);
}

/// ==========================================
/// 2. HDA CONTROLLER RESET & INIT
/// ==========================================
extern "C" void hda_init_controller(uint32_t base) {
    hda_base_addr = base;
    hda_dac_nid = 0;
    hda_pin_nid = 0;
    hda_output_stream_offset = 0;
    hda_corb_wp = 0;
    
    /// 1. Controller Reset (CRST=0 in GCTL)
    hda_write32(0x08, hda_read32(0x08) & ~0x01);
    hda_delay(100000);
    
    for(int i=0; i<1000000; i++) {
        if ((hda_read32(0x08) & 0x01) == 0) break;
        asm volatile("nop");
    }
    
    /// 2. Aufwecken (CRST=1)
    hda_write32(0x08, hda_read32(0x08) | 0x01);
    for(int i=0; i<1000000; i++) {
        if (hda_read32(0x08) & 0x01) break;
        asm volatile("nop");
    }
    
    /// 3. Warten bis Codecs sich melden
    hda_delay(500000);
    
    /// 4. Alle Status-Flags löschen
    hda_write32(0x20, 0xFFFFFFFF); /// INTSTS
    hda_write16(0x0E, 0xFFFF);     /// STATESTS
    
    /// 5. Finde den Output-Stream Offset (für Audioausgabe später)
    uint16_t gcap = hda_read16(0x00);
    uint8_t iss = (gcap >> 8) & 0x0F;
    hda_output_stream_offset = 0x80 + (iss * 0x20);
    
    /// =====================
    /// KEIN CORB/RIRB Setup nötig!
    /// Wir benutzen jetzt die Immediate Command Interface (Register 0x60/0x64/0x68)
    /// Das ist PIO-basiert und braucht NULL DMA!
    /// =====================
    
    /// 6. Auto-Scan aller Codecs (0-14)
    uint32_t codec_id = 0; 
    uint32_t afg_start = 0;
    hda_delay(200000);
    
    for(uint32_t c=0; c<15; c++) {
        
        uint32_t root_nodes = hda_send_verb(c, 0, 0xF00, 0x04);
        if (c == 0) {
            extern uint32_t hda_debug_c0_resp;
            hda_debug_c0_resp = root_nodes; /// Speichern für die UI!
        }
        
        if (root_nodes != 0 && root_nodes != 0xFFFFFFFF) {
            uint32_t start = (root_nodes >> 16) & 0xFF;
            uint32_t count = root_nodes & 0xFF;
            
            /// Suche die Audio Function Group (AFG)
            for(uint32_t n=start; n<start+count; n++) {
                uint32_t ftype = hda_send_verb(c, n, 0xF00, 0x05); /// Function Group Type
                if ((ftype & 0x7F) == 0x01) { /// 0x01 = Audio Function Group
                    afg_start = n;
                    codec_id = c;
                    break;
                }
            }
            if (afg_start != 0) break;
        }
    }
    
    if (afg_start == 0) {
        str_cpy(cmd_status, "HDA: NO CODEC!");
        return;
    }
    
    hda_codec_id = codec_id;
    
    /// 8. AFG Power State auf D0 (voll an!) setzen
    hda_send_verb(codec_id, afg_start, 0x705, 0x00); /// Set Power State = D0
    hda_delay(100000); /// Hardware braucht Zeit zum Aufwachen!
    
    /// 9. Suche DAC und Output Pin
    uint32_t afg_nodes = hda_send_verb(codec_id, afg_start, 0xF00, 0x04);
    uint32_t node_start = (afg_nodes >> 16) & 0xFF;
    uint32_t node_count = afg_nodes & 0xFF;
    
    for(uint32_t i = 0; i < node_count; i++) {
        uint32_t nid = node_start + i;
        uint32_t caps = hda_send_verb(codec_id, nid, 0xF00, 0x09);
        uint32_t type = (caps >> 20) & 0x0F;
        
        /// Audio Output (DAC) = Type 0
        if (type == 0 && hda_dac_nid == 0) {
            hda_dac_nid = nid;
        }
        
        /// Pin Complex = Type 4
        if (type == 4 && hda_pin_nid == 0) {
            uint32_t cfg = hda_send_verb(codec_id, nid, 0xF1C, 0); /// Config Default
            uint32_t port_conn = (cfg >> 30) & 0x03;
            uint32_t default_device = (cfg >> 20) & 0x0F;
            
            /// Nicht "No Physical Connection" (port_conn != 1)
            /// Bevorzuge: Line Out (0x00), Speaker (0x01), HP Out (0x02)
            if (port_conn != 1 && (default_device <= 0x02)) {
                hda_pin_nid = nid;
            }
        }
    }
    
    /// Fallback: Nimm irgendeinen Pin der nicht "No Connection" ist
    if (hda_pin_nid == 0) {
        for(uint32_t i = 0; i < node_count; i++) {
            uint32_t nid = node_start + i;
            uint32_t caps = hda_send_verb(codec_id, nid, 0xF00, 0x09);
            uint32_t type = (caps >> 20) & 0x0F;
            if (type == 4) {
                uint32_t cfg = hda_send_verb(codec_id, nid, 0xF1C, 0);
                if (((cfg >> 30) & 0x03) != 1) {
                    hda_pin_nid = nid;
                    break;
                }
            }
        }
    }
    
    if (hda_dac_nid == 0 || hda_pin_nid == 0) {
        str_cpy(cmd_status, "HDA: NO DAC/PIN!");
        return;
    }
    
    /// 10. DAC und Pin konfigurieren
    
    /// DAC Power State = D0
    hda_send_verb(codec_id, hda_dac_nid, 0x705, 0x00);
    
    /// DAC Stream Format: 48 kHz, 16-bit, Stereo
    hda_send_verb(codec_id, hda_dac_nid, 0x200, 0x0011); 
    
    /// DAC Converter Stream/Channel: Stream Tag 1, Channel 0
    hda_send_verb(codec_id, hda_dac_nid, 0x706, 0x10);
    
    /// Set Amp Gain/Mute auf DAC: Output, Links+Rechts, Unmute, Max Volume
    /// 4-bit verb 0x3, Payload: B000 = Output|Left|Right, 7F = max gain
    hda_send_verb(codec_id, hda_dac_nid, 0x3, 0xB07F);
    
    /// Pin Power State = D0
    hda_send_verb(codec_id, hda_pin_nid, 0x705, 0x00);
    
    /// Pin Control: Output Enable (Bit 6) + HP Enable (Bit 7)
    hda_send_verb(codec_id, hda_pin_nid, 0x707, 0xC0);
    
    /// Pin EAPD: Enable (Bit 1)
    hda_send_verb(codec_id, hda_pin_nid, 0x70C, 0x02); 
    
    /// Set Amp Gain/Mute auf Pin: Output, Links+Rechts, Unmute, Max Volume
    hda_send_verb(codec_id, hda_pin_nid, 0x3, 0xB07F);
    
    /// Connection Select: Erste Verbindung (Index 0)
    hda_send_verb(codec_id, hda_pin_nid, 0x701, 0x00);
    
    /// 11. GPIO aktivieren (wichtig für Laptops und viele Desktop-Boards!)
    /// GPIO Enable, Direction, Data an der AFG Node
    hda_send_verb(codec_id, afg_start, 0x715, 0xFF); /// GPIO Enable
    hda_send_verb(codec_id, afg_start, 0x716, 0xFF); /// GPIO Direction = Output
    hda_send_verb(codec_id, afg_start, 0x717, 0xFF); /// GPIO Data = High
    
    str_cpy(cmd_status, "HDA READY");
}

/// ==========================================
/// Stream Reset Helper (echte Hardware braucht korrekte Sequenz!)
/// ==========================================
static void hda_stream_reset(uint32_t stream_base) {
    /// 1. Stream stoppen
    hda_write8(stream_base + 0x00, 0);
    hda_delay(5000);
    
    /// 2. Reset setzen (SRST Bit 0)
    hda_write8(stream_base + 0x00, 0x01);
    
    /// 3. Warten bis Controller SRST=1 bestätigt (PFLICHT auf echter HW!)
    for(int i=0; i<100000; i++) {
        if (hda_read8(stream_base + 0x00) & 0x01) break;
        asm volatile("nop");
    }
    
    /// 4. Reset löschen
    hda_write8(stream_base + 0x00, 0x00);
    
    /// 5. Warten bis Controller SRST=0 bestätigt
    for(int i=0; i<100000; i++) {
        if ((hda_read8(stream_base + 0x00) & 0x01) == 0) break;
        asm volatile("nop");
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
    
    /// 1. Stream Reset (korrekte Sequenz!)
    hda_stream_reset(stream_base);
    
    /// 2. Rechteckwelle in RAM rendern (feste Adresse - NICHT auf CORB/RIRB!)
    uint32_t pcm_addr = HDA_PCM_PHYS;  /// 0x02210000 (34MB + 64K)
    int16_t* pcm_buf = (int16_t*)pcm_addr;
    
    uint32_t samples_per_cycle = 48000 / freq;
    if (samples_per_cycle == 0) samples_per_cycle = 1;
    
    /// Stereo: 2 Samples pro Frame (L+R), 48000 Frames = 1 Sekunde
    uint32_t total_frames = 48000;
    uint32_t total_bytes = total_frames * 2 * sizeof(int16_t); /// frames * channels * sample_size
    
    for(uint32_t f=0; f<total_frames; f++) {
        uint32_t pos = f % samples_per_cycle;
        int16_t sample = (pos < (samples_per_cycle / 2)) ? 10000 : -10000;
        pcm_buf[f*2]     = sample; /// Links
        pcm_buf[f*2 + 1] = sample; /// Rechts
    }
    
    /// 3. BDL Setup (1 Eintrag) - BDL liegt bei HDA_BDL_PHYS (0x02202000)
    volatile uint32_t* bdl = (volatile uint32_t*)HDA_BDL_PHYS;
    bdl[0] = pcm_addr;       /// Buffer Address Low
    bdl[1] = 0;              /// Buffer Address High
    bdl[2] = total_bytes;    /// Buffer Length
    bdl[3] = 1;              /// IOC (Interrupt On Completion)
    
    /// Cache flushen damit DMA die echten Daten sieht!
    asm volatile("wbinvd" ::: "memory");
    
    /// 4. Stream Descriptor konfigurieren
    hda_write32(stream_base + 0x18, HDA_BDL_PHYS); /// BDLPL = physikalische Adresse!
    hda_write32(stream_base + 0x1C, 0);                           /// BDLPU
    
    hda_write32(stream_base + 0x08, total_bytes);     /// CBL (Cyclic Buffer Length)
    hda_write16(stream_base + 0x0C, 0);               /// LVI = 0 (1 BDL Eintrag, Index 0)
    
    hda_write16(stream_base + 0x12, 0x0011);          /// FMT: 48kHz, 16-bit, Stereo
    hda_write8(stream_base + 0x02, (1 << 4));         /// Stream Number = 1
    
    /// 5. Status löschen und STARTEN!
    hda_write8(stream_base + 0x03, 0x1C);             /// Alle Status-Bits löschen (BCIS, FIFOE, DESE)
    hda_write8(stream_base + 0x00, 0x02 | 0x04);      /// RUN=1, IOCE=1 (Ohne Reset-Bit!)
}

extern "C" void play_hda_earthquake() {
    if (hda_output_stream_offset == 0 || hda_dac_nid == 0) return;
    
    uint32_t stream_base = hda_output_stream_offset;
    
    /// 1. Stream Reset
    hda_stream_reset(stream_base);
    
    /// 2. BDL und PCM Setup
    uint32_t pcm_addr = HDA_PCM_PHYS;  /// 0x02210000 (34MB + 64K)
    int16_t* pcm_buf = (int16_t*)pcm_addr;
    
    /// Erdbeben berechnen (30000 Frames stereo)
    /// Wir mischen eine tiefe Sub-Bass Triangle-Welle (ca. 35 Hz) mit extrem tiefem Grollen (Brown Noise)
    uint32_t total_frames = 30000;
    uint32_t total_bytes = total_frames * 2 * sizeof(int16_t);
    
    int rumble = 0;
    int phase = 0;
    for(uint32_t f=0; f<total_frames; f++) {
        /// 1. Tiefer Sub-Bass (ca. 35 Hz bei 48000 Hz Sample Rate = 1371 Samples Periode)
        phase = (phase + 1) % 1371;
        /// Dreieck-Welle von -20550 bis +20550
        int tri = (phase < 685) ? (phase * 60) - 20550 : ((1371 - phase) * 60) - 20550;
        
        /// 2. Dunkles, unheilvolles Grollen (stark gefiltertes Rauschen)
        int noise = (int)(random() & 0xFFFF) - 32768; /// -32768 bis +32767
        rumble = ((rumble * 127) + noise) / 128;      /// Starker Tiefpass-Filter
        
        /// 3. Mischen: Sub-Bass gibt die Urgewalt, Grollen gibt das unregelmäßige Beben
        int final_sample = tri + (rumble * 4);
        
        /// Clipping verhindern
        if (final_sample > 32767) final_sample = 32767;
        if (final_sample < -32768) final_sample = -32768;
        
        pcm_buf[f*2]     = (int16_t)final_sample; /// Links
        pcm_buf[f*2 + 1] = (int16_t)final_sample; /// Rechts
    }
    
    /// BDL eintragen - direkt an der physikalischen Adresse!
    volatile uint32_t* bdl = (volatile uint32_t*)HDA_BDL_PHYS;
    bdl[0] = pcm_addr;
    bdl[1] = 0;
    bdl[2] = total_bytes;
    bdl[3] = 1; /// IOC
    
    /// Cache flushen!
    asm volatile("wbinvd" ::: "memory");
    
    /// Stream Descriptor konfigurieren
    hda_write32(stream_base + 0x18, HDA_BDL_PHYS); /// BDLPL = physikalische Adresse!
    hda_write32(stream_base + 0x1C, 0);
    
    hda_write32(stream_base + 0x08, total_bytes);
    hda_write16(stream_base + 0x0C, 0);
    
    hda_write16(stream_base + 0x12, 0x0011); /// 48 kHz, 16-bit, Stereo
    hda_write8(stream_base + 0x02, (1 << 4)); /// Stream Number = 1
    
    /// Status löschen und STARTEN!
    hda_write8(stream_base + 0x03, 0x1C);
    hda_write8(stream_base + 0x00, 0x02 | 0x04);
}

extern "C" void play_hda_wav(uint32_t pcm_addr, uint32_t size_bytes, uint16_t sample_rate, uint16_t channels, uint16_t bits) {
    if (hda_output_stream_offset == 0 || hda_dac_nid == 0) return;
    uint32_t stream_base = hda_output_stream_offset;
    
    hda_stream_reset(stream_base);
    
    volatile uint32_t* bdl = (volatile uint32_t*)HDA_BDL_PHYS;
    bdl[0] = pcm_addr;
    bdl[1] = 0;
    bdl[2] = size_bytes;
    bdl[3] = 1; /// IOC
    
    asm volatile("wbinvd" ::: "memory");
    
    uint16_t fmt = 0;
    if (sample_rate == 44100) fmt |= (1 << 14); 
    
    if (bits == 16) fmt |= (1 << 4);
    else if (bits == 20) fmt |= (2 << 4);
    else if (bits == 24) fmt |= (3 << 4);
    else if (bits == 32) fmt |= (4 << 4);
    
    fmt |= (channels - 1) & 0x0F;
    
    hda_send_verb(hda_codec_id, hda_dac_nid, 0x200, fmt);
    
    hda_write32(stream_base + 0x18, HDA_BDL_PHYS);
    hda_write32(stream_base + 0x1C, 0);
    
    hda_write32(stream_base + 0x08, size_bytes);
    hda_write16(stream_base + 0x0C, 0);
    
    hda_write16(stream_base + 0x12, fmt);
    hda_write8(stream_base + 0x02, (1 << 4)); 
    
    hda_write8(stream_base + 0x03, 0x1C);
    hda_write8(stream_base + 0x00, 0x02 | 0x04);
}
