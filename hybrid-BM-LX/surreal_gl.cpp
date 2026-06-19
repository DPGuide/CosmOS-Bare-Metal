#include "surreal_gl.h"

extern "C" void Put(int x, int y, uint32_t c);
extern uint32_t* bb;
extern uint32_t screen_w;
extern uint32_t screen_h;
extern uint32_t screen_pitch;

// --- MATH & MATRIX CORE ---
struct Mat4 {
    float m[4][4];
};

static Mat4 mat_ident() {
    Mat4 res = {0};
    res.m[0][0] = 1.0f; res.m[1][1] = 1.0f;
    res.m[2][2] = 1.0f; res.m[3][3] = 1.0f;
    return res;
}

static Mat4 mat_mul(const Mat4& a, const Mat4& b) {
    Mat4 res = {0};
    for(int r=0; r<4; r++) {
        for(int c=0; c<4; c++) {
            res.m[r][c] = a.m[r][0]*b.m[0][c] + a.m[r][1]*b.m[1][c] + 
                          a.m[r][2]*b.m[2][c] + a.m[r][3]*b.m[3][c];
        }
    }
    return res;
}

static sglVec3 mat_mul_vec(const Mat4& m, const sglVec3& v) {
    sglVec3 res;
    res.x = v.x*m.m[0][0] + v.y*m.m[0][1] + v.z*m.m[0][2] + m.m[0][3];
    res.y = v.x*m.m[1][0] + v.y*m.m[1][1] + v.z*m.m[1][2] + m.m[1][3];
    res.z = v.x*m.m[2][0] + v.y*m.m[2][1] + v.z*m.m[2][2] + m.m[2][3];
    return res;
}

// Fast Sine/Cosine approximation
static float sgl_abs(float x) { return x < 0 ? -x : x; }
static float sgl_fmod(float x, float y) { return x - (int)(x / y) * y; }

static float sgl_sinf(float x) {
    const float PI = 3.14159265f;
    x = sgl_fmod(x + PI, PI * 2) - PI;
    const float B = 4.0f / PI;
    const float C = -4.0f / (PI * PI);
    float y = B * x + C * x * sgl_abs(x);
    const float P = 0.225f;
    return P * (y * sgl_abs(y) - y) + y;
}
static float sgl_cosf(float x) {
    return sgl_sinf(x + 1.57079632f);
}

// --- STATE MACHINE ---
#define MAX_STACK 16
static Mat4 matrix_stack[MAX_STACK];
static int stack_ptr = 0;
static Mat4 current_matrix = mat_ident();

static int current_mode = 0;
static uint32_t current_color = 0xFFFFFF;

#define MAX_VERTS 128
static sglVec3 vert_buffer[MAX_VERTS];
static uint32_t color_buffer[MAX_VERTS];
static int vert_count = 0;

static int vp_x = 0, vp_y = 0, vp_w = 800, vp_h = 600;
static bool depth_test = false;
static bool enable_lighting = false;

// Z-Buffer (fixed size for viewport max)
#define MAX_ZBUFFER (1024*768)
static float* z_buffer = 0;

// Fast Inverse Square Root
static float sgl_invSqrt(float number) {
    union { float f; uint32_t i; } conv = { .f = number };
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
}

