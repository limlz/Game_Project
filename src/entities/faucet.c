/*************************************************************************
@file		faucet.c
@Author		Nur Nadia Natasya Binte Mohd Taha nurnadianatasya.b@digipen.edu
@Co-authors Tan Choon Ming choonming.tan@digipen.edu
@brief		This file contains the function definitions for the faucet entity,
			including stream drawing and updating, as well as faucet drawing.
			The faucet releases a stream of water droplets that can clean dirt
			on the plate when activated. The stream's behavior, including its
			position, velocity, and interaction with dirt, is managed within
			this file.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include <stdio.h>
#include "dirt.h"
#include "timer.h"
#include "day.h"
#include "faucet.h"
#include "img_font_init.h"

/* ----------------------------------------------------------------------
   Forward declarations for upgrade-related helpers implemented in upgrades.c.
   These provide faucet power/cooldown info shared with the upgrades system.
   ---------------------------------------------------------------------- */
float FaucetGetPowerMultiplier(void);
int   FaucetGetCooldownLevel(void);
float FaucetGetCooldownBase(void);

#define MAX_DROPLETS		220
#define AOE_DURATION		3.0f

/* Per-level cooldown behaviour for the faucet stream. */
static const float FaucetCooldownReductionPerLevel = 1.0f;
static const int   FaucetCooldownMaxLevel = 10;

typedef struct {
	CP_Vector	position;
	CP_Vector	velocity;
	float		radius;
} Stream;

Stream streamlist[MAX_DROPLETS];

/* Tracks fractional extra cleaning power that builds up over multiple hits. */
static float faucetPowerOverflow = 0.0f;

static void Faucet_UpdateCooldownValue(void);

//*----------------------   DRAWING FUNCTIONS : FOR STREAM AND FAUCET   ------------------------*//

static int		stream_on = 0;
static int		faucet_on = 0;
static float	rotation1 = 45.0f;
static float	rotation2 = 90.0f;
int				movement = 1;

void DrawStream(void)
{
	for (int i = 0; i < MAX_DROPLETS; i++)
	{
		CP_Settings_Stroke(CP_Color_Create(123, 183, 220, 100));
		CP_Settings_Fill(CP_Color_Create(135, 206, 235, 120));
		CP_Graphics_DrawCircle(streamlist[i].position.x, streamlist[i].position.y, 40.0f);
		CP_Settings_NoStroke();
	}
}

void DrawFaucet(void)
{
	float faucet_x = CP_System_GetWindowWidth() * 0.5f;
	float faucet_y = 35.0f;
	float radius = 140.0f;
	float rotation1_max = 135.0f;
	float rotation2_max = 180.0f;

	if (CP_Input_KeyTriggered(KEY_W))
		faucet_on = 1;

	if (CheckGameRunning() && Day_IsInGameplay())
	{
		if (faucet_on == 1)
		{
			rotation1 += 60.0f * CP_System_GetDt() * movement;
			rotation2 += 60.0f * CP_System_GetDt() * movement;

			if (rotation1 >= rotation1_max && rotation2 >= rotation2_max)
			{
				movement = -1;
			}
			if (movement == -1 && rotation1 <= 45.0f && rotation2 <= 90.0f)
			{
				movement = 1;
				faucet_on = 0;
			}
		}
	}

	CP_Settings_Stroke(CP_Color_Create(206, 216, 220, 250));
	CP_Settings_Fill(CP_Color_Create(164, 164, 164, 255));
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation1, 5.0f);
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation2, 5.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 225));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, radius - 20.0f);
}

//*-----------------------------------   STREAM CONTROLS   -------------------------------------*//

void InitStream(void)
{
	Faucet_UpdateCooldownValue();
	faucetPowerOverflow = 0.0f;
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float offset = 300.0f; //diameter of plate = 600.0f (plate.c)

	for (int i = 0; i < MAX_DROPLETS; i++)
	{
		streamlist[i].position = CP_Vector_Set(CP_Random_RangeFloat(center_x - offset, center_x + offset),
			CP_Random_RangeFloat((float)-CP_System_GetWindowHeight(), -20.0f));
		streamlist[i].velocity = CP_Vector_Set(0, 0);
	}
}

