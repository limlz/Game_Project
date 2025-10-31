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
int oldMouseWheel = 0;

// --- Scroll config ---
static const float HEADER_HEIGHT = 110.0f;
static const float HEADER_TOP_OFFSET = 70.0f;     // distance from panelTop to header center
static const float LIST_TOP_MARGIN = 40.0f;       // gap below header
static const float LIST_BOTTOM_MARGIN = 40.0f;    // gap above bottom of panel

// Spacing (more room between items)
static const float RowSpacing = 120.0f;           // distance between item centers
static const float RowHeight = 96.0f;             // background capsule height
static const float CostButtonWidth = 110.0f;
static const float CostButtonHeight = 80.0f;

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

static float listScroll = 0.0f; // current scroll offset of the list (in pixels)

// Forward decls
static void draw_shop_item(int itemNum, const char* name, const char* description, int cost,
    int rowIndex, int upgradeable,
    float listTop, float listHeight, float panelLeft, float panelRight);
static void draw_shop(void);
static void shop_menu(void);
static void draw_next_day_button(float headerCenterY);

// Helper: clamp
static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void draw_shop_item(int itemNum, const char* name, const char* description, int cost,
    int rowIndex, int upgradeable,
    float listTop, float listHeight, float panelLeft, float panelRight) {

    // Compute this row's center Y inside the scrollable list area
    float itemY = listTop + (RowHeight * 0.5f) + (float)rowIndex * RowSpacing - listScroll;

    // Cull if item is outside the visible list region (with small padding)
    if (itemY < listTop - RowHeight || itemY >(listTop + listHeight + RowHeight))
        return;

    float iconX = panelLeft + 110.0f;
    float textX = panelLeft + 190.0f;
    float costX = panelRight - 115.0f;

    // Row background
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 70));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, itemY, SHOP_WIDTH - 120.0f, RowHeight, 0.0f, 18.0f);

    // Icon
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

    // Text
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(30.0f);
    CP_Font_DrawText(name, textX, itemY - 30.0f);

    CP_Settings_TextSize(20.0f);
    CP_Font_DrawTextBox(description, textX, itemY + 8.0f, 360.0f);

    // Cost / Buy button
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
    // Slide the whole panel in/out
    if (shopToggle && offset > MIN_OFFSET) {
        offset -= 5000.0f * CP_System_GetDt();
    }
    else if (!shopToggle && offset < MAX_OFFSET) {
        offset += 5000.0f * CP_System_GetDt();
    }

    float panelCenterY = CENTER_Y_POS + offset;
    float panelTop = panelCenterY - SHOP_HEIGHT / 2.0f;
    float panelLeft = CENTER_X_POS - SHOP_WIDTH / 2.0f;
    float panelRight = CENTER_X_POS + SHOP_WIDTH / 2.0f;

    // Panel background
    CP_Settings_Fill(CP_Color_Create(211, 211, 211, 255));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, panelCenterY, SHOP_WIDTH, SHOP_HEIGHT, 0.0f, 30.0f);

    // Header
    float headerCenterY = panelTop + HEADER_TOP_OFFSET;
    CP_Settings_Fill(CP_Color_Create(121, 212, 237, 255));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, headerCenterY, SHOP_WIDTH - 120.0f, HEADER_HEIGHT, 0.0f, 26.0f);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(50.0f);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Font_DrawText("Shop Menu", CENTER_X_POS, headerCenterY);

    if (Day_IsReadyForNextDay()) {
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

    sprintf_s(currentLvlText, sizeof(currentLvlText), "Upgrades Power of Sponge\nCurrent Level: %d", get_SpongePower());

    float totalReduction = (float)Soap_GetDrainUpgradeLevel() * 0.1f;
    sprintf_s(soapUpgradeDescription, sizeof(soapUpgradeDescription),
        "Reduces soap drain speed\nLevel: %d (-%.1f%%)", Soap_GetDrainUpgradeLevel(), totalReduction);

    int faucetCooldownLevel = Faucet_GetCooldownLevel();
    float baseCooldown = Faucet_GetCooldownBase();
    float currentCooldown = return_cooldown();
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
    int hasRoomba = RoombaPurchase() ? 1 : 0;
    int totalItems = hasRoomba ? 6 : 5;

    // Compute content height and clamp scroll
    float contentHeight = (float)totalItems * RowSpacing; // spacing governs layout
    float maxScroll = contentHeight - listHeight;
    if (maxScroll < 0.0f) maxScroll = 0.0f;
    listScroll = clampf(listScroll, 0.0f, maxScroll);

    // Draw a subtle list background
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 30));
    CP_Graphics_DrawRectAdvanced(CENTER_X_POS, listTop + listHeight * 0.5f,
        SHOP_WIDTH - 80.0f, listHeight, 0.0f, 18.0f);

    // Scrollbar (right side)
    float scrollBarX = panelRight - 20.0f;
    float scrollBarW = 8.0f;
    float scrollBarH = listHeight;
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 30));
    CP_Graphics_DrawRectAdvanced(scrollBarX, listTop + scrollBarH * 0.5f, scrollBarW, scrollBarH, 0.0f, 4.0f);

    // Thumb size proportional to visible fraction
    float thumbH = (contentHeight <= 0.0f) ? scrollBarH : (listHeight * listHeight / contentHeight);
    if (thumbH < 24.0f) thumbH = 24.0f;
    float thumbMaxTravel = scrollBarH - thumbH;
    float thumbY = listTop + (maxScroll == 0.0f ? 0.0f : (listScroll / maxScroll) * thumbMaxTravel) + thumbH * 0.5f;

    CP_Settings_Fill(CP_Color_Create(80, 80, 80, 160));
    CP_Graphics_DrawRectAdvanced(scrollBarX, thumbY, scrollBarW, thumbH, 0.0f, 4.0f);

    // Mouse wheel scroll (only when cursor is over the list area)
    float mx = CP_Input_GetMouseX();
    float my = CP_Input_GetMouseY();

    int newWheel = (int)CP_Input_MouseWheel();   // assumed cumulative count
    int wheelDelta = (int)CP_Input_MouseWheel();
    oldMouseWheel = 0; // not used in that case

    if (wheelDelta != 0 && mx >= (CENTER_X_POS - SHOP_WIDTH / 2.0f) && mx <= (CENTER_X_POS + SHOP_WIDTH / 2.0f)
        && my >= listTop && my <= listTop + listHeight) {
        // negative delta usually means wheel scrolled down; adjust if your API is opposite
        listScroll = clampf(listScroll - (float)wheelDelta * 40.0f, 0.0f, maxScroll);
        oldMouseWheel = newWheel;
    }


    // --- Draw items in order, using continuous row indices (0..totalItems-1) ---
    int row = 0;

    // Sponge Power
    draw_shop_item(0, "Sponge Power", currentLvlText, Upgrades_GetSpongeCost(),
        row++, sponge_upgradeable(), listTop, listHeight, panelLeft, panelRight);

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
    if (hasRoomba) {
        draw_shop_item(2, "Cleaning Robot", "Upgrades robot that auto cleans", Upgrades_GetRoombaCost(),
            row++, 1, listTop, listHeight, panelLeft, panelRight);
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