extern "C" {

void sglEnable(int cap) {
    if (cap == SGL_DEPTH_TEST) depth_test = true;
    if (cap == SGL_LIGHTING) enable_lighting = true;
}

void sglDisable(int cap) {
    if (cap == SGL_DEPTH_TEST) depth_test = false;
    if (cap == SGL_LIGHTING) enable_lighting = false;
}

void sglViewport(int x, int y, int width, int height) {
    vp_x = x; vp_y = y; vp_w = width; vp_h = height;
    if (!z_buffer) {
        // Find a free RAM area for Z-Buffer, or just use an array in BSS
        // Since we are in bare metal, let's declare a static array.
    }
}

// Static Z-Buffer
static float static_z_buffer[MAX_ZBUFFER];

void sglClear(uint32_t color) {
    // We only clear the viewport area
    for (int y = 0; y < vp_h; y++) {
        int sy = vp_y + y;
        if (sy < 0 || sy >= (int)screen_h) continue;
        uint32_t* row = (uint32_t*)((uint8_t*)bb + (sy * screen_pitch));
        for (int x = 0; x < vp_w; x++) {
            int sx = vp_x + x;
            if (sx < 0 || sx >= (int)screen_w) continue;
            row[sx] = color;
            if (depth_test) {
                int z_idx = y * vp_w + x;
                if (z_idx < MAX_ZBUFFER) static_z_buffer[z_idx] = -10000.0f; // Very far behind
            }
        }
    }
}

void sglClearDepth() {
    for (int y = 0; y < vp_h; y++) {
        for (int x = 0; x < vp_w; x++) {
            int z_idx = y * vp_w + x;
            if (z_idx < MAX_ZBUFFER) static_z_buffer[z_idx] = 100000.0f;
        }
    }
}

void sglPushMatrix() {
    if (stack_ptr < MAX_STACK - 1) {
        matrix_stack[stack_ptr++] = current_matrix;
    }
}

void sglPopMatrix() {
    if (stack_ptr > 0) {
        current_matrix = matrix_stack[--stack_ptr];
    }
}

void sglLoadIdentity() {
    current_matrix = mat_ident();
}

void sglTranslatef(float x, float y, float z) {
    Mat4 t = mat_ident();
    t.m[0][3] = x; t.m[1][3] = y; t.m[2][3] = z;
    current_matrix = mat_mul(current_matrix, t);
}

void sglRotatef(float angle, float x, float y, float z) {
    float rad = angle * 3.14159265f / 180.0f;
    float c = sgl_cosf(rad);
    float s = sgl_sinf(rad);
    Mat4 r = mat_ident();
    
    // Simplified: assuming rotation around primary axes
    if (z > 0.5f) {
        r.m[0][0] = c; r.m[0][1] = -s;
        r.m[1][0] = s; r.m[1][1] = c;
    } else if (y > 0.5f) {
        r.m[0][0] = c; r.m[0][2] = s;
        r.m[2][0] = -s; r.m[2][2] = c;
    } else if (x > 0.5f) {
        r.m[1][1] = c; r.m[1][2] = -s;
        r.m[2][1] = s; r.m[2][2] = c;
    }
    current_matrix = mat_mul(current_matrix, r);
}

void sglScalef(float x, float y, float z) {
    Mat4 s_mat = mat_ident();
    s_mat.m[0][0] = x; s_mat.m[1][1] = y; s_mat.m[2][2] = z;
    current_matrix = mat_mul(current_matrix, s_mat);
}

void sglColor3f(float r, float g, float b) {
    int ir = (int)(r * 255.0f); if (ir > 255) ir = 255; if (ir < 0) ir = 0;
    int ig = (int)(g * 255.0f); if (ig > 255) ig = 255; if (ig < 0) ig = 0;
    int ib = (int)(b * 255.0f); if (ib > 255) ib = 255; if (ib < 0) ib = 0;
    current_color = (ir << 16) | (ig << 8) | ib;
}

void sglColor3ub(uint8_t r, uint8_t g, uint8_t b) {
    current_color = (r << 16) | (g << 8) | b;
}

void sglBegin(int mode) {
    current_mode = mode;
    vert_count = 0;
}

// --- RASTERIZATION CORE ---
static float sglEdge(const sglVec3& a, const sglVec3& b, float px, float py) {
    return (px - a.x) * (b.y - a.y) - (py - a.y) * (b.x - a.x);
}

static void sglDrawPoint(sglVec3 v0, uint32_t col) {
    float z0 = v0.z + 5.0f; if (z0 < 0.1f) return;
    float fov = 200.0f;
    int px = (int)(vp_x + vp_w/2.0f + (v0.x/z0)*fov);
    int py = (int)(vp_y + vp_h/2.0f - (v0.y/z0)*fov);
    
    if (px >= vp_x && px < vp_x + vp_w - 1 && py >= vp_y && py < vp_y + vp_h - 1) {
        if (depth_test) {
            int z_idx = (py - vp_y) * vp_w + (px - vp_x);
            if (z_idx >= 0 && z_idx < MAX_ZBUFFER && v0.z < static_z_buffer[z_idx]) {
                static_z_buffer[z_idx] = v0.z;
                uint32_t* row = (uint32_t*)((uint8_t*)bb + (py * screen_pitch));
                row[px] = col; row[px+1] = col;
                ((uint32_t*)((uint8_t*)bb + ((py+1)*screen_pitch)))[px] = col;
                ((uint32_t*)((uint8_t*)bb + ((py+1)*screen_pitch)))[px+1] = col;
            }
        } else {
            uint32_t* row = (uint32_t*)((uint8_t*)bb + (py * screen_pitch));
            row[px] = col; row[px+1] = col;
            ((uint32_t*)((uint8_t*)bb + ((py+1)*screen_pitch)))[px] = col;
            ((uint32_t*)((uint8_t*)bb + ((py+1)*screen_pitch)))[px+1] = col;
        }
    }
}

static void sglDrawRasterTriangle(sglVec3 v0, sglVec3 v1, sglVec3 v2, uint32_t col) {
    float z0 = v0.z + 5.0f;
    float z1 = v1.z + 5.0f;
    float z2 = v2.z + 5.0f;
    if (z0 < 0.1f || z1 < 0.1f || z2 < 0.1f) return; // Fast Z-Clip (verhindert Glitches)
    
    float fov = 200.0f;
    sglVec3 p0 = { vp_x + vp_w/2.0f + (v0.x/z0)*fov, vp_y + vp_h/2.0f - (v0.y/z0)*fov, v0.z };
    sglVec3 p1 = { vp_x + vp_w/2.0f + (v1.x/z1)*fov, vp_y + vp_h/2.0f - (v1.y/z1)*fov, v1.z };
    sglVec3 p2 = { vp_x + vp_w/2.0f + (v2.x/z2)*fov, vp_y + vp_h/2.0f - (v2.y/z2)*fov, v2.z };

    int minx = (int)(p0.x < p1.x ? (p0.x < p2.x ? p0.x : p2.x) : (p1.x < p2.x ? p1.x : p2.x));
    int miny = (int)(p0.y < p1.y ? (p0.y < p2.y ? p0.y : p2.y) : (p1.y < p2.y ? p1.y : p2.y));
    int maxx = (int)(p0.x > p1.x ? (p0.x > p2.x ? p0.x : p2.x) : (p1.x > p2.x ? p1.x : p2.x));
    int maxy = (int)(p0.y > p1.y ? (p0.y > p2.y ? p0.y : p2.y) : (p1.y > p2.y ? p1.y : p2.y));

    if (minx < vp_x) minx = vp_x;
    if (miny < vp_y) miny = vp_y;
    if (maxx >= vp_x + vp_w) maxx = vp_x + vp_w - 1;
    if (maxy >= vp_y + vp_h) maxy = vp_y + vp_h - 1;

    float area = sglEdge(p0, p1, p2.x, p2.y);
    if (area <= 0.0001f && area >= -0.0001f) return; // Only cull degenerate (zero area) triangles
    float inv_area = 1.0f / area;
    
    uint32_t final_col = col;
    if (enable_lighting) {
        // Calculate normal
        sglVec3 e1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
        sglVec3 e2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
        sglVec3 n = {
            e1.y * e2.z - e1.z * e2.y,
            e1.z * e2.x - e1.x * e2.z,
            e1.x * e2.y - e1.y * e2.x
        };
        float invLen = sgl_invSqrt(n.x*n.x + n.y*n.y + n.z*n.z);
        n.x *= invLen; n.y *= invLen; n.z *= invLen;
        
        // Sun direction (from top-right)
        sglVec3 sun = { 0.5f, 0.8f, -0.3f };
        float sunInv = sgl_invSqrt(sun.x*sun.x + sun.y*sun.y + sun.z*sun.z);
        sun.x *= sunInv; sun.y *= sunInv; sun.z *= sunInv;
        
        float dot = n.x * sun.x + n.y * sun.y + n.z * sun.z;
        if (dot < 0.0f) dot = -dot; // Backface handle
        
        float ambient = 0.35f;
        float intensity = ambient + (1.0f - ambient) * dot;
        if (intensity > 1.0f) intensity = 1.0f;
        
        uint8_t cr = (col >> 16) & 0xFF;
        uint8_t cg = (col >> 8) & 0xFF;
        uint8_t cb = col & 0xFF;
        cr = (uint8_t)(cr * intensity);
        cg = (uint8_t)(cg * intensity);
        cb = (uint8_t)(cb * intensity);
        final_col = (cr << 16) | (cg << 8) | cb;
    }

    for (int y = miny; y <= maxy; y++) {
        if (y < 0 || y >= (int)screen_h) continue;
        uint32_t* row = (uint32_t*)((uint8_t*)bb + (y * screen_pitch));
        for (int x = minx; x <= maxx; x++) {
            if (x < 0 || x >= (int)screen_w) continue;
            
            float w0 = sglEdge(p1, p2, x, y);
            float w1 = sglEdge(p2, p0, x, y);
            float w2 = sglEdge(p0, p1, x, y);
            
            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                if (depth_test) {
                    float b0 = w0 * inv_area;
                    float b1 = w1 * inv_area;
                    float b2 = w2 * inv_area;
                    float z = b0*p0.z + b1*p1.z + b2*p2.z;
                    
                    int z_idx = (y - vp_y) * vp_w + (x - vp_x);
                    if (z_idx >= 0 && z_idx < MAX_ZBUFFER) {
                        if (z < static_z_buffer[z_idx]) {
                            static_z_buffer[z_idx] = z;
                            row[x] = final_col;
                        }
                    }
                } else {
                    row[x] = final_col;
                }
            }
        }
    }
}

