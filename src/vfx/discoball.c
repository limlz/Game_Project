/*************************************************************************
@file       discoball.c
@Author     Chiu Jun Wen (j.chiu@digipen.edu)
@Co-authors NIL
@brief      This file contains the function definitions for the disco ball
			effect

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "discoball.h"
#include <math.h>

typedef struct {
	float hue;
	float change_speed;
	float brightness;
}Disco;

static Disco disco;

void InitDiscoLighting(void) {
	disco.hue = 0.0f;
	disco.change_speed = 1.0f;
	disco.brightness = 200.0f;
}

void UpdateDiscoLighting(float dt) {
	disco.hue += dt * 255.0f * disco.change_speed;
	if (disco.hue >= 255) {
		disco.hue -= 255.0f;
	}
	disco.brightness = 60.0f + 50.0f * sin(CP_System_GetSeconds() * 10.0f);
	CP_ColorHSL c = CP_ColorHSL_Create(disco.hue, 100, 50, disco.brightness);
	CP_Settings_Fill(CP_Color_FromColorHSL(c));
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth()/2.0f, CP_System_GetWindowHeight()/2.0f, CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
}