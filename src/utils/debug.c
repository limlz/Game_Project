/*************************************************************************
@file		debug.c
@Author		Lim Liang Zhou l.liangzhou@digipen.edu
@Co-authors	nil
@brief		This file contains the function definitions for the debug
			overlay that displays various game information and
			allows for debug actions.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "roomba.h"
#include "dirt.h"
#include "sponge.h"
#include "money.h"
#include "utils.h"
#include "soap.h"
#include "day.h"
#include "faucet.h"
#include "img_font_init.h"
#include <stdio.h>

#define ADD_SUB_BOX_HEIGHT			40   // height of + / - buttons
#define ADD_SUB_BOX_WIDTH			120  // width of + / - buttons
#define BOX_WIDTH 					250  // width of large debug boxes

CP_Color debug_box_color;        // background color for info boxes
CP_Color addition_color;         // color for "add" button areas
CP_Color subtraction_color;      // color for "subtract" button areas

int currently_debugging = 0;     // global flag: 1 if debug overlay is on

int IsCurrentlyDebugging(void) {
	return currently_debugging;  // getter for debug state
}

void StartDebugging(void) {
	currently_debugging = 1;     // enable debug overlay
}

void StopDebugging(void) {
	currently_debugging = 0;     // disable debug overlay
}

void DebugPrint(void) {
	// set font + render settings for debug overlay text
	CP_Font_Set(montserrat_light);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	// define colors used by debug overlay
	debug_box_color = CP_Color_Create(240, 240, 240, 255);
	addition_color = CP_Color_Create(0, 255, 0, 200);
	subtraction_color = CP_Color_Create(255, 0, 0, 200);

	// force debugging on while this function runs
	currently_debugging = 1;

	char display[50];            // buffer for formatted debug strings
	float text_x, text_y, box_x, box_y;

	// draw rectangles using top-left corner coordinates
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_TextSize(20.0f);

	// position debug UI near right edge of window
	text_x = CP_System_GetWindowWidth() - 250;
	box_x = CP_System_GetWindowWidth() - 260;
	box_y = 200.0f;

	/* ------------------ Button: Force Next Day ------------------ */
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("Force Next Day", box_x + 125, box_y);               // label
	CP_Settings_Fill(addition_color);
	CP_Graphics_DrawRect(box_x, box_y + 20, BOX_WIDTH, ADD_SUB_BOX_HEIGHT); // clickable box
	if (IsAreaClicked(box_x + BOX_WIDTH / 2, box_y + 20 + ADD_SUB_BOX_HEIGHT / 2,
		BOX_WIDTH, ADD_SUB_BOX_HEIGHT, CP_Input_GetMouseX(), CP_Input_GetMouseY())
		&& CP_Input_MouseClicked()) {
		DayForceEndDay(); // trigger next day immediately
	}

	/* ------------------ Buttons: Money + / - ------------------ */
	box_y = 290.0f;
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("Money", box_x + 125, box_y); // label

	// add money button
	CP_Settings_Fill(addition_color);
	CP_Graphics_DrawRect(box_x, box_y + 20, ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT);
	if (IsAreaClicked(box_x + ADD_SUB_BOX_WIDTH / 2, box_y + 20 + ADD_SUB_BOX_HEIGHT / 2,
		ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT, CP_Input_GetMouseX(), CP_Input_GetMouseY())
		&& CP_Input_MouseClicked()) {
		IncrementMoney(100); // add 100 money
	}

	// subtract money button
	CP_Settings_Fill(subtraction_color);
	CP_Graphics_DrawRect(box_x + 130, box_y + 20, ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT);
	if (IsAreaClicked(box_x + 130 + ADD_SUB_BOX_WIDTH / 2, box_y + 20 + ADD_SUB_BOX_HEIGHT / 2,
		ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT, CP_Input_GetMouseX(), CP_Input_GetMouseY())
		&& CP_Input_MouseClicked()) {
		DecrementMoney(100); // subtract 100 money
	}

	/* ------------------ Top-left box: FPS ------------------ */
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	text_y = 30;
	CP_Settings_Fill(debug_box_color);
	CP_Graphics_DrawRect(text_x - 10, text_y - 20, 250, 40); // background

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	sprintf_s(display, sizeof(display), "FPS: %.2f", CP_System_GetFrameRate());
	CP_Font_DrawText(display, text_x, text_y);

	/* ------------------ Roomba debug box (only if purchased) ------------------ */
	if (RoombaPurchase()) {
		text_y = CP_System_GetWindowHeight() - 290;
		CP_Settings_Fill(debug_box_color);
		CP_Graphics_DrawRect(text_x - 10, text_y - 20, 250, 160); // background panel

		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		sprintf_s(display, sizeof(display), "Roomba X: %.2f", GetRoombaX());
		CP_Font_DrawText(display, text_x, text_y);

		sprintf_s(display, sizeof(display), "Roomba Y: %.2f", GetRoombaY());
		CP_Font_DrawText(display, text_x, text_y + 20);

		sprintf_s(display, sizeof(display), "Roomba speed: %.2f", GetRoombaSpeed());
		CP_Font_DrawText(display, text_x, text_y + 40);

		sprintf_s(display, sizeof(display), "Roomba strength: %d", GetRoombaStrength());
		CP_Font_DrawText(display, text_x, text_y + 60);

		sprintf_s(display, sizeof(display), "Roomba scrubbing: %s", GetRoombaCurrentlyScrubbing() ? "Yes" : "No");
		CP_Font_DrawText(display, text_x, text_y + 80);

		sprintf_s(display, sizeof(display), "Target dirt index : %d", GetClosestDirtIndex());
		CP_Font_DrawText(display, text_x, text_y + 100);

		// shows opacity of the "closest dirt" entry
		sprintf_s(display, sizeof(display), "Target dirt opacity: %d", dirt_list[GetClosestDirtIndex()].opacity);
		CP_Font_DrawText(display, text_x, text_y + 120);
	}

	/* ------------------ Main debug info box (mouse/game stats) ------------------ */
	CP_Settings_Fill(debug_box_color);
	text_y = CP_System_GetWindowHeight() - 500;
	CP_Graphics_DrawRect(text_x - 10, text_y - 20, 250, 180); // background panel

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	sprintf_s(display, sizeof(display), "Mouse X: %.0f", CP_Input_GetMouseX());
	CP_Font_DrawText(display, text_x, text_y);

	sprintf_s(display, sizeof(display), "Mouse Y: %.0f", CP_Input_GetMouseY());
	CP_Font_DrawText(display, text_x, text_y + 20);

	sprintf_s(display, sizeof(display), "Total dirt opacity: %d", GetTotalOpacity());
	CP_Font_DrawText(display, text_x, text_y + 40);

	sprintf_s(display, sizeof(display), "Equipped sponge?: %s", IsSpongeEquipped() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y + 60);

	sprintf_s(display, sizeof(display), "Sponge power: %d", GetSpongePower());
	CP_Font_DrawText(display, text_x, text_y + 80);

	sprintf_s(display, sizeof(display), "Scrubbing?: %s", IsScrubbing() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y + 100);

	sprintf_s(display, sizeof(display), "Current Plate value: %d", GetPlateValue());
	CP_Font_DrawText(display, text_x, text_y + 120);

	sprintf_s(display, sizeof(display), "Total earnings / score: %d", GetTotalEarned());
	CP_Font_DrawText(display, text_x, text_y + 140);

	/* ------------------ Soap info box (left side) ------------------ */
	text_x = 20;
	text_y = text_y - 150;
	CP_Settings_Fill(debug_box_color);
	CP_Graphics_DrawRect(text_x - 10, text_y - 20, 250, 80); // background panel

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	sprintf_s(display, sizeof(display), "Sponge can scrub: %s", SoapCanScrub() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y);

	sprintf_s(display, sizeof(display), "Soap is full: %s", SoapIsFull() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y + 20);

	// shows current drain upgrade level (0 if none)
	sprintf_s(display, sizeof(display), "Soap drain level: %i", SoapGetDrainUpgradeLevel() ? SoapGetDrainUpgradeLevel() : 0);
	CP_Font_DrawText(display, text_x, text_y + 40);

	/* ------------------ Faucet/stream info box (left side) ------------------ */
	text_y = text_y + 110;
	CP_Settings_Fill(debug_box_color);
	CP_Graphics_DrawRect(text_x - 10, text_y - 20, 250, 120); // background panel

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	sprintf_s(display, sizeof(display), "Stream cooldown: %.2f", Faucet_ReturnCooldown());
	CP_Font_DrawText(display, text_x, text_y);

	// clamp runtime left to 0 if negative
	sprintf_s(display, sizeof(display), "Stream runtime left: %.2f", Faucet_ReturnAOETimeLeft() < 0 ? 0 : Faucet_ReturnAOETimeLeft());
	CP_Font_DrawText(display, text_x, text_y + 20);

	sprintf_s(display, sizeof(display), "Stream ready: %s", Faucet_ReturnIsAttackReady() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y + 40);

	sprintf_s(display, sizeof(display), "Stream active: %s", Faucet_ReturnStreamOn() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y + 60);

	sprintf_s(display, sizeof(display), "Stream power level: %i", Faucet_GetPowerLevel());
	CP_Font_DrawText(display, text_x, text_y + 80);

	// restore default draw modes for other rendering code after debug overlay
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
}