void sglVertex3f(float x, float y, float z) {
    if (vert_count < MAX_VERTS) {
        sglVec3 v = {x, y, z};
        vert_buffer[vert_count] = mat_mul_vec(current_matrix, v);
        color_buffer[vert_count] = current_color;
        vert_count++;
        
        if (current_mode == 4 && vert_count == 1) { // SGL_POINTS
            sglDrawPoint(vert_buffer[0], color_buffer[0]);
            vert_count = 0;
        } else if (current_mode == SGL_TRIANGLES && vert_count == 3) {
            sglDrawRasterTriangle(vert_buffer[0], vert_buffer[1], vert_buffer[2], color_buffer[0]);
            vert_count = 0;
        } else if (current_mode == SGL_QUADS && vert_count == 4) {
            sglDrawRasterTriangle(vert_buffer[0], vert_buffer[1], vert_buffer[2], color_buffer[0]);
            sglDrawRasterTriangle(vert_buffer[0], vert_buffer[2], vert_buffer[3], color_buffer[0]);
            vert_count = 0;
        }
    }
}

void sglEnd() {
    vert_count = 0;
}

} // extern "C"

// --- EPIC HELPER FUNCTIONS ---
void sglDrawCube(float size) {
    float s = size / 2.0f;
    sglBegin(SGL_QUADS);
    
    uint32_t col = current_color;
    uint8_t r = (uint8_t)((col >> 16) & 0xFF);
    uint8_t g = (uint8_t)((col >> 8) & 0xFF);
    uint8_t b = (uint8_t)(col & 0xFF);
    
    // Front (Full brightness)
    sglColor3ub(r, g, b);
    sglVertex3f(-s, -s,  s); sglVertex3f( s, -s,  s);
    sglVertex3f( s,  s,  s); sglVertex3f(-s,  s,  s);
    
    // Back (0.5x brightness)
    sglColor3ub(r/2, g/2, b/2);
    sglVertex3f(-s, -s, -s); sglVertex3f(-s,  s, -s);
    sglVertex3f( s,  s, -s); sglVertex3f( s, -s, -s);
    
    // Top (0.9x brightness)
    sglColor3ub((r*9)/10, (g*9)/10, (b*9)/10);
    sglVertex3f(-s,  s, -s); sglVertex3f(-s,  s,  s);
    sglVertex3f( s,  s,  s); sglVertex3f( s,  s, -s);
    
    // Bottom (0.3x brightness)
    sglColor3ub((r*3)/10, (g*3)/10, (b*3)/10);
    sglVertex3f(-s, -s, -s); sglVertex3f( s, -s, -s);
    sglVertex3f( s, -s,  s); sglVertex3f(-s, -s,  s);
    
    // Right (0.7x brightness)
    sglColor3ub((r*7)/10, (g*7)/10, (b*7)/10);
    sglVertex3f( s, -s, -s); sglVertex3f( s,  s, -s);
    sglVertex3f( s,  s,  s); sglVertex3f( s, -s,  s);
    
    // Left (0.7x brightness)
    sglColor3ub((r*7)/10, (g*7)/10, (b*7)/10);
    sglVertex3f(-s, -s, -s); sglVertex3f(-s, -s,  s);
    sglVertex3f(-s,  s,  s); sglVertex3f(-s,  s, -s);
    
    sglEnd();
}

