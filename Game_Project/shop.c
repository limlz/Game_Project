#include "cprocessing.h"
#include <stdio.h>
#include "utils.h"

// Required to get power level of sponge
#include "sponge.h"

// Handles transactions
#include "money.h"
#include "day.h"
#include "dirt.h"
#include "plate.h"
#include "timer.h"
#include "soap.h"

// X and Y position of the shop menu (temporary)
float x_pos = 1400, y_pos = 300;
int shopToggle = 0;

char currentLvlText[20];
char upgradeText[20];
char soapCostText[20];

int upgradeCost = 3;
int increment = 1;
int soapCost = 2;

void upgrade_sponge_button(void) {
	// Upgrade button
	CP_Settings_Fill(CP_Color_Create(255, 100, 255, 100));
	CP_Graphics_DrawRect(x_pos, y_pos + 200, 100, 100);

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
	CP_Settings_TextSize(30.0f);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	sprintf_s(upgradeText, sizeof(upgradeText), "Upgrade Cost : %d", upgradeCost);
	CP_Font_DrawTextBox(upgradeText, x_pos - 50, y_pos + 200, 100);

	// TODO: Deduct money when upgrading sponge and also edge case to prevent upgrading if not enough money
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(x_pos, y_pos + 200, 100, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {

			// Check if player has enough money to upgrade sponge
			if (get_current_money() >= upgradeCost) {
				decrement_money(upgradeCost);
				upgrade_Sponge();

				// Increase cost for next upgrade
				upgradeCost += increment;
				increment++;
			}
		}
	}
}

void soap_purchase_button(void)
{
	float buttonSize = 100.0f;
	float buttonX = x_pos + 120.0f;
	float buttonY = y_pos + 200.0f;

	int soapAvailable = !Soap_IsFull();
	CP_Color buttonColor = CP_Color_Create(255, 220, 0, soapAvailable ? 255 : 120);

	CP_Settings_Fill(buttonColor);
	CP_Graphics_DrawRect(buttonX, buttonY, buttonSize, buttonSize);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(30.0f);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("Soap", buttonX, buttonY - 15.0f);

	CP_Settings_TextSize(22.0f);
	sprintf_s(soapCostText, sizeof(soapCostText), "Cost: %d", soapCost);
	CP_Font_DrawText(soapCostText, buttonX, buttonY + 20.0f);

	if (CP_Input_MouseClicked() &&
		IsAreaClicked(buttonX, buttonY, buttonSize, buttonSize, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		if (soapAvailable && get_current_money() >= soapCost) {
			decrement_money(soapCost);
			Soap_Refill();
		}
	}
}

void shop_menu(void) {
	float cx = CP_System_GetWindowWidth() * 0.5f;
	float cy = CP_System_GetWindowHeight() * 0.5f;

	float panelW = 360.0f, panelH = 520.0f;
	float btnX = cx;          // base x for centered child widgets
	float btnY = cy;          // base y for centered child widgets

	CP_Settings_Fill(CP_Color_Create(100, 100, 255, 100));
	CP_Graphics_DrawRect(x_pos, y_pos, 300, 500);

	CP_Settings_TextSize(50.0f);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
	CP_Font_DrawText("Shop Menu", x_pos, y_pos - 200);

	upgrade_sponge_button();
	soap_purchase_button();
	sprintf_s(currentLvlText, sizeof(currentLvlText), "Current Level : %d", get_SpongePower());
	CP_Font_DrawText(currentLvlText, x_pos - 50, y_pos);

	if (Day_IsReadyForNextDay()) {
		// center will be handled below; use btnX/btnY from the centering section
		float btnW = 220.0f, btnH = 80.0f;

		CP_Settings_Fill(CP_Color_Create(160, 255, 180, 255));
		CP_Graphics_DrawRectAdvanced(btnX + 600.0f, btnY + 150.0f, btnW, btnH, 0.0f, 18.0f);

		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Settings_Fill(CP_Color_Create(30, 60, 30, 255));
		CP_Settings_TextSize(28.0f);
		CP_Font_DrawText("Next Day", btnX + 600.0f, btnY + 150.0f);

		if (CP_Input_MouseClicked() &&
			IsAreaClicked(btnX + 600.0f, btnY + 150.0f, btnW, btnH, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			timeReset();
			Day_StartCurrentDay();
			init_dirt();
			change_plate();
			Day_ClearReadyForNextDay();
			shopToggle = 0;  // close overlay
		}
	}
}

void shop_init(void) {
	if (CP_Input_KeyTriggered(KEY_F)) {
		shopToggle = (shopToggle == 0) ? 1 : 0;
	}

	if (shopToggle) {
		shop_menu();
	}
	else {
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
		CP_Font_DrawText("Press [F] to open Shop", x_pos, y_pos - 200);
	}
}