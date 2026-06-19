#include <stdint.h>
#include "schneider_lang.h"
#include "surreal_gl.h"

// === FEHLENDE GLOBALE VARIABLEN ===
volatile int os_app_x = 312;
volatile int os_app_y = 234;
volatile int os_app_w = 800;
volatile int os_app_h = 600;
int meow_timer = 0;

extern _50 TextC(_43 cp, _43 y, _71 _30* s, _89 col, _44 bold);
extern _50 DrawRoundedRect(_43 x, _43 y, _43 rw, _43 rh, _43 r, _89 c);
extern _50 DrawGlassRect(_43 x, _43 y, _43 rw, _43 rh, _43 r, _89 c);
extern _43 input_cooldown;
int mouse_wheel = 0;
extern _44 mouse_just_pressed;
extern _43 mouse_x, mouse_y;
extern bool mouse_right_down;
extern uint32_t frame;
extern _44 is_over_rect(_43 mx, _43 my, _43 x, _43 y, _43 w, _43 h);

extern uint32_t frame;
extern volatile uint64_t system_ticks; // NEU: Für den FPS Zähler!

// === KEYBOARD STATE ===
extern "C" uint8_t os2_key_scancode;
static bool sc_keys[256] = {false};

// === PERFEKTE START-KAMERA (3rd Person Action Sicht) ===
float sc_cam_rot_x = 30.0f;  // POSITIV: Kamera blickt von oben nach unten!
float sc_cam_rot_y = 0.0f;   // Geradeaus
float sc_cam_dist = -30.0f;  // Zoom nach hinten (Abstand zur Katze)

float sc_cat_x = 0.0f;
float sc_cat_z = 0.0f;
float sc_cat_rot = 0.0f;
float sc_walk_t = 0.0f; // Animations-Timer fürs Laufen

// === INVENTORY SYSTEM ===
struct ScInvSlot { int type; int value; int count; };
ScInvSlot sc_inv[20] = {0}; // 20 Slots gesamt (5 Hotbar + 15 Rucksack)
bool sc_show_inv = false;
int sc_msg_timer = 0;
// === CHAR SLOTS & 9 LIVES ===
struct ScSlot {
    int c, p, a, w, f;
    int lives;
    uint32_t playtime;
	uint32_t deathtime;
	uint32_t record_playtime;
    bool active;
};
ScSlot sc_slots[3] = {{0}};
int sc_current_slot = 0;
int sc_player_lives = 9;

// === SURREAL GRAFIK EDITOR: MORPH SYSTEM ===
struct CatMorph {
    // Kopf
    float head_scale;      // Default: 1.0
    float ear_height;      // Default: 1.0
    float ear_width;       // Default: 1.0
    float ear_spread;      // Default: 1.0
    // Koerper
    float body_width;      // Default: 1.0
    float body_height;     // Default: 1.0
    float body_depth;      // Default: 1.0
    // Arme
    float arm_length;      // Default: 1.0
    float arm_thickness;   // Default: 1.0
    // Beine
    float leg_length;      // Default: 1.0
    float leg_thickness;   // Default: 1.0
    // Schwanz
    float tail_length;     // Default: 1.0
    float tail_curl;       // Default: 1.0
    float tail_thickness;  // Default: 1.0
    // Gesicht
    float eye_size;        // Default: 1.0
    float eye_distance;    // Default: 1.0
    float pupil_size;      // Default: 1.0
    float nose_size;       // Default: 1.0
};

static void morph_reset(CatMorph* m) {
    m->head_scale = 1.0f;   m->ear_height = 1.0f;    m->ear_width = 1.0f;     m->ear_spread = 1.0f;
    m->body_width = 1.0f;   m->body_height = 1.0f;    m->body_depth = 1.0f;
    m->arm_length = 1.0f;   m->arm_thickness = 1.0f;
    m->leg_length = 1.0f;   m->leg_thickness = 1.0f;
    m->tail_length = 1.0f;  m->tail_curl = 1.0f;      m->tail_thickness = 1.0f;
    m->eye_size = 1.0f;     m->eye_distance = 1.0f;   m->pupil_size = 1.0f;    m->nose_size = 1.0f;
}

CatMorph player_morph;
CatMorph slot_morphs[3];
bool morph_initialized = false;

// Editor State
int ed_selected_part = 0;     // 0=Kopf, 1=Ohren, 2=Koerper, 3=Arme, 4=Beine, 5=Schwanz, 6=Augen, 7=Nase, 8=Items
int ed_selected_slider = -1;  // Welcher Slider wird gerade gezogen

float ed_cam_rot_y = 180.0f;  // FIX: 180 Grad, damit die Katze uns ansieht!
float ed_cam_rot_x = 0.0f;   // Editor Kamera X-Rotation
float ed_cam_dist = 35.0f;    // NEU: Eigene Zoom-Variable für den Editor!

bool ed_wireframe = true;     // Wireframe Overlay aktiv
bool ed_show_items = false;   // Item-Tab aktiv

// Hilfsfunktion: Wandelt Zahlen in Text um, damit wir Stack-Zahlen zeichnen können
static void num_to_str(int n, char* buf) {
    if(n==0) { buf[0]='0'; buf[1]=0; return; }
    int i=0; char rev[16];
    while(n>0) { rev[i++] = '0' + (n%10); n/=10; }
    for(int j=0; j<i; j++) buf[j] = rev[i-1-j];
    buf[i] = 0;
}

int sc_last_mx = 0;
int sc_last_my = 0;

int sc_state = 0;
int sc_attack_l_timer = 0; // Timer für Linken Arm
int sc_attack_r_timer = 0; // Timer für Rechten Arm

int drop_active = 0;
int drop_type = 0; 
float drop_x = 0.0f;
float drop_z = 0.0f;
int drop_value = 0; 
char drop_name[64] = "NOTHING";
int drop_stat = 0;

#define MAX_ENEMIES 5
float sc_cat_y = 0.0f;       // Höhe der Katze
float sc_cat_vy = 0.0f;      // Sprung-Geschwindigkeit
int sc_jump_count = 0;       // 0 = Boden, 1 = Sprung, 2 = Salto
float sc_cat_flip = 0.0f;    // Drehwinkel für den Salto

float sc_enemy_x[MAX_ENEMIES] = {20.0f, -20.0f, 30.0f};
float sc_enemy_z[MAX_ENEMIES] = {20.0f, 20.0f, -20.0f};
float sc_enemy_rot[MAX_ENEMIES] = {180.0f, 180.0f, 0.0f};
int sc_enemy_hp[MAX_ENEMIES] = {5, 6, 7, 8, 9};
int sc_enemy_hit_timer[MAX_ENEMIES] = {0};
int sc_enemy_attack_timer[MAX_ENEMIES] = {0};
int sc_enemy_respawn_timer[MAX_ENEMIES] = {0}; // NEU: RESPAWN TIMER

int sc_player_hp = 10; // NEU: 10 LEBEN!
int sc_player_hit_timer = 0;

int player_color_idx = 0;   
int player_pattern_idx = 0; 
int player_armor_idx = 0;   
int player_weapon_idx = 0;  
int player_face_idx = 0;    

int enemy_color_idx[MAX_ENEMIES] = {1, 2, 3};
int enemy_pattern_idx[MAX_ENEMIES] = {0};
int enemy_armor_idx[MAX_ENEMIES] = {0};
int enemy_weapon_idx[MAX_ENEMIES] = {0};
int enemy_face_idx[MAX_ENEMIES] = {0};

const uint8_t voxel_cat_pixels[12][12] = {
    {1,0,0,0,0,0,0,0,0,0,1,0},
    {1,2,1,0,0,0,0,0,0,1,2,1},
    {1,2,2,1,1,1,1,1,1,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,3,2,2,2,2,1,3,2,1},
    {1,2,3,4,2,2,5,2,3,4,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,1},
    {0,1,2,2,2,2,2,2,2,2,1,0},
    {0,1,1,2,2,2,2,2,2,1,1,0},
    {1,2,2,1,1,1,1,1,1,2,2,1},
    {1,2,2,1,0,0,0,0,1,2,2,1},
    {0,1,1,0,0,0,0,0,0,1,1,0}
};

// === HOCHPRÄZISE TRIGONOMETRIE GEGEN DIE FELL-LÖCHER ===
static float app_sinf(float x) {
    x = x - (int)(x / 6.2831853f) * 6.2831853f;
    if (x < 0) x += 6.2831853f;
    x -= 3.14159265f; // Verschiebung auf das Zentrum 0
    
    float abs_x = x < 0 ? -x : x;
    float y = (4.0f / 3.14159265f) * x - (4.0f / (3.14159265f * 3.14159265f)) * x * abs_x;
    return -y;
}
static float app_cosf(float x) { return app_sinf(x + 1.5707963f); }

static void GetCatColor(int idx, uint8_t* r, uint8_t* g, uint8_t* b) {
    if (idx == 0)      { *r = 255; *g = 140; *b = 0;   } // Orange
    else if (idx == 1) { *r = 140; *g = 50;  *b = 180; } // Lila (Deep Purple)
    else if (idx == 2) { *r = 50;  *g = 150; *b = 255; } // Blue
    else if (idx == 3) { *r = 240; *g = 240; *b = 240; } // White
    else if (idx == 4) { *r = 30;  *g = 30;  *b = 40;  } // Black
    else if (idx == 5) { *r = 255; *g = 175; *b = 185; } // Rosa (Soft Rose)
    else if (idx == 6) { *r = 255; *g = 20;  *b = 147; } // Pink (Hot Pink)
    else               { *r = 220; *g = 30;  *b = 30;  } // Rot (Crimson)
}

static void DrawBox(float px, float py, float pz, float sx, float sy, float sz) {
    sglPushMatrix(); sglTranslatef(px, py, pz); sglScalef(sx, sy, sz); sglDrawCube(1.0f); sglPopMatrix();
}

static void DrawSurrealBlob(float px, float py, float pz, float sx, float sy, float sz) {
    sglPushMatrix(); sglTranslatef(px, py, pz); sglScalef(sx * 0.5f, sy * 0.5f, sz * 0.5f); 
    sglDrawSphere(1.0f, 24); 
    sglPopMatrix();
}

