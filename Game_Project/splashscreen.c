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
CP_Image logo;
float fade_timer = 0.0f;
float fade_duration = 1.5f;
bool fading_in = true;
float dt;
float alpha;
float global_timer = 0.0f;

void SplashScreenInit(void)
{
	logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
}

void SplashScreenUpdate(void)
{
	dt = CP_System_GetDt();
	fade_timer += dt;
	global_timer += dt;
	if (fade_timer> fade_duration) {
		fading_in = !fading_in;
		fade_timer = 0.0f;
	}

	if (fading_in) {
		alpha = (fade_timer / fade_duration) * 255.0f;
	}
	else
	{
		alpha = 255.0f - (fade_timer / fade_duration) * 255.0f;
	}

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_DrawAdvanced(logo, (CP_System_GetWindowWidth() / 2), (CP_System_GetWindowHeight() / 2), CP_Image_GetWidth(logo), CP_Image_GetHeight(logo), alpha, 0);

	
	if (global_timer > 3.0) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void SplashScreenExit(void)
{
	void CP_Image_Free(logo);
}