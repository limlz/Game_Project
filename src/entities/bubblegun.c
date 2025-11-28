/*************************************************************************
@file       bubblegun.c
@Author     Lim Liang Zhou l.liangzhou@digipen.edu
@Co-authors nil
@brief      This file contains the functions to handle bullet spawning, updating,
            and rendering for the bubble gun entity in the main menu.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/

// bullets.c
#include "bubblegun.h"
#include "cprocessing.h"

#define MAX_BULLETS 300   // maximum number of bullets allowed at once

typedef struct {
    CP_Vector pos;        // current position
    CP_Vector dir;        // direction of travel (should be normalized)
    float speed;          // movement speed (units per second)
    float radius;         // bullet circle radius
    CP_Color color;       // bullet color data (currently not used in draw)
    int active;           // 1 = in use, 0 = free slot
} Bullet;

static Bullet g_bullets[MAX_BULLETS]; // fixed-size bullet pool (no malloc)

void BulletsInit(void) {
    // mark every bullet slot as unused at startup
    for (int i = 0; i < MAX_BULLETS; ++i) g_bullets[i].active = 0;
}

void BulletsClear(void) {
    // clear all bullets (e.g., when restarting / leaving menu)
    for (int i = 0; i < MAX_BULLETS; ++i) g_bullets[i].active = 0;
}

void BulletsSpawn(CP_Vector origin, CP_Vector target, float speed, float radius) {
    // find the first inactive bullet slot and reuse it
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!g_bullets[i].active) {
            // compute direction vector from origin -> target
            CP_Vector v = CP_Vector_Subtract(target, origin);
            CP_Vector dir = CP_Vector_Normalize(v);

            // initialize bullet data
            g_bullets[i].pos = origin;
            g_bullets[i].dir = dir;
            g_bullets[i].speed = speed;
            g_bullets[i].radius = radius;
            g_bullets[i].color = CP_Color_Create(0, 0, 255, 255); // stored color (not used in drawing below)

            // activate this bullet
            g_bullets[i].active = 1;
            break; // only spawn one bullet per call
        }
    }
}

void BulletsUpdateAndDraw(void) {
    // dt = time step for frame-independent movement
    float dt = CP_System_GetDt();

    // window size for offscreen culling
    float w = (float)CP_System_GetWindowWidth();
    float h = (float)CP_System_GetWindowHeight();

    // update + draw every active bullet
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!g_bullets[i].active) continue; // skip unused slots

        // integrate position: pos += dir * speed * dt
        CP_Vector step = CP_Vector_Scale(g_bullets[i].dir, g_bullets[i].speed * dt);
        g_bullets[i].pos = CP_Vector_Add(g_bullets[i].pos, step);

        // set visual settings and draw bullet circle
        CP_Settings_Fill(CP_Color_Create(135, 206, 235, 150));
        CP_Settings_Stroke(CP_Color_Create(145, 216, 245, 200));
        CP_Graphics_DrawCircle(g_bullets[i].pos.x, g_bullets[i].pos.y, g_bullets[i].radius);

        // deactivate bullet if it goes outside the window bounds (with radius padding)
        float r = g_bullets[i].radius;
        if (g_bullets[i].pos.x < -r || g_bullets[i].pos.x > w + r || g_bullets[i].pos.y < -r || g_bullets[i].pos.y > h + r) {
            g_bullets[i].active = 0;
        }
        // repeated offscreen check (same as above)
        if (g_bullets[i].pos.x < -r || g_bullets[i].pos.x > w + r || g_bullets[i].pos.y < -r || g_bullets[i].pos.y > h + r) {
            g_bullets[i].active = 0;
        }
    }
}
