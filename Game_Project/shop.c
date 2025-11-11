#include "cprocessing.h"
#include <stdio.h>

#include "img_font_init.h"
#include "utils.h"
#include "sponge.h"
#include "money.h"
#include "day.h"
#include "dirt.h"
#include "plate.h"
#include "timer.h"
#include "soap.h"
#include "roomba.h"
#include "faucet.h"
#include "upgrades.h"

#define CENTER_X_POS (CP_System_GetWindowWidth() / 2.0f)
#define CENTER_Y_POS (CP_System_GetWindowHeight() / 2.0f)
#define SHOP_WIDTH 1000.0f
#define SHOP_HEIGHT 750.0f
#define MIN_OFFSET 100.0f
#define MAX_OFFSET 1000.0f
int oldMouseWheel = 0;


static const float HEADER_HEIGHT = 110.0f;
static const float HEADER_TOP_OFFSET = 70.0f;   
static const float LIST_TOP_MARGIN = 40.0f;       
static const float LIST_BOTTOM_MARGIN = 40.0f;    


static const float RowSpacing = 120.0f;          
static const float RowHeight = 96.0f;             
static const float CostButtonWidth = 100.0f;
static const float CostButtonHeight = 70.0f;

static float x_pos = 1500.0f;
static float y_pos = 300.0f;
static float offset = 1000.0f;
int shop_toggle = 0;

static char currentLvlText[80];
static char soapUpgradeDescription[120];
static char faucetPowerDescription[120];
static char faucetCooldownDescription[120];

static int hamstaLoaded = 0;

static float listScroll = 0.0f; 

static void draw_shop_item(int itemNum, const char* name, const char* description, int cost,
int rowIndex, int upgradeable,
float listTop, float listHeight, float panelLeft, float panelRight);
static void draw_shop(void);
static void shop_menu(void);
static void draw_next_day_button(float headerCenterY);


static void draw_shop_item(int itemNum, const char* name, const char* description, int cost, int rowIndex, int upgradeable, float listTop, float listHeight, float panelLeft, float panelRight) {

    float itemY = listTop + (RowHeight * 0.5f) + (float)rowIndex * RowSpacing - listScroll;


    if (itemY <= listTop + RowHeight*0.4 || itemY > (listTop + listHeight - RowHeight*0.5))
        return;

    float iconX = panelLeft + 110.0f;
    float textX = panelLeft + 190.0f;
    float costX = panelRight - 115.0f;

    // Row background
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 70));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, itemY, SHOP_WIDTH - 120.0f, RowHeight, 0.0f, 18.0f);

    // Icon
    switch (itemNum) {
    // 0: Sponge, 1: Soap Refill, 2: Roomba (conditional), 3: Soap Saver, 4: Stream Power, 5: Stream Cooldown
    case 0:
		CP_Image_Draw(sponge_cat, iconX, itemY, 80.0f, 80.0f, 255);
        break;
    case 1:
        CP_Image_Draw(soap_bottle, iconX, itemY, 80.0f, 80.0f, 255);
        break;
    case 2:
        CP_Image_Draw(hamstamugshot, iconX, itemY, 100.0f, 85.0f, 255);
        break;
    case 3:
        CP_Image_Draw(soap_bottle, iconX, itemY, 80.0f, 80.0f, 255);
        break;
    case 4:
        CP_Settings_Fill(CP_Color_Create(0, 160, 255, 230));
        CP_Graphics_DrawEllipse(iconX, itemY, 58.0f, 78.0f);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 230));
        CP_Graphics_DrawRectAdvanced(iconX, itemY - 2.0f, 10.0f, 40.0f, 0.0f, 6.0f);
        CP_Graphics_DrawRectAdvanced(iconX, itemY + 10.0f, 32.0f, 12.0f, 0.0f, 6.0f);
        break;
    case 5:
        CP_Settings_Fill(CP_Color_Create(200, 200, 200, 230));
        CP_Graphics_DrawEllipse(iconX, itemY, 80.0f, 80.0f);
        CP_Settings_Fill(CP_Color_Create(120, 120, 120, 255));
        CP_Graphics_DrawCircle(iconX, itemY, 22.0f);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Graphics_DrawRectAdvanced(iconX, itemY + 2.0f, 8.0f, 36.0f, 0.0f, 4.0f);
        CP_Graphics_DrawRectAdvanced(iconX + 20.0f, itemY, 26.0f, 8.0f, 0.0f, 4.0f);
        break;
    default:
        break;
    }

    // Text
    CP_Settings_Fill(black);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(30.0f);
    CP_Font_DrawText(name, textX, itemY - 30.0f);

    CP_Settings_TextSize(20.0f);
    CP_Font_DrawTextBox(description, textX, itemY + 8.0f, 360.0f);

    // Cost / Buy button
    char costText[28];
    if (upgradeable) {
        CP_Settings_Fill(green);
        sprintf_s(costText, sizeof(costText), "Cost\n%d", cost);
    }
    else {
        CP_Settings_Fill(grey);
        sprintf_s(costText, sizeof(costText), "Maxed Out");
    }
    CP_Graphics_DrawRectAdvanced(costX, itemY, CostButtonWidth, CostButtonHeight, 0.0f, 18.0f);

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(24.0f);
    CP_Font_DrawTextBox(costText, costX - 45.0f, itemY - 12.0f, 100.0f);

    // Handle click only if upgradeable and inside visible list region
    if (upgradeable && CP_Input_MouseClicked() &&
        IsAreaClicked(costX, itemY, CostButtonWidth, CostButtonHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        switch (itemNum) {
        case 0: Upgrades_AttemptSpongeUpgrade(); break;
        case 1: Upgrades_AttemptSoapPurchase(); break;
        case 2: Upgrades_AttemptRoombaUpgrade(); break;
        case 3: Upgrades_AttemptSoapDrainUpgrade(); break;
        case 4: Upgrades_AttemptFaucetPowerUpgrade(); break;
        case 5: Upgrades_AttemptFaucetCooldownUpgrade(); break;
        default: break;
        }
    }
}

