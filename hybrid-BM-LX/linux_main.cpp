#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <thread>
extern int mouse_wheel;
// Globals used by app.cpp and kernel_linux.cpp
extern uint32_t input_cooldown;
extern bool mouse_just_pressed;
extern uint32_t mouse_x;
extern uint32_t mouse_y;
extern bool mouse_right_down;
extern uint8_t key_scancode;
extern uint32_t frame;
// --- BARE METAL 3D FIX: VARIABLEN BEKANNT MACHEN ---
extern volatile int os_app_x;
extern volatile int os_app_y;
extern volatile int os_app_w;
extern volatile int os_app_h;
extern "C" void run_smash_cats_engine(int wx, int wy, int ww, int wh, bool is_blocked);

extern "C" {
    // Provided by kernel_main_linux.cpp
    extern volatile bool app_window_active;
    extern uint32_t app_mouse_x, app_mouse_y;
    extern int meow_timer;
    extern uint32_t* fb;
    
    extern uint32_t linux_mx, linux_my;
    extern bool linux_mdown;
    extern uint8_t linux_scancode;
    
    extern volatile uint64_t system_ticks;
    extern volatile uint64_t os2_system_ticks;
    
    uint8_t linux_wav_buffer[10 * 1024 * 1024];
    uint32_t linux_wav_len = 0;
    uint32_t linux_wav_pos = 0;
    
    void play_hda_wav(uint64_t pcm_addr, uint32_t size_bytes, uint16_t sample_rate, uint16_t channels, uint16_t bits) {
        if (size_bytes > sizeof(linux_wav_buffer)) size_bytes = sizeof(linux_wav_buffer);
        // Copy if not already using the buffer directly
        if ((uint64_t)linux_wav_buffer != pcm_addr) {
            memcpy(linux_wav_buffer, (void*)pcm_addr, size_bytes);
        }
        linux_wav_len = size_bytes;
        linux_wav_pos = 0;
    }
    
    // Global buffers for kernel_main_linux.cpp to avoid segfaults
    uint8_t global_buf_mbr[4096];
    uint8_t global_buf_dir[4096];
    uint8_t global_tmp_dir[4096];
}

// Exported functions
extern "C" void cosmos_main(void* boot_info);
extern void run_smash_cats_engine(int wx, int wy, int ww, int wh, bool is_blocked);

// SDL keycode to PS/2 scancode conversion
static uint8_t sdl_to_ps2(SDL_Keycode key) {
    switch (key) {
        case SDLK_ESCAPE: return 0x01;
        case SDLK_1: return 0x02; case SDLK_2: return 0x03; case SDLK_3: return 0x04;
        case SDLK_4: return 0x05; case SDLK_5: return 0x06; case SDLK_6: return 0x07;
        case SDLK_7: return 0x08; case SDLK_8: return 0x09; case SDLK_9: return 0x0A;
        case SDLK_0: return 0x0B;
        case SDLK_MINUS: return 0x0C; case SDLK_EQUALS: return 0x0D;
        case SDLK_BACKSPACE: return 0x0E; case SDLK_TAB: return 0x0F;
        case SDLK_q: return 0x10; case SDLK_w: return 0x11; case SDLK_e: return 0x12;
        case SDLK_r: return 0x13; case SDLK_t: return 0x14; case SDLK_z: return 0x15;
        case SDLK_u: return 0x16; case SDLK_i: return 0x17; case SDLK_o: return 0x18;
        case SDLK_p: return 0x19;
        case SDLK_RETURN: return 0x1C;
        case SDLK_a: return 0x1E; case SDLK_s: return 0x1F; case SDLK_d: return 0x20;
        case SDLK_f: return 0x21; case SDLK_g: return 0x22; case SDLK_h: return 0x23;
        case SDLK_j: return 0x24; case SDLK_k: return 0x25; case SDLK_l: return 0x26;
        case SDLK_y: return 0x2C; case SDLK_x: return 0x2D; case SDLK_c: return 0x2E;
        case SDLK_v: return 0x2F; case SDLK_b: return 0x30; case SDLK_n: return 0x31;
        case SDLK_m: return 0x32;
        case SDLK_COMMA: return 0x33; case SDLK_PERIOD: return 0x34;
        case SDLK_SLASH: return 0x35;
        case SDLK_SPACE: return 0x39;
        case SDLK_F1: return 0x3B; case SDLK_F2: return 0x3C; case SDLK_F3: return 0x3D;
        case SDLK_F4: return 0x3E; case SDLK_F5: return 0x3F; case SDLK_F6: return 0x40;
        case SDLK_F7: return 0x41; case SDLK_F8: return 0x42; case SDLK_F9: return 0x43;
        case SDLK_F10: return 0x44; case SDLK_F11: return 0x85; case SDLK_F12: return 0x86;
        case SDLK_UP: return 0x48; case SDLK_DOWN: return 0x50;
        case SDLK_LEFT: return 0x4B; case SDLK_RIGHT: return 0x4D;
        default: return 0;
    }
}

