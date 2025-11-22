/*************************************************************************
@file       credits.c
@Author     Chiu Jun Wen (j.chiu@digipen.edu)
@Co-authors
@brief      This file contains the function definitions for the credits
			screen

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/

#include "credits.h"
#include "cprocessing.h"
#include <stdio.h>
#include "mainmenu.h"
#include "settings.h"
#include "utils.h"
#include "bubbles.h"

#define MAX_LINES 200

typedef struct 
{
	CP_Image image;
	float width;
	float height;
}Image;

typedef struct {
	float x;
	float y;
}Mouse;

static Mouse mouse;
static Image back_arrow;
static Image digipen;

CP_Font montserrat_light;
CP_Font superwater;
FILE *creditsFile;
CP_Color black;
CP_Color button_blue;
CP_Color white;
CP_Color digipen_red;

char *creditsLines[MAX_LINES];
int totalLines;
float scrollY;
float speed;
char buffer[256];

void credits_Init(void) {
	
	back_arrow.image = CP_Image_Load("Assets/back_arrow.png");
	black = CP_Color_Create(0, 0, 0, 255);
	button_blue = CP_Color_Create(123, 183, 220, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	digipen_red = CP_Color_Create(152, 1, 46, 255);

	// load image
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	digipen.image = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
	digipen.height = CP_Image_GetHeight(digipen.image) / 2.0f;
	digipen.width = CP_Image_GetWidth(digipen.image) / 2.0f;

	// load font
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	superwater = CP_Font_Load("Assets/SuperWater.ttf");

	// load credits file
	errno_t err = fopen_s(&creditsFile, "Assets/credits.txt", "r");
	if (err != 0 || creditsFile == NULL) {
		return;
	}
	totalLines = 0;
	while (fgets(buffer, sizeof(buffer), creditsFile) && totalLines < MAX_LINES) {

		buffer[strcspn(buffer, "\r\n")] = 0; //removes newline

		creditsLines[totalLines] = _strdup(buffer);
		totalLines++;
	}
	
	// unload credits file
	fclose(creditsFile);

	//set credit scroll speed
	scrollY = CP_System_GetWindowHeight();
	speed = 90.0f;

	//initialise bubble
	BubblesInit();
}
void credits_Update(void) {
	float deltaTime = CP_System_GetDt();
	mouse.x = CP_Input_GetMouseX();
	mouse.y = CP_Input_GetMouseY();
	float settings_pop = 0.0f;

	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	CP_Image_Draw(digipen.image, CP_System_GetWindowWidth() / 2.0f, scrollY, digipen.width, digipen.height, 255);

	CP_Settings_Fill(digipen_red);
	CP_Font_Set(montserrat_light);
	CP_Font_DrawText("All Content (C) 2025 Digipen Institute of Technology Singpoare. All Rights Reserved.", CP_System_GetWindowWidth() / 2.0f + 3.0f, scrollY + digipen.height / 2.0f + 30.0f);

	CP_Settings_NoStroke();
	if (IsAreaClicked(120, CP_System_GetWindowHeight() - 120, 100, 100, mouse.x, mouse.y)) {
		settings_pop = 10.0f;
		BubblesManual(mouse.x, mouse.y);

		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 120.0f, 75.0f, 0.0f, 20.0f);
		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Back", 120, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
	CP_Image_Draw(back_arrow.image, 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + settings_pop, 100.0f + settings_pop, 255);

	// Draw credits
	CP_Font_Set(superwater);
	CP_Settings_TextSize(30.0f);
	for (int i = 0; i < totalLines; i++) {
		CP_Settings_Fill(white);
		CP_Font_DrawText(creditsLines[i], CP_System_GetWindowWidth() / 2.0f + 3.0f, scrollY + digipen.height / 2.0f + 18.0f + i * 15.0f + 100.0f);
		CP_Settings_Fill(button_blue);
		CP_Font_DrawText(creditsLines[i], CP_System_GetWindowWidth() / 2.0f, scrollY + digipen.height/2.0f + 15.0f + i * 15.0f + 100.0f);
	}

	// scroll upwards
	scrollY -= speed * deltaTime;

	// go back main menu (temporary)
	if (CP_Input_KeyTriggered(KEY_C)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

	//draw bubbles
	Bubbles_Draw();
}
void credits_Exit(void) {
	CP_Font_Free(montserrat_light);
	CP_Image_Free(digipen.image);
	CP_Font_Free(superwater);
	CP_Image_Free(back_arrow.image);
}  