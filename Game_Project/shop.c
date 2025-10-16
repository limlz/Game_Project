#include "cprocessing.h"

#include <stdio.h>

#include "utils.h"

// Required to get power level of sponge
#include "sponge.h"

// Handles transactions
#include "money.h"

// X and Y position of the shop menu (temporary)
float x_pos = 1400, y_pos = 300;
int shopToggle = 0;

char currentLvlText[20];
char upgradeText[20];

int upgradeCost = 5;
int increment = 1;

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

void shop_menu(void) {
	CP_Settings_Fill(CP_Color_Create(100, 100, 255, 100));
	CP_Graphics_DrawRect(x_pos, y_pos, 300, 500);

	CP_Settings_TextSize(50.0f);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
	CP_Font_DrawText("Shop Menu", x_pos, y_pos - 200);

	upgrade_sponge_button();
	sprintf_s(currentLvlText, sizeof(currentLvlText), "Current Level : %d", get_SpongePower());
	CP_Font_DrawText(currentLvlText, x_pos - 50, y_pos);
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