/*************************************************************************
@file       confetti.c
@Author     Chiu Jun Wen (j.chiu@digipen.edu)
@Co-authors
@brief      This file contains the function definitions for creating
            the confetti particle effect

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/

#include "confetti.h"
#include "cprocessing.h"

#define MAX_CONFETTI 1000

typedef struct {
	float x, y;
	float vx, vy;
	float rotation;
	float size;
	CP_Color color;
	int active;
} Confetti;

static Confetti confetti[MAX_CONFETTI];

void InitConfetti(void) {
    for (int i = 0; i < MAX_CONFETTI; i++) {
        confetti[i].x = CP_Random_RangeFloat(0, CP_System_GetWindowWidth());    //horizontal spawn location range
        confetti[i].y = CP_Random_RangeFloat(-1000, 0);                         //vertical spawn location range
        confetti[i].vx = CP_Random_RangeFloat(-50, 50);                         //horizontal velocity
        confetti[i].vy = CP_Random_RangeFloat(100, 300);                        //vertical velocity
        confetti[i].rotation = CP_Random_RangeFloat(0, 360); 
        confetti[i].size = CP_Random_RangeFloat(4, 10); 
        confetti[i].color = CP_Color_Create(
            CP_Random_RangeInt(180, 255),
            CP_Random_RangeInt(140, 255),
            CP_Random_RangeInt(140, 255),
            255
        );
        confetti[i].active = 1;
    }
}

void UpdateConfetti(float dt) {
    for (int i = 0; i < MAX_CONFETTI; i++) {
        if (!confetti[i].active) continue;

        confetti[i].y += confetti[i].vy * dt;                                   //confetti vertical movement
        confetti[i].x += confetti[i].vx * dt;                                   //confetti horizontal movement
        confetti[i].rotation += 360 * dt;                                       //confetti rotation
        confetti[i].vy += 110 * dt;                                             //gravity      
        confetti[i].vx *= 1.0f;                                                 //slow or speed up horizontal movement

        if (confetti[i].y > CP_System_GetWindowHeight() + 20)                   //delete confetti after falling to the bottom
            confetti[i].active = 0;

        CP_Settings_Stroke(confetti[i].color);
        CP_Settings_Fill(confetti[i].color);
        CP_Graphics_DrawRectAdvanced(confetti[i].x, confetti[i].y,
            confetti[i].size, confetti[i].size, confetti[i].rotation, 0.0f);
    }
}