/*************************************************************************
@file		splashscreen.c
@Author		Ng Cher Kai Dan (cherkaidan.ng@digipen.edu)
@Co-authors Chiu Jun Wen (j.chiu@digipen.edu)
@brief		Splash Screen that displays the digipen logo

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "mainmenu.h"
#include <stdbool.h>

CP_Font montserrat_light;
CP_Color white;
CP_Image logo;
float fade_timer = 0.0f;
float fade_duration = 2.0f;
bool fading_in = true;
float dt;
float alpha;
float global_timer = 0.0f;

void SplashScreenInit(void)
{	
	// load logo and font
	logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	CP_Font_Set(montserrat_light);
	// initialise white color
	white = CP_Color_Create(255, 255, 255, 255);
}

void SplashScreenUpdate(void)
{	
	// update fade and global timers
	dt = CP_System_GetDt();
	fade_timer += dt;
	global_timer += dt;

	// handle fade in and fade out
	if (fade_timer> fade_duration) {
		fading_in = !fading_in;
		fade_timer = 0.0f;
	}

	if (fading_in) { 
		// calculate alpha for fade in based on timer
		alpha = 255.0f - (fade_timer / fade_duration) * 255.0f;
	}
	else
	{
		// calculate alpha for fade out based on timer
		alpha = (fade_timer / fade_duration) * 255.0f;
	}

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

	// tint whole screen black based on alpha value
	CP_Settings_Tint(CP_Color_Create(0, 0, 0, (int)alpha));

	// draw logo at center of screen
	CP_Image_DrawAdvanced(logo, (CP_System_GetWindowWidth() / 2), (CP_System_GetWindowHeight() / 2), CP_Image_GetWidth(logo), CP_Image_GetHeight(logo), 255, 0);
	
	// draw copyright text at bottom of screen
	CP_Settings_Fill(white);
	CP_Settings_TextSize(30.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("All Content Copyright (C) 2025 Digipen Institute of Technology Singpoare. All Rights Reserved.", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - 100.0f);
	
	// transition to main menu after 4 seconds
	if (global_timer > 4.0f) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void SplashScreenExit(void)
{
	// free resources
	CP_Image_Free(logo);
	CP_Font_Free(montserrat_light);
	// reset tint for next game state
	CP_Settings_NoTint();
}