static void draw_next_day_button(float headerCenterY) {
    float panelRight = CENTER_X_POS + SHOP_WIDTH / 2.0f;
    float buttonX = panelRight - 170.0f;
    float buttonY = headerCenterY;
    float btnW = 220.0f;
    float btnH = 70.0f;

    CP_Settings_Fill(CP_Color_Create(160, 255, 180, 255));
    CP_Graphics_DrawRectAdvanced(buttonX - 10.0f, buttonY, btnW, btnH, 0.0f, 18.0f);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_Fill(CP_Color_Create(30, 60, 30, 255));
    CP_Settings_TextSize(28.0f);
    CP_Font_DrawText("Ready!", buttonX - 10.0f, buttonY);

    if (CP_Input_MouseClicked() && IsAreaClicked(buttonX, buttonY, btnW, btnH, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        TimeReset();
        Day_StartCurrentDay();
        InitDirt();
        ChangePlate();
        Day_ClearReadyForNextDay();
        shop_toggle = 0;
    }
}

static void draw_shop(void) {
    // Slide the whole panel in/out
    if (shop_toggle && offset > MIN_OFFSET) {
        offset -= 5000.0f * CP_System_GetDt();
    }
    else if (!shop_toggle && offset < MAX_OFFSET) {
        offset += 5000.0f * CP_System_GetDt();
    }
    offset = CP_Math_ClampInt(offset, MIN_OFFSET, MAX_OFFSET);
    float panelCenterY = CENTER_Y_POS + offset;
    float panelTop = panelCenterY - SHOP_HEIGHT / 2.0f;
    float panelLeft = CENTER_X_POS - SHOP_WIDTH / 2.0f;
    float panelRight = CENTER_X_POS + SHOP_WIDTH / 2.0f;

    // Panel background'
    if (Day_IsReadyForNextDay()) {
        CP_Graphics_ClearBackground(blue_chalk);
    }

    CP_Settings_Fill(CP_Color_Create(211, 211, 211, 255));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, panelCenterY, SHOP_WIDTH, SHOP_HEIGHT, 0.0f, 30.0f);

    // Header
    float headerCenterY = panelTop + HEADER_TOP_OFFSET;
    CP_Settings_Fill(blue_grey);
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, headerCenterY, SHOP_WIDTH - 120.0f, HEADER_HEIGHT, 0.0f, 26.0f);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(50.0f);
    CP_Settings_Fill(dark_grey);
    CP_Font_DrawText("Shop Menu", CENTER_X_POS, headerCenterY);

    if (Day_IsReadyForNextDay()) {
        MoneyDisplay();
        draw_next_day_button(headerCenterY);
    }

    int faucetPowerLevel = Faucet_GetPowerLevel();
    float powerBonus = (Faucet_GetPowerMultiplier() - 1.0f) * 100.0f;
    if (powerBonus < 0.0f) powerBonus = 0.0f;
    if (powerBonus > 50.0f) powerBonus = 50.0f;

    if (Faucet_CanUpgradePower()) {
        float nextBonus = (float)(faucetPowerLevel + 1) * 5.0f;
        if (nextBonus > 50.0f) nextBonus = 50.0f;
        sprintf_s(faucetPowerDescription, sizeof(faucetPowerDescription),
            "Boosts stream cleaning speed\nLevel: %d (+%.0f%%) Next: +%.0f%%",
            faucetPowerLevel, powerBonus, nextBonus);
    }
    else {
        sprintf_s(faucetPowerDescription, sizeof(faucetPowerDescription),
            "Boosts stream cleaning speed\nLevel: %d (+%.0f%%) Maxed Out",
            faucetPowerLevel, powerBonus);
    }

    sprintf_s(currentLvlText, sizeof(currentLvlText), "Upgrades Power of Sponge\nCurrent Level: %d", GetSpongePower());

    float totalReduction = (float)Soap_GetDrainUpgradeLevel() * 0.1f;
    sprintf_s(soapUpgradeDescription, sizeof(soapUpgradeDescription),
        "Reduces soap drain speed\nLevel: %d (-%.1f%%)", Soap_GetDrainUpgradeLevel(), totalReduction);

    int faucetCooldownLevel = Faucet_GetCooldownLevel();
    float baseCooldown = Faucet_GetCooldownBase();
    float currentCooldown = Faucet_ReturnCooldown();
    float cooldownReduction = baseCooldown - currentCooldown;
    if (cooldownReduction < 0.0f) cooldownReduction = 0.0f;

    if (Faucet_CanUpgradeCooldown()) {
        float nextCooldown = baseCooldown - (float)(faucetCooldownLevel + 1);
        if (nextCooldown < 0.0f) nextCooldown = 0.0f;
        sprintf_s(faucetCooldownDescription, sizeof(faucetCooldownDescription),
            "Reduces stream cooldown Level: %d (-%.0fs) Current: %.0fs Next: %.0fs",
            faucetCooldownLevel, cooldownReduction, currentCooldown, nextCooldown);
    }
    else {
        sprintf_s(faucetCooldownDescription, sizeof(faucetCooldownDescription),
            "Reduces stream cooldown Level: %d (-%.0fs) Current: %.0fs Maxed Out",
            faucetCooldownLevel, cooldownReduction, currentCooldown);
    }

    // === Scrollable list geometry ===
    float listTop = (headerCenterY + HEADER_HEIGHT * 0.5f) + LIST_TOP_MARGIN;
    float listBottom = panelTop + SHOP_HEIGHT - LIST_BOTTOM_MARGIN;
    float listHeight = listBottom - listTop;

    // Determine how many items we have and their order (sequential rows)
    // 0: Sponge, 1: Soap Refill, 2: Roomba (conditional), 3: Soap Saver, 4: Stream Power, 5: Stream Cooldown
    int totalItems = RoombaPurchase() ? 6 : 5;

    // Compute content height and clamp scroll
    float contentHeight = (float)totalItems * RowSpacing + 150.0f; // spacing governs layout
    float maxScroll = contentHeight - listHeight;
    if (maxScroll < 0.0f) {
        maxScroll = 0.0f;// no scrolling if content fits
    }
    listScroll = CP_Math_ClampFloat(listScroll, 0.0f, maxScroll);

    // Draw a subtle list background
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 30));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, listTop + listHeight * 0.5f, SHOP_WIDTH - 80.0f, listHeight + 30, 0.0f, 18.0f);

    // Scrollbar (right side)
    float scrollBarX = panelRight - 20.0f;
    float scrollBarW = 8.0f;
    float scrollBarH = listHeight;
    CP_Settings_Fill(dark_grey);
    CP_Graphics_DrawRectAdvanced(scrollBarX, listTop + scrollBarH * 0.5f, scrollBarW, scrollBarH, 0.0f, 4.0f);

    // Thumb size proportional to visible fraction
    float thumbH = (contentHeight <= 0.0f) ? scrollBarH : (listHeight * listHeight / contentHeight);
    if (thumbH < 24.0f) thumbH = 24.0f;
    float thumbMaxTravel = scrollBarH - thumbH;
    float thumbY = listTop + (maxScroll == 0.0f ? 0.0f : (listScroll / maxScroll) * thumbMaxTravel) + thumbH * 0.5f;

	// Color of scroll thumb
    CP_Settings_Fill(blue_grey);
    CP_Graphics_DrawRectAdvanced(scrollBarX, thumbY, scrollBarW, thumbH, 0.0f, 4.0f);

    // Mouse wheel scroll (only when cursor is over the list area)
    float mx = CP_Input_GetMouseX();
    float my = CP_Input_GetMouseY();

    int newWheel = (int)CP_Input_MouseWheel();   // assumed cumulative count
    int wheelDelta = (int)CP_Input_MouseWheel();
    oldMouseWheel = 0; // not used in that case

    if (wheelDelta != 0 && mx >= (CENTER_X_POS - SHOP_WIDTH / 2.0f) && mx <= (CENTER_X_POS + SHOP_WIDTH / 2.0f)
        && my >= listTop && my <= listTop + listHeight) {
        listScroll = CP_Math_ClampFloat(listScroll - (float)wheelDelta * 40.0f, 0.0f, maxScroll);
        oldMouseWheel = newWheel;
    }


    int row = 0;

    // Sponge Power
    draw_shop_item(0, "Sponge Power", currentLvlText, Upgrades_GetSpongeCost(),
        row++, SpongeUpgradeable(), listTop, listHeight, panelLeft, panelRight);

    // Stream Power
    draw_shop_item(4, "Stream Power", faucetPowerDescription, Upgrades_GetFaucetPowerCost(),
        row++, Faucet_CanUpgradePower(), listTop, listHeight, panelLeft, panelRight);

    // Soap Refill
    draw_shop_item(1, "Soap Refill", "Refills soap to MAX", Upgrades_GetSoapCost(),
        row++, !Soap_IsFull(), listTop, listHeight, panelLeft, panelRight);

    // Soap Saver
    draw_shop_item(3, "Soap Saver", soapUpgradeDescription, Upgrades_GetSoapDrainCost(),
        row++, Soap_CanUpgradeDrain(), listTop, listHeight, panelLeft, panelRight);

    // Stream Cooldown
    draw_shop_item(5, "Stream Cooldown", faucetCooldownDescription, Upgrades_GetFaucetCooldownCost(),
        row++, Faucet_CanUpgradeCooldown(), listTop, listHeight, panelLeft, panelRight);

    // Roomba (conditional)
    if (RoombaPurchase()) {
        draw_shop_item(2, "Cleaning Robot", "Upgrades robot that auto cleans", Upgrades_GetRoombaCost(),
            row++, 1, listTop, listHeight, panelLeft, panelRight);
    }
}

static void shop_menu(void) {
    draw_shop();
}

void shop_init(void) {
    if (CheckGameRunning()) {
        CP_Settings_TextSize(24.0f);
        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
        CP_Image_Draw(cart, x_pos, y_pos - 200.0f, 80.0f, 80.0f, 255);
        CP_Font_DrawText("Shop [F]", x_pos, y_pos - 150.0f);
    }

    if (CP_Input_KeyTriggered(KEY_F)) {
        shop_toggle = (shop_toggle == 0) ? 1 : 0;
        if (shop_toggle) {
			UnequipSponge();
        }
    }

    if (shop_toggle || offset < MAX_OFFSET) {
        shop_menu();
    }
}

int IsShopOpen(void) {
    return shop_toggle;
}

void ForceCloseShop(void) {
    shop_toggle = 0;
}