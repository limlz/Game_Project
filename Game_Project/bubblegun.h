#pragma once
// bullets.h
#pragma once
#include "cprocessing.h"
// Initialize internal state (call once, e.g., in your scene init)
void Bullets_Init(void);

// Spawn a bullet from origin toward target.
// Optional overrides: speed (pixels/sec, pass <=0 to use default), radius (pixels), and color (RGBA).
void Bullets_Spawn(CP_Vector origin, CP_Vector target, float speed, float radius);

// Update and render all bullets (call every frame in your update/draw)
void Bullets_UpdateAndDraw(void);

// Optional: clear all bullets (e.g., on scene change)
void Bullets_Clear(void);