// === DEZENTER & NATÜRLICHER GLANZ-NEBEL (Keine Billardkugel mehr!) ===
static void DrawStardustFur(float px, float py, float pz, float sx, float sy, float sz, int pattern_idx, float anim_t, uint8_t base_r, uint8_t base_g, uint8_t base_b, bool add_bristles) {
    uint8_t cr = base_r, cg = base_g, cb = base_b;
    if (pattern_idx == 1) { cr = (cr * 3) / 4; cg = (cg * 3) / 4; cb = (cb * 3) / 4; } 
    else if (pattern_idx == 2) { cr = cr + (255-cr)/2; cg = cg + (255-cg)/2; cb = cb + (255-cb)/2; }
    
    // 1. Der feste Körper (schattiert)
    sglColor3ub(cr, cg, cb); 
    DrawSurrealBlob(px, py, pz, sx, sy, sz);
    
    // 2. Die dezent glänzende Nebel-Haut
    if (add_bristles) {
        sglPushMatrix(); 
        sglTranslatef(px, py, pz);
        sglScalef(sx * 0.5f, sy * 0.5f, sz * 0.5f); 
        
        sglBegin(4); // Modus 4 = SGL_POINTS
        
        int lat_bands = 50;  
        int lon_slices = 80; 
        
        // Optimierter Half-Vector für die Lichtreflexion
        float hx = 0.22f;
        float hy = 0.39f;
        float hz = 0.89f;
        
        for(int i = 0; i <= lat_bands; i++) {
            float lat = 3.14159265f * (-0.5f + (float)i / lat_bands); 
            float z_norm = app_sinf(lat); 
            float r = app_cosf(lat);
            
            for(int j = 0; j < lon_slices; j++) {
                float offset = (i % 2 == 0) ? 0.0f : 0.5f;
                float lon = 6.2831853f * ((float)j + offset) / lon_slices;        
                
                float x_norm = app_cosf(lon) * r;
                float y_norm = app_sinf(lon) * r;
                
                float spec = x_norm * hx + y_norm * hy + z_norm * hz;
                if (spec < 0.0f) spec = 0.0f;
                
                // EXPONENTEN-BOOST: Wir erhöhen die Potenz massiv auf spec^32.
                // Dadurch schrumpft der gigantische weiße Fleck zu einem winzigen, scharfen Glanzpunkt!
                float spec2 = spec * spec;
                float spec4 = spec2 * spec2;
                float spec16 = spec4 * spec4;
                float spec32 = spec16 * spec16; 
                
                // DÄMPFUNG: Wir multiplizieren mit 0.35f, damit das Highlight das Fell nicht komplett weiß blendet,
                // sondern nur einen dezenten, echten Seidenglanz erzeugt.
                float final_spec = spec32 * 0.35f;
                
                uint8_t pr = cr + (uint8_t)((255 - cr) * final_spec);
                uint8_t pg = cg + (uint8_t)((255 - cg) * final_spec);
                uint8_t pb = cb + (uint8_t)((255 - cb) * final_spec);
                sglColor3ub(pr, pg, pb);
                
                float wave = app_sinf(anim_t * 5.0f + (float)(i * 7 + j * 13)) * 0.006f;
                float abstand = 1.015f + wave; 
                
                sglVertex3f(x_norm * abstand, y_norm * abstand, z_norm * abstand);
            }
        }
        sglEnd();
        sglPopMatrix();
    }
}

// === 2. DIE SCHÖNHEITS-OP FÜR DIE AUGEN (Tiefe Glubschaugen) ===
static void DrawCatFace(uint8_t mr, uint8_t mg, uint8_t mb, const CatMorph* morph) {
    float ed = morph ? morph->eye_distance : 1.0f;
    float es = morph ? morph->eye_size : 1.0f;
    float ps = morph ? morph->pupil_size : 1.0f;
    float ns = morph ? morph->nose_size : 1.0f;
    
    // Linkes Auge (Große Kugel, aber ganz tief in den Kopf geschoben! z=0.15)
    sglPushMatrix(); sglTranslatef(-0.35f * ed, 0.15f, 0.15f); 
    sglColor3ub(230, 230, 220); 
    sglDrawSphere(0.4f * es, 16); 
    sglTranslatef(0.0f, 0.0f, 0.35f * es); // Pupille auf die Oberfläche schieben
    sglColor3ub(20, 25, 20); 
    sglDrawSphere(0.15f * ps, 12); 
    sglPopMatrix();
    
    // Rechtes Auge (Große Kugel, ganz tief im Kopf)
    sglPushMatrix(); sglTranslatef(0.35f * ed, 0.15f, 0.15f); 
    sglColor3ub(230, 230, 220); 
    sglDrawSphere(0.4f * es, 16); 
    sglTranslatef(0.0f, 0.0f, 0.35f * es); 
    sglColor3ub(20, 25, 20); 
    sglDrawSphere(0.15f * ps, 12); 
    sglPopMatrix();
    
    // Nase (Etwas tiefer gesetzt)
    sglPushMatrix(); sglTranslatef(0.0f, -0.2f, 0.4f); 
    sglColor3ub(180, 100, 110); 
    sglDrawSphere(0.15f * ns, 8); 
    sglPopMatrix();
}

void Draw3DCat(float scale, uint8_t mr, uint8_t mg, uint8_t mb, float anim_t, float walk_t, int attack_l, int attack_r, int pattern_idx, int armor_idx, int face_idx, int weapon_idx, const CatMorph* morph = 0) {
    sglPushMatrix(); sglScalef(scale, scale, scale);
    
    // Morph-Werte extrahieren (Fallback: 1.0)
    float m_hs = morph ? morph->head_scale : 1.0f;
    float m_eh = morph ? morph->ear_height : 1.0f;
    float m_ew = morph ? morph->ear_width : 1.0f;
    float m_es = morph ? morph->ear_spread : 1.0f;
    float m_bw = morph ? morph->body_width : 1.0f;
    float m_bh = morph ? morph->body_height : 1.0f;
    float m_bd = morph ? morph->body_depth : 1.0f;
    float m_al = morph ? morph->arm_length : 1.0f;
    float m_at = morph ? morph->arm_thickness : 1.0f;
    float m_ll = morph ? morph->leg_length : 1.0f;
    float m_lt = morph ? morph->leg_thickness : 1.0f;
    float m_tl = morph ? morph->tail_length : 1.0f;
    float m_tc = morph ? morph->tail_curl : 1.0f;
    float m_tt = morph ? morph->tail_thickness : 1.0f;

    // --- KÖRPER & BAUCH ---
    if (armor_idx >= 1) { 
        sglPushMatrix(); sglColor3ub(100, 100, 100); DrawBox(0.0f, 0.0f, 0.0f, 4.2f * m_bw, 3.0f * m_bh, 4.0f * m_bd);
        if (armor_idx == 2) { 
            sglPushMatrix(); sglTranslatef(-2.5f * m_bw, 2.0f * m_bh, -1.0f); sglRotatef(30.0f + app_sinf(anim_t*5.0f)*20.0f, 0.0f, 0.0f, 1.0f); 
            sglColor3ub(200, 240, 255); DrawBox(-3.0f, 1.0f, 0.0f, 6.0f, 0.5f, 3.0f); sglPopMatrix(); 
        }
        sglPopMatrix(); 
    } else {
        DrawStardustFur(0.0f, 0.0f, 0.0f, 4.0f * m_bw, 6.0f * m_bh, 3.5f * m_bd, pattern_idx, anim_t, mr, mg, mb, true); 
        uint8_t br = (uint8_t)(mr + (255-mr)/3), bg = (uint8_t)(mg + (255-mg)/3), bb2 = (uint8_t)(mb + (255-mb)/3);
        DrawStardustFur(0.0f, -0.5f, 0.5f, 3.5f * m_bw, 5.0f * m_bh, 3.8f * m_bd, pattern_idx, anim_t, br, bg, bb2, true); 
    }

    // --- KOPF & OHREN ---
    sglPushMatrix(); sglTranslatef(0.0f, 4.5f * m_bh, 0.0f); 
    DrawStardustFur(0.0f, 0.0f, 0.0f, 5.5f * m_hs, 5.5f * m_hs, 5.0f * m_hs, pattern_idx, anim_t, mr, mg, mb, true); 
    DrawStardustFur(-1.8f * m_es, 3.0f * m_eh, 0.0f, 1.5f * m_ew, 2.5f * m_eh, 1.5f * m_ew, pattern_idx, anim_t, mr, mg, mb, true); // Ohr Links
    DrawStardustFur(1.8f * m_es, 3.0f * m_eh, 0.0f, 1.5f * m_ew, 2.5f * m_eh, 1.5f * m_ew, pattern_idx, anim_t, mr, mg, mb, true);  // Ohr Rechts
    
    // --- GESICHT / VISIER ---
    sglPushMatrix(); sglTranslatef(0.0f, 0.2f, 2.2f * m_hs); 
    if (face_idx == 0) { // Cyber Visor
        sglColor3ub(50, 50, 200); DrawBox(0.0f, 0.2f, 2.2f, 2.5f, 1.0f, 0.5f);
        sglColor3ub(0, 0, 0); DrawBox(1.0f, 0.2f, 2.5f, 0.8f, 0.4f, 0.2f);
        sglColor3ub(0, 0, 0); DrawBox(-1.0f, 0.2f, 2.5f, 0.8f, 0.4f, 0.2f);
        sglColor3ub(255, 100, 100); DrawBox(0.0f, -0.4f, 2.5f, 0.3f, 0.2f, 0.2f); // Nase unterm Visier
    } else {
        DrawCatFace(mr, mg, mb, morph);
        sglColor3ub(255, 100, 100); DrawBox(0.0f, -0.2f, 0.2f, 0.4f, 0.2f, 0.2f); // Normale Nase
    }
    sglPopMatrix();
    sglPopMatrix(); // Kopf Ende

    uint8_t dr = (uint8_t)(mr * 8/10), dg = (uint8_t)(mg * 8/10), db = (uint8_t)(mb * 8/10);
    float leg_swing = app_sinf(walk_t) * 2.0f;

    // --- BEINE ---
    DrawStardustFur(-1.2f * m_lt, -3.5f * m_ll, leg_swing, 1.5f * m_lt, 4.0f * m_ll, 1.5f * m_lt, pattern_idx, anim_t, dr, dg, db, true); 
    DrawStardustFur(1.2f * m_lt, -3.5f * m_ll, -leg_swing, 1.5f * m_lt, 4.0f * m_ll, 1.5f * m_lt, pattern_idx, anim_t, dr, dg, db, true);
    DrawStardustFur(-1.2f * m_lt, -5.5f * m_ll, leg_swing + 0.5f, 1.6f * m_lt, 1.0f, 2.5f * m_lt, 0, anim_t, 255, 255, 255, true); 
    DrawStardustFur(1.2f * m_lt, -5.5f * m_ll, -leg_swing + 0.5f, 1.6f * m_lt, 1.0f, 2.5f * m_lt, 0, anim_t, 255, 255, 255, true);

    // --- LINKER ARM ---
    sglPushMatrix();
    sglTranslatef(-2.5f * m_bw, 2.0f, 0.0f);
    if (attack_l > 0) {
        float progress = (15.0f - attack_l) / 15.0f; 
        sglRotatef(app_sinf(progress * 3.14159f) * -90.0f, 1.0f, 0.0f, 0.0f); 
    } else { sglRotatef(leg_swing * -15.0f, 1.0f, 0.0f, 0.0f); }
    DrawStardustFur(0.0f, -1.5f * m_al, 0.0f, 1.2f * m_at, 3.5f * m_al, 1.2f * m_at, pattern_idx, anim_t, dr, dg, db, true); 
    DrawStardustFur(0.0f, -3.5f * m_al, 0.0f, 1.2f * m_at, 1.2f * m_at, 1.2f * m_at, 0, anim_t, 255, 255, 255, true); 
    sglPopMatrix();

    // --- RECHTER ARM & SCHWERT ---
    sglPushMatrix();
    sglTranslatef(2.5f * m_bw, 2.0f, 0.0f);
    if (attack_r > 0) {
        float progress = (15.0f - attack_r) / 15.0f; 
        sglRotatef(app_sinf(progress * 3.14159f) * -90.0f, 1.0f, 0.0f, 0.0f); 
    } else { sglRotatef(leg_swing * 15.0f, 1.0f, 0.0f, 0.0f); }
    DrawStardustFur(0.0f, -1.5f * m_al, 0.0f, 1.2f * m_at, 3.5f * m_al, 1.2f * m_at, pattern_idx, anim_t, dr, dg, db, true); 
    DrawStardustFur(0.0f, -3.5f * m_al, 0.0f, 1.2f * m_at, 1.2f * m_at, 1.2f * m_at, 0, anim_t, 255, 255, 255, true); 

    if (weapon_idx == 0) {
        sglPushMatrix();
        sglTranslatef(0.0f, -4.5f * m_al, 0.5f);
        sglColor3ub(100, 50, 20);  DrawBox(0.0f, -0.5f, 0.0f, 0.3f, 1.0f, 0.3f); 
        sglColor3ub(80, 80, 80);   DrawBox(0.0f, -1.2f, 0.0f, 1.2f, 0.2f, 0.4f); 
        sglColor3ub(200, 190, 50); DrawBox(0.0f, -3.5f, 0.0f, 0.2f, 2.0f, 0.2f); 
        sglPopMatrix();
    }
    sglPopMatrix();
    
    // --- SCHWANZ ---
    float tail_wave = app_sinf(anim_t * 4.0f) * 1.5f * m_tc;
    DrawStardustFur(tail_wave * 0.3f, -1.0f, -2.5f * m_tl, 1.5f * m_tt, 1.5f * m_tt, 3.5f * m_tl, pattern_idx, anim_t, mr, mg, mb, true); 
    DrawStardustFur(tail_wave * 0.6f, -1.5f, -4.5f * m_tl, 1.5f * m_tt, 1.5f * m_tt, 3.0f * m_tl, pattern_idx, anim_t, mr, mg, mb, true);
    DrawStardustFur(tail_wave * 1.0f, -1.0f, -6.5f * m_tl, 1.5f * m_tt, 1.5f * m_tt, 2.5f * m_tl, pattern_idx, anim_t, mr, mg, mb, true); 
    DrawStardustFur(tail_wave * 1.5f, -0.5f, -8.0f * m_tl, 1.2f * m_tt, 1.2f * m_tt, 1.5f * m_tl, pattern_idx, anim_t, dr, dg, db, true);

    sglPopMatrix();
}

