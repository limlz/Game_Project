#include "cprocessing.h"
#include <stdio.h>
#include "dirt.h"
#include "timer.h"
#include "day.h"
#include "faucet.h"
#include "img_font_init.h"

#define MAX_DROPLETS		220
#define AOE_DURATION		3.0f

static const float FaucetBaseCooldown = 20.0f;
static const float FaucetCooldownReductionPerLevel = 1.0f;
static const int FaucetCooldownMaxLevel = 10;

typedef struct {
	CP_Vector	position;
	CP_Vector	velocity;
	float		radius;
} Stream;

Stream streamlist[MAX_DROPLETS];

static float faucetPowerOverflow = 0.0f;

static void Faucet_UpdateCooldownValue(void);

//*----------------------   DRAWING FUNCTIONS : FOR STREAM AND FAUCET   ------------------------*//

static int		stream_on = 0;
static int		faucet_on = 0;
static float	rotation1 = 45.0f;
static float	rotation2 = 90.0f;
int				movement = 1;

void draw_stream(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		CP_Settings_Stroke(CP_Color_Create(123, 183, 220, 100));
		CP_Settings_Fill(CP_Color_Create(135, 206, 235, 120));
		CP_Graphics_DrawCircle(streamlist[i].position.x, streamlist[i].position.y, 40.0f);
		CP_Settings_NoStroke();
	}
}

void draw_faucet(void) {
	float faucet_x = CP_System_GetWindowWidth() * 0.5f;
	float faucet_y = 35.0f;
	float radius = 140.0f;
	float rotation1_max = 135.0f;
	float rotation2_max = 180.0f;

	if (CP_Input_KeyTriggered(KEY_W))
		faucet_on = 1;

	if (CheckGameRunning() && Day_IsInGameplay()) {
		if (faucet_on == 1) {
			rotation1 += 60.0f * CP_System_GetDt() * movement;
			rotation2 += 60.0f * CP_System_GetDt() * movement;
			if (rotation1 >= rotation1_max && rotation2 >= rotation2_max) {
				movement = -1;
			}
			if (movement == -1 && rotation1 <= 45.0f && rotation2 <= 90.0f) {
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


void stream_init(void) {
	faucetPowerOverflow = 0.0f;
	Faucet_UpdateCooldownValue();

	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float offset = 300.0f; //diameter of plate = 600.0f (plate.c)

	for (int i = 0; i < MAX_DROPLETS; i++) {
		streamlist[i].position = CP_Vector_Set(CP_Random_RangeFloat(center_x - offset, center_x + offset),
			CP_Random_RangeFloat((float)-CP_System_GetWindowHeight(), -20.0f));
		streamlist[i].velocity = CP_Vector_Set(0, 0);
	}
}

void update_stream(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		streamlist[i].velocity = CP_Vector_Set(0, CP_Random_RangeFloat(4, 8));
		streamlist[i].position = CP_Vector_Add(streamlist[i].position, streamlist[i].velocity);

		if (streamlist[i].position.y > CP_System_GetWindowHeight())
			streamlist[i].position.y = CP_Random_RangeFloat(-100.0f, 0.0f);
	}

}

// function to stop the stream but ensure the droplets keep falling; to reset their position, 
// with no velocity when they go off screen

void stop_stream(void) {
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float offset = 300.0f;

	for (int i = 0; i < MAX_DROPLETS; i++) {
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
int		opacity = 0;



void draw_stream_timer(void) {

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

	if (attack_ready == 1) {
		opacity = 0;
	}

	if (CheckGameRunning() && Day_IsInGameplay()) {
		if (attack_ready == 0) {
			if (cooldown_left >= 0.0f && cooldown_left <= FaucetBaseCooldown) {
				opacity = max_opacity;
				cooldown_left -= CP_System_GetDt();
			}
			else {
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

	if (attack_ready == 0) {
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

void clean_dirt_with_stream(void) {
	if (CheckGameRunning() && Day_IsInGameplay()) {
		for (int i = 0; i < MAX_DROPLETS; i++) {
			for (int j = 0; j < GetNumberOfDirt(); j++) {
				float distance = CP_Math_Distance(streamlist[i].position.x, streamlist[i].position.y, dirt_list[j].position_x, dirt_list[j].position_y);
				float cleaning_radius = 50.0f;

				if (distance < cleaning_radius) {
					float multiplier = Faucet_GetPowerMultiplier();
					float damage = multiplier;
					int removal = (int)damage;
					float fractional = damage - (float)removal;

					faucetPowerOverflow += fractional;
					if (faucetPowerOverflow >= 1.0f) {
						int bonus = (int)faucetPowerOverflow;
						removal += bonus;
						faucetPowerOverflow -= (float)bonus;
					}

					if (removal < 1) {
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


void AOE_stream() {
	draw_stream_timer();
	draw_stream();
	clean_dirt_with_stream();

	if (CP_Input_KeyTriggered(KEY_W) && attack_ready == 1 && stream_on == 0) {
		stream_on = 1;
		aoe_time_left = AOE_DURATION;
	}

	//set to 3 seconds for AOE attack
	if (CheckGameRunning() && Day_IsInGameplay()) {
		if (stream_on == 1) {
			update_stream();
			aoe_time_left -= CP_System_GetDt();
			if (aoe_time_left <= 0.0f) {
				stream_on = 0;
				attack_ready = 0;
				cooldown_left = cooldown;
			}

		}
		else {
			stop_stream();
		}
	}
}

static void Faucet_UpdateCooldownValue(void) {
	float minCooldown = FaucetBaseCooldown - (float)FaucetCooldownMaxLevel * FaucetCooldownReductionPerLevel;
	if (minCooldown < 0.0f) {
		minCooldown = 0.0f;
	}

	float newCooldown = FaucetBaseCooldown - (float)Faucet_GetCooldownLevel() * FaucetCooldownReductionPerLevel;
	if (newCooldown < minCooldown) {
		newCooldown = minCooldown;
	}

	cooldown = newCooldown;
	if (cooldown_left > cooldown) {
		cooldown_left = cooldown;
	}
}

void reduce_cooldown(float reduction) {
	float minCooldown = FaucetBaseCooldown - (float)FaucetCooldownMaxLevel * FaucetCooldownReductionPerLevel;
	if (minCooldown < 0.0f) {
		minCooldown = 0.0f;
	}

	cooldown -= reduction;
	if (cooldown < minCooldown) {
		cooldown = minCooldown;
	}

	if (cooldown_left > cooldown) {
		cooldown_left = cooldown;
	}
}

void reset_cooldown(void) {
	Faucet_UpdateCooldownValue();
}

float return_cooldown(void) {
	return cooldown;
}

float return_aoe_time_left(void) {
	return aoe_time_left;
}

int return_is_attack_ready(void) {
	return attack_ready;
}

int return_stream_on(void) {
	return stream_on;
}