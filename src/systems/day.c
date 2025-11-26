/*************************************************************************
@file day.c
@Author Tan Choon Ming (choonming.tan@digipen.edu)
@Co-authors
@brief Controls the game’s day system and progression.
       Handles:
       - Day counter
       - Number of plates cleaned
       - Goal scaling
       - Day completion popup
       - Transition into next day / entering shop
       - Tutorial-specific day logic

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
#include "sponge.h"

/* Whether the day-complete popup is visible (1 = visible). */
static int show_day_complete_popup = 0;

/* Base values controlling day goal scaling. */
static const int DAY_BASE_GOAL = 5;     // Initial goal for Day 0
static const int GOAL_STEP = 1;         // Plates added per new day

/* Current day system state */
static int day = 0;
static int goal = 5;
static int cleaned = 0;

/* 1 = player is actively washing dishes;
   0 = popup/shop state after completing the day. */
static int in_gameplay = 1;

/* 1 = shop shows the “Next Day” button. */
static int ready_for_next_day = 0;

/* Countdown timer before transitioning automatically into next day. */
static float time_beforenext;

/* Used for tutorial sequence timing during Day 1. */
int TutorialFinishing = 1;

/* Resets all day-related values when the game begins or restarts. */
void DayInit(void) {
    day = 0;
    goal = DAY_BASE_GOAL;
    cleaned = 0;
    in_gameplay = 1;
    show_day_complete_popup = 0;
    time_beforenext = 1.5f;    // delay before switching into gameplay/shop
}

/* Starts the current day (e.g., after popup disappears). */
void DayStartCurrentDay(void) {
    cleaned = 0;
    in_gameplay = 1;
    show_day_complete_popup = 0;
}

/* Called whenever a plate is fully cleaned. Handles goal progression. */
void DayOnPlateCleaned(void) {
    if (!in_gameplay)
        return;

    cleaned += 1;

    /* If goal reached, end gameplay and show day completion popup. */
    if (cleaned >= goal) {
        day += 1;
        goal = DAY_BASE_GOAL + day * GOAL_STEP;   // scale next day's goal
        cleaned = 0;
        in_gameplay = 0;
        show_day_complete_popup = 1;
        TimeStop();                                // freeze timer
    }
}

/* Returns 1 when in dish-washing mode, 0 during popup/shop. */
int DayIsInGameplay(void) {
    return in_gameplay;
}

/* Basic getters for UI and logic. */
int DayGetDay(void) { return day; }
int DayGetCleaned(void) { return cleaned; }
int DayGetGoal(void) { return goal; }

/* Draws the Day number and plate progress on screen. */
void DayDrawHUD(float x, float y) {
    char buffer[64];

    CP_Font_Set(montserrat_light);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(28.0f);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

    if (DayGetDay() >= 0) {
        snprintf(buffer, sizeof(buffer), "Day: %d", day);
        CP_Font_DrawText(buffer, x, y);

        snprintf(buffer, sizeof(buffer), "Plates: %d / %d", cleaned, goal);
        CP_Font_DrawText(buffer, x + 30.0f, y + 30.0f);
    }
}

/* Draws the day-complete popup overlay and controls transition into next day. */
void DayDrawPopup(void) {
    char buffer[64];

    if (!show_day_complete_popup)
        return;

    float centerX = (float)CP_System_GetWindowWidth() / 2.0f;
    float centerY = (float)CP_System_GetWindowHeight() / 2.0f;

    CP_Graphics_ClearBackground(blue_chalk);
    CP_Font_Set(sub_font);
    CP_Settings_Fill(dark_grey);
    CP_Settings_TextSize(60.0f);

    /* Special first-time tutorial popup message (for Day 1). */
    if (day == 1 && TutorialIsActive() == 1) {

        /* Ensures this block only runs once during the tutorial transition. */
        if (TutorialFinishing) {
            time_beforenext = 3.5f;
            TutorialFinishing = 0;
        }

        /* Draw layered popup background circles. */
        CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 155));
        CP_Settings_Fill(CP_Color_Create(230, 230, 230, 255));
        CP_Graphics_DrawCircle(centerX, centerY, 600);

        CP_Settings_Fill(CP_Color_Create(210, 210, 210, 255));
        CP_Graphics_DrawCircle(centerX, centerY, 400);

        /* Hamster picture */
        CP_Image_Draw(hamstamugshot, centerX, centerY - 120.0f, 300.0f, 245.0f, 255);

        /* Message panel */
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
    else {
        /* Generic popup for all non-tutorial days. */
        snprintf(buffer, sizeof(buffer), "DAY %d", day);
        CP_Font_DrawText(buffer, centerX, centerY);
    }

    /* Countdown timer before automatically starting the next day. */
    time_beforenext -= CP_System_GetDt();

    /* Auto-transition into next day once timer finishes. */
    if (time_beforenext <= 0.0f) {

        /* Close tutorial window if still open. */
        ForceTutorialClose();

        /* Reset timer & start the next day. */
        TimeReset();
        DayStartCurrentDay();

        /* Refresh dirt & plate for new day. */
        InitDirt();
        ChangePlate();

        /* Unequip sponge so player must re-equip. */
        UnequipSponge();

        /* Reset popup state. */
        time_beforenext = 1.5f;
        show_day_complete_popup = 0;

        /* For days AFTER day 1, we open the shop
           and show the “Next Day” button. */
        if (DayGetDay() > 1) {
            ready_for_next_day = 1;
            extern int shop_toggle;
            shop_toggle = 1;
            TimeStop();       // Shop mode: timer frozen
        }
    }
}

/* Returns whether the “Next Day” button should be displayed in the shop. */
int DayIsReadyForNextDay(void) {
    return ready_for_next_day;
}

/* Clears the “Next Day ready” flag (used when continuing). */
void DayClearReadyForNextDay(void) {
    ready_for_next_day = 0;
}

/* Forces the day to end immediately and shows the day complete popup. */
void DayForceEndDay(void) {
    if (in_gameplay) {
        day += 1;
        goal = DAY_BASE_GOAL + day * GOAL_STEP;
        cleaned = 0;
        in_gameplay = 0;
        show_day_complete_popup = 1;
        TimeStop();
    }
}
