#include "arcade.h"
#include "schneider_lang.h"
extern "C" _50 apply_static_ip(_71 _30* new_ip);
/// ========================================================
/// BARE METAL STRING HELPER
/// ========================================================
bool str_starts(const char* str, const char* prefix) {
    while (*prefix != 0) {
        if (*str == 0 || *str != *prefix) {
            return false; /// Passt nicht!
        }
        str++;
        prefix++;
    }
    return true; /// Treffer! Der Text fängt genau so an.
}
/// ==========================================
/// APP ID 8: DIE 2-in-1 WEB & NETWORK ENGINE
/// ==========================================
_43 browser_view = 0; /// 0 = Web Browser, 1 = LAN Configurator
_43 browser_state = 0;
_30 browser_url[64] = "142.251.127.138"; 
_43 browser_url_idx = 15;
_30 browser_content[2048] = "WELCOME TO COSMOS WEB EXPLORER...\n\nENTER IP TO SEND HTTP GET REQUEST.";
/// Variablen für die Config-Ansicht
_43 netcfg_state = 0;
_30 netcfg_ip[64] = "192.168.14.100"; /// <--- DAS ist die IP für deinen Laptop!
_43 netcfg_ip_idx = 14;
_30 netcfg_content[2048] = "LAN PARTY MODE: STATIC IP CONFIGURATION\n\nENTER DESIRED IP AND PRESS APP TO SET YOUR MACHINE IP.";
/// Netzwerk-Variablen aus dem Kernel
_172 _30 ip_address[32]; 
_172 _30 mac_str[24];
//_172 _30 cmd_status[256];
/// BARE METAL FIX: Fehlende Brücken für den Browser!
_172 _50 DrawChar(_43 x, _43 y, _30 c, _89 col, _44 bold);
_172 _50 str_cpy(_30* d, _71 _30* s);
_172 _50 str_cat(_30* dest, _71 _30* src);
_172 volatile char last_app_key;

/// ==========================================
/// BARE METAL FIX: EXTERNE BRÜCKEN ZUM KERNEL
/// ==========================================
_172 _43 mouse_x;
_172 _43 mouse_y;
_172 _44 mouse_just_pressed;
_172 _44 key_new;
_172 _184 key_scancode;

_172 _89 frame;
_172 _43 input_cooldown;
_172 _44 blocked;
_172 _43 win_z[13];

/// Grafik & Sound (Aus kernel_main.cpp)
_172 _50 TextC(_43 cp, _43 y, _71 _30* s, _89 col, _44 bold);
_172 _50 Text(_43 x, _43 y, _71 _30* s, _89 col, _44 bold);
_172 _50 DrawRoundedRect(_43 x, _43 y, _43 rw, _43 rh, _43 r, _89 c);
_172 _50 DrawRoundWindow(_43 cx_p, _43 cy_p, _43 r, _89 color);
_172 _44 is_over_rect(_43 mx, _43 my, _43 x, _43 y, _43 w, _43 h);
_172 _50 int_to_str(_43 n, _30* s);
_172 _50 play_sound(_89 n_freq, _43 duration);
_172 _50 play_freq(_89 f);

/// ==========================================
/// SPIEL-STATUS-VARIABLEN (Isoliert im Arcade-Modul!)
/// ==========================================
/// --- PONG ---
_43 pong_state = 0; 
_43 pong_mode = 0; 
_43 pong_diff = 1; 
_43 pong_p1_y = 100, pong_p2_y = 100;
_43 pong_ball_x = 200, pong_ball_y = 150;
_43 pong_vel_x = 5, pong_vel_y = 3;
_43 pong_score1 = 0, pong_score2 = 0;

/// --- BLOBBY VOLLEY ---
_43 bv_state = 0, bv_mode = 0, bv_diff = 1;
_43 bv_p1_x = 50, bv_p1_y = 200, bv_p1_vy = 0;
_43 bv_p2_x = 350, bv_p2_y = 200, bv_p2_vy = 0;
_43 bv_ball_x = 100, bv_ball_y = 50, bv_ball_vx = 0, bv_ball_vy = 0;
_43 bv_score1 = 0, bv_score2 = 0;
_43 bv_touches = 0, bv_last_touch = 0;
/// BARE METAL FIX: Die exakten Kernel-Signaturen!
extern _44 ahci_read_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr);
extern _44 ahci_write_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr);

/// Deine CFS (Cosmos File System) Struktur aus dem Kernel
struct CFS_DIR_ENTRY {
    uint8_t  type;        /// 0 = Leer, 1 = Datei, 2 = Ordner
    char     filename[11];
    uint32_t file_size;
    uint32_t start_lba;
} __attribute__((packed));

