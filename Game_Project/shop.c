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
#include "roomba.h"

#define CENTER_X_POS CP_System_GetWindowWidth() / 2.0f
#define CENTER_Y_POS CP_System_GetWindowHeight() / 2.0f
#define SHOP_WIDTH 1000.0f
#define SHOP_HEIGHT 750.0f
#define MIN_OFFSET 100.0f
#define MAX_OFFSET 1000.0f

// X and Y position of the shop menu (temporary)
float x_pos = 1400, y_pos = 300; offset = 1000;
int shopToggle = 0;

int roomba_purchased = 0;

char currentLvlText[100];
char upgradeText[20];
char soapCostText[20];

int upgradeCost = 3;
int increment = 1;
int soapCost = 2;

int roombaUpgradeCost = 20;
int incrementRoomba = 5;

CP_Image hamsta;


void upgrade_sponge_button(void) {
	// Check if player has enough money to upgrade sponge
	if (GetCurrentMoney() >= upgradeCost) {
		DecrementMoney(upgradeCost);
		upgrade_Sponge();

		// Increase cost for next upgrade
		upgradeCost += increment;
		increment++;
	}
}

void upgrade_roomba_button(void) {
	// Check if player has enough money to upgrade sponge
	if (GetCurrentMoney() >= roombaUpgradeCost) {
		DecrementMoney(roombaUpgradeCost);
		AddRoombaStrength(2);
		AddRoombaSpeed(20.0f);

		// Increase cost for next upgrade
		roombaUpgradeCost += incrementRoomba;
		incrementRoomba++;
	}
}

void soap_purchase_button(void)
{
	int soapAvailable = !Soap_IsFull();

	if (soapAvailable && GetCurrentMoney() >= soapCost) {
		DecrementMoney(soapCost);
		Soap_Refill();
	}
	/*
	float buttonSize = 100.0f;
	float buttonX = x_pos + 120.0f;
	float buttonY = y_pos + 200.0f;

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
		if (soapAvailable && GetCurrentMoney() >= soapCost) {
			DecrementMoney(soapCost);
			Soap_Refill();
		}
	}*/
}
void draw_shop_item(int itemNum, char* name, char* description, int cost, float height_offset, int upgradeable) {
	switch (itemNum) {
	case 0:
		CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
		CP_Graphics_DrawRect(CENTER_X_POS - 300, CENTER_Y_POS + offset + height_offset + 25, 100, 100);
		break;
	case 1:
		break;
	case 2:
		CP_Image_Draw(hamsta, CENTER_X_POS - 300, CENTER_Y_POS + offset + height_offset + 25, 150, 120, 255);
		break;

	}

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(40.0f);
	CP_Font_DrawText(name, CENTER_X_POS - 200, CENTER_Y_POS + offset + height_offset);

	CP_Settings_TextSize(20.0f);
	CP_Font_DrawTextBox(description, CENTER_X_POS - 200, CENTER_Y_POS + offset + height_offset + 50, 300);

	char costText[20];
	if (upgradeable) {
		CP_Settings_Fill(CP_Color_Create(0, 200, 0, 100));
		sprintf_s(costText, sizeof(costText), "Cost \n %d", cost);
	}
	else {
		CP_Settings_Fill(CP_Color_Create(200, 0, 0, 100));
		sprintf_s(costText, sizeof(costText), "Sold Out!");
	}

	CP_Graphics_DrawRectAdvanced(CENTER_X_POS + 400, CENTER_Y_POS + offset + height_offset + 25, 100.0f, 100.0f, 0.0f, 20.0f);
	
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(30.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawTextBox(costText, CENTER_X_POS + 370, CENTER_Y_POS + offset + height_offset + 15, 60);	
	
	if (upgradeable && CP_Input_MouseClicked() 
		&& IsAreaClicked(CENTER_X_POS + 400, CENTER_Y_POS + offset + height_offset + 25, 100.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		//TODO: Handle purchase
		switch (itemNum) {
		case 0:
			upgrade_sponge_button();
			break;
		case 1:
			soap_purchase_button();
			break;
		case 2:
			upgrade_roomba_button();
			break;
		}
	}
}

void draw_shop(void) {
	if (shopToggle && offset > MIN_OFFSET) {
		offset -= 5000 * CP_System_GetDt();
	}
	else if (!shopToggle && offset < MAX_OFFSET) {
		offset += 5000 * CP_System_GetDt();
	}
	CP_Settings_Fill(CP_Color_Create(211, 211, 211, 255));
	CP_Graphics_DrawRectAdvanced(CENTER_X_POS, CENTER_Y_POS + offset, SHOP_WIDTH, SHOP_HEIGHT,0, 30.0f);

	CP_Settings_Fill(CP_Color_Create(121, 212, 237, 255));
	CP_Graphics_DrawRect(CENTER_X_POS, offset + 75, 400.0f, 100.0f);
	
	CP_Settings_TextSize(50.0f);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
	CP_Font_DrawText("Shop Menu", CENTER_X_POS, offset + 75);

	sprintf_s(currentLvlText, sizeof(currentLvlText), "Upgrades Power of Sponge \n Current Level : %d", get_SpongePower());
	draw_shop_item(0, "Sponge Power", currentLvlText, upgradeCost, -250, sponge_upgradeable());

	draw_shop_item(1, "Soap Refill", "Refills soap to MAX", soapCost, -100, !Soap_IsFull());

	if (RoombaPurchase()) {
		draw_shop_item(2, "Cleaning Robot", "Upgrades robot that auto cleans", roombaUpgradeCost, 50, !roomba_purchased);
	}
}

void shop_menu(void) {
	float cx = CP_System_GetWindowWidth() * 0.5f;
	float cy = CP_System_GetWindowHeight() * 0.5f;

	float panelW = 360.0f, panelH = 520.0f;
	float btnX = cx;          // base x for centered child widgets
	float btnY = cy;          // base y for centered child widgets

	draw_shop();

	//soap_purchase_button();

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
			InitDirt();
			ChangePlate();
			Day_ClearReadyForNextDay();
			shopToggle = 0;  // close overlay
		}
	}
}

void shop_init(void) {
	hamsta = CP_Image_Load("Assets/hamstermugshot.gif");
	if (CP_Input_KeyTriggered(KEY_F)) {
		shopToggle = (shopToggle == 0) ? 1 : 0;
	}
	if (shopToggle || offset < MAX_OFFSET) {
		shop_menu();
	}
	
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
	CP_Font_DrawText("Press [F] to open Shop", x_pos, y_pos - 200);
}