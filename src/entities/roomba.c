/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "utils.h"
#include "bubbles.h"
#include "dirt.h"
#include "timer.h"
#include "day.h"
#include "debug.h"
#include "img_font_init.h"

#define ROOMBA_PRICE         20

// ROOMBA
float roomba_x = 0, roomba_y = 0, roomba_angle = 0;
int roomba_width = 50;
int roomba_currently_scrubbing = 0;
int ham_purchased = 0;
int closest_dirt = 0;
int roomba_strength = 3;
float roomba_speed = 100.0f;
int current_frame = 0;
float anitimer = 0.0f;
float elapsed = 2.0f;
float frame_duration = 0.1f; // seconds per frame

int GetRoombaStrength(void) {
	return roomba_strength;
}

void AddRoombaStrength(int add_roomba) {
	roomba_strength += add_roomba;
}

void MinusRoombaStrength(int minus_roomba) {
	roomba_strength -= minus_roomba;
}

void AddRoombaSpeed(int add_roomba) {
	roomba_speed += add_roomba;
	roomba_speed = CP_Math_ClampInt(roomba_speed, 0, 400);
}

void MinusRoombaSpeed(int minus_roomba) {
	roomba_speed -= minus_roomba;
}

void ResetRoomba(void) {
	ham_purchased = 0;
	roomba_strength = 3;
	roomba_speed = 100.0f;
}

void InitRoomba(void) {
	roomba_x = CP_System_GetWindowWidth() / 2;
	roomba_y = CP_System_GetWindowHeight() / 2;
}

void PurchaseRoomba(void) {
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BASELINE);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	if (!ham_purchased) {
		CP_Image_Draw(front, CP_System_GetWindowWidth()- 130, CP_System_GetWindowHeight() - 210, 200, 140, 255);
		char roomba_price[50];
		CP_Font_Set(montserrat_light);
		CP_Settings_TextSize(24.0f);
		sprintf_s(roomba_price, sizeof(roomba_price), "Cost: $%d", ROOMBA_PRICE);
		CP_Font_DrawText(roomba_price, CP_System_GetWindowWidth() - 135, CP_System_GetWindowHeight() - 120);
		if (IsAreaClicked(CP_System_GetWindowWidth() - 130, CP_System_GetWindowHeight() - 250, 190, 150, CP_Input_GetMouseX(), CP_Input_GetMouseY()) && CP_Input_MouseClicked()) {
			if (GetCurrentMoney() >= ROOMBA_PRICE) {
				DecrementMoney(ROOMBA_PRICE);
				ham_purchased = 1;
			}
			else {
				elapsed = 0.0f;
			}
		}
	}
	elapsed += CP_System_GetDt();
	if (elapsed < 2.0f)
	{
		CP_Settings_Fill(CP_Color_Create(244, 3, 48, 255));
		CP_Font_DrawText("Not enough money!", CP_System_GetWindowWidth() - 135, CP_System_GetWindowHeight() - 310);
	}
}

void RoombaFunction(void) {
	// target and position vectors
	CP_Vector roomba_v = CP_Vector_Set(roomba_x, roomba_y);
	CP_Vector dirt_v;
	closest_dirt = -1;
	roomba_currently_scrubbing = 0;
	float closestDist = 999999.0f;
	for (int i = 0; i < GetNumberOfDirt(); i++) { // 5 dirt spots: 0–4
		if (dirt_list[i].opacity > 0) { // only consider visible dirt
			float d = CP_Math_Distance(roomba_x, roomba_y, dirt_list[i].position_x, dirt_list[i].position_y);
			if (d < closestDist) {
				closestDist = d;
				closest_dirt = i;
			}
		}
	}
	dirt_v = CP_Vector_Set(dirt_list[closest_dirt].position_x, dirt_list[closest_dirt].position_y);

	// compute direction (from RoombaFunction -> dirt)
	CP_Vector dir = CP_Vector_Subtract(dirt_v, roomba_v);

	// compute facing angle
	CP_Vector up = CP_Vector_Set(0, 1);
	roomba_angle = CP_Vector_AngleCW(up, CP_Vector_Negate(dir));

	// move directly toward dirt if not close enough
	if (CheckGameRunning() && Day_IsInGameplay() && !IsTimerStopped()) {
		if (CP_Vector_Distance(dirt_v, roomba_v) > 3) {
			dir = CP_Vector_Normalize(dir);
			float speed = roomba_speed * CP_System_GetDt();
			CP_Vector move = CP_Vector_Scale(dir, speed);
			roomba_v = CP_Vector_Add(roomba_v, move);

			// update position
			roomba_x = roomba_v.x;
			roomba_y = roomba_v.y;
		}
		else {
			dirt_list[closest_dirt].opacity -= roomba_strength;
			dirt_list[closest_dirt].opacity = CP_Math_ClampInt(dirt_list[closest_dirt].opacity, 0, 200);
			SpawnBubble(dirt_list[closest_dirt].position_x, dirt_list[closest_dirt].position_y);
			roomba_currently_scrubbing = 1;
		}
	}


	// draw da RoombaFunction
	anitimer += CP_System_GetDt();

    // Switch frames when time passes threshold
	if (anitimer >= frame_duration)
	{
		anitimer = 0;
		current_frame = (current_frame + 1) % 3;
	}


	float x = CP_System_GetWindowWidth() / 2.0f;
	float y = CP_System_GetWindowHeight() / 2.0f;

	// Display the current image
	if (!IsCurrentlyDebugging()) {
		switch (current_frame)
		{
		case 0:
			CP_Image_Draw(jiggle1, roomba_x - 50, roomba_y + 30, 150, 150, 255);
			break;
		case 1:
			CP_Image_Draw(jiggle2, roomba_x - 50, roomba_y + 30, 150, 150, 255);
			break;
		case 2:
			CP_Image_Draw(jiggle3, roomba_x - 50, roomba_y + 30, 150, 150, 255);
			break;
		}
	}
	else {
		CP_Settings_Fill(roomba_color);
		CP_Graphics_DrawCircle(roomba_x, roomba_y, roomba_width);
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 100));
		CP_Graphics_DrawTriangleAdvanced(roomba_x, roomba_y - roomba_width / 2, roomba_x - roomba_width / 2 * 0.6f, roomba_y + roomba_width / 2 * 0.5f, roomba_x + roomba_width / 2 * 0.6f, roomba_y + roomba_width / 2 * 0.5f, roomba_angle);
		CP_Settings_Stroke(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawLine(roomba_x, roomba_y, dirt_list[closest_dirt].position_x, dirt_list[closest_dirt].position_y);
		CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

	}
}

int RoombaPurchase(void) {
	return ham_purchased;
}

int GetClosestDirtIndex(void) {
	return closest_dirt;
}

int GetRoombaCurrentlyScrubbing(void) {
	return roomba_currently_scrubbing;
}

float GetRoombaX(void) {
	return roomba_x;
}

float GetRoombaY(void) {
	return roomba_y;
}

float GetRoombaSpeed(void) {
	return roomba_speed;
}