void UpdateStream(void)
{
	for (int i = 0; i < MAX_DROPLETS; i++)
	{
		streamlist[i].velocity = CP_Vector_Set(0, CP_Random_RangeFloat(4, 8));
		streamlist[i].position = CP_Vector_Add(streamlist[i].position, streamlist[i].velocity);

		if (streamlist[i].position.y > CP_System_GetWindowHeight())
			streamlist[i].position.y = CP_Random_RangeFloat(-100.0f, 0.0f);
	}
}

// function to stop the stream but ensure the droplets keep falling; to reset their position, 
// with no velocity when they go off screen

void StopStream(void)
{
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float offset = 300.0f;

	for (int i = 0; i < MAX_DROPLETS; i++)
	{
		streamlist[i].position = CP_Vector_Add(streamlist[i].position, streamlist[i].velocity);

		if (streamlist[i].position.y > CP_System_GetWindowHeight()) {
			streamlist[i].position.y = CP_Random_RangeFloat(-100.0f, 0.0f);
			streamlist[i].position = CP_Vector_Set(CP_Random_RangeFloat(center_x - offset, center_x + offset),
				CP_Random_RangeFloat((float)-CP_System_GetWindowHeight(), -20.0f));
			streamlist[i].velocity = CP_Vector_Set(0, 0);
		}

	}
}

//*----------------------   STREAM IMPLEMENTATION : TIMER & COOLDOWN   -------------------------*//

static float	time_left = 0.0f;
static float	cooldown = 0.0f;
static float	cooldown_left = 0.0f;

static int		attack_ready = 1;
int				opacity = 0;

void DrawStreamTimer(void)
{
	float radius = 60.0f;
	float faucet_x = 110.0f;
	float faucet_y = 705.0f;
	int max_opacity = 150;

	char timer_text[5];

	// white circular background for logo
	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, 2 * radius + 0.5f);

	// faucet mini drawing
	CP_Settings_Stroke(CP_Color_Create(206, 216, 220, 250));
	CP_Settings_Fill(CP_Color_Create(164, 164, 164, 255));
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation1, 5.0f);
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation2, 5.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 225));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, radius - 20.0f);

	if (attack_ready == 1)
	{
		opacity = 0;
	}

	if (CheckGameRunning() && Day_IsInGameplay())
	{
		if (attack_ready == 0) {
			/* Use the base cooldown value from the upgrades system. */
			if (cooldown_left >= 0.0f && cooldown_left <= FaucetGetCooldownBase())
			{
				opacity = max_opacity;
				cooldown_left -= CP_System_GetDt();
			}
			else
			{
				attack_ready = 1;
			}
		}
	}

	// opacity circle for logo
	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, opacity));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, 2 * radius + 0.5f);

	// [W] text
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
	CP_Settings_TextSize(20.0f);
	CP_Font_DrawText("Stream [W]", 110.0f, 780.0f);

	if (attack_ready == 0)
	{
		CP_Font_Set(sub_font);
		CP_Settings_Fill(red);
		CP_Settings_TextSize(100.0f);
		snprintf(timer_text, sizeof(timer_text), "%.0f", cooldown_left);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText(timer_text, faucet_x, faucet_y);
	}
	CP_Font_Set(montserrat_light);
}


//*---------------------   STREAM IMPLEMENTATION : AOE CLEANING EFFECT   -----------------------*//

static float	aoe_time_left = 0.0f;