// === DIE PRE-CACHED BLUME DES LEBENS (Maximal FPS & Hauchdünn!) ===
static void DrawFluidMeadow(float anim_t) {
    sglPushMatrix();
    sglDisable(SGL_LIGHTING); 
    sglBegin(4); // Modus 4 = SGL_POINTS
    
    // STATIC CACHE: Wir berechnen das Muster nur ein EINZIGES MAL!
    // Das spart zehntausende Sinus/Cosinus Rechnungen pro Frame.
    static bool is_cached = false;
    static int pt_count = 0;
    static float pt_x[40000];
    static float pt_z[40000];
    static uint8_t pt_r[40000];
    static uint8_t pt_g[40000];
    static uint8_t pt_b[40000];
    
    if (!is_cached) {
        unsigned int seed = 1337; 
        float R = 28.0f; 
        float v_dist = R * 0.8660254f; 
        
        for(int row = -10; row <= 10; row++) {
            for(int col = -10; col <= 10; col++) {
                
                float offset = (row % 2 != 0) ? (R * 0.5f) : 0.0f;
                float cx = col * R + offset;
                float cz = row * v_dist;
                
                // 72 winzige Punkte pro Kreis (exakt alle 5 Grad). 
                // Dadurch entsteht eine hauchdünne, perfekte Linie!
                for(int a = 0; a < 360; a += 5) { 
                    float angle = (float)a * 3.14159f / 180.0f; 
                    float px = cx + app_cosf(angle) * R;
                    float pz = cz + app_sinf(angle) * R;
                    
                    if (px > -240.0f && px < 240.0f && pz > -240.0f && pz < 240.0f) {
                        seed = seed * 1103515245 + 12345;
                        pt_x[pt_count] = px;
                        pt_z[pt_count] = pz;
                        pt_r[pt_count] = 80 + (seed % 80);         // Rot
                        pt_g[pt_count] = 10 + ((seed >> 4) % 15);  // Grün
                        pt_b[pt_count] = 100 + ((seed >> 8) % 90); // Blau
                        pt_count++;
                        
                        if (pt_count >= 39999) break; // Speicher-Sicherheit
                    }
                }
                if (pt_count >= 39999) break;
            }
            if (pt_count >= 39999) break;
        }
        is_cached = true; // Muster ist gespeichert, nie wieder neu rechnen!
    }

    // --- RENDERING LOOP (Extrem schnell, da 0 Mathematik!) ---
    for(int i = 0; i < pt_count; i++) {
        sglColor3ub(pt_r[i], pt_g[i], pt_b[i]);
        sglVertex3f(pt_x[i], 1.0f, pt_z[i]);
    }
    
    sglEnd();
    sglEnable(SGL_LIGHTING); 
    sglPopMatrix();
}

