/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include <stdio.h>
#include "timer.h"
#include "mainmenu.h"
#include "plate.h"
#include "dirt.h"
#include "game.h"
#include "img_font_init.h"
#include "confetti.h"
#include "shop.h"
#include "tutorial.h"

static int show_day_complete_popup = 0;  // 1 = popup visible
static const int DAY_BASE_GOAL = 5;
static const int GOAL_STEP = 1;

static int day = 0;
static int goal = 5;
static int cleaned = 0;
static int in_gameplay = 1; // 1 = washing, 0 = popup/shop
static int ready_for_next_day = 0;   // 1 means: show Next Day button in Shop
static float time_beforenext;

int TutorialFinishing = 1;

void Day_Init(void) {
    day = 0;
    goal = DAY_BASE_GOAL;
    cleaned = 0;
    in_gameplay = 1;
    show_day_complete_popup = 0;
    time_beforenext = 1.5f;
}

void Day_StartCurrentDay(void) {
    cleaned = 0;
    in_gameplay = 1;
    show_day_complete_popup = 0;
}

void Day_OnPlateCleaned(void) {
    if (!in_gameplay) return;

    cleaned += 1;
    if (cleaned >= goal) {
        day += 1;
        goal = DAY_BASE_GOAL + day * GOAL_STEP;
        cleaned = 0;
        in_gameplay = 0;
        show_day_complete_popup = 1;   // show popup overlay
        TimeStop();                    // stop timer when target reached
    }
}

int Day_IsInGameplay(void) {
    return in_gameplay;
}

int Day_GetDay(void) { return day; }
int Day_GetCleaned(void) { return cleaned; }
int Day_GetGoal(void) { return goal; }

void Day_DrawHUD(float x, float y) {
    char buffer[64];
    CP_Font_Set(montserrat_light);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(28.0f);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    
    if (Day_GetDay() >= 0) {
        snprintf(buffer, sizeof(buffer), "Day: %d", day);
        CP_Font_DrawText(buffer, x, y);

        snprintf(buffer, sizeof(buffer), "Plates: %d / %d", cleaned, goal);
        CP_Font_DrawText(buffer, x + 30.0f, y + 30.0f);
    }
}


void Day_DrawPopup(void) {
    char buffer[64];

    if (!show_day_complete_popup)
        return;

    float centerX = (float)CP_System_GetWindowWidth() / 2.0f;
    float centerY = (float)CP_System_GetWindowHeight() / 2.0f;

    CP_Graphics_ClearBackground(blue_chalk);
    CP_Font_Set(sub_font);
    CP_Settings_Fill(dark_grey);
    CP_Settings_TextSize(60.0f);
  
    if (day == 1 && TutorialIsActive() == 1) {
        if (TutorialFinishing) {
			time_beforenext = 3.5f;
			TutorialFinishing = 0;
        }
        CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 155));
        CP_Settings_Fill(CP_Color_Create(230, 230, 230, 255));
        CP_Graphics_DrawCircle(centerX, centerY, 600);
        CP_Settings_Fill(CP_Color_Create(210, 210, 210, 255));
        CP_Graphics_DrawCircle(centerX, centerY, 400);

        CP_Image_Draw(hamstamugshot, centerX, centerY- 120.0f, 300.0f, 245.0f, 255);
        CP_Settings_NoStroke();
        CP_Settings_Fill(button_blue);
        CP_Graphics_DrawRectAdvanced(centerX, centerY, 650.0f, 125.0f, 0, 20.0f);
        CP_Font_Set(montserrat_light);
        CP_Settings_Fill(white);
        CP_Settings_TextSize(40.0f);
        CP_Font_DrawText("You're a natural! Keep on scrubbing!", centerX, centerY - 20.0f);
        CP_Font_DrawText("Can you be the best in town?", centerX, centerY + 35.0f);
        CP_Font_Set(sub_font);
        CP_Settings_Fill(dark_grey);
        CP_Settings_TextSize(60.0f);
        CP_Font_DrawText("DAY ONE LOADING...", centerX, 870.0f);
    }
    else if (day == 1) {
        snprintf(buffer, sizeof(buffer), "DAY %d", day);
        CP_Font_DrawText(buffer, centerX, centerY);
    }
    else {
        snprintf(buffer, sizeof(buffer), "DAY %d", day);
        CP_Font_DrawText(buffer, centerX, centerY);
    }


    time_beforenext -= CP_System_GetDt();

    if (time_beforenext <= 0.0f) {
        ForceTutorialClose();
        TimeReset();
        Day_StartCurrentDay();
        InitDirt();
        ChangePlate();
        UnequipSponge();
        time_beforenext = 1.5f;
        show_day_complete_popup = 0;

        if (Day_GetDay() > 1) {
            ready_for_next_day = 1;
            extern int shop_toggle;
            shop_toggle = 1;
            TimeStop();
        }
       
    }
}

int Day_IsReadyForNextDay(void) { return ready_for_next_day; }
void Day_ClearReadyForNextDay(void) { ready_for_next_day = 0; }

void Day_ForceEndDay(void) {
    if (in_gameplay) {
        day += 1;
        goal = DAY_BASE_GOAL + day * GOAL_STEP;
        cleaned = 0;
        in_gameplay = 0;
        show_day_complete_popup = 1;   // show popup overlay
        TimeStop();                    // stop timer when target reached
    }
}