void CleanDirtWithStream(void)
{
	if (CheckGameRunning() && Day_IsInGameplay())
	{
		for (int i = 0; i < MAX_DROPLETS; i++) {

			for (int j = 0; j < GetNumberOfDirt(); j++)
			{
				float distance = CP_Math_Distance(streamlist[i].position.x, streamlist[i].position.y, dirt_list[j].position_x, dirt_list[j].position_y);
				float cleaning_radius = 50.0f;

				if (distance < cleaning_radius)
				{
					/* Use upgraded faucet power multiplier from the upgrades system. */
					float multiplier = FaucetGetPowerMultiplier();
					float damage = multiplier;
					int removal = (int)damage;
					float fractional = damage - (float)removal;
					faucetPowerOverflow += fractional;

					if (faucetPowerOverflow >= 1.0f)
					{
						int bonus = (int)faucetPowerOverflow;
						removal += bonus;
						faucetPowerOverflow -= (float)bonus;
					}

					if (removal < 1)
					{
						removal = 1;
					}

					dirt_list[j].opacity -= removal;

					if (dirt_list[j].opacity < 0)
						dirt_list[j].opacity = 0;
				}
			}
		}
	}
}


void AOEStream(void)
{
	DrawStreamTimer();
	DrawStream();
	CleanDirtWithStream();

	if (CP_Input_KeyTriggered(KEY_W) && attack_ready == 1 && stream_on == 0)
	{
		stream_on = 1;
		aoe_time_left = AOE_DURATION;
	}

	//set to 3 seconds for AOE attack
	if (CheckGameRunning() && Day_IsInGameplay())
	{
		if (stream_on == 1)
		{
			UpdateStream();
			aoe_time_left -= CP_System_GetDt();

			if (aoe_time_left <= 0.0f)
			{
				stream_on = 0;
				attack_ready = 0;
				cooldown_left = cooldown;
			}
		}
		else
		{
			StopStream();
		}
	}
	else if (!Day_IsInGameplay())
	{
		InitStream();
		stream_on = 0;
		attack_ready = 1;
	}
}

/* Re-computes the effective cooldown value based on the current upgrade level. */
static void Faucet_UpdateCooldownValue(void) {
	/* Minimum cooldown allowed based on maximum upgrade level. */
	float minCooldown = FaucetGetCooldownBase() - (float)FaucetCooldownMaxLevel * FaucetCooldownReductionPerLevel;
	if (minCooldown < 0.0f) {
		minCooldown = 0.0f;
	}

	/* Reduce cooldown by current upgrade level. */
	float newCooldown = FaucetGetCooldownBase() - (float)FaucetGetCooldownLevel() * FaucetCooldownReductionPerLevel;
	if (newCooldown < minCooldown) {
		newCooldown = minCooldown;
	}

	cooldown = newCooldown;
	if (cooldown_left > cooldown) {
		cooldown_left = cooldown;
	}
}

/* Reduces the cooldown by a custom amount, clamped by the minimum allowed. */
void Faucet_ReduceCooldown(float reduction)
{
	float minCooldown = FaucetGetCooldownBase() - (float)FaucetCooldownMaxLevel * FaucetCooldownReductionPerLevel;

	if (minCooldown < 0.0f)
	{
		minCooldown = 0.0f;
	}

	cooldown -= reduction;
	if (cooldown < minCooldown)
	{
		cooldown = minCooldown;
	}

	if (cooldown_left > cooldown)
	{
		cooldown_left = cooldown;
	}
}

/* Re-syncs the cooldown timer when faucet cooldown upgrades change. */
void Faucet_ResetCooldown(void) { Faucet_UpdateCooldownValue(); }

/* Returns the current faucet cooldown value (seconds). */
float Faucet_ReturnCooldown(void) { return cooldown; }

/* Returns remaining AOE stream time left (seconds). */
float Faucet_ReturnAOETimeLeft(void) { return aoe_time_left; }

/* Returns non-zero if the stream attack is ready to be fired. */
int Faucet_ReturnIsAttackReady(void) { return attack_ready; }

/* Returns non-zero if the stream is currently active. */
int Faucet_ReturnStreamOn(void) { return stream_on; }
