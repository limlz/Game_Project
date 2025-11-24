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
//#include "discoball.h"

#define MAX_LINES 200
#define DEFAULT_SPEED 90

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
static Image background;

CP_Font montserrat_light;
CP_Font superwater;
FILE *credits_file;

CP_Color black;
CP_Color button_blue;
CP_Color white;
CP_Color digipen_red;

char *credits_lines[MAX_LINES];
int total_lines;
char buffer[256];

float scroll_y;
float speed;

float timer = 0.0f;

void credits_Init(void) {
	// initialise color
	black = CP_Color_Create(0, 0, 0, 255);
	button_blue = CP_Color_Create(123, 183, 220, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	digipen_red = CP_Color_Create(152, 1, 46, 255);

	// load image
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	digipen.image = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
	digipen.height = CP_Image_GetHeight(digipen.image) / 2.0f;
	digipen.width = CP_Image_GetWidth(digipen.image) / 2.0f;
	back_arrow.image = CP_Image_Load("Assets/back_arrow.png");
	background.image = CP_Image_Load("Assets/background.png");

	// load font
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	superwater = CP_Font_Load("Assets/SuperWater.ttf");

	// load credits file
	errno_t err = fopen_s(&credits_file, "Assets/credits.txt", "r");
	if (err != 0 || credits_file == NULL) {
		return;
	}
	total_lines = 0;
	while (fgets(buffer, sizeof(buffer), credits_file) && total_lines < MAX_LINES) {

		buffer[strcspn(buffer, "\r\n")] = 0; //removes newline

		credits_lines[total_lines] = _strdup(buffer); //allocate memory and copy line to credits_lines array
		total_lines++;
	}
	
	// unload credits file
	fclose(credits_file);

	//set credit scroll speed
	scroll_y = (float)CP_System_GetWindowHeight();
	speed = DEFAULT_SPEED;

	//initialise bubble
	BubblesInit();

	//initialise disco
	//InitDiscoLighting();
}
void credits_Update(void) {
	float dt = CP_System_GetDt();
	timer += dt;
	mouse.x = CP_Input_GetMouseX();
	mouse.y = CP_Input_GetMouseY();

	//initialise popping effect for settings button
	float back_pop = 0.0f;

	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));
	CP_Image_Draw(background.image, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	CP_Image_Draw(digipen.image, CP_System_GetWindowWidth() / 2.0f, scroll_y, digipen.width, digipen.height, 255);

	CP_Settings_Fill(digipen_red);
	CP_Font_Set(montserrat_light);
	CP_Font_DrawText("All Content Copyright (C) 2025 Digipen Institute of Technology Singpoare. All Rights Reserved.", CP_System_GetWindowWidth() / 2.0f + 3.0f, scroll_y + digipen.height / 2.0f + 30.0f);

	if (CP_Input_MouseDown(MOUSE_BUTTON_1) || CP_Input_KeyDown(KEY_SPACE) || CP_Input_KeyDown(KEY_ENTER)) {
		speed = 400.0f;
	}
	else {
		speed = DEFAULT_SPEED;
	}

	CP_Settings_NoStroke();
	if (IsAreaClicked(120, (float)CP_System_GetWindowHeight() - 120, 100, 100, mouse.x, mouse.y)) {
		//set popping effect of button active with value of 10.0f
		back_pop = 10.0f;

		//draw bubble at mouse cursor
		BubblesManual(mouse.x, mouse.y);

		//draw back button tool tip
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
	//draw back arrow button with popping effect
	CP_Image_Draw(back_arrow.image, 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + back_pop, 100.0f + back_pop, 255);

	// Draw credits
	CP_Font_Set(superwater);
	CP_Settings_TextSize(30.0f);
	for (int i = 0; i < total_lines; i++) {
		CP_Settings_Fill(white);
		CP_Font_DrawText(credits_lines[i], CP_System_GetWindowWidth() / 2.0f + 3.0f, scroll_y + digipen.height / 2.0f + 18.0f + i * 15.0f + 100.0f);
		CP_Settings_Fill(button_blue);
		CP_Font_DrawText(credits_lines[i], CP_System_GetWindowWidth() / 2.0f, scroll_y + digipen.height/2.0f + 15.0f + i * 15.0f + 100.0f);
	}

	// scroll upwards
	scroll_y -= speed * dt;

	// go back main menu (temporary)
	if (CP_Input_KeyTriggered(KEY_C)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

	//draw bubbles
	Bubbles_Draw();

	//disco lighting
	//UpdateDiscoLighting(dt);

	//go back to main menu after credit ends with buffer 30 pixels
	if (GetCreditsEndPosition() < 0.0f - 30.0f) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}
void credits_Exit(void) {
	//free assets
	CP_Font_Free(montserrat_light);
	CP_Image_Free(digipen.image);
	CP_Font_Free(superwater);
	CP_Image_Free(back_arrow.image);
}  

float GetTotalCreditsDuration(void)
{	
	//to calculate total credits run time
	float window_height = (float)CP_System_GetWindowHeight();
	float image_position_height = digipen.height / 2.0f;
	float total_height = window_height + image_position_height + 15.0f * total_lines + 100.0f; //15.0f for the buffer between lines
																							   //100.0f for the buffer betwee image and first line

	return total_height / speed;  
}

float GetCreditsEndPosition(void)
{
	//to calculate the tail(end) position of the credits
	float last_line_y = 
		scroll_y
		+ digipen.height / 2.0f
		+ 15.0f * total_lines
		+ 100.0f;

	return last_line_y;
}