void sglDrawPyramid(float size) {
    float s = size / 2.0f;
    
    sglBegin(SGL_TRIANGLES);
    // Front
    sglColor3f(1.0f, 0.5f, 0.0f);
    sglVertex3f(0.0f, size, 0.0f);
    sglVertex3f(-s, -s, s);
    sglVertex3f(s, -s, s);
    
    // Right
    sglColor3f(0.8f, 0.4f, 0.0f);
    sglVertex3f(0.0f, size, 0.0f);
    sglVertex3f(s, -s, s);
    sglVertex3f(s, -s, -s);
    
    // Back
    sglColor3f(0.6f, 0.3f, 0.0f);
    sglVertex3f(0.0f, size, 0.0f);
    sglVertex3f(s, -s, -s);
    sglVertex3f(-s, -s, -s);
    
    // Left
    sglColor3f(0.4f, 0.2f, 0.0f);
    sglVertex3f(0.0f, size, 0.0f);
    sglVertex3f(-s, -s, -s);
    sglVertex3f(-s, -s, s);
    sglEnd();
    
    // Bottom Quad
    sglBegin(SGL_QUADS);
    sglColor3f(0.2f, 0.1f, 0.0f);
    sglVertex3f(-s, -s, -s);
    sglVertex3f(s, -s, -s);
    sglVertex3f(s, -s, s);
    sglVertex3f(-s, -s, s);
    sglEnd();
}

