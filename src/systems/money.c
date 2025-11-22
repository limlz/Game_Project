/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include <stdio.h>
#include "utils.h"
#include "dirt.h"
#include "img_font_init.h"


char money_text[64];
int current_dollars = 0;
int total_earned = 0;
int dollars_per_plate = 1;

// returns current money
int GetCurrentMoney(void) {
	return current_dollars;
}

// returns total money earned
int GetTotalEarned(void) {
	return total_earned;
}

// function to increase money
void IncrementMoney(int money_change) {
	current_dollars += money_change;
	total_earned += money_change;
}

// function to decrease money
void DecrementMoney(int money_change) {
	current_dollars -= money_change;
}

void ResetMoney(void) {
	current_dollars = 0;
	total_earned = 0;
}

// function to increase or reduce value of plates
void SetPlateValue(int plate_change) {
	dollars_per_plate = plate_change;
}

int GetPlateValue(void) {
	return dollars_per_plate;
}

// function to draw current money
void MoneyDisplay(void) {
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_Set(montserrat_light);
	CP_Settings_TextSize(30);
	sprintf_s(money_text,32, "Money: %d",current_dollars);
	CP_Font_DrawTextBox(money_text, 755, 35, 90);
}