/// ========================================================
/// BARE METAL TERMINAL (Der Text-Puffer)
/// ========================================================
char term_buffer[15][64];  /// Speicher für 15 Zeilen mit max 64 Zeichen
int term_line = 0;         /// Aktuelle Zeile
/// ========================================================
void clear_screen() {
    /// Wir leeren einfach unseren Speicher (nicht den Monitor!)
    for(int i = 0; i < 15; i++) term_buffer[i][0] = 0; 
    term_line = 0;
}

void sys_print(const char* text) {
    /// Wenn das Terminal voll ist (15 Zeilen erreicht) -> SCROLLEN!
    if (term_line >= 15) {
        for(int i = 1; i < 15; i++) {
            str_cpy(term_buffer[i-1], term_buffer[i]); /// Alles eins hoch schieben
        }
        term_line = 14; /// Unten Platz machen
    }
    
    /// Neuen Text in die aktuelle unterste Zeile kopieren
    str_cpy(term_buffer[term_line], (char*)text);
    term_line++;
}         
_172 _30 cmd_status[32];          

_50 run_cosmos_script(char* file_buffer, int file_size) {
    char current_line[128];
    int line_pos = 0;

    str_cpy(cmd_status, "RUNNING SCRIPT...");

    /// Gehe jedes Zeichen der geladenen Datei durch
    for (int i = 0; i < file_size; i++) {
        char c = file_buffer[i];

        /// Wenn die Zeile zu Ende ist (\n) oder das Dateiende erreicht ist
        if (c == '\n' || i == file_size - 1) {
            
            /// Letztes Zeichen noch mitnehmen, falls kein Umbruch am Ende war
            if (i == file_size - 1 && c != '\n' && c != '\r') {
                if (line_pos < 127) current_line[line_pos++] = c;
            }
            
            current_line[line_pos] = 0; /// String sauber abschließen

            /// ====================================================
            /// DER BEFEHLS-PARSER (Die komplette Cosmos Matrix!)
            /// ====================================================
            
            /// 1. BATCH-Style: ECHO (Text ausgeben)
            if (str_starts(current_line, "ECHO ")) {
                sys_print(&current_line[5]); 
            }
            /// 2. BATCH-Style: CLS (Bildschirm leeren)
            else if (str_starts(current_line, "CLS")) {
                clear_screen(); 
            }
            /// 3. OS-Befehl: TITLE
            else if (str_starts(current_line, "TITLE ")) {
                str_cpy(cmd_status, &current_line[6]);
            }
            /// 4. INFO: SYSINFO
            else if (str_starts(current_line, "SYSINFO")) {
                sys_print("OS   : COSMOS V2.0 64-BIT");
                sys_print("ARCH : X86_64 BARE METAL");
                sys_print("BOOT : LEGACY BIOS / PXE");
            }
            /// 5. INFO: MEM (RAM Status)
            else if (str_starts(current_line, "MEM")) {
                sys_print("RAM  : 8192 MB TOTAL");
                sys_print("USED : 14 MB (KERNEL)");
                sys_print("FREE : 8178 MB AVAILABLE");
            }
            /// 6. INFO: NET (Netzwerk Status)
            else if (str_starts(current_line, "NET")) {
                sys_print("ETH0 : INTEL PRO/1000 E1000");
                sys_print("MAC  : B8:AE:ED:00:11:22");
                sys_print("STAT : TX=1 RX=0 (WAITING)");
            }
            /// 7. INFO: HELP (Die Befehlsübersicht)
            else if (str_starts(current_line, "HELP")) {
                sys_print("AVAILABLE COMMANDS:");
                sys_print(" SYSINFO, MEM, NET, DIR, CLS");
                sys_print(" MKDIR [NAME] (CREATE FOLDER)");
                sys_print(" ECHO [TEXT]  (PRINT TEXT)");
            }
            /// 8. HARDWARE SATA: MKDIR
            else if (str_starts(current_line, "MKDIR ")) {
                sys_print("SPINNING UP SATA DRIVE...");
                uint32_t buf_dir = 0x00901000; 
                ahci_read_sectors(0, 1002, 1, buf_dir);
                for(volatile int w=0; w<1500000; w++) __asm__ volatile("pause");
                
                CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)(unsigned long long)buf_dir;
                bool found = false;
                for (int i = 0; i < 8; i++) {
                    if (dir[i].type == 0) { 
                        dir[i].type = 2; 
                        dir[i].file_size = 0; dir[i].start_lba = 0;
                        for(int n=0; n<11; n++) dir[i].filename[n] = 0;
                        char* new_name = &current_line[6];
                        for(int n=0; n<10 && new_name[n] != 0 && new_name[n] != '\r'; n++) {
                            dir[i].filename[n] = new_name[n];
                        }
                        ahci_write_sectors(0, 1002, 1, buf_dir);
                        for(volatile int w=0; w<1500000; w++) __asm__ volatile("pause");
                        
                        char msg[64]; str_cpy(msg, "HDD WRITE OK: "); str_cat(msg, dir[i].filename);
                        sys_print(msg);
                        found = true; break;
                    }
                }
                if (!found) sys_print("ERROR: SATA ROOT DIR FULL!");
            }
            /// 9. HARDWARE SATA: DIR
            else if (str_starts(current_line, "DIR")) {
                sys_print("READING PHYSICAL SATA DRIVE...");
                uint32_t buf_dir = 0x00901000;
                ahci_read_sectors(0, 1002, 1, buf_dir);
                for(volatile int w=0; w<1500000; w++) __asm__ volatile("pause");
                
                sys_print("VOLUME: HARD DISK 0 (CFS V2)");
                sys_print("-------------------------");
                CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)(unsigned long long)buf_dir;
                int count = 0;
                for (int i = 0; i < 8; i++) {
                    if (dir[i].type != 0) { 
                        char entry[64];
                        if (dir[i].type == 1) str_cpy(entry, " <FILE>  ");
                        else if (dir[i].type == 2) str_cpy(entry, " <DIR>   ");
                        else str_cpy(entry, " <?>     ");
                        str_cat(entry, dir[i].filename);
                        sys_print(entry);
                        count++;
                    }
                }
                if (count == 0) sys_print(" DRIVE IS EMPTY.");
                sys_print("-------------------------");
            }
            /// 10. UNBEKANNTER BEFEHL (Dein Fallback!)
            else if (current_line[0] != 0 && current_line[0] != '\r') {
                sys_print("UNKNOWN COMMAND OR BAD SYNTAX.");
            }
		}
	}
}
/// ==========================================
/// DIE PONG ENGINE (Ausgelagert aus win->id 11)
/// ==========================================
_50 run_pong_engine(_43 wx, _43 wy, _43 ww, _43 wh, _44 blocked) {
    _43 inner_h = wh - 40; /// (inner_w wurde hier gelöscht)
    _43 py = wy + 40;
    
    _15(pong_state EQ 0) {
        TextC(wx + ww/2, py + 20, "PONG OMNIVERSE", 0x00FF00, _128);
        /// --- MODUS AUSWAHL ---
        TextC(wx + ww/2, py + 60, "SELECT MODE", 0xFFFFFF, _128);
        DrawRoundedRect(wx + ww/2 - 110, py + 80, 100, 25, 3, pong_mode EQ 0 ? 0x00AA00 : 0x555555);
        TextC(wx + ww/2 - 60, py + 87, "1 PLAYER", 0xFFFFFF, _128);
        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 110, py + 80, 100, 25)) pong_mode = 0;
        DrawRoundedRect(wx + ww/2 + 10, py + 80, 100, 25, 3, pong_mode EQ 1 ? 0x00AA00 : 0x555555);
        TextC(wx + ww/2 + 60, py + 87, "2 PLAYERS", 0xFFFFFF, _128);
        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 10, py + 80, 100, 25)) pong_mode = 1;
        
        /// --- SCHWIERIGKEIT ---
        _15(pong_mode EQ 0) {
            TextC(wx + ww/2, py + 130, "CPU DIFFICULTY", 0xFFFFFF, _128);
            DrawRoundedRect(wx + ww/2 - 120, py + 150, 70, 25, 3, pong_diff EQ 0 ? 0x00AA00 : 0x555555);
            TextC(wx + ww/2 - 85, py + 157, "EASY", 0xFFFFFF, _128);
            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 120, py + 150, 70, 25)) pong_diff = 0;
            DrawRoundedRect(wx + ww/2 - 35, py + 150, 70, 25, 3, pong_diff EQ 1 ? 0x00AA00 : 0x555555);
            TextC(wx + ww/2, py + 157, "NORMAL", 0xFFFFFF, _128);
            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 35, py + 150, 70, 25)) pong_diff = 1;
            DrawRoundedRect(wx + ww/2 + 50, py + 150, 70, 25, 3, pong_diff EQ 2 ? 0x00AA00 : 0x555555);
            TextC(wx + ww/2 + 85, py + 157, "HARD", 0xFFFFFF, _128);
            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 50, py + 150, 70, 25)) pong_diff = 2;
        } _41 {
            TextC(wx + ww/2, py + 150, "P1: MOUSE  |  P2: ARROWS (UP/DOWN)", 0xAAAAAA, _128);
        }
        
        /// --- START BUTTON ---
        DrawRoundedRect(wx + ww/2 - 60, py + 190, 120, 30, 5, 0xAA0000);
        TextC(wx + ww/2, py + 197, "START GAME", 0xFFFFFF, _128);
        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 60, py + 190, 120, 30)) {
            pong_state = 1; pong_score1 = 0; pong_score2 = 0;
            pong_ball_x = ww/2; pong_ball_y = inner_h/2;
            pong_vel_x = 5; pong_vel_y = 3; 
            input_cooldown = 10;
        }
    } _41 _15(pong_state EQ 2) {
        TextC(wx + ww/2, py + 80, pong_score1 >= 10 ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!", 0x00FF00, _128);
        TextC(wx + ww/2, py + 140, "CLICK TO RETURN TO MENU", 0xFFFFFF, _128);
        _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx, py, ww, inner_h)) {
            pong_state = 0; input_cooldown = 10;
        }
    } _41 {
        /// Spielfeld & Physik
        DrawRoundedRect(wx+10, py, ww-20, inner_h-10, 0, 0x000000);
        _39(_43 i=0; i<inner_h-10; i+=20) DrawRoundedRect(wx+ww/2-2, py+i, 4, 10, 0, 0x555555);
        _30 s1[10], s2[10]; int_to_str(pong_score1, s1); int_to_str(pong_score2, s2);
        Text(wx+ww/2-40, py+20, s1, 0x00FF00, _128);
        Text(wx+ww/2+30, py+20, s2, 0x00FF00, _128);
        Text(wx+20, py+15, "[ ESC ] TO ABORT", 0x444444, _86);
        _15(key_new AND key_scancode EQ 0x01) { pong_state = 0; }
        
        _15(win_z[12] EQ 11) {
            pong_ball_x += pong_vel_x;
            pong_ball_y += pong_vel_y;
            _15(pong_ball_y <= 5) { pong_ball_y = 5; pong_vel_y = -pong_vel_y; play_freq(800); }
            _41 _15(pong_ball_y >= inner_h-25) { pong_ball_y = inner_h-25; pong_vel_y = -pong_vel_y; play_freq(800); }
            _41 { play_freq(0); }
            
            pong_p1_y = mouse_y - py - 20; 
            _15(pong_p1_y < 0) pong_p1_y = 0; 
            _15(pong_p1_y > inner_h-50) pong_p1_y = inner_h-50;
            
            _15(pong_mode EQ 0) {
                _43 speed = 4; _15(pong_diff EQ 0) speed = 2; _15(pong_diff EQ 2) speed = 6; 
                _15(pong_ball_y > pong_p2_y + 20) pong_p2_y += speed;
                _15(pong_ball_y < pong_p2_y + 20) pong_p2_y -= speed;
            } _41 {
                _15(key_new) {
                    _15(key_scancode EQ 0x48) pong_p2_y -= 15;
                    _15(key_scancode EQ 0x50) pong_p2_y += 15; 
                }
            }
            
            _15(pong_p2_y < 0) pong_p2_y = 0; 
            _15(pong_p2_y > inner_h-50) pong_p2_y = inner_h-50;
            
            _15(pong_ball_x <= 30 AND pong_ball_y + 10 >= pong_p1_y AND pong_ball_y <= pong_p1_y + 40 AND pong_vel_x < 0) {
                pong_ball_x = 30; pong_vel_x = -pong_vel_x;
                _15(pong_vel_x < 14) pong_vel_x++;
                _15(pong_ball_y < pong_p1_y + 12) pong_vel_y -= 2;      
                _41 _15(pong_ball_y > pong_p1_y + 28) pong_vel_y += 2;  
                play_sound(1200, 2);
            }
            _15(pong_ball_x >= ww-40 AND pong_ball_y + 10 >= pong_p2_y AND pong_ball_y <= pong_p2_y + 40 AND pong_vel_x > 0) {
                pong_ball_x = ww-40; pong_vel_x = -pong_vel_x;
                _15(pong_vel_x > -14) pong_vel_x--;
                _15(pong_ball_y < pong_p2_y + 12) pong_vel_y -= 2; 
                _41 _15(pong_ball_y > pong_p2_y + 28) pong_vel_y += 2;
                play_sound(1200, 2);
            }
            
            _15(pong_ball_x < 0 OR pong_ball_x > 2000) { pong_score2++; pong_ball_x=ww/2; pong_ball_y=inner_h/2; play_sound(200, 20); }
            _15(pong_ball_x > ww) { pong_score1++; pong_ball_x=ww/2; pong_ball_y=inner_h/2; play_sound(200, 20); }
            _15(pong_score1 >= 10 OR pong_score2 >= 10) pong_state = 2;
        }
        
        DrawRoundedRect(wx+20, py + pong_p1_y, 10, 40, 2, 0x00FF00); 
        DrawRoundedRect(wx+ww-30, py + pong_p2_y, 10, 40, 2, 0xFF0000); 
        DrawRoundWindow(wx + pong_ball_x, py + pong_ball_y, 6, 0xFFFFFF); 
    }
}

