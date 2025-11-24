/*************************************************************************
@file		roomba.c
@Author		lim liang zhou (l.liangzhou@digipen.edu)
@Co-authors	Nur Nadia Natasya Binte Mohd Taha (nurnadianatasya.b@digipen.edu)
@brief		This file contains the function definitions for the Roomba entity,
			including its initialization, purchasing, movement toward dirt,
			scrubbing functionality, and idle behaviour when unassigned.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/

#include <math.h>
#include "cprocessing.h"
#include "utils.h"
#include "bubbles.h"
#include "dirt.h"
#include "timer.h"
#include "day.h"
#include "debug.h"
#include "roomba.h"
#include "img_font_init.h"

#ifndef MAX_ROOMBA
#define MAX_ROOMBA 4
#endif

// Just needs to be >= maximum number of dirts you ever spawn
#ifndef MAX_DIRT
#define MAX_DIRT 64
#endif

#define ROOMBA_PRICE         20
#define ROOMBA_SCRUB_RADIUS  5.0f   // Radius within which Roomba starts scrubbing

// -----------------------------------------------------------------------------
// Internal Roomba representation
// -----------------------------------------------------------------------------
typedef struct RoombaData
{
	float position_x;
	float position_y;
	float angle;
	int   current_dirt_index;
	int   currently_scrubbing;
	int   current_frame;
	float anim_timer;
} RoombaData;

// ROOMBA SYSTEM STATE
static RoombaData roombas[MAX_ROOMBA];

int   roomba_width = 50;
int   roomba_currently_scrubbing = 0;    // 1 if any Roomba is scrubbing this frame
int   ham_purchased = 0;    // number of active Roombas
int   roomba_strength = 3;
float roomba_speed = 100.0f;
float elapsed = 2.0f;
float frame_duration = 0.1f; // seconds per frame

// -----------------------------------------------------------------------------
// Getters / setters
// -----------------------------------------------------------------------------
int GetRoombaStrength(void)
{
	return roomba_strength;
}

void AddRoombaStrength(int add_roomba)
{
	roomba_strength += add_roomba;
}

void MinusRoombaStrength(int minus_roomba)
{
	roomba_strength -= minus_roomba;
}

void AddRoombaSpeed(int add_roomba)
{
	roomba_speed += (float)add_roomba;
	roomba_speed = (float)CP_Math_ClampInt((int)roomba_speed, 0, 400);
}

void MinusRoombaSpeed(int minus_roomba)
{
	roomba_speed -= (float)minus_roomba;
}

float GetRoombaSpeed(void)
{
	return roomba_speed;
}

// Returns number of Roombas purchased (0 means none)
int RoombaPurchase(void)
{
	return ham_purchased;
}

// For backward compatibility: use the first Roomba’s dirt index / position
int GetClosestDirtIndex(void)
{
	if (ham_purchased > 0)
		return roombas[0].current_dirt_index;
	return -1;
}

int GetRoombaCurrentlyScrubbing(void)
{
	return roomba_currently_scrubbing;
}

float GetRoombaX(void)
{
	if (ham_purchased > 0)
		return roombas[0].position_x;
	return 0.0f;
}

float GetRoombaY(void)
{
	if (ham_purchased > 0)
		return roombas[0].position_y;
	return 0.0f;
}

// -----------------------------------------------------------------------------
// Lifecycle
// -----------------------------------------------------------------------------
void ResetRoomba(void)
{
	ham_purchased = 0;
	roomba_strength = 3;
	roomba_speed = 100.0f;
	roomba_currently_scrubbing = 0;
	elapsed = 2.0f;

	for (int i = 0; i < MAX_ROOMBA; ++i)
	{
		roombas[i].position_x = 0.0f;
		roombas[i].position_y = 0.0f;
		roombas[i].angle = 0.0f;
		roombas[i].current_dirt_index = -1;
		roombas[i].currently_scrubbing = 0;
		roombas[i].current_frame = 0;
		roombas[i].anim_timer = 0.0f;
	}
}

void InitRoomba(void)
{
	// Just reset the system; Roombas will be placed when purchased
	ResetRoomba();
}

// -----------------------------------------------------------------------------
// Purchasing
// -----------------------------------------------------------------------------
void PurchaseRoomba(void)
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BASELINE);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

	// Allow purchasing until we hit MAX_ROOMBA
	if (ham_purchased < MAX_ROOMBA)
	{
		CP_Image_Draw(front,
			CP_System_GetWindowWidth() - 130,
			CP_System_GetWindowHeight() - 210,
			200, 140, 255);

		char roomba_price[50];
		CP_Font_Set(montserrat_light);
		CP_Settings_TextSize(24.0f);
		sprintf_s(roomba_price, sizeof(roomba_price), "Cost: $%d", ROOMBA_PRICE);
		CP_Font_DrawText(roomba_price,
			CP_System_GetWindowWidth() - 135,
			CP_System_GetWindowHeight() - 120);

		if (IsAreaClicked(CP_System_GetWindowWidth() - 130,
			CP_System_GetWindowHeight() - 250,
			190, 150,
			CP_Input_GetMouseX(),
			CP_Input_GetMouseY()) &&
			CP_Input_MouseClicked())
		{
			if (GetCurrentMoney() >= ROOMBA_PRICE)
			{
				DecrementMoney(ROOMBA_PRICE);

				// Spawn a new Roomba at center
				float start_x = CP_System_GetWindowWidth() / 2.0f;
				float start_y = CP_System_GetWindowHeight() / 2.0f;

				roombas[ham_purchased].position_x = start_x;
				roombas[ham_purchased].position_y = start_y;
				roombas[ham_purchased].angle = 0.0f;
				roombas[ham_purchased].current_dirt_index = -1;
				roombas[ham_purchased].currently_scrubbing = 0;
				roombas[ham_purchased].current_frame = 0;
				roombas[ham_purchased].anim_timer = 0.0f;

				ham_purchased++;
			}
			else
			{
				// Trigger "Not enough money!" message
				elapsed = 0.0f;
			}
		}
	}

	// Not enough money feedback timer
	elapsed += CP_System_GetDt();
	if (elapsed < 2.0f)
	{
		CP_Settings_Fill(CP_Color_Create(244, 3, 48, 255));
		CP_Font_DrawText("Not enough money!",
			CP_System_GetWindowWidth() - 135,
			CP_System_GetWindowHeight() - 310);
	}
}

// -----------------------------------------------------------------------------
// Update & Draw
// Multiple Roombas:
//  - Nearest-neighbor global matching between Roombas and visible dirts
//  - Automatic reassignment each frame
//  - Roombas with no dirt target go to a rest area and wiggle/idly spin
// -----------------------------------------------------------------------------
void RoombaFunction(void)
{
	if (ham_purchased <= 0)
		return;

	int dirt_count = GetNumberOfDirt();
	int maxR = ham_purchased;
	if (maxR > MAX_ROOMBA)
		maxR = MAX_ROOMBA;

	int maxD = dirt_count;
	if (maxD > MAX_DIRT)
		maxD = MAX_DIRT;

	float dt = CP_System_GetDt();

	// -----------------------------------------------------------------
	// Build distance matrix for visible dirts only
	// -----------------------------------------------------------------
	float dist[MAX_ROOMBA][MAX_DIRT];
	int has_any_visible_dirt = 0;

	if (maxD > 0)
	{
		for (int r = 0; r < maxR; ++r)
		{
			for (int d = 0; d < maxD; ++d)
			{
				if (dirt_list[d].opacity <= 0)
				{
					// treat invisible/cleaned dirt as "infinitely far"
					dist[r][d] = 999999.0f;
				}
				else
				{
					has_any_visible_dirt = 1;
					dist[r][d] = CP_Math_Distance(
						roombas[r].position_x, roombas[r].position_y,
						dirt_list[d].position_x, dirt_list[d].position_y);
				}
			}
		}
	}

	// Greedy global nearest-pair matching
	int assigned_dirt_for_roomba[MAX_ROOMBA];
	int assigned_roomba_for_dirt[MAX_DIRT];

	for (int r = 0; r < maxR; ++r)
		assigned_dirt_for_roomba[r] = -1;
	for (int d = 0; d < maxD; ++d)
		assigned_roomba_for_dirt[d] = -1;

	if (has_any_visible_dirt)
	{
		int maxAssignments = (maxR < maxD) ? maxR : maxD;
		int assignments = 0;

		while (assignments < maxAssignments)
		{
			float bestDist = 999999.0f;
			int bestR = -1;
			int bestD = -1;

			for (int r = 0; r < maxR; ++r)
			{
				if (assigned_dirt_for_roomba[r] != -1)
					continue;

				for (int d = 0; d < maxD; ++d)
				{
					if (assigned_roomba_for_dirt[d] != -1)
						continue;

					if (dist[r][d] < bestDist)
					{
						bestDist = dist[r][d];
						bestR = r;
						bestD = d;
					}
				}
			}

			if (bestR == -1 || bestD == -1)
				break;

			assigned_dirt_for_roomba[bestR] = bestD;
			assigned_roomba_for_dirt[bestD] = bestR;
			++assignments;
		}
	}

	roomba_currently_scrubbing = 0;

	// Rest "home" area: around the CENTER of the screen in a small circle
	float center_x = CP_System_GetWindowWidth() / 2.0f;
	float center_y = CP_System_GetWindowHeight() / 2.0f;
	float rest_radius = 40.0f;
	float angle_step = (maxR > 0) ? (2.0f * 3.14159265f / maxR) : 0.0f;

	for (int r = 0; r < maxR; ++r)
	{
		int target_index = assigned_dirt_for_roomba[r];
		roombas[r].current_dirt_index = target_index;
		roombas[r].currently_scrubbing = 0;

		int is_game_running = CheckGameRunning() && Day_IsInGameplay() && !IsTimerStopped();
		int is_idle = (!is_game_running || target_index == -1);

		if (!is_idle)
		{
			// -----------------------------------------------------------------
			// WORK MODE: Move toward assigned dirt and scrub (no overshoot)
			// -----------------------------------------------------------------
			CP_Vector roomba_v = CP_Vector_Set(roombas[r].position_x, roombas[r].position_y);
			CP_Vector dirt_v = CP_Vector_Set(
				dirt_list[target_index].position_x,
				dirt_list[target_index].position_y);

			CP_Vector dir = CP_Vector_Subtract(dirt_v, roomba_v);
			CP_Vector up = CP_Vector_Set(0, 1);

			// Distance to dirt center
			float dist_to_dirt = CP_Vector_Distance(dirt_v, roomba_v);

			// Update angle (used for debug)
			roombas[r].angle = CP_Vector_AngleCW(up, CP_Vector_Negate(dir));

			const float scrub_radius = ROOMBA_SCRUB_RADIUS;

			if (dist_to_dirt > scrub_radius)
			{
				dir = CP_Vector_Normalize(dir);

				// Desired step this frame
				float max_step = roomba_speed * dt;

				// Clamp step so we never overshoot the dirt
				if (max_step > dist_to_dirt)
					max_step = dist_to_dirt;

				CP_Vector move = CP_Vector_Scale(dir, max_step);
				roomba_v = CP_Vector_Add(roomba_v, move);

				roombas[r].position_x = roomba_v.x;
				roombas[r].position_y = roomba_v.y;
			}
			else
			{
				// Scrub!
				dirt_list[target_index].opacity -= roomba_strength;
				dirt_list[target_index].opacity =
					CP_Math_ClampInt(dirt_list[target_index].opacity, 0, 200);

				SpawnBubble(dirt_list[target_index].position_x,
					dirt_list[target_index].position_y);

				roombas[r].currently_scrubbing = 1;
				roomba_currently_scrubbing = 1;
			}
		}
		else
		{
			// -----------------------------------------------------------------
			// IDLE MODE: Go "home" to the center & wiggle around
			// -----------------------------------------------------------------
			float target_angle = angle_step * r;
			CP_Vector rest_target = CP_Vector_Set(
				center_x + cosf(target_angle) * rest_radius,
				center_y + sinf(target_angle) * rest_radius);

			CP_Vector pos = CP_Vector_Set(roombas[r].position_x, roombas[r].position_y);
			float dist_to_rest = CP_Vector_Distance(pos, rest_target);

			// Walk to home spot
			if (dist_to_rest > 3.0f)
			{
				CP_Vector dir = CP_Vector_Subtract(rest_target, pos);
				dir = CP_Vector_Normalize(dir);
				float speed = roomba_speed * 0.5f * dt; // slower, lazy walk
				CP_Vector move = CP_Vector_Scale(dir, speed);
				pos = CP_Vector_Add(pos, move);

				roombas[r].position_x = pos.x;
				roombas[r].position_y = pos.y;
			}

			// Slow spin while idle (visible in debug)
			roombas[r].angle += dt * 1.5f;
		}

		// Animation timer
		roombas[r].anim_timer += dt;
		if (roombas[r].anim_timer >= frame_duration)
		{
			roombas[r].anim_timer = 0.0f;
			roombas[r].current_frame = (roombas[r].current_frame + 1) % 3;
		}

		int is_idle_now = (!CheckGameRunning() || !Day_IsInGameplay() || IsTimerStopped() || target_index == -1);
		float draw_x = roombas[r].position_x;
		float draw_y = roombas[r].position_y;

		// Draw
		if (!IsCurrentlyDebugging())
		{
			switch (roombas[r].current_frame)
			{
			case 0:
				CP_Image_Draw(jiggle1, draw_x - 50,
					draw_y + 30, 150, 150, 255);
				break;
			case 1:
				CP_Image_Draw(jiggle2, draw_x - 50,
					draw_y + 30, 150, 150, 255);
				break;
			case 2:
				CP_Image_Draw(jiggle3, draw_x - 50,
					draw_y + 30, 150, 150, 255);
				break;
			}
		}
		else
		{
			// Debug visualization
			CP_Settings_Fill(roomba_color);
			CP_Graphics_DrawCircle(roombas[r].position_x,
				roombas[r].position_y,
				roomba_width);

			CP_Settings_Fill(CP_Color_Create(255, 0, 0, 100));
			CP_Graphics_DrawTriangleAdvanced(
				roombas[r].position_x,
				roombas[r].position_y - roomba_width / 2,
				roombas[r].position_x - roomba_width / 2 * 0.6f,
				roombas[r].position_y + roomba_width / 2 * 0.5f,
				roombas[r].position_x + roomba_width / 2 * 0.6f,
				roombas[r].position_y + roomba_width / 2 * 0.5f,
				roombas[r].angle);

			if (!is_idle_now && target_index != -1)
			{
				CP_Settings_Stroke(CP_Color_Create(255, 0, 0, 255));
				CP_Graphics_DrawLine(
					roombas[r].position_x,
					roombas[r].position_y,
					dirt_list[target_index].position_x,
					dirt_list[target_index].position_y);
				CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
			}
		}
	}
}