void sglDrawSphere(float radius, int segments) {
    uint32_t base_c = current_color;
    uint8_t br = (base_c >> 16) & 0xFF; 
    uint8_t bg = (base_c >> 8) & 0xFF; 
    uint8_t bb_col = base_c & 0xFF;
    
    for (int i = 0; i < segments; i++) {
        float lat0 = 3.14159265f * (-0.5f + (float)i / segments);
        float z0  = radius * sgl_sinf(lat0);
        float zr0 = radius * sgl_cosf(lat0);
        
        float lat1 = 3.14159265f * (-0.5f + (float)(i+1) / segments);
        float z1  = radius * sgl_sinf(lat1);
        float zr1 = radius * sgl_cosf(lat1);
        
        sglBegin(SGL_QUADS);
        for (int j = 0; j <= segments; j++) {
            float lng0 = 2.0f * 3.14159265f * (float)j / segments;
            float x0 = sgl_cosf(lng0);
            float y0 = sgl_sinf(lng0);
            
            float lng1 = 2.0f * 3.14159265f * (float)(j+1) / segments;
            float x1 = sgl_cosf(lng1);
            float y1 = sgl_sinf(lng1);
            
            float nx = (x0+x1)/2.0f; float ny = (y0+y1)/2.0f; float nz = (z0+z1)/(2.0f*radius);
            float light = nx*0.4f + ny*0.7f + nz*0.6f; 
            if (light < 0.0f) light = 0.0f; if (light > 1.0f) light = 1.0f;
            
            uint8_t r = (uint8_t)(br * (0.2f + 0.8f * light));
            uint8_t g = (uint8_t)(bg * (0.2f + 0.8f * light));
            uint8_t b = (uint8_t)(bb_col * (0.2f + 0.8f * light));
            sglColor3ub(r, g, b);
            
            sglVertex3f(x0 * zr0, y0 * zr0, z0);
            sglVertex3f(x1 * zr0, y1 * zr0, z0);
            sglVertex3f(x1 * zr1, y1 * zr1, z1);
            sglVertex3f(x0 * zr1, y0 * zr1, z1);
        }
        sglEnd();
    }
    current_color = base_c;
}
