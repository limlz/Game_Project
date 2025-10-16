#include "cprocessing.h"

#include <stdio.h>

#include "utils.h"

#include "dirt.h"


char money_text[64];
int current_dollars = 0;
int total_earned = 0;
int dollars_per_plate = 1;

// returns current money
int get_current_money(void) {
	return current_dollars;
}

// returns total money earned
int get_total_earned(void) {
	return total_earned;
}

// function to increase money
void increment_money(int money_change) {
	current_dollars += money_change;
	total_earned += money_change;
}

// function to decrease money
void decrement_money(int money_change) {
	current_dollars -= money_change;
}

// function to increase or reduce value of plates
void increment_decrement_plate_value(int plate_change) {
	dollars_per_plate += plate_change;
}

// function to draw current money
void money_display(void) {
	if (dirt_removed()) {
		current_dollars += dollars_per_plate;
		total_earned += dollars_per_plate;
	}
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(30);
	sprintf_s(money_text,32, "Money: %d",current_dollars);
	CP_Font_DrawTextBox(money_text, 750, 50, 90);
}
