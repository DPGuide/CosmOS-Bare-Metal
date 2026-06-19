#pragma once
#include <stdint.h>

#define SGL_TRIANGLES 1
#define SGL_QUADS 2
#define SGL_POINTS 3

#define SGL_DEPTH_TEST 1
#define SGL_LIGHTING 2

extern "C" {
    void sglEnable(int cap);
    void sglDisable(int cap);
    void sglClear(uint32_t color);
    void sglClearDepth();
    void sglBegin(int mode);
    void sglEnd();
    void sglVertex3f(float x, float y, float z);
    void sglColor3f(float r, float g, float b);
    void sglColor3ub(uint8_t r, uint8_t g, uint8_t b);
    
    void sglPushMatrix();
    void sglPopMatrix();
    void sglLoadIdentity();
    void sglTranslatef(float x, float y, float z);
    void sglRotatef(float angle, float x, float y, float z);
    void sglScalef(float x, float y, float z);
    
    void sglViewport(int x, int y, int width, int height);
}

// Helper structures for 3D
struct sglVec3 { float x, y, z; };
struct sglColor { uint8_t r, g, b; };

// Epic Helper Functions
void sglDrawCube(float size);
void sglDrawPyramid(float size);
void sglDrawSphere(float radius, int segments);
void sglPerspective(float fov, float aspect, float zNear, float zFar);