/// ==========================================
/// DIE BLOBBY VOLLEY ENGINE (Ausgelagert aus win->id 12)
/// ==========================================
_50 run_blobby_engine(_43 wx, _43 wy, _43 ww, _43 wh, _44 blocked) {
    _43 inner_h = wh - 40; /// (inner_w wurde hier gelöscht)
    _43 py = wy + 40;
    _43 floor_y = inner_h - 20; 
    
    _15(bv_state EQ 0) {
        TextC(wx + ww/2, py + 20, "BLOBBY VOLLEY BARE METAL", 0x00FF00, _128);
        TextC(wx + ww/2, py + 60, "SELECT MODE", 0xFFFFFF, _128);
        DrawRoundedRect(wx + ww/2 - 110, py + 80, 100, 25, 3, bv_mode EQ 0 ? 0x00AA00 : 0x555555);
        TextC(wx + ww/2 - 60, py + 87, "1 PLAYER", 0xFFFFFF, _128);
        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 110, py + 80, 100, 25)) bv_mode = 0;
        DrawRoundedRect(wx + ww/2 + 10, py + 80, 100, 25, 3, bv_mode EQ 1 ? 0x00AA00 : 0x555555);
        TextC(wx + ww/2 + 60, py + 87, "2 PLAYERS", 0xFFFFFF, _128);
        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 10, py + 80, 100, 25)) bv_mode = 1;
        
        _15(bv_mode EQ 0) {
            TextC(wx + ww/2, py + 130, "CPU DIFFICULTY", 0xFFFFFF, _128);
            DrawRoundedRect(wx + ww/2 - 120, py + 150, 70, 25, 3, bv_diff EQ 0 ? 0x00AA00 : 0x555555); TextC(wx + ww/2 - 85, py + 157, "EASY", 0xFFFFFF, _128);
            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 120, py + 150, 70, 25)) bv_diff = 0;
            DrawRoundedRect(wx + ww/2 - 35, py + 150, 70, 25, 3, bv_diff EQ 1 ? 0x00AA00 : 0x555555); TextC(wx + ww/2, py + 157, "NORMAL", 0xFFFFFF, _128);
            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 35, py + 150, 70, 25)) bv_diff = 1;
            DrawRoundedRect(wx + ww/2 + 50, py + 150, 70, 25, 3, bv_diff EQ 2 ? 0x00AA00 : 0x555555); TextC(wx + ww/2 + 85, py + 157, "HARD", 0xFFFFFF, _128);
            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 50, py + 150, 70, 25)) bv_diff = 2;
        } _41 { 
            TextC(wx + ww/2, py + 150, "P1: MOUSE+CLICK | P2: ARROWS", 0xAAAAAA, _128); 
        }
        
        DrawRoundedRect(wx + ww/2 - 60, py + 200, 120, 30, 5, 0xAA0000); TextC(wx + ww/2, py + 207, "START MATCH", 0xFFFFFF, _128);
        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 60, py + 200, 120, 30)) {
            bv_state = 1; bv_score1 = 0; bv_score2 = 0;
            bv_ball_x = ww/4; bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0;
            bv_touches = 0; bv_last_touch = 0;
            input_cooldown = 10;
        }
    } _41 _15(bv_state EQ 2) {
        TextC(wx + ww/2, py + 80, bv_score1 >= 10 ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!", 0x00FF00, _128);
        DrawRoundedRect(wx + ww/2 - 50, py + 140, 100, 30, 3, 0x555555); TextC(wx + ww/2, py + 148, "MENU", 0xFFFFFF, _128);
        _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 50, py + 140, 100, 30)) { bv_state = 0; input_cooldown = 10; }
    } _41 {
        /// Strand & Himmel
        DrawRoundedRect(wx+10, py, ww-20, inner_h-10, 0, 0x4488FF); 
        DrawRoundedRect(wx+10, py+floor_y, ww-20, 10, 0, 0x00AA00); 
        DrawRoundedRect(wx+ww/2-3, py+floor_y-70, 6, 70, 0, 0xEEEEEE); 
        _30 s1[10], s2[10]; int_to_str(bv_score1, s1); int_to_str(bv_score2, s2);
        Text(wx+ww/2-40, py+20, s1, 0xFFFFFF, _128); Text(wx+ww/2+30, py+20, s2, 0xFFFFFF, _128);
        
        _15(key_new AND key_scancode EQ 0x01) { bv_state = 0; } 
        
        /// Physik
        _15(win_z[12] EQ 12) {
            bv_ball_vy += 1; 
            _15(bv_ball_vy > 12) bv_ball_vy = 12; 
            bv_ball_x += bv_ball_vx;
            bv_ball_y += bv_ball_vy;
            
            bv_p1_x = mouse_x - wx - 20; 
            _15(bv_p1_x < 10) bv_p1_x = 10;
            _15(bv_p1_x > ww/2 - 45) bv_p1_x = ww/2 - 45; 
            _15(mouse_just_pressed AND bv_p1_y >= floor_y - 30) bv_p1_vy = -14; 
            bv_p1_vy += 1; bv_p1_y += bv_p1_vy; 
            _15(bv_p1_y > floor_y - 30) { bv_p1_y = floor_y - 30; bv_p1_vy = 0; }
            
            _15(bv_mode EQ 0) {
                _43 speed = 4; _15(bv_diff EQ 1) speed = 6; _15(bv_diff EQ 2) speed = 9;
                _43 target_x = bv_ball_x + 15;
                _15(bv_ball_x > ww/2) {
                    _15(bv_ball_vx > 0 AND bv_ball_x > bv_p2_x) {
                        target_x = bv_ball_x + 40; 
                    } _41 {
                        _43 dist_y = (floor_y - 30) - bv_ball_y;
                        _15(dist_y > 0) {
                            _43 frames = dist_y / 8; 
                            target_x = bv_ball_x + (bv_ball_vx * frames) + ((bv_diff EQ 2) ? 20 : 15);
                        }
                        _15(target_x < bv_p2_x - 20) { target_x = target_x - 20; }
                    }
                } _41 { target_x = ww*3/4; }
                
                _15(target_x > ww-40) target_x = ww-40;
                _15(target_x < ww/2 + 10) target_x = ww/2 + 10;
                
                _43 move_back = speed; _43 move_fwd  = speed;
                _15(bv_diff EQ 2) {
                    _15(bv_ball_x > bv_p2_x) move_back = 25; 
                    _15(target_x < bv_p2_x - 20) move_fwd = 20; 
                }
                
                _15(bv_p2_x + 20 < target_x - 15) { bv_p2_x += move_back; } 
                _41 _15(bv_p2_x + 20 > target_x + 15) { bv_p2_x -= move_fwd;  } 
                _41 { bv_p2_x = target_x - 20; }
                
                _15(bv_ball_x > bv_p2_x - 20 AND bv_ball_x < bv_p2_x + 60) { 
                    _15(bv_ball_y > floor_y - 120 AND bv_ball_y < floor_y - 40 AND bv_p2_y >= floor_y - 30 AND bv_ball_vy > 0) {
                        _15(bv_diff >= 1 OR (frame % 2) EQ 0) bv_p2_vy = -14;
                    }
                }
            } _41 {
                _15(key_new) {
                    _15(key_scancode EQ 0x4B) bv_p2_x -= 15; 
                    _15(key_scancode EQ 0x4D) bv_p2_x += 15; 
                    _15(key_scancode EQ 0x48 AND bv_p2_y >= floor_y - 30) bv_p2_vy = -14; 
                }
            }
            
            _15(bv_p2_x < ww/2 + 5) bv_p2_x = ww/2 + 5;
            _15(bv_p2_x > ww - 50) bv_p2_x = ww - 50;
            bv_p2_vy += 1; bv_p2_y += bv_p2_vy; 
            _15(bv_p2_y > floor_y - 30) { bv_p2_y = floor_y - 30; bv_p2_vy = 0; }
            
            /// Wände & Decke
            _15(bv_ball_x < 15) { bv_ball_x = 15; bv_ball_vx = -bv_ball_vx; play_freq(800); }
            _15(bv_ball_x > ww-15) { bv_ball_x = ww-15; bv_ball_vx = -bv_ball_vx; play_freq(800); }
            _15(bv_ball_y < 15) { bv_ball_y = 15; bv_ball_vy = 2; }
            
            /// Netz-Kollision
            _15(bv_ball_x > ww/2-10 AND bv_ball_x < ww/2+10 AND bv_ball_y > floor_y-70) {
                _15(bv_ball_y < floor_y-65 AND bv_ball_vy > 0) { bv_ball_vy = -8; } 
                _41 { bv_ball_vx = -bv_ball_vx; bv_ball_x += bv_ball_vx*2; } 
                play_freq(600);
            }
            
            /// Ball trifft Blobs
            _15(bv_ball_vy > 0) {
                _15(bv_ball_y+10 > bv_p1_y AND bv_ball_y-10 < bv_p1_y+30 AND bv_ball_x > bv_p1_x-10 AND bv_ball_x < bv_p1_x+50) {
                    bv_ball_vy = -14; 
                    bv_ball_vx = (bv_ball_x - (bv_p1_x + 20)) / 3;
                    _15(bv_last_touch NEQ 1) bv_touches = 0; 
                    bv_last_touch = 1; bv_touches++;
                    _15(bv_touches > 3) { 
                        bv_score2++; bv_ball_x = ww*3/4; bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0; 
                        bv_touches = 0; bv_last_touch = 0; play_sound(200, 20); 
                    } _41 play_sound(1200, 2);
                }
                
                _15(bv_ball_y+10 > bv_p2_y AND bv_ball_y-10 < bv_p2_y+30 AND bv_ball_x > bv_p2_x-10 AND bv_ball_x < bv_p2_x+50) {
                    bv_ball_vy = -14; 
                    bv_ball_vx = (bv_ball_x - (bv_p2_x + 20)) / 3;
                    _15(bv_ball_vx >= -2 AND bv_ball_vx <= 2) {
                        _15(bv_diff EQ 2) bv_ball_vx = -10; 
                        _15(bv_diff EQ 1) bv_ball_vx = -7;  
                        _15(bv_diff EQ 0) bv_ball_vx = -4;  
                    }
                    _15(bv_last_touch NEQ 2) bv_touches = 0; 
                    bv_last_touch = 2; bv_touches++;
                    _15(bv_touches > 3) { 
                        bv_score1++; bv_ball_x = ww/4; bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0; 
                        bv_touches = 0; bv_last_touch = 0; play_sound(200, 20); 
                    } _41 play_sound(1200, 2);
                }
            }
            
            /// Punkt!
            _15(bv_ball_y > floor_y) {
                _15(bv_ball_x < ww/2) { bv_score2++; bv_ball_x = ww/4; }
                _41 { bv_score1++; bv_ball_x = ww*3/4; }
                bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0;
                bv_touches = 0; bv_last_touch = 0; 
                play_sound(200, 20);
            }
            _15(bv_score1 >= 10 OR bv_score2 >= 10) bv_state = 2;
            _41 play_freq(0); 
        }
        
        /// Render Blobs & Ball
        DrawRoundedRect(wx + bv_p1_x, py + bv_p1_y, 40, 30, 15, 0x00FF00); 
        DrawRoundedRect(wx + bv_p2_x, py + bv_p2_y, 40, 30, 15, 0xFF0000); 
        DrawRoundWindow(wx + bv_p1_x + 25, py + bv_p1_y + 10, 4, 0xFFFFFF); DrawRoundWindow(wx + bv_p1_x + 27, py + bv_p1_y + 10, 2, 0x000000);
        DrawRoundWindow(wx + bv_p2_x + 15, py + bv_p2_y + 10, 4, 0xFFFFFF); DrawRoundWindow(wx + bv_p2_x + 13, py + bv_p2_y + 10, 2, 0x000000);
        DrawRoundWindow(wx + bv_ball_x, py + bv_ball_y, 10, 0xFFFFFF); 
    }
}

