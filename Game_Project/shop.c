#include "cprocessing.h"
#include <stdio.h>

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

static float x_pos = 1400.0f;
static float y_pos = 300.0f;
static float offset = 1000.0f;
int shopToggle = 0;

static char currentLvlText[80];
static char soapUpgradeDescription[120];
static char faucetPowerDescription[120];
static char faucetCooldownDescription[120];

static CP_Image hamsta;
static int hamstaLoaded = 0;

static const float RowBaseOffset = 165.0f;
static const float RowSpacing = 87.0f;
static const float RowHeight = 80.0f;
static const float CostButtonWidth = 100.0f;
static const float CostButtonHeight = 75.0f;

static void draw_shop_item(int itemNum, const char* name, const char* description, int cost, int rowIndex, int upgradeable);
static void draw_shop(void);
static void shop_menu(void);
static void draw_next_day_button(float headerCenterY);

static void draw_shop_item(int itemNum, const char* name, const char* description, int cost, int rowIndex, int upgradeable) {
    float panelCenterY = CENTER_Y_POS + offset;
    float panelTop = panelCenterY - SHOP_HEIGHT / 2.0f;
    float panelLeft = CENTER_X_POS - SHOP_WIDTH / 2.0f;
    float panelRight = CENTER_X_POS + SHOP_WIDTH / 2.0f;

    float itemY = panelTop + RowBaseOffset + (float)rowIndex * RowSpacing;
    float iconX = panelLeft + 110.0f;
    float textX = panelLeft + 190.0f;
    float costX = panelRight - 115.0f;

    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 70));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, itemY, SHOP_WIDTH - 120.0f, RowHeight, 0.0f, 18.0f);

    switch (itemNum) {
    case 0:
        CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
        CP_Graphics_DrawRectAdvanced(iconX, itemY, 70.0f, 70.0f, 0.0f, 14.0f);
        break;
    case 1:
        CP_Settings_Fill(CP_Color_Create(230, 230, 255, 255));
        CP_Graphics_DrawEllipse(iconX, itemY, 60.0f, 80.0f);
        CP_Settings_Fill(CP_Color_Create(140, 200, 255, 255));
        CP_Graphics_DrawEllipse(iconX, itemY - 8.0f, 38.0f, 50.0f);
        break;
    case 2:
        if (hamstaLoaded) {
            CP_Image_Draw(hamsta, iconX, itemY, 110.0f, 90.0f, 255);
        }
        break;
    case 3:
        CP_Settings_Fill(CP_Color_Create(140, 200, 255, 220));
        CP_Graphics_DrawEllipse(iconX, itemY, 68.0f, 52.0f);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 210));
        CP_Graphics_DrawRectAdvanced(iconX, itemY + 2.0f, 28.0f, 38.0f, 0.0f, 10.0f);
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

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(28.0f);
    CP_Font_DrawText(name, textX, itemY - 28.0f);

    CP_Settings_TextSize(20.0f);
    CP_Font_DrawTextBox(description, textX, itemY + 10.0f, 320.0f);

    char costText[28];
    if (upgradeable) {
        CP_Settings_Fill(CP_Color_Create(0, 200, 0, 140));
        sprintf_s(costText, sizeof(costText), "Cost\n%d", cost);
    }
    else {
        CP_Settings_Fill(CP_Color_Create(150, 150, 150, 160));
        sprintf_s(costText, sizeof(costText), "Maxed Out");
    }

    CP_Graphics_DrawRectAdvanced(costX, itemY, CostButtonWidth, CostButtonHeight, 0.0f, 18.0f);

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(24.0f);
    CP_Font_DrawTextBox(costText, costX, itemY - 10.0f, 90.0f);

    if (upgradeable && CP_Input_MouseClicked() && IsAreaClicked(costX, itemY, CostButtonWidth, CostButtonHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        switch (itemNum) {
        case 0:
            Upgrades_AttemptSpongeUpgrade();
            break;
        case 1:
            Upgrades_AttemptSoapPurchase();
            break;
        case 2:
            Upgrades_AttemptRoombaUpgrade();
            break;
        case 3:
            Upgrades_AttemptSoapDrainUpgrade();
            break;
        case 4:
            Upgrades_AttemptFaucetPowerUpgrade();
            break;
        case 5:
            Upgrades_AttemptFaucetCooldownUpgrade();
            break;
        default:
            break;
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
    CP_Font_DrawText("Next Day", buttonX - 10.0f, buttonY);

    if (CP_Input_MouseClicked() && IsAreaClicked(buttonX, buttonY, btnW, btnH, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
        timeReset();
        Day_StartCurrentDay();
        InitDirt();
        ChangePlate();
        Day_ClearReadyForNextDay();
        shopToggle = 0;
    }
}

static void draw_shop(void) {
    if (shopToggle && offset > MIN_OFFSET) {
        offset -= 5000.0f * CP_System_GetDt();
    }
    else if (!shopToggle && offset < MAX_OFFSET) {
        offset += 5000.0f * CP_System_GetDt();
    }

    float panelCenterY = CENTER_Y_POS + offset;
    float panelTop = panelCenterY - SHOP_HEIGHT / 2.0f;

    CP_Settings_Fill(CP_Color_Create(211, 211, 211, 255));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, panelCenterY, SHOP_WIDTH, SHOP_HEIGHT, 0.0f, 30.0f);

    float headerCenterY = panelTop + 70.0f;
    CP_Settings_Fill(CP_Color_Create(121, 212, 237, 255));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, headerCenterY, SHOP_WIDTH - 120.0f, 110.0f, 0.0f, 26.0f);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(50.0f);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Font_DrawText("Shop Menu", CENTER_X_POS, headerCenterY);

    if (Day_IsReadyForNextDay()) {
        draw_next_day_button(headerCenterY);
    }

    sprintf_s(currentLvlText, sizeof(currentLvlText), "Upgrades Power of Sponge\nCurrent Level: %d", get_SpongePower());
    draw_shop_item(0, "Sponge Power", currentLvlText, Upgrades_GetSpongeCost(), 0, sponge_upgradeable());

    int faucetPowerLevel = Faucet_GetPowerLevel();
    float powerBonus = (Faucet_GetPowerMultiplier() - 1.0f) * 100.0f;
    if (powerBonus < 0.0f) {
        powerBonus = 0.0f;
    }
    if (powerBonus > 50.0f) {
        powerBonus = 50.0f;
    }

    if (Faucet_CanUpgradePower()) {
        float nextBonus = (float)(faucetPowerLevel + 1) * 5.0f;
        if (nextBonus > 50.0f) {
            nextBonus = 50.0f;
        }
        sprintf_s(faucetPowerDescription, sizeof(faucetPowerDescription),
            "Boosts stream cleaning speed\nLevel: %d (+%.0f%%) Next: +%.0f%%",
            faucetPowerLevel, powerBonus, nextBonus);
    }
    else {
        sprintf_s(faucetPowerDescription, sizeof(faucetPowerDescription),
            "Boosts stream cleaning speed\nLevel: %d (+%.0f%%) Maxed Out",
            faucetPowerLevel, powerBonus);
    }

    draw_shop_item(4, "Stream Power", faucetPowerDescription, Upgrades_GetFaucetPowerCost(), 1, Faucet_CanUpgradePower());
    draw_shop_item(1, "Soap Refill", "Refills soap to MAX", Upgrades_GetSoapCost(), 2, !Soap_IsFull());

    float totalReduction = (float)Soap_GetDrainUpgradeLevel() * 0.1f;
    sprintf_s(soapUpgradeDescription, sizeof(soapUpgradeDescription), "Reduces soap drain speed\nLevel: %d (-%.1f%%)", Soap_GetDrainUpgradeLevel(), totalReduction);
    draw_shop_item(3, "Soap Saver", soapUpgradeDescription, Upgrades_GetSoapDrainCost(), 3, Soap_CanUpgradeDrain());

    int faucetCooldownLevel = Faucet_GetCooldownLevel();
    float baseCooldown = Faucet_GetCooldownBase();
    float currentCooldown = return_cooldown();
    float cooldownReduction = baseCooldown - currentCooldown;
    if (cooldownReduction < 0.0f) {
        cooldownReduction = 0.0f;
    }

    if (Faucet_CanUpgradeCooldown()) {
        float nextCooldown = baseCooldown - (float)(faucetCooldownLevel + 1);
        if (nextCooldown < 0.0f) {
            nextCooldown = 0.0f;
        }
        sprintf_s(faucetCooldownDescription, sizeof(faucetCooldownDescription),
            "Reduces stream cooldown Level: %d (-%.0fs) Current: %.0fs Next: %.0fs",
            faucetCooldownLevel, cooldownReduction, currentCooldown, nextCooldown);
    }
    else {
        sprintf_s(faucetCooldownDescription, sizeof(faucetCooldownDescription),
            "Reduces stream cooldown Level: %d (-%.0fs) Current: %.0fs Maxed Out",
            faucetCooldownLevel, cooldownReduction, currentCooldown);
    }
    draw_shop_item(5, "Stream Cooldown", faucetCooldownDescription, Upgrades_GetFaucetCooldownCost(), 4, Faucet_CanUpgradeCooldown());

    if (RoombaPurchase()) {
        draw_shop_item(2, "Cleaning Robot", "Upgrades robot that auto cleans", Upgrades_GetRoombaCost(), 5, 1);
    }
}

static void shop_menu(void) {
    draw_shop();
}

void shop_init(void) {
    if (!hamstaLoaded) {
        hamsta = CP_Image_Load("Assets/hamstermugshot.gif");
        hamstaLoaded = 1;
    }

    if (CP_Input_KeyTriggered(KEY_F)) {
        shopToggle = (shopToggle == 0) ? 1 : 0;
    }

    if (shopToggle || offset < MAX_OFFSET) {
        shop_menu();
    }

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Font_DrawText("Press [F] to open Shop", x_pos, y_pos - 200.0f);
}