// Globals for simple tone generation
uint32_t linux_current_freq = 0;
uint32_t linux_sound_duration = 0; // in samples (or just ms)

extern "C" void play_linux_freq(uint32_t freq) {
    linux_current_freq = freq;
    linux_sound_duration = 0xFFFFFFFF; // Play indefinitely until play_freq(0) is called
}

extern "C" void play_linux_sound(uint32_t freq, uint32_t duration_ms) {
    linux_current_freq = freq;
    linux_sound_duration = (44100 * duration_ms) / 1000;
}

// Audio Callback
void audio_callback(void* userdata, Uint8* stream, int len) {
    static float phase = 0.0f;
    int16_t* out = (int16_t*)stream;
    int samples = len / 2;
    
    if (linux_wav_len > 0 && linux_wav_pos < linux_wav_len) {
        int bytes_to_copy = len;
        if (linux_wav_pos + bytes_to_copy > linux_wav_len) {
            bytes_to_copy = linux_wav_len - linux_wav_pos;
        }
        memcpy(stream, linux_wav_buffer + linux_wav_pos, bytes_to_copy);
        linux_wav_pos += bytes_to_copy;
        
        if (bytes_to_copy < len) {
            memset(stream + bytes_to_copy, 0, len - bytes_to_copy);
            linux_wav_len = 0;
        }
        return;
    }

    if (meow_timer > 0) {
        meow_timer--;
        
        float t = 1.0f - ((float)meow_timer / 20.0f);
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        
        float freq = 900.0f - 500.0f * t;
        float phase_inc = freq / 44100.0f;
        
        for (int i=0; i<samples; i++) {
            phase += phase_inc;
            if (phase > 1.0f) phase -= 1.0f;
            out[i] = (phase < 0.5f) ? 4000 : -4000;
        }
        return;
    } 
    
    if (linux_current_freq > 0 && linux_sound_duration > 0) {
        float phase_inc = (float)linux_current_freq / 44100.0f;
        for (int i=0; i<samples; i++) {
            phase += phase_inc;
            if (phase > 1.0f) phase -= 1.0f;
            out[i] = (phase < 0.5f) ? 4000 : -4000;
            
            if (linux_sound_duration != 0xFFFFFFFF) {
                linux_sound_duration--;
                if (linux_sound_duration == 0) {
                    linux_current_freq = 0;
                    // Zero out the rest
                    for (int j=i+1; j<samples; j++) out[j] = 0;
                    break;
                }
            }
        }
    } else {
        memset(stream, 0, len);
    }
}
#include "boot_info.h"
#include <sys/mman.h>

