#include "cprocessing.h"
#include <stdio.h>
#include "timer.h"
#include "mainmenu.h"
#include "plate.h"
#include "dirt.h"
#include "game.h"

/* ---------------- Global State ---------------- */
static int show_day_complete_popup = 0;  // 1 = popup visible
static const int DAY_BASE_GOAL = 5;
static const int GOAL_STEP = 1;

static int g_day = 0;
static int g_goal = 5;
static int g_cleaned = 0;
static int g_inGameplay = 1; // 1 = washing, 0 = popup/shop
static int g_readyForNextDay = 0;   // 1 means: show Next Day button in Shop


/* ---------------- Core Day Functions ---------------- */
void Day_Init(void)
{
    g_day = 0;
    g_goal = DAY_BASE_GOAL;
    g_cleaned = 0;
    g_inGameplay = 1;
    show_day_complete_popup = 0;
}

void Day_StartCurrentDay(void)
{
    g_cleaned = 0;
    g_inGameplay = 1;
    show_day_complete_popup = 0;
}

void Day_OnPlateCleaned(void)
{
    if (!g_inGameplay) return;

    g_cleaned += 1;
    if (g_cleaned >= g_goal)
    {
        g_day += 1;
        g_goal = DAY_BASE_GOAL + g_day * GOAL_STEP;
        g_cleaned = 0;
        g_inGameplay = 0;
        show_day_complete_popup = 1;   // show popup overlay
        timeStop();                    // stop timer when target reached
    }
}

int Day_IsInGameplay(void)
{
    return g_inGameplay;
}

/* ---------------- HUD Drawing ---------------- */
void Day_DrawHUD(float x, float y)
{
    char buffer[64];
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(28.0f);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

    snprintf(buffer, sizeof(buffer), "Day: %d", g_day);
    CP_Font_DrawText(buffer, x, y);

    snprintf(buffer, sizeof(buffer), "Plates: %d / %d", g_cleaned, g_goal);
    CP_Font_DrawText(buffer, x + 32.5f, y + 30.0f);
}

int Day_GetDay(void) { return g_day; }
int Day_GetCleaned(void) { return g_cleaned; }
int Day_GetGoal(void) { return g_goal; }

/* ---------------- Popup Drawing ---------------- */
void Day_DrawPopup(void)
{
    if (!show_day_complete_popup)
        return;

    float centerX = CP_System_GetWindowWidth() / 2.0f;
    float centerY = CP_System_GetWindowHeight() / 2.0f;

    // Dim background
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 150));
    CP_Graphics_DrawRect(centerX, centerY, 1600, 900);

    // Popup box
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Graphics_DrawRect(centerX, centerY, 800, 400);

    CP_Settings_TextSize(45.0f);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("Good Job! All plates are washed for the day.", centerX, centerY - 100);
    CP_Font_DrawText("Move on to the next day?", centerX, centerY - 40);

    // Buttons
    float btnW = 250.0f, btnH = 80.0f, gap = 300.0f;
    float btnY = centerY + 100.0f;
    const char* btnText[3] = { "Next Day", "Head to Shop", "Save & Exit" };
    CP_Color btnColor[3] = {
        CP_Color_Create(100,255,100,255),
        CP_Color_Create(100,200,255,255),
        CP_Color_Create(255,120,120,255)
    };
    float btnX[3] = { centerX - gap, centerX, centerX + gap };

    float mx = CP_Input_GetMouseX();
    float my = CP_Input_GetMouseY();

    for (int i = 0; i < 3; ++i)
    {
        float left = btnX[i] - btnW / 2.0f;
        float top = btnY - btnH / 2.0f;

        CP_Settings_Fill(btnColor[i]);
        CP_Graphics_DrawRectAdvanced(btnX[i], btnY, btnW, btnH, 0, 25.0f);

        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
        CP_Settings_TextSize(30.0f);
        CP_Font_DrawText(btnText[i], btnX[i], btnY);

        if (CP_Input_MouseClicked())
        {
            if (mx >= left && mx <= left + btnW && my >= top && my <= top + btnH)
            {
                switch (i)
                {
                case 0: // Next Day
                    timeReset();
                    Day_StartCurrentDay();
                    InitDirt();
                    change_plate();
                    show_day_complete_popup = 0;
                    g_readyForNextDay = 0;          // <-- clear
                    break;

                case 1: // Head to Shop
                    show_day_complete_popup = 0;
                    g_readyForNextDay = 1;          // <-- tell shop to show Next Day button
                    extern int shopToggle;
                    shopToggle = 1;
                    break;

                case 2: // Save & Exit
                    show_day_complete_popup = 0;
                    CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
                    break;
                }
            }
        }
    }
}
int Day_IsReadyForNextDay(void) { return g_readyForNextDay; }
void Day_ClearReadyForNextDay(void) { g_readyForNextDay = 0; }

void Day_ForceEndDay(void)
{
    if (g_inGameplay)
    {
        g_day += 1;
        g_goal = DAY_BASE_GOAL + g_day * GOAL_STEP;
        g_cleaned = 0;
        g_inGameplay = 0;
        show_day_complete_popup = 1;   // show popup overlay
        timeStop();                    // stop timer when target reached
    }
}