extern "C" void run_smash_cats_engine(int wx, int wy, int ww, int wh, bool is_blocked) {
    // === NEU: Eigener FPS Zähler für das HUD ===
    static uint64_t sc_last_fps_tick = 0;
    static int sc_fps_counter = 0;
    static int sc_display_fps = 60;
    
    sc_fps_counter++;
    if (system_ticks - sc_last_fps_tick >= 1000) {
        sc_display_fps = sc_fps_counter;
        sc_fps_counter = 0;
        sc_last_fps_tick = system_ticks;
    }
    
    sglViewport(wx, wy + 35, ww, wh - 35);
    
    // MORPH INITIALISIERUNG (nur einmal!)
    if (!morph_initialized) {
        morph_reset(&player_morph);
        for (int i = 0; i < 3; i++) morph_reset(&slot_morphs[i]);
        morph_initialized = true;
    }
    
    // TASTEN VERARBEITEN (Jetzt mit deiner echten Variable: os2_key_scancode)
    if (os2_key_scancode != 0) {
        if (os2_key_scancode < 0x80) sc_keys[os2_key_scancode] = true;
        else sc_keys[os2_key_scancode & 0x7F] = false;
        os2_key_scancode = 0; // Direkt aufräumen
    }

    static bool last_mouse_right = false;
    bool mouse_right_just_pressed = mouse_right_down && !last_mouse_right;
    last_mouse_right = mouse_right_down;
    
    static bool last_space = false;
    bool space_just_pressed = sc_keys[0x39] && !last_space;
    last_space = sc_keys[0x39];
    
    // NEU: Taste 'I' abfragen
    static bool last_i_key = false;
    bool i_just_pressed = sc_keys[0x17] && !last_i_key;
    last_i_key = sc_keys[0x17];
    
    // NEU: ESC-Taste (Scancode 0x01) für Rückkehr ins Menü!
    static bool last_esc = false;
    bool esc_just_pressed = sc_keys[0x01] && !last_esc;
    last_esc = sc_keys[0x01];
    
    // NEU: Taste 'E' (Scancode 0x12) fuer den Grafik-Editor!
    static bool last_e_key = false;
    bool e_just_pressed = sc_keys[0x12] && !last_e_key;
    last_e_key = sc_keys[0x12];

    if (i_just_pressed && sc_state == 1) { sc_show_inv = !sc_show_inv; }
    
    // Wenn ESC gedrückt wird, sofort zurück zum Character Creator
    if (esc_just_pressed && sc_state == 1) { sc_state = 0; sc_show_inv = false; }
    
    // E-Taste: Creator -> Editor
    if (e_just_pressed && sc_state == 0) { 
        sc_state = 3; 
        ed_cam_rot_y = 180.0f; // Wieder auf Gesicht drehen
        ed_cam_rot_x = 15.0f; 
        ed_cam_dist = 35.0f;   // Zoom resetten
        ed_selected_part = 0; 
    }
    
    // ESC: Editor -> Creator
    if (esc_just_pressed && sc_state == 3) { sc_state = 0; }
    
    if (sc_msg_timer > 0) sc_msg_timer--;

    if (!is_blocked) {
        // === KAMERA DREHEN (Spiel & Editor) ===
        if (mouse_right_down && !mouse_right_just_pressed) {
            if (sc_state == 3) {
                // Editor Kamera drehen!
                ed_cam_rot_y += (float)(mouse_x - sc_last_mx) * 0.5f;
                ed_cam_rot_x += (float)(mouse_y - sc_last_my) * 0.5f;
                if (ed_cam_rot_x < -30.0f) ed_cam_rot_x = -30.0f;
                if (ed_cam_rot_x > 60.0f) ed_cam_rot_x = 60.0f;
            } else {
                // Spiel Kamera drehen!
                sc_cam_rot_y += (float)(mouse_x - sc_last_mx) * 0.5f;
                sc_cam_rot_x += (float)(mouse_y - sc_last_my) * 0.5f;
                if (sc_cam_rot_x < -20.0f) sc_cam_rot_x = -20.0f;
                if (sc_cam_rot_x > 60.0f) sc_cam_rot_x = 60.0f;
            }
        }
        
        // === KAMERA ZOOMEN (Spiel & Editor) ===
        if (mouse_wheel != 0) {
            if (sc_state == 3) {
                ed_cam_dist -= (float)mouse_wheel * 2.5f; 
                if (ed_cam_dist < 10.0f) ed_cam_dist = 10.0f; // Nah ran
                if (ed_cam_dist > 80.0f) ed_cam_dist = 80.0f; // Weit weg
            } else {
                sc_cam_dist -= (float)mouse_wheel * 2.5f; 
                if (sc_cam_dist < 12.0f) sc_cam_dist = 12.0f; 
                if (sc_cam_dist > 60.0f) sc_cam_dist = 60.0f; 
            }
            mouse_wheel = 0; 
        }
        
        // === PANZER-STEUERUNG (Freies Drehen & Schräg gehen!) ===
        bool is_walking = false;
        
        // A und D = Butterweich auf der Stelle drehen
        if (sc_keys[0x1E]) sc_cat_rot += 4.5f; // A (Links drehen)
        if (sc_keys[0x20]) sc_cat_rot -= 4.5f; // D (Rechts drehen)
        
        // W und S = Exakt in Blickrichtung laufen!
        if (sc_keys[0x11] || sc_keys[0x1F]) {
            float rad = sc_cat_rot * 0.01745329f; // Winkel in Bogenmaß
            float move_x = app_sinf(rad);
            float move_z = app_cosf(rad);
            
            if (sc_keys[0x11]) { // W = Vorwärts
                sc_cat_x += move_x * 1.5f; 
                sc_cat_z += move_z * 1.5f; 
                is_walking = true; 
            }
            if (sc_keys[0x1F]) { // S = Rückwärts
                sc_cat_x -= move_x * 1.0f; // Rückwärts geht man etwas langsamer
                sc_cat_z -= move_z * 1.0f; 
                is_walking = true; 
            }
        }
        
        // ARENA-BEGRENZUNG (Der kleine Park, 50x50 Einheiten)
        // ARENA-BEGRENZUNG (Jetzt 10x größer: von -240 bis +240)
        if (sc_cat_x > 240.0f) sc_cat_x = 240.0f;
        if (sc_cat_x < -240.0f) sc_cat_x = -240.0f;
        if (sc_cat_z > 240.0f) sc_cat_z = 240.0f;
        if (sc_cat_z < -240.0f) sc_cat_z = -240.0f;
        
        if (is_walking && sc_jump_count == 0) sc_walk_t += 0.4f; 
        else if (sc_jump_count == 0) sc_walk_t = 0.0f;
        
        // SPRUNG & SALTO LOGIK!
        if (space_just_pressed && sc_state == 1) {
            if (sc_jump_count == 0) {
                sc_cat_vy = 3.5f; // Normaler Sprung
                sc_jump_count = 1;
            } else if (sc_jump_count == 1) {
                sc_cat_vy = 3.0f; // Salto in der Luft!
                sc_jump_count = 2;
            }
        }
        
        // BOXEN LINKS (Mausklick)
        if (mouse_just_pressed && sc_state == 1 && sc_attack_l_timer == 0) {
            sc_attack_l_timer = 15;
            for (int i=0; i<MAX_ENEMIES; i++) {
                float dx = sc_cat_x - sc_enemy_x[i]; float dz = sc_cat_z - sc_enemy_z[i];
                float dist = (dx<0?-dx:dx) + (dz<0?-dz:dz);
                if (dist < 15.0f && sc_enemy_hp[i] > 0) { sc_enemy_hp[i]--; sc_enemy_hit_timer[i] = 10; }
            }
        }
        
        // BOXEN RECHTS (Rechtsklick)
        if (mouse_right_just_pressed && sc_state == 1 && sc_attack_r_timer == 0) {
            sc_attack_r_timer = 15;
            for (int i=0; i<MAX_ENEMIES; i++) {
                float dx = sc_cat_x - sc_enemy_x[i]; float dz = sc_cat_z - sc_enemy_z[i];
                float dist = (dx<0?-dx:dx) + (dz<0?-dz:dz);
                if (dist < 15.0f && sc_enemy_hp[i] > 0) { sc_enemy_hp[i]--; sc_enemy_hit_timer[i] = 10; }
            }
        }
    }
    
    // PHYSIK UPDATE (Gravitation & Salto-Attacke)
    sc_cat_y += sc_cat_vy;
    sc_cat_vy -= 0.35f; // Schwerkraft
    
    if (sc_jump_count == 2) {
        // ROTATION VORWÄRTS: Wir addieren positiv (Engine freundlich!)
        sc_cat_flip += 22.0f; 
        if (sc_cat_flip >= 360.0f) sc_cat_flip = 360.0f; // Bei einem Salto exakt stoppen
        
        // Salto ist eine Flächenattacke!
        for (int i=0; i<MAX_ENEMIES; i++) {
            float dx = sc_cat_x - sc_enemy_x[i]; float dz = sc_cat_z - sc_enemy_z[i];
            float dist = (dx<0?-dx:dx) + (dz<0?-dz:dz);
            if (dist < 18.0f && sc_enemy_hp[i] > 0 && sc_enemy_hit_timer[i] == 0) { 
                sc_enemy_hp[i]--; sc_enemy_hit_timer[i] = 10; 
            }
        }
    }

    // Harte Kollision mit dem Boden
    if (sc_cat_y <= 0.0f) {
        sc_cat_y = 0.0f; // Boden berührt
        sc_cat_vy = 0.0f;
        sc_jump_count = 0;
    }
    
    // DIE MAGIE: Wenn wir gelandet sind, aber schief stehen, 
    // drehen wir die Katze butterweich zurück! KEINE harter Matrix-Schnitt mehr!
    if (sc_jump_count != 2 && sc_cat_flip > 0.0f) {
        sc_cat_flip += 35.0f; // Schnell zu Ende drehen
        if (sc_cat_flip >= 360.0f) sc_cat_flip = 0.0f; // Perfekter Reset auf 0!
    }
    
    if (sc_attack_l_timer > 0) sc_attack_l_timer--;
    if (sc_attack_r_timer > 0) sc_attack_r_timer--;
	
    // ==========================================
    // ENEMY LOGIC (Patrouille + Zentrum-Fokus + Aggro)
    // ==========================================
    if (sc_state == 1) {
        for (int i=0; i<5; i++) {
            if (sc_enemy_hit_timer[i] > 0) sc_enemy_hit_timer[i]--;
            
            if (sc_enemy_hp[i] > 0) {
                float target_x = sc_cat_x; float target_z = sc_cat_z; int target_id = -1; float min_dist = 9999.0f;
                if (sc_player_hp > 0) { 
                    float pdx = sc_cat_x - sc_enemy_x[i]; float pdz = sc_cat_z - sc_enemy_z[i]; 
                    min_dist = (pdx<0?-pdx:pdx) + (pdz<0?-pdz:pdz); 
                }
                
                for (int j=0; j<5; j++) {
                    if (i != j && sc_enemy_hp[j] > 0) {
                        float edx = sc_enemy_x[j] - sc_enemy_x[i]; float edz = sc_enemy_z[j] - sc_enemy_z[i]; 
                        float edist = (edx<0?-edx:edx) + (edz<0?-edz:edz);
                        if (edist < min_dist) { min_dist = edist; target_x = sc_enemy_x[j]; target_z = sc_enemy_z[j]; target_id = j; }
                    }
                }
                
                float dx = target_x - sc_enemy_x[i]; float dz = target_z - sc_enemy_z[i];
                float adx = dx<0?-dx:dx; float adz = dz<0?-dz:dz; 
                
                // === AGGRO-RADIUS: GANZE MAP (Immer auf den Spieler zu!) ===
                if (min_dist < 99999.0f) { // Vorher 60.0f
                    if (min_dist > 8.0f) {
                        // VERFOLGEN (Aggro!)
                        sc_enemy_x[i] += (dx / min_dist) * 0.4f; sc_enemy_z[i] += (dz / min_dist) * 0.4f;
                        if (adx > adz) { sc_enemy_rot[i] = dx > 0 ? 90.0f : 270.0f; } else { sc_enemy_rot[i] = dz > 0 ? 0.0f : 180.0f; }
                    } else if (sc_enemy_attack_timer[i] == 0) { 
                        sc_enemy_attack_timer[i] = 20; 
                    }
                } else {
                    // === PATROUILLE (Zieht immer zur Mitte zurück!) ===
                    // Distanz zur Map-Mitte (0,0) berechnen
                    float cx = -sc_enemy_x[i];
                    float cz = -sc_enemy_z[i];
                    float cdist = (cx<0?-cx:cx) + (cz<0?-cz:cz);
                    
                    // Modulo 628 (2 * Pi * 100) verhindert Matrix-Glitch und Aufblähen absolut sicher!
                    float roam_rad = (float)((frame + i * 314) % 628) / 100.0f; 
                    float rx = app_cosf(roam_rad);
                    float rz = app_sinf(roam_rad);
                    
                    // Gummiband-Effekt: Wenn weiter als 120 Einheiten weg, zieht es sie stark zur Mitte
                    float pull = (cdist > 120.0f) ? 0.8f : 0.15f; 
                    
                    float ncx = (cdist > 0.0f) ? (cx / cdist) : 0.0f;
                    float ncz = (cdist > 0.0f) ? (cz / cdist) : 0.0f;
                    
                    float move_x = (rx * (1.0f - pull)) + (ncx * pull);
                    float move_z = (rz * (1.0f - pull)) + (ncz * pull);
                    
                    sc_enemy_x[i] += move_x * 0.15f;
                    sc_enemy_z[i] += move_z * 0.15f;
                    
                    // Sichere 4-Wege-Rotation beim Schlendern (verhindert Glitches)
                    float amx = move_x < 0 ? -move_x : move_x;
                    float amz = move_z < 0 ? -move_z : move_z;
                    if (amx > amz) { sc_enemy_rot[i] = move_x > 0 ? 90.0f : 270.0f; } 
                    else           { sc_enemy_rot[i] = move_z > 0 ? 0.0f : 180.0f; }
                }
                
                if (sc_enemy_attack_timer[i] > 0) {
                    sc_enemy_attack_timer[i]--;
                    if (sc_enemy_attack_timer[i] == 10 && min_dist < 12.0f) { 
                        if (target_id == -1 && sc_player_hp > 0) { sc_player_hp--; sc_player_hit_timer = 15; } 
                        else if (target_id != -1) { sc_enemy_hp[target_id]--; sc_enemy_hit_timer[target_id] = 10; }
                    }
                }
            } else if (sc_enemy_hp[i] == 0) {
                if (!drop_active) {
                    drop_active = 1; drop_x = sc_enemy_x[i]; drop_z = sc_enemy_z[i]; drop_type = 1 + (frame % 4);
                    if (drop_type == 1) { drop_value = 1; drop_stat = 1; } else if (drop_type == 2) { drop_value = 1 + (frame % 2); } else if (drop_type == 3) { drop_value = 1 + (frame % 2); } else { drop_value = 1 + (frame % 2); }
                }
                sc_enemy_hp[i] = -1; 
                
                // 20 bis 40 Sekunden Respawn (1200 + max 1200 Frames)
                sc_enemy_respawn_timer[i] = 1200 + (frame % 1200); 
                
            } else if (sc_enemy_hp[i] == -1) {
                if (sc_enemy_respawn_timer[i] > 0) {
                    sc_enemy_respawn_timer[i]--;
                    if (sc_enemy_respawn_timer[i] <= 0) {
                        sc_enemy_hp[i] = 3 + ((frame + i * 17) % 7); 
                        sc_enemy_x[i] = ((float)(((frame * 17) + (i * 123)) % 480)) - 240.0f; 
                        sc_enemy_z[i] = ((float)(((frame * 31) + (i * 321)) % 480)) - 240.0f;
                        enemy_color_idx[i] = frame % 8; 
                        enemy_pattern_idx[i] = (frame/2) % 3; enemy_armor_idx[i] = (frame/3) % 3; enemy_weapon_idx[i] = (frame/4) % 3; enemy_face_idx[i] = (frame/5) % 3;
                    }
                }
            }
        }
        
        // === ITEM AUFHEBEN & INS INVENTAR STAPELN ===
        if (drop_active) {
            float pdx = sc_cat_x - drop_x; float pdz = sc_cat_z - drop_z;
            if (pdx*pdx + pdz*pdz < 25.0f) {
                drop_active = 0;
                
                // 1. Schauen, ob wir es stapeln können
                int found_slot = -1;
                for(int s=0; s<20; s++) { 
                    if(sc_inv[s].type == drop_type && sc_inv[s].value == drop_value) { found_slot = s; break; } 
                }
                // 2. Wenn nicht stapelbar, ersten leeren Slot suchen
                if(found_slot == -1) { 
                    for(int s=0; s<20; s++) { if(sc_inv[s].count == 0) { found_slot = s; break; } } 
                }
                // 3. Item ins Inventar packen!
                if(found_slot != -1) {
                    sc_inv[found_slot].type = drop_type;
                    sc_inv[found_slot].value = drop_value;
                    sc_inv[found_slot].count++;
                }
                
                // Effekt/Ausrüstung wie bisher anwenden
                if (drop_type == 1 && sc_player_hp < 10) sc_player_hp += drop_value; 
                else if (drop_type == 2) player_armor_idx = drop_value;
                else if (drop_type == 3) player_weapon_idx = drop_value;
                else if (drop_type == 4) player_face_idx = drop_value;
                
                sc_player_hit_timer = 5;
                sc_msg_timer = 120; // NEU: Text bleibt 120 Frames (2 Sekunden) sichtbar!
            }
        }
    }
    
    if (sc_player_hit_timer > 0) sc_player_hit_timer--;
    
    // === TOD, 9 LIVES & GAME OVER RESET ===
    if (sc_state == 1 && sc_player_hp <= 0) { 
        if (input_cooldown == 0) {
            input_cooldown = 60; 
            
            sc_player_lives--;
            sc_slots[sc_current_slot].lives = sc_player_lives;
            
            // NEU: Rekord checken und speichern (bevor irgendwas resettet wird!)
            if (sc_slots[sc_current_slot].playtime > sc_slots[sc_current_slot].record_playtime) {
                sc_slots[sc_current_slot].record_playtime = sc_slots[sc_current_slot].playtime;
            }
            
            // NEU: Wir speichern die exakte Playtime als "Deathtime" ab!
            sc_slots[sc_current_slot].deathtime = sc_slots[sc_current_slot].playtime;

            for (int s = 0; s < 20; s++) {
                if (sc_inv[s].count > 0) {
                    int lose_chance = 15 + (frame % 41);
                    if ((int)((frame + s * 13) % 100) < lose_chance) { sc_inv[s].count = 0; sc_inv[s].type = 0; }
                }
            }
        }
        if (input_cooldown == 1) { 
            if (sc_player_lives <= 0) {
                // GAME OVER! SLOT WIRD GELÖSCHT!
                sc_slots[sc_current_slot].active = false;
                sc_slots[sc_current_slot].lives = 9;
                sc_slots[sc_current_slot].playtime = 0;
                for(int i=0; i<20; i++) sc_inv[i].count = 0; // Inventar weg
                sc_state = 0; // Zurück zum Startbildschirm
            } else {
                sc_state = 2; // Normaler Tod, darf respawnen
            }
            sc_show_inv = false; 
        } 
    }
    
    sc_last_mx = mouse_x; sc_last_my = mouse_y;
    float anim_t = (float)frame * 0.05f;

    // ==========================================
    // RENDERING (3D Szene + UI)
    // ==========================================
    if (sc_state == 0 || sc_state == 1 || sc_state == 3) {
        sglClear(sc_state == 1 ? 0x05080C : 0x0C1218);
        sglClearDepth();
        sglEnable(SGL_DEPTH_TEST);
        
        // HIER DIE WIESE SOFORT ZEICHNEN (Und nur exakt EINMAL!)
        DrawFluidMeadow(anim_t);
        sglLoadIdentity();
        
        // Kamera Setup
        if (sc_state == 0) {
            // FIX: Kamera ist jetzt FEST! Keine Endlos-Rotation (anim_t) mehr!
            // Das verhindert den Matrix-Overflow/Grafik-Bug im Spiel endgültig.
            sglTranslatef(0.0f, -6.0f, 35.0f); 
            sglRotatef(10.0f, 1.0f, 0.0f, 0.0f);
            sglRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Katze schaut nach vorne!
            sglScalef(1.8f, 1.8f, 1.8f);
        } else if (sc_state == 3) {
            // EDITOR-KAMERA (Nutzt jetzt die ed_cam_dist Variable!)
            sglTranslatef(0.0f, -6.0f, ed_cam_dist); 
            sglRotatef(ed_cam_rot_x, 1.0f, 0.0f, 0.0f);
            sglRotatef(ed_cam_rot_y, 0.0f, 1.0f, 0.0f); 
            sglScalef(1.8f, 1.8f, 1.8f);
        } else {
            // SPIEL-KAMERA
            sglTranslatef(0.0f, 0.0f, sc_cam_dist);
            sglRotatef(sc_cam_rot_x, 1.0f, 0.0f, 0.0f);
            sglRotatef(sc_cam_rot_y, 0.0f, 1.0f, 0.0f);
            sglTranslatef(-sc_cat_x, -12.0f, -sc_cat_z);
        }
        
        // === SPIELER RENDERN ===
        sglPushMatrix();
        if (sc_state == 1) { 
            sglTranslatef(sc_cat_x, 6.5f + sc_cat_y, sc_cat_z);
            sglRotatef(sc_cat_rot, 0.0f, 1.0f, 0.0f); 
            if (sc_cat_flip > 0.0f) { 
                sglTranslatef(0.0f, 3.0f, 0.0f); 
                sglRotatef(sc_cat_flip, -1.0f, 0.0f, 0.0f); 
                sglTranslatef(0.0f, -3.0f, 0.0f); 
            } 
        } else if (sc_state == 3) {
            sglTranslatef(0.0f, 6.5f, 0.0f); // Editor: Katze in der Mitte
        } else { 
            sglTranslatef(0.0f, 6.5f, 0.0f); 
        }
        
        sglEnable(SGL_LIGHTING); 
        uint8_t cr, cg, cb; 
        GetCatColor(player_color_idx, &cr, &cg, &cb);
        
        if (sc_player_hit_timer > 0) {
            Draw3DCat(0.7f, 255, 50, 50, anim_t * 2.0f, sc_walk_t, sc_attack_l_timer, sc_attack_r_timer, player_pattern_idx, player_armor_idx, player_face_idx, player_weapon_idx, &player_morph);
        } else {
            Draw3DCat(0.7f, cr, cg, cb, anim_t, sc_walk_t, sc_attack_l_timer, sc_attack_r_timer, player_pattern_idx, player_armor_idx, player_face_idx, player_weapon_idx, &player_morph);
        }
        
        sglDisable(SGL_LIGHTING); 
        sglPopMatrix();
        // === GEGNER RENDERN ===
        for (int i=0; i<5; i++) { // Auch hier max 5 Gegner!
            if (sc_state == 1 && (sc_enemy_hp[i] > 0 || sc_enemy_hit_timer[i] > 0)) {
                sglPushMatrix(); 
                sglTranslatef(sc_enemy_x[i], 6.5f, sc_enemy_z[i]); 
                sglRotatef(sc_enemy_rot[i], 0.0f, 1.0f, 0.0f);
                if (sc_enemy_attack_timer[i] > 0) { 
                    float elunge = app_sinf((float)sc_enemy_attack_timer[i] / 20.0f * 3.14159f) * 5.0f; 
                    sglTranslatef(0.0f, 0.0f, elunge); 
                }
                sglEnable(SGL_LIGHTING); 
                uint8_t ecr, ecg, ecb; 
                GetCatColor(enemy_color_idx[i], &ecr, &ecg, &ecb);
                float dx = sc_cat_x - sc_enemy_x[i]; 
                float dz = sc_cat_z - sc_enemy_z[i]; 
                float dist = (dx<0?-dx:dx) + (dz<0?-dz:dz);
                // NEU: Wenn weit weg -> Langsames Schlendern (1.5f). Wenn Aggro -> Sprint (4.0f)
                float enemy_walk_t = 0.0f;
                if (dist > 8.0f) {
                    if (dist < 60.0f) enemy_walk_t = anim_t * 4.0f; 
                    else enemy_walk_t = anim_t * 1.5f;              
                }
                if (sc_enemy_hit_timer[i] > 0) {
                    Draw3DCat(0.7f, 255, 50, 50, anim_t * 2.0f, enemy_walk_t, 0, sc_enemy_attack_timer[i], enemy_pattern_idx[i], enemy_armor_idx[i], enemy_face_idx[i], enemy_weapon_idx[i]);
                } else {
                    Draw3DCat(0.7f, ecr, ecg, ecb, anim_t * 0.8f, enemy_walk_t, 0, sc_enemy_attack_timer[i], enemy_pattern_idx[i], enemy_armor_idx[i], enemy_face_idx[i], enemy_weapon_idx[i]);
                }
                sglDisable(SGL_LIGHTING); 
                sglPopMatrix();
            }
        }
        // === ITEMS AUF DEM BODEN RENDERN ===
        if (drop_active && sc_state == 1) {
            sglPushMatrix(); 
            sglTranslatef(drop_x, 2.0f + app_sinf(anim_t*3.0f)*0.5f, drop_z); 
            sglRotatef(anim_t * 60.0f, 0.0f, 1.0f, 0.0f);
            // --- NEU: WEISSE UMRANDUNG (Leuchtender Rahmen) ---
            sglDisable(SGL_LIGHTING);
            sglColor3ub(255, 255, 255); // Strahlendes Weiß
            float w = 2.2f + app_sinf(anim_t * 8.0f) * 0.2f; // Pulsiert leicht
            float t = 0.15f; // Dicke der Rahmenlinie
            // Wir zeichnen 4 dünne weiße Stangen als Quadrat um das Item
            DrawBox(0.0f, 0.0f, w, w, t, t);   // Vorne
            DrawBox(0.0f, 0.0f, -w, w, t, t);  // Hinten
            DrawBox(w, 0.0f, 0.0f, t, t, w);   // Rechts
            DrawBox(-w, 0.0f, 0.0f, t, t, w);  // Links
            // --- EIGENTLICHES ITEM (IN FARBE) ---
            sglEnable(SGL_LIGHTING);
            if (drop_type == 1) { sglColor3ub(200, 40, 40); sglDrawSphere(1.5f, 16); }
            else if (drop_type == 2) { sglColor3ub(130, 135, 140); DrawBox(0.0f, 0.0f, 0.0f, 2.0f, 2.5f, 1.5f); sglColor3ub(80, 85, 90); DrawBox(0.0f, 0.0f, 1.0f, 1.0f, 1.5f, 0.5f); }
            else if (drop_type == 3) { sglColor3ub(200, 190, 50); DrawBox(0.0f, 1.0f, 0.0f, 0.5f, 3.0f, 1.0f); sglColor3ub(100, 50, 20); DrawBox(0.0f, -1.0f, 0.0f, 1.5f, 0.5f, 1.5f); }
            else { sglColor3ub(50, 50, 200); DrawBox(0.0f, 0.0f, 0.0f, 2.0f, 1.0f, 1.0f); sglColor3ub(0, 0, 0); DrawBox(1.0f, 0.0f, 0.6f, 0.6f, 0.4f, 0.2f); DrawBox(-1.0f, 0.0f, 0.6f, 0.6f, 0.4f, 0.2f); }
            sglDisable(SGL_LIGHTING); 
            sglPopMatrix();
        }
        // ==========================================
        // UI RENDERING
        // ==========================================
        if (sc_state == 0) {
            TextC(wx + ww/2, wy + 20, "CHARACTER CREATOR", 0xFFFFFF, _128);
            
            // === 3 CHARAKTER SLOTS ===
            for(int sl=0; sl<3; sl++) {
                uint32_t bg_col = (sc_current_slot == sl) ? 0x00FF00 : 0x555555;
                DrawRoundedRect(wx + ww/2 - 130 + sl*90, wy + 40, 80, 20, 2, bg_col);
                char sbuf[8] = "SLOT 1"; sbuf[5] = '1' + sl;
                TextC(wx + ww/2 - 90 + sl*90, wy + 45, sbuf, (sc_current_slot == sl) ? 0x000000 : 0xFFFFFF, _86);
                
                if(mouse_just_pressed && input_cooldown == 0 && is_over_rect(mouse_x, mouse_y, wx + ww/2 - 130 + sl*90, wy + 40, 80, 20)) {
                    sc_current_slot = sl; input_cooldown = 10;
                    if(sc_slots[sl].active) {
                        player_color_idx = sc_slots[sl].c; player_pattern_idx = sc_slots[sl].p;
                        player_armor_idx = sc_slots[sl].a; player_weapon_idx = sc_slots[sl].w;
                        player_face_idx = sc_slots[sl].f;
                    } else { player_armor_idx = 0; player_weapon_idx = 0; player_face_idx = 0; }
                }
            }
            
            // === MENÜ KNÖPFE (Links) ===
            int ypos = 80; 
            TextC(wx + 30, wy + ypos + 4, "COLOR", 0xAAAAAA, _86); DrawRoundedRect(wx + 110, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 116, wy + ypos+4, "<", 0xFFFFFF, _86); DrawRoundedRect(wx + 140, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 146, wy + ypos+4, ">", 0xFFFFFF, _86); ypos += 40;
            TextC(wx + 30, wy + ypos + 4, "PATTERN", 0xAAAAAA, _86); DrawRoundedRect(wx + 110, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 116, wy + ypos+4, "<", 0xFFFFFF, _86); DrawRoundedRect(wx + 140, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 146, wy + ypos+4, ">", 0xFFFFFF, _86); ypos += 40;
            TextC(wx + 30, wy + ypos + 4, "ARMOR", 0xAAAAAA, _86); DrawRoundedRect(wx + 110, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 116, wy + ypos+4, "<", 0xFFFFFF, _86); DrawRoundedRect(wx + 140, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 146, wy + ypos+4, ">", 0xFFFFFF, _86); ypos += 40;
            TextC(wx + 30, wy + ypos + 4, "WEAPON", 0xAAAAAA, _86); DrawRoundedRect(wx + 110, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 116, wy + ypos+4, "<", 0xFFFFFF, _86); DrawRoundedRect(wx + 140, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 146, wy + ypos+4, ">", 0xFFFFFF, _86); ypos += 40;
            TextC(wx + 30, wy + ypos + 4, "FACE", 0xAAAAAA, _86); DrawRoundedRect(wx + 110, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 116, wy + ypos+4, "<", 0xFFFFFF, _86); DrawRoundedRect(wx + 140, wy + ypos, 20, 20, 2, 0x555555); TextC(wx + 146, wy + ypos+4, ">", 0xFFFFFF, _86);
            
            if (input_cooldown == 0 && mouse_just_pressed && !is_blocked) {
                int yp = 80; 
                if (is_over_rect(mouse_x, mouse_y, wx + 110, wy + yp, 20, 20)) { player_color_idx = (player_color_idx + 7) % 8; input_cooldown = 10; }
                if (is_over_rect(mouse_x, mouse_y, wx + 140, wy + yp, 20, 20)) { player_color_idx = (player_color_idx + 1) % 8; input_cooldown = 10; } yp += 40;
                if (is_over_rect(mouse_x, mouse_y, wx + 110, wy + yp, 20, 20)) { player_pattern_idx = (player_pattern_idx + 2) % 3; input_cooldown = 10; }
                if (is_over_rect(mouse_x, mouse_y, wx + 140, wy + yp, 20, 20)) { player_pattern_idx = (player_pattern_idx + 1) % 3; input_cooldown = 10; } yp += 40;
                if (is_over_rect(mouse_x, mouse_y, wx + 110, wy + yp, 20, 20)) { player_armor_idx = (player_armor_idx + 2) % 3; input_cooldown = 10; }
                if (is_over_rect(mouse_x, mouse_y, wx + 140, wy + yp, 20, 20)) { player_armor_idx = (player_armor_idx + 1) % 3; input_cooldown = 10; } yp += 40;
                if (is_over_rect(mouse_x, mouse_y, wx + 110, wy + yp, 20, 20)) { player_weapon_idx = (player_weapon_idx + 2) % 3; input_cooldown = 10; }
                if (is_over_rect(mouse_x, mouse_y, wx + 140, wy + yp, 20, 20)) { player_weapon_idx = (player_weapon_idx + 1) % 3; input_cooldown = 10; } yp += 40;
                if (is_over_rect(mouse_x, mouse_y, wx + 110, wy + yp, 20, 20)) { player_face_idx = (player_face_idx + 2) % 3; input_cooldown = 10; }
                if (is_over_rect(mouse_x, mouse_y, wx + 140, wy + yp, 20, 20)) { player_face_idx = (player_face_idx + 1) % 3; input_cooldown = 10; }
            }
            
            // === STATS PANEL (Rechts) ===
            int st_w = 180;
            int st_x = wx + ww - st_w - 30;
            int st_y = wy + 80;
            
            DrawGlassRect(st_x, st_y, st_w, 140, 4, 0x11111A);
            TextC(st_x + st_w/2, st_y + 10, "CHARACTER STATS", 0xAAAAAA, _86);
            
            char lbuf[16] = "LIVES: 9"; 
            int draw_lives = sc_slots[sc_current_slot].active ? sc_slots[sc_current_slot].lives : 9;
            lbuf[7] = '0' + draw_lives;
            TextC(st_x + st_w/2, st_y + 40, lbuf, 0xFF00FF, _86); 
            
            // 2. ENERGY (Dynamisch berechnet!)
            char ebuf[16] = "ENERGY: 10"; 
            int draw_energy = sc_slots[sc_current_slot].active ? sc_player_hp : 10;
            if (draw_energy < 0) draw_energy = 0;
            if (draw_energy > 10) draw_energy = 10;
            
            if (draw_energy < 10) { 
                ebuf[8] = '0' + draw_energy; ebuf[9] = 0; 
            } else { 
                ebuf[8] = '1'; ebuf[9] = '0'; ebuf[10] = 0; 
            }
            TextC(st_x + st_w/2, st_y + 65, ebuf, 0x00FF00, _86); 
            
            uint32_t pt = sc_slots[sc_current_slot].playtime / 60;
            int p_m = pt / 60; int p_s = pt % 60;
            char pbuf[32] = "TIME: 00:00";
            pbuf[6] = '0'+(p_m/10); pbuf[7] = '0'+(p_m%10); 
            pbuf[9] = '0'+(p_s/10); pbuf[10] = '0'+(p_s%10);
            TextC(st_x + st_w/2, st_y + 90, pbuf, 0x00FFFF, _86); 
            
            uint32_t rt = sc_slots[sc_current_slot].record_playtime / 60;
            int r_m = rt / 60; int r_s = rt % 60;
            char rbuf[32] = "RECORD: 00:00";
            rbuf[8] = '0'+(r_m/10); rbuf[9] = '0'+(r_m%10); 
            rbuf[11] = '0'+(r_s/10); rbuf[12] = '0'+(r_s%10);
            TextC(st_x + st_w/2, st_y + 115, rbuf, 0xFFD700, _86); 
            
            // === BUTTONS ===
            DrawRoundedRect(wx + ww/2 - 50, wy + wh - 60, 100, 30, 5, 0xAA0000); 
            TextC(wx + ww/2, wy + wh - 52, "FIGHT!", 0xFFFFFF, _128);
            
            DrawRoundedRect(wx + ww/2 - 50, wy + wh - 100, 100, 28, 4, 0x006666);
            TextC(wx + ww/2, wy + wh - 90, "EDIT [E]", 0x00FFCC, _86);
            
            if (input_cooldown == 0 && mouse_just_pressed && !is_blocked) {
                // Klick auf "EDIT"
                if (is_over_rect(mouse_x, mouse_y, wx + ww/2 - 50, wy + wh - 100, 100, 28)) {
                    sc_state = 3; ed_cam_rot_y = 180.0f; ed_cam_rot_x = 15.0f; ed_cam_dist = 35.0f; ed_selected_part = 0; input_cooldown = 10;
                }
                // Klick auf "FIGHT"
                else if (is_over_rect(mouse_x, mouse_y, wx + ww/2 - 50, wy + wh - 60, 100, 30)) {
                    sc_slots[sc_current_slot].c = player_color_idx; sc_slots[sc_current_slot].p = player_pattern_idx;
                    sc_slots[sc_current_slot].a = player_armor_idx; sc_slots[sc_current_slot].w = player_weapon_idx;
                    sc_slots[sc_current_slot].f = player_face_idx; sc_slots[sc_current_slot].active = true;
                    
                    sc_player_lives = sc_slots[sc_current_slot].lives;
                    if(sc_player_lives <= 0) { 
                        sc_player_lives = 9; 
                        sc_slots[sc_current_slot].playtime = 0; 
                        sc_slots[sc_current_slot].deathtime = 0; 
                        sc_player_hp = 0; // Zwingt das Spiel zum echten Neustart!
                    }
                    sc_slots[sc_current_slot].lives = sc_player_lives;
                    
                    // === PAUSE ODER NEUSTART CHECK ===
                    if (sc_player_hp <= 0 || sc_slots[sc_current_slot].playtime == 0) {
                        sc_cam_dist = -30.0f; 
                        sc_cam_rot_x = 30.0f;
                        for (int i=0; i<5; i++) { 
                            enemy_color_idx[i] = (frame + i) % 8; enemy_pattern_idx[i] = (frame/2 + i) % 3; enemy_armor_idx[i] = (frame/3 + i) % 3; enemy_weapon_idx[i] = (frame/4 + i) % 3; enemy_face_idx[i] = (frame/5 + i) % 3; 
                            sc_enemy_x[i] = app_cosf((float)i * 1.2f) * 30.0f; sc_enemy_z[i] = app_sinf((float)i * 1.2f) * 30.0f;
                            sc_enemy_hp[i] = 3 + ((frame + i * 17) % 7); 
                            sc_enemy_attack_timer[i] = 0; sc_enemy_hit_timer[i] = 0; 
                        }
                        sc_cat_x = 0; 
                        sc_cat_z = 0; 
                        sc_player_hp = 10; 
                        drop_active = 0; 
                    }
                    
                    sc_state = 1; // Spiel starten / fortsetzen
                    input_cooldown = 20;
                }
            }
        } 
        // ==========================================
        // INGAME HUD & INVENTAR (sc_state == 1)
        // ==========================================
        else if (sc_state == 1) {
            if (!is_blocked) sc_slots[sc_current_slot].playtime++; 
            int hud_y = wy + 25; 
            
            DrawGlassRect(wx + 15, hud_y - 4, ww - 30, 20, 2, 0x111111);
            
            // 1. LIVES
            char lbuf[16] = "LIVES: 9"; lbuf[7] = '0' + sc_player_lives;
            TextC(wx + 55, hud_y, lbuf, 0xFF00FF, _86); 
            
            // 2. ENERGY
            for(int i=0; i<10; i++) { 
                uint32_t cP = (i < sc_player_hp) ? 0x00FF00 : 0x550000; 
                DrawRoundedRect(wx + 95 + i*16, hud_y - 2, 12, 10, 2, cP); 
            }
            
            // 3. INGAME-UHRZEIT
            int total_s = 20 + (sc_slots[sc_current_slot].playtime / 60);
            int cur_s = total_s % 60; int total_m = 38 + (total_s / 60); int cur_m = total_m % 60; int total_h = 22 + (total_m / 60); int cur_h = total_h % 24;
            char tbuf[32] = "06.17.2026 - 00:00:00"; 
            tbuf[13] = '0'+(cur_h/10); tbuf[14] = '0'+(cur_h%10); tbuf[16] = '0'+(cur_m/10); tbuf[17] = '0'+(cur_m%10); tbuf[19] = '0'+(cur_s/10); tbuf[20] = '0'+(cur_s%10);
            TextC(wx + ww/2 - 20, hud_y, tbuf, 0x00FF00, _86); 
            
            // === 4. FPS IN WEISS (Exakt zwischen Uhrzeit und Playtime) ===
            char fbuf[16] = "FPS: 60";
            if (sc_display_fps >= 100) {
                fbuf[5] = '0' + (sc_display_fps / 100) % 10;
                fbuf[6] = '0' + (sc_display_fps / 10) % 10;
                fbuf[7] = '0' + sc_display_fps % 10;
                fbuf[8] = 0;
            } else {
                fbuf[5] = '0' + (sc_display_fps / 10) % 10;
                fbuf[6] = '0' + sc_display_fps % 10;
                fbuf[7] = 0;
            }
            TextC(wx + ww/2 + 130, hud_y, fbuf, 0xFFFFFF, _86); 
            
            // 5. PLAYTIME & DEATHTIME
            uint32_t pt = sc_slots[sc_current_slot].playtime / 60;
            int p_m = pt / 60; int p_s = pt % 60;
            uint32_t dt = sc_slots[sc_current_slot].deathtime / 60;
            int d_m = dt / 60; int d_s = dt % 60;
            char pbuf[64] = "PLAY: 00:00 (00:00)";
            pbuf[6] = '0'+(p_m/10); pbuf[7] = '0'+(p_m%10); 
            pbuf[9] = '0'+(p_s/10); pbuf[10] = '0'+(p_s%10);
            pbuf[13] = '0'+(d_m/10); pbuf[14] = '0'+(d_m%10); 
            pbuf[16] = '0'+(d_s/10); pbuf[17] = '0'+(d_s%10);
            TextC(wx + ww - 100, hud_y, pbuf, 0x00FFFF, _86); 

            // === INVENTAR HOTBAR ===
            int slot_w = 38; int slot_space = 4; int bar_w = 5 * (slot_w + slot_space);
            int start_x = wx + ww/2 - bar_w/2; int start_y = wy + wh - 45;
            const char* hovered_item_name = 0;
            for(int s=0; s<5; s++) {
                int px = start_x + s*(slot_w+slot_space); int py = start_y;
                bool is_hovered = is_over_rect(mouse_x, mouse_y, px, py, slot_w, slot_w);
                DrawRoundedRect(px, py, slot_w, slot_w, 3, is_hovered ? 0x666666 : 0x333333);
                
                if(sc_inv[s].count > 0) {
                    if (is_hovered) {
                        hovered_item_name = (sc_inv[s].type == 1) ? "- CATNIP (CLICK TO HEAL)" : "- EQUIP/UNEQUIP (CLICK X TO DROP)";
                        bool over_del = is_over_rect(mouse_x, mouse_y, px + slot_w - 12, py - 4, 14, 14);
                        DrawRoundedRect(px + slot_w - 12, py - 4, 14, 14, 2, over_del ? 0xFF0000 : 0xAA0000);
                        TextC(px + slot_w - 5, py - 1, "X", 0xFFFFFF, _86);
                        
                        if (mouse_just_pressed && input_cooldown == 0) {
                            if (over_del) { 
                                sc_inv[s].count = 0; sc_inv[s].type = 0; 
                            } else {
                                if (sc_inv[s].type == 1 && sc_player_hp < 10) { sc_player_hp += sc_inv[s].value; if (sc_player_hp > 10) sc_player_hp = 10; sc_inv[s].count--; if (sc_inv[s].count == 0) sc_inv[s].type = 0; sc_msg_timer = 60; } 
                                else if (sc_inv[s].type == 2) { player_armor_idx = (player_armor_idx == sc_inv[s].value) ? 0 : sc_inv[s].value; }
                                else if (sc_inv[s].type == 3) { player_weapon_idx = (player_weapon_idx == sc_inv[s].value) ? 0 : sc_inv[s].value; } 
                                else if (sc_inv[s].type == 4) { player_face_idx = (player_face_idx == sc_inv[s].value) ? 0 : sc_inv[s].value; }
                            }
                            input_cooldown = 10;
                        }
                    }
                    int ix = px + 5; int iy = py + 5; int iw = slot_w - 10;
                    if(sc_inv[s].type == 1) { DrawRoundedRect(ix+iw/2-2, iy, 4, iw, 1, 0xCC3333); DrawRoundedRect(ix, iy+iw/2-2, iw, 4, 1, 0xCC3333); }
                    else if(sc_inv[s].type == 2) { DrawRoundedRect(ix, iy, iw, iw, 3, 0x888888); DrawRoundedRect(ix+4, iy+4, iw-8, iw-8, 1, 0x555555); }
                    else if(sc_inv[s].type == 3) { DrawRoundedRect(ix+iw/2-2, iy, 4, iw, 1, 0xCCCC33); DrawRoundedRect(ix+iw/2-6, iy+iw-8, 12, 3, 1, 0x888822); }
                    else if(sc_inv[s].type == 4) { DrawRoundedRect(ix, iy+iw/2-4, iw, 8, 2, 0x3333CC); DrawRoundedRect(ix+4, iy+iw/2-2, 6, 4, 1, 0x000000); DrawRoundedRect(ix+iw-10, iy+iw/2-2, 6, 4, 1, 0x000000); }
                    if(sc_inv[s].count > 1) { char cbuf[16]; num_to_str(sc_inv[s].count, cbuf); TextC(ix + 15, iy + 15, cbuf, 0xFFFFFF, _86); }
                }
            }
            
            // === INVENTAR RUCKSACK ===
            static int inv_wx = -1; static int inv_wy = -1; static bool is_dragging = false;
            if (inv_wx == -1) { inv_wx = wx + ww/2 - 110; inv_wy = wy + wh/2 - 100; }
            if (sc_show_inv) {
                if (mouse_just_pressed) {
                    if (!is_dragging && is_over_rect(mouse_x, mouse_y, inv_wx, inv_wy, 220, 30)) { is_dragging = true; } 
                    else if (is_dragging) { is_dragging = false; }
                }
                if (is_dragging) { inv_wx = mouse_x - 110; inv_wy = mouse_y - 15; }
                DrawGlassRect(inv_wx, inv_wy, 220, 185, 5, 0x1A1A1A);
                TextC(inv_wx + 110, inv_wy + 10, "INVENTORY (DRAG TO MOVE)", 0xAAAAAA, _86);
                
                int inv_sx = inv_wx + 15; int inv_sy = inv_wy + 35;
                for(int s=0; s<20; s++) {
                    int col_idx = s % 5; int row_idx = s / 5;
                    int px = inv_sx + col_idx*(slot_w+slot_space); int py = inv_sy + row_idx*(slot_w+slot_space);
                    bool is_hovered = is_over_rect(mouse_x, mouse_y, px, py, slot_w, slot_w);
                    DrawRoundedRect(px, py, slot_w, slot_w, 3, is_hovered ? 0x666666 : 0x2A2A2A);
                    
                    if(sc_inv[s].count > 0) {
                        if(is_hovered) {
                            hovered_item_name = (sc_inv[s].type == 1) ? "- CATNIP (CLICK TO HEAL)" : "- EQUIP/UNEQUIP (CLICK X TO DROP)";
                            bool over_del = is_over_rect(mouse_x, mouse_y, px + slot_w - 12, py - 4, 14, 14);
                            DrawRoundedRect(px + slot_w - 12, py - 4, 14, 14, 2, over_del ? 0xFF0000 : 0xAA0000);
                            TextC(px + slot_w - 5, py - 1, "X", 0xFFFFFF, _86);
                            
                            if (mouse_just_pressed && input_cooldown == 0) {
                                if (over_del) { 
                                    sc_inv[s].count = 0; sc_inv[s].type = 0; 
                                } else {
                                    if (sc_inv[s].type == 1 && sc_player_hp < 10) { sc_player_hp += sc_inv[s].value; if (sc_player_hp > 10) sc_player_hp = 10; sc_inv[s].count--; if (sc_inv[s].count == 0) sc_inv[s].type = 0; sc_msg_timer = 60; } 
                                    else if (sc_inv[s].type == 2) { player_armor_idx = (player_armor_idx == sc_inv[s].value) ? 0 : sc_inv[s].value; }
                                    else if (sc_inv[s].type == 3) { player_weapon_idx = (player_weapon_idx == sc_inv[s].value) ? 0 : sc_inv[s].value; } 
                                    else if (sc_inv[s].type == 4) { player_face_idx = (player_face_idx == sc_inv[s].value) ? 0 : sc_inv[s].value; }
                                }
                                input_cooldown = 10;
                            }
                        }
                        int ix = px + 5; int iy = py + 5; int iw = slot_w - 10;
                        if(sc_inv[s].type == 1) { DrawRoundedRect(ix+iw/2-2, iy, 4, iw, 1, 0xCC3333); DrawRoundedRect(ix, iy+iw/2-2, iw, 4, 1, 0xCC3333); }
                        else if(sc_inv[s].type == 2) { DrawRoundedRect(ix, iy, iw, iw, 3, 0x888888); DrawRoundedRect(ix+4, iy+4, iw-8, iw-8, 1, 0x555555); }
                        else if(sc_inv[s].type == 3) { DrawRoundedRect(ix+iw/2-2, iy, 4, iw, 1, 0xCCCC33); DrawRoundedRect(ix+iw/2-6, iy+iw-8, 12, 3, 1, 0x888822); }
                        else if(sc_inv[s].type == 4) { DrawRoundedRect(ix, iy+iw/2-4, iw, 8, 2, 0x3333CC); DrawRoundedRect(ix+4, iy+iw/2-2, 6, 4, 1, 0x000000); DrawRoundedRect(ix+iw-10, iy+iw/2-2, 6, 4, 1, 0x000000); }
                        if(sc_inv[s].count > 1) { char cbuf[16]; num_to_str(sc_inv[s].count, cbuf); TextC(ix + 15, iy + 15, cbuf, 0xFFFFFF, _86); }
                    }
                }
                if (hovered_item_name != 0) { TextC(inv_wx + 110, inv_wy - 15, hovered_item_name, 0x3355CC, _86); }
            }
            if (sc_msg_timer > 0) {
                TextC(wx + ww/2 - 70, wy + 80, "ITEM COLLECTED!", 0xFFFF00, _128);
            }
        } 
        // ==========================================
        // SURREAL GRAFIK EDITOR (sc_state == 3)
        // ==========================================
        else if (sc_state == 3) {
            // === HINTERGRUND PANEL (Links) ===
            int panel_x = wx + 10;
            int panel_y = wy + 40;
            int panel_w = 140;
            DrawGlassRect(panel_x, panel_y, panel_w, 310, 4, 0x111122);
            
            TextC(panel_x + panel_w/2, panel_y + 5, "SURREAL EDITOR", 0x00FFCC, _128);
            TextC(panel_x + panel_w/2, panel_y + 22, "BODY PARTS", 0x888888, _86);
            
            const char* part_names[] = {
                "HEAD", "EARS", "BODY", "ARMS", "LEGS", 
                "TAIL", "EYES", "NOSE", "ITEMS"
            };
            int part_count = 9;
            
            for (int p = 0; p < part_count; p++) {
                int by = panel_y + 38 + p * 28;
                bool is_sel = (ed_selected_part == p);
                bool is_hov = is_over_rect(mouse_x, mouse_y, panel_x + 5, by, panel_w - 10, 24);
                
                uint32_t btn_col = is_sel ? 0x00AA88 : (is_hov ? 0x335566 : 0x222233);
                DrawRoundedRect(panel_x + 5, by, panel_w - 10, 24, 3, btn_col);
                TextC(panel_x + panel_w/2, by + 6, part_names[p], is_sel ? 0x000000 : 0xCCCCCC, _86);
                
                if (mouse_just_pressed && is_hov && input_cooldown == 0) {
                    ed_selected_part = p; input_cooldown = 8;
                }
            }
            
            // === WIREFRAME TOGGLE ===
            int wf_y = panel_y + 38 + part_count * 28 + 5;
            bool wf_hov = is_over_rect(mouse_x, mouse_y, panel_x + 5, wf_y, panel_w - 10, 20);
            DrawRoundedRect(panel_x + 5, wf_y, panel_w - 10, 20, 3, ed_wireframe ? 0x007755 : 0x333333);
            TextC(panel_x + panel_w/2, wf_y + 4, ed_wireframe ? "WIREFRAME: ON" : "WIREFRAME: OFF", 0xFFFFFF, _86);
            if (mouse_just_pressed && wf_hov && input_cooldown == 0) {
                ed_wireframe = !ed_wireframe; input_cooldown = 8;
            }
            
            // === SLIDER PANEL (Rechts) ===
            int sl_panel_x = wx + ww - 200;
            int sl_panel_y = wy + 40;
            int sl_panel_w = 185;
            DrawGlassRect(sl_panel_x, sl_panel_y, sl_panel_w, 280, 4, 0x111122);
            
            struct SliderDef { const char* name; float* value; };
            SliderDef sliders[4];
            int slider_count = 0;
            CatMorph* pm = &player_morph;
            
            if (ed_selected_part == 0) { sliders[0] = {"SIZE", &pm->head_scale}; slider_count = 1; } 
            else if (ed_selected_part == 1) { sliders[0] = {"HEIGHT", &pm->ear_height}; sliders[1] = {"WIDTH", &pm->ear_width}; sliders[2] = {"DISTANCE", &pm->ear_spread}; slider_count = 3; } 
            else if (ed_selected_part == 2) { sliders[0] = {"WIDTH", &pm->body_width}; sliders[1] = {"HEIGHT", &pm->body_height}; sliders[2] = {"DEPTH", &pm->body_depth}; slider_count = 3; } 
            else if (ed_selected_part == 3) { sliders[0] = {"LENGTH", &pm->arm_length}; sliders[1] = {"THICKNESS", &pm->arm_thickness}; slider_count = 2; } 
            else if (ed_selected_part == 4) { sliders[0] = {"LENGTH", &pm->leg_length}; sliders[1] = {"THICKNESS", &pm->leg_thickness}; slider_count = 2; } 
            else if (ed_selected_part == 5) { sliders[0] = {"LENGTH", &pm->tail_length}; sliders[1] = {"KRUEMMUNG", &pm->tail_curl}; sliders[2] = {"THICKNESS", &pm->tail_thickness}; slider_count = 3; } 
            else if (ed_selected_part == 6) { sliders[0] = {"SIZE", &pm->eye_size}; sliders[1] = {"DISTANCE", &pm->eye_distance}; sliders[2] = {"PUPIL", &pm->pupil_size}; slider_count = 3; } 
            else if (ed_selected_part == 7) { sliders[0] = {"SIZE", &pm->nose_size}; slider_count = 1; } 
            
            TextC(sl_panel_x + sl_panel_w/2, sl_panel_y + 5, part_names[ed_selected_part], 0x00FFCC, _128);
            
            float slider_min = 0.2f; float slider_max = 3.0f; int sl_w = 140;
            for (int s = 0; s < slider_count; s++) {
                int sy = sl_panel_y + 35 + s * 55;
                TextC(sl_panel_x + sl_panel_w/2, sy, sliders[s].name, 0xAAAACC, _86);
                int rail_x = sl_panel_x + 22; int rail_y = sy + 18;
                DrawRoundedRect(rail_x, rail_y, sl_w, 8, 4, 0x333344);
                
                float val = *(sliders[s].value);
                float norm = (val - slider_min) / (slider_max - slider_min);
                if (norm < 0.0f) norm = 0.0f; if (norm > 1.0f) norm = 1.0f;
                int knob_x = rail_x + (int)(norm * (float)(sl_w - 12));
                
                bool knob_hov = is_over_rect(mouse_x, mouse_y, knob_x - 2, rail_y - 4, 16, 16);
                DrawRoundedRect(knob_x, rail_y - 3, 12, 14, 3, knob_hov ? 0x00FFAA : 0x00CC88);
                
                if (mouse_just_pressed && is_over_rect(mouse_x, mouse_y, rail_x, rail_y - 6, sl_w, 20)) { ed_selected_slider = s; }
                if (ed_selected_slider == s && mouse_right_down == false) {
                    float rel = (float)(mouse_x - rail_x) / (float)sl_w;
                    if (rel < 0.0f) rel = 0.0f; if (rel > 1.0f) rel = 1.0f;
                    *(sliders[s].value) = slider_min + rel * (slider_max - slider_min);
                }
                
                int val_int = (int)(val * 100.0f);
                char vbuf[16]; vbuf[0] = '0' + (val_int / 100) % 10; vbuf[1] = '.'; vbuf[2] = '0' + (val_int / 10) % 10; vbuf[3] = '0' + val_int % 10; vbuf[4] = 0;
                TextC(sl_panel_x + sl_panel_w - 25, sy, vbuf, 0xFFFFFF, _86);
            }
            
            if (mouse_just_pressed && ed_selected_slider >= 0) {
                bool hit_any = false;
                for (int s = 0; s < slider_count; s++) {
                    int sy = sl_panel_y + 35 + s * 55; int rail_y = sy + 18; int rail_x = sl_panel_x + 22;
                    if (is_over_rect(mouse_x, mouse_y, rail_x, rail_y - 6, sl_w, 20)) { ed_selected_slider = s; hit_any = true; }
                }
                if (!hit_any) ed_selected_slider = -1;
            }
            
            if (slider_count == 0 && ed_selected_part == 8) {
                TextC(sl_panel_x + sl_panel_w/2, sl_panel_y + 50, "ITEMS BECOME", 0x888888, _86);
                TextC(sl_panel_x + sl_panel_w/2, sl_panel_y + 68, "AT CREATOR", 0x888888, _86);
                TextC(sl_panel_x + sl_panel_w/2, sl_panel_y + 86, "ADJUSTED", 0x888888, _86);
            }
            
            // === BOTTOM BUTTONS ===
            int btn_y = wy + wh - 50; int btn_w = 100; int btn_h = 28;
            
            int std_x = wx + ww/2 - 160;
            bool std_hov = is_over_rect(mouse_x, mouse_y, std_x, btn_y, btn_w, btn_h);
            DrawRoundedRect(std_x, btn_y, btn_w, btn_h, 4, std_hov ? 0xCC8800 : 0x884400); TextC(std_x + btn_w/2, btn_y + 8, "STANDARD", 0xFFFFFF, _86);
            if (mouse_just_pressed && std_hov && input_cooldown == 0) { morph_reset(&player_morph); input_cooldown = 10; }
            
            int sav_x = wx + ww/2 - 50;
            bool sav_hov = is_over_rect(mouse_x, mouse_y, sav_x, btn_y, btn_w, btn_h);
            DrawRoundedRect(sav_x, btn_y, btn_w, btn_h, 4, sav_hov ? 0x00AA55 : 0x006633); TextC(sav_x + btn_w/2, btn_y + 8, "SAVE", 0xFFFFFF, _86);
            if (mouse_just_pressed && sav_hov && input_cooldown == 0) { slot_morphs[sc_current_slot] = player_morph; input_cooldown = 10; }
            
            int bck_x = wx + ww/2 + 60;
            bool bck_hov = is_over_rect(mouse_x, mouse_y, bck_x, btn_y, btn_w, btn_h);
            DrawRoundedRect(bck_x, btn_y, btn_w, btn_h, 4, bck_hov ? 0xAA0000 : 0x660000); TextC(bck_x + btn_w/2, btn_y + 8, "BACK", 0xFFFFFF, _86);
            if (mouse_just_pressed && bck_hov && input_cooldown == 0) { sc_state = 0; input_cooldown = 10; }
            
            // === WIREFRAME OVERLAY ===
            if (ed_wireframe) {
                sglPushMatrix(); sglTranslatef(0.0f, 6.5f, 0.0f); sglEnable(SGL_DEPTH_TEST); sglDisable(SGL_LIGHTING);
                uint8_t wr, wg, wb; GetCatColor(player_color_idx, &wr, &wg, &wb);
                sglBegin(4); // SGL_POINTS
                float bw_m = player_morph.body_width; float bh_m = player_morph.body_height; float bd_m = player_morph.body_depth;
                for (int lat = 0; lat <= 16; lat++) {
                    float la = 3.14159f * (-0.5f + (float)lat / 16.0f); float z_n = app_sinf(la); float r = app_cosf(la);
                    for (int lon = 0; lon < 24; lon++) {
                        float lo = 6.28318f * (float)lon / 24.0f; float x_n = app_cosf(lo) * r; float y_n = app_sinf(lo) * r;
                        sglColor3ub(0, 255, 200); sglVertex3f(x_n * 2.0f * bw_m, y_n * 3.0f * bh_m, z_n * 1.75f * bd_m);
                    }
                }
                float hs_m = player_morph.head_scale;
                for (int lat = 0; lat <= 12; lat++) {
                    float la = 3.14159f * (-0.5f + (float)lat / 12.0f); float z_n = app_sinf(la); float r = app_cosf(la);
                    for (int lon = 0; lon < 18; lon++) {
                        float lo = 6.28318f * (float)lon / 18.0f; float x_n = app_cosf(lo) * r; float y_n = app_sinf(lo) * r;
                        sglColor3ub(0, 200, 255); sglVertex3f(x_n * 2.75f * hs_m, 4.5f * bh_m + y_n * 2.75f * hs_m, z_n * 2.5f * hs_m);
                    }
                }
                sglEnd(); sglEnable(SGL_LIGHTING); sglPopMatrix();
            }
            TextC(wx + ww/2, wy + 25, "RIGHT-CLICK: ROTATE  | E/ESC: BACK", 0x666688, _86);
        }
    } else if (sc_state == 2) {
        sglClear(0x110000); sglClearDepth();
        if (sc_player_hp <= 0) TextC(wx + ww/2 - 60, wy + wh/2 - 30, "YOU DIED!", 0xFF5555, _128); else TextC(wx + ww/2 - 50, wy + wh/2 - 30, "YOU WIN!", 0x55FF55, _128);
        DrawRoundedRect(wx + ww/2 - 80, wy + wh/2 + 20, 160, 30, 5, 0xAA0000); TextC(wx + ww/2 - 65, wy + wh/2 + 28, "ENTER TO RESPAWN", 0xFFFFFF, _128);
        if (input_cooldown == 0 && mouse_just_pressed && !is_blocked && is_over_rect(mouse_x, mouse_y, wx + ww/2 - 80, wy + wh/2 + 20, 160, 30)) { sc_state = 0; input_cooldown = 20; }
    }
}