int main(int argc, char** argv) {
    void* bare_metal_ram = mmap(NULL, 1024 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (bare_metal_ram == MAP_FAILED) {
        printf("WARNING: Failed to allocate bare-metal 1GB RAM space!\n");
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "MeinOS Hybrid Linux",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1920, 1080,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window) return 1;

    // Allocate framebuffer memory for Cosmos OS to draw into
    fb = new uint32_t[1920 * 1080];

    SDL_GLContext gl_ctx = SDL_GL_CreateContext(window);
    
    // Hide hardware cursor so only Cosmos OS Aero cursor is visible
    SDL_ShowCursor(SDL_DISABLE);
    if (!gl_ctx) return 1;
    
    SDL_GL_SetSwapInterval(1); // VSync

    // Init Audio
    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 2048;
    want.callback = audio_callback;
    SDL_AudioDeviceID audio_dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (audio_dev > 0) SDL_PauseAudioDevice(audio_dev, 0);

    // MOCK A HARD DRIVE FOR LINUX (Disk Manager)
    extern uint32_t drive_count;
    // Drive setup is now properly handled in ahci_mount_drive()

    // Boot Cosmos OS in a background thread!
    BootInfo boot_info = {0};
    boot_info.screen_width = 800;
    boot_info.screen_height = 600;
    boot_info.framebuffer_pitch = 800 * 4;
    
    std::thread cosmos_thread(cosmos_main, &boot_info);
    cosmos_thread.detach();

    // Create an OpenGL texture for the Cosmos OS framebuffer
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bool running = true;
    uint32_t last_ticks_ms = SDL_GetTicks();
    while (running) {
        SDL_Event event;
        
        // --- Update system_ticks (1 tick per ms, matching bare-metal PIT at 1000 Hz) ---
        uint32_t now_ms = SDL_GetTicks();
        system_ticks += (now_ms - last_ticks_ms);
        os2_system_ticks += (now_ms - last_ticks_ms);
        last_ticks_ms = now_ms;
        
        int mx, my;
        uint32_t mb = SDL_GetMouseState(&mx, &my);
        
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        
        // Cosmos OS expects 800x600 (Desktop Mode)
        // Let's scale mouse coordinates
        float scale_x = (float)boot_info.screen_width / (float)w;
        float scale_y = (float)boot_info.screen_height / (float)h;
        
        linux_mx = (uint32_t)(mx * scale_x);
        linux_my = (uint32_t)(my * scale_y);
        
        if (mb & SDL_BUTTON(1)) {
            if (!linux_mdown) mouse_just_pressed = true;
            linux_mdown = true;
        } else {
            linux_mdown = false;
        }
        
        mouse_right_down = (mb & SDL_BUTTON(3)) != 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            
            if (event.type == SDL_KEYDOWN) {
                linux_scancode = sdl_to_ps2(event.key.keysym.sym);
            }
			if (event.type == SDL_MOUSEWHEEL) {
				mouse_wheel = event.wheel.y;
			}
            if (event.type == SDL_KEYUP) {
                uint8_t sc = sdl_to_ps2(event.key.keysym.sym);
                if (sc != 0) {
                    linux_scancode = sc | 0x80; // Release scancode
                }
            }
        }
        
        if (input_cooldown > 0) input_cooldown--;

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Upload the Cosmos OS Framebuffer to the texture
        if (fb != nullptr) {
            glViewport(0, 0, w, h);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, boot_info.screen_width, boot_info.screen_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, fb);

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
		if (app_window_active) {
            // 1. GANZ WICHTIG: Tiefenpuffer leeren!
            // Das sorgt dafür, dass die 3D-Katze räumlich VOR dem Cosmos-Desktop platziert wird.
            glClear(GL_DEPTH_BUFFER_BIT);

            // 2. 3D Engine im sicheren Main-Thread aufrufen!
            run_smash_cats_engine(os_app_x, os_app_y, os_app_w, os_app_h, false);

            // 3. Scissor-Test wieder ausmachen, damit der nächste Frame nicht kaputt geht
            glDisable(GL_SCISSOR_TEST);
        }

        // DEIN ALTER CODE:
        SDL_GL_SwapWindow(window);
        frame++;
    }

    if (audio_dev > 0) SDL_CloseAudioDevice(audio_dev);
    SDL_GL_DeleteContext(gl_ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
