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
static int inGameplay = 1; // 1 = washing, 0 = popup/shop
static int readyForNextDay = 0;   // 1 means: show Next Day button in Shop
static float time_beforenext;

int TutorialFinishing = 1;

void Day_Init(void) {
    day = 0;
    goal = DAY_BASE_GOAL;
    cleaned = 0;
    inGameplay = 1;
    show_day_complete_popup = 0;
    time_beforenext = 1.5f;
}

void Day_StartCurrentDay(void) {
    cleaned = 0;
    inGameplay = 1;
    show_day_complete_popup = 0;
}

void Day_OnPlateCleaned(void) {
    if (!inGameplay) return;

    cleaned += 1;
    if (cleaned >= goal) {
        day += 1;
        goal = DAY_BASE_GOAL + day * GOAL_STEP;
        cleaned = 0;
        inGameplay = 0;
        show_day_complete_popup = 1;   // show popup overlay
        TimeStop();                    // stop timer when target reached
    }
}

int Day_IsInGameplay(void) {
    return inGameplay;
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

    float centerX = CP_System_GetWindowWidth() / 2.0f;
    float centerY = CP_System_GetWindowHeight() / 2.0f;

    CP_Graphics_ClearBackground(blue_chalk);
    CP_Font_Set(sub_font);
    CP_Settings_Fill(dark_grey);
    CP_Settings_TextSize(60.0f);

    if (day == 1 && TutorialIsActive() == 1) {
        if (TutorialFinishing) {
			time_beforenext = 3.0f;
			TutorialFinishing = 0;
        }
        snprintf(buffer, sizeof(buffer), "TUTORIAL COMPLETE!");
        CP_Font_DrawText(buffer, centerX, centerY);
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
        TimeReset();
        Day_StartCurrentDay();
        InitDirt();
        ChangePlate();
        UnequipSponge();
        time_beforenext = 1.5f;
        show_day_complete_popup = 0;

        if (Day_GetDay() > 1) {
            readyForNextDay = 1;
            extern int shop_toggle;
            shop_toggle = 1;
            TimeStop();
        }
       
    }
}

int Day_IsReadyForNextDay(void) { return readyForNextDay; }
void Day_ClearReadyForNextDay(void) { readyForNextDay = 0; }

void Day_ForceEndDay(void) {
    if (inGameplay) {
        day += 1;
        goal = DAY_BASE_GOAL + day * GOAL_STEP;
        cleaned = 0;
        inGameplay = 0;
        show_day_complete_popup = 1;   // show popup overlay
        TimeStop();                    // stop timer when target reached
    }
}