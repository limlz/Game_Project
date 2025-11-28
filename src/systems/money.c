/*************************************************************************
@file		money.c
@Author		Lim Liang Zhou (l.liangzhou@digipen.edu)
@Co-authors	nil
@brief		This file contains the function definitions for managing
			the in-game money system, including tracking current money,
			total earnings, plate value, and displaying money on screen.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include <stdio.h>
#include "utils.h"
#include "dirt.h"
#include "img_font_init.h"

char money_text[64];        // buffer for displaying money text on screen
int current_dollars = 0;    // current money the player has right now
int total_earned = 0;       // total money earned across the run (score)
int dollars_per_plate = 1;  // how much a plate is worth (reward amount)

// returns current money
int GetCurrentMoney(void) {
	return current_dollars;  // expose current money value
}

// returns total money earned
int GetTotalEarned(void) {
	return total_earned;     // expose total earned value (often used as score)
}

// function to increase money
void IncrementMoney(int money_change) {
	current_dollars += money_change; // add to current wallet
	total_earned += money_change;    // also add to total earned score
}

// function to decrease money
void DecrementMoney(int money_change) {
	current_dollars -= money_change; // subtract from current wallet
}

void ResetMoney(void) {
	current_dollars = 0;     // reset wallet
	total_earned = 0;        // reset score
}

// function to increase or reduce value of plates
void SetPlateValue(int plate_change) {
	dollars_per_plate = plate_change; // set plate reward amount
}

int GetPlateValue(void) {
	return dollars_per_plate; // return plate reward amount
}

// function to draw current money
void MoneyDisplay(void) {
	// set up text rendering style
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_Set(montserrat_light);
	CP_Settings_TextSize(30);

	// format display string using current money amount
	sprintf_s(money_text, 32, "Money: %d", current_dollars);

	// draw money text inside a text box at the UI position
	CP_Font_DrawTextBox(money_text, 755, 35, 90);
}