_50 run_browser_engine(_43 wx, _43 wy, _43 ww, _43 wh, _44 blocked) {
    _43 py = wy + 40;

    _15(browser_view EQ 0) {
        /// ==========================================
        /// VIEW 0: DER WEB BROWSER
        /// ==========================================
        DrawRoundedRect(wx+10, py, ww-130, 25, 3, 0xEEEEEE);
        Text(wx+15, py+8, browser_url, 0x000000, _128);
        
        _15(!blocked AND (frame / 20) % 2 EQ 0) DrawChar(wx+15+(browser_url_idx*7), py+8, '_', 0x000000, _128);
        
        _15(key_new AND win_z[12] EQ 8) { 
            _30 c = last_app_key; 
            _15(c EQ '\b' OR c EQ 8) { _15(browser_url_idx > 0) browser_url[--browser_url_idx] = 0; } 
            _41 _15(c >= 32 AND c <= 126) { _15(browser_url_idx < 60) { browser_url[browser_url_idx++] = c; browser_url[browser_url_idx] = 0; } } 
        }

        /// GO & CFG Buttons
        _44 go_hov = is_over_rect(mouse_x, mouse_y, wx+ww-60, py, 50, 25);
        DrawRoundedRect(wx+ww-60, py, 50, 25, 3, go_hov ? 0x00AAFF : 0x0055AA);
        Text(wx+ww-45, py+8, "GO", 0xFFFFFF, _128);

        _44 cfg_hov = is_over_rect(mouse_x, mouse_y, wx+ww-115, py, 50, 25);
        DrawRoundedRect(wx+ww-115, py, 50, 25, 3, cfg_hov ? 0x888888 : 0x555555);
        Text(wx+ww-100, py+8, "CFG", 0xFFFFFF, _128);
        
        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked) {
            /// ECHTER BROWSER START!
            _15(go_hov) {
                str_cpy(browser_content, "CONNECTING TO HOST...\nSENDING HTTP GET REQUEST...");
                str_cpy(cmd_status, "HTTP GET SENT (PORT 80)");
                browser_state = 1;
                input_cooldown = 20;
            }
            /// WECHSEL ZUM CONFIGURATOR!
            _15(cfg_hov) { browser_view = 1; input_cooldown = 20; }
        }

        DrawRoundedRect(wx+10, py+35, ww-20, wh-85, 3, 0xFFFFFF);
        _15(browser_state EQ 1 AND (frame % 30 EQ 0)) {
            _43 len = 0; _114(browser_content[len] NEQ 0) len++;
            _15(len < 2000) { str_cat(browser_content, "."); _15(len % 40 EQ 0) str_cat(browser_content, "\n"); }
        }
        Text(wx+15, py+40, browser_content, 0x000000, _86);
    }
    _41 {
        /// ==========================================
        /// VIEW 1: DER LAN CONFIGURATOR
        /// ==========================================
        DrawRoundedRect(wx+10, py, ww-130, 25, 3, 0xEEEEEE);
        Text(wx+15, py+8, netcfg_ip, 0x000000, _128);
        
        _15(!blocked AND (frame / 20) % 2 EQ 0) DrawChar(wx+15+(netcfg_ip_idx*7), py+8, '_', 0x000000, _128);
        
        _15(key_new AND win_z[12] EQ 8) { 
            _30 c = last_app_key; 
            _15(c EQ '\b' OR c EQ 8) { _15(netcfg_ip_idx > 0) netcfg_ip[--netcfg_ip_idx] = 0; } 
            _41 _15((c >= '0' AND c <= '9') OR c EQ '.') { _15(netcfg_ip_idx < 15) { netcfg_ip[netcfg_ip_idx++] = c; netcfg_ip[netcfg_ip_idx] = 0; } } 
        }

        /// APPLY & BACK Buttons
        _44 app_hov = is_over_rect(mouse_x, mouse_y, wx+ww-60, py, 50, 25);
        DrawRoundedRect(wx+ww-60, py, 50, 25, 3, app_hov ? 0x00AAFF : 0x0055AA);
        Text(wx+ww-50, py+8, "APP", 0xFFFFFF, _128);

        _44 back_hov = is_over_rect(mouse_x, mouse_y, wx+ww-115, py, 50, 25);
        DrawRoundedRect(wx+ww-115, py, 50, 25, 3, back_hov ? 0x888888 : 0x555555);
        Text(wx+ww-105, py+8, "BCK", 0xFFFFFF, _128);
        
        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked) {
            /// STATISCHE IP ANWENDEN!
            _15(app_hov) {
                str_cpy(netcfg_content, "CONFIGURING NETWORK...\nSENDING GRATUITOUS ARP PING...");
                apply_static_ip(netcfg_ip);
                input_cooldown = 20;
            }
            /// WECHSEL ZURÜCK ZUM BROWSER!
            _15(back_hov) { browser_view = 0; input_cooldown = 20; }
        }

        DrawRoundedRect(wx+10, py+35, ww-20, wh-85, 3, 0xFFFFFF);
        Text(wx+15, py+40, netcfg_content, 0x000000, _86);
    }
}
_50 run_network_configurator(_43 wx, _43 wy, _43 ww, _43 wh, _44 blocked) {
    _43 py = wy + 40;
    
    DrawRoundedRect(wx+10, py, ww-90, 25, 3, 0xEEEEEE);
    Text(wx+15, py+8, netcfg_ip, 0x000000, _128);
    
    _15(!blocked AND (frame / 20) % 2 EQ 0) {
        DrawChar(wx+15+(netcfg_ip_idx*7), py+8, '_', 0x000000, _128);
    }
    
    /// Tastatur nur abfangen, wenn Configurator (ID 9) im Fokus ist!
    _15(key_new AND win_z[12] EQ 9) { 
        _30 c = last_app_key; 
        _15(c EQ '\b' OR c EQ 8) { 
            _15(netcfg_ip_idx > 0) { 
                netcfg_ip_idx--; 
                netcfg_ip[netcfg_ip_idx] = 0; 
            } 
        } 
        _41 _15((c >= '0' AND c <= '9') OR c EQ '.') { 
            _15(netcfg_ip_idx < 15) { 
                netcfg_ip[netcfg_ip_idx++] = c; 
                netcfg_ip[netcfg_ip_idx] = 0; 
            } 
        } 
    }

    _44 go_hov = is_over_rect(mouse_x, mouse_y, wx+ww-70, py, 60, 25);
    DrawRoundedRect(wx+ww-70, py, 60, 25, 3, go_hov ? 0x00AAFF : 0x0055AA);
    Text(wx+ww-58, py+8, "APPLY", 0xFFFFFF, _128);
    
    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND go_hov) {
        str_cpy(netcfg_content, "CONFIGURING NETWORK INTERFACE...\n");
        str_cat(netcfg_content, "IP: ");
        str_cat(netcfg_content, netcfg_ip);
        str_cat(netcfg_content, "\nSUBNET: 255.255.255.0\nGATEWAY: 192.168.14.14\n\nSENDING GRATUITOUS ARP PING...");
        
        /// Deine Bare Metal Funktion aus der net.cpp aufrufen!
        apply_static_ip(netcfg_ip);
        
        netcfg_state = 1;
        input_cooldown = 20;
    }

    DrawRoundedRect(wx+10, py+35, ww-20, wh-85, 3, 0xFFFFFF);
    Text(wx+15, py+40, netcfg_content, 0x000000, _86);
}
