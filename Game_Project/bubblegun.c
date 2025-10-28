// bullets.c
#include "bubblegun.h"
#include "cprocessing.h" 

#define MAX_BULLETS 300

typedef struct {
    CP_Vector pos;
    CP_Vector dir;
    float speed;
    float radius;
    CP_Color color;
    int active;
} Bullet;

static Bullet g_bullets[MAX_BULLETS];

void BulletsInit(void) {
    for (int i = 0; i < MAX_BULLETS; ++i) g_bullets[i].active = 0;
}

void BulletsClear(void) {
    for (int i = 0; i < MAX_BULLETS; ++i) g_bullets[i].active = 0;
}

void BulletsSpawn(CP_Vector origin, CP_Vector target, float speed, float radius) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!g_bullets[i].active) {
            CP_Vector v = CP_Vector_Subtract(target, origin);
            CP_Vector dir = CP_Vector_Normalize(v);

            g_bullets[i].pos = origin;
            g_bullets[i].dir = dir;
            g_bullets[i].speed = speed;
            g_bullets[i].radius = radius;
            g_bullets[i].color = CP_Color_Create(0,0,255,255);

            g_bullets[i].active = 1;
            break;
        }
    }
}

void BulletsUpdateAndDraw(void) {
    float dt = CP_System_GetDt();
    float w = (float)CP_System_GetWindowWidth();
    float h = (float)CP_System_GetWindowHeight();

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!g_bullets[i].active) continue;

        // integrate
        CP_Vector step = CP_Vector_Scale(g_bullets[i].dir, g_bullets[i].speed * dt);
        g_bullets[i].pos = CP_Vector_Add(g_bullets[i].pos, step);

        // draw
        CP_Settings_Fill(CP_Color_Create(135, 206, 235,150));
        CP_Settings_Stroke(CP_Color_Create(145, 216, 245, 200));
        CP_Graphics_DrawCircle(g_bullets[i].pos.x, g_bullets[i].pos.y, g_bullets[i].radius);

        // kill offscreen
        float r = g_bullets[i].radius;
        if (g_bullets[i].pos.x < -r || g_bullets[i].pos.x > w + r || g_bullets[i].pos.y < -r || g_bullets[i].pos.y > h + r) {
            g_bullets[i].active = 0;
        }
    }
}
