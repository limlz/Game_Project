#include "cprocessing.h"
#include "roomba.h"
#include "dirt.h"
#include "sponge.h"
#include "money.h"
#include "utils.h"

#define ADD_SUB_BOX_HEIGHT			40
#define ADD_SUB_BOX_WIDTH			120

CP_Color debug_box_color;
CP_Color addition_color;
CP_Color subtraction_color;

int currently_debugging = 0;

int is_currently_debugging(void) {
	return currently_debugging;
}

void start_debugging(void) {
	currently_debugging = 1;
}

void stop_debugging(void) {
	currently_debugging = 0;
}

void debug_print(void) {
	debug_box_color = CP_Color_Create(200, 200, 200, 50);
	addition_color = CP_Color_Create(0, 255, 0, 200);
	subtraction_color = CP_Color_Create(255, 0, 0, 200);
	currently_debugging = 1;
	char display[50];
	float text_x, text_y, box_x, box_y;
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_TextSize(20.0f);
	text_x = CP_System_GetWindowWidth() - 250;
	box_x = CP_System_GetWindowWidth() - 260;
	box_y = 205.0f;


	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("Money", box_x + 125, box_y);
	CP_Settings_Fill(addition_color);
	CP_Graphics_DrawRect(box_x, box_y + 20, ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT);
	if (IsAreaClicked(box_x + ADD_SUB_BOX_WIDTH / 2, box_y + 20 + ADD_SUB_BOX_HEIGHT / 2, ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT,CP_Input_GetMouseX(), CP_Input_GetMouseY()) && CP_Input_MouseClicked()) {
		increment_money(100);
	}

	CP_Settings_Fill(subtraction_color);
	CP_Graphics_DrawRect(box_x + 130, box_y + 20, ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT);
	if (IsAreaClicked(box_x + 130 + ADD_SUB_BOX_WIDTH / 2, box_y + 20 + ADD_SUB_BOX_HEIGHT / 2, ADD_SUB_BOX_WIDTH, ADD_SUB_BOX_HEIGHT, CP_Input_GetMouseX(), CP_Input_GetMouseY()) && CP_Input_MouseClicked()) {
		decrement_money(100);
	}

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	if (roomba_purchase()) {
		text_y = CP_System_GetWindowHeight() - 300;
		CP_Settings_Fill(debug_box_color);
		CP_Graphics_DrawRect(text_x - 10, text_y - 20, 250, 160);

		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		sprintf_s(display, sizeof(display), "Roomba X: %.2f", get_roomba_x());
		CP_Font_DrawText(display, text_x, text_y);

		sprintf_s(display, sizeof(display), "Roomba Y: %.2f", get_roomba_y());
		CP_Font_DrawText(display, text_x, text_y + 20);

		sprintf_s(display, sizeof(display), "Roomba speed: %.2f", get_roomba_speed());
		CP_Font_DrawText(display, text_x, text_y + 40);

		sprintf_s(display, sizeof(display), "Roomba strength: %d", get_roomba_strength());
		CP_Font_DrawText(display, text_x, text_y + 60);

		sprintf_s(display, sizeof(display), "Roomba scrubbing: %s", get_roomba_currently_scrubbing() ? "Yes" : "No");
		CP_Font_DrawText(display, text_x, text_y + 80);

		sprintf_s(display, sizeof(display), "Target dirt index : %d", get_closest_dirt_index());
		CP_Font_DrawText(display, text_x, text_y + 100);

		sprintf_s(display, sizeof(display), "Target dirt opacity: %d", dirtList[get_closest_dirt_index()].opacity);
		CP_Font_DrawText(display, text_x, text_y + 120);

	}

	CP_Settings_Fill(debug_box_color);
	text_y = CP_System_GetWindowHeight() - 550;
	CP_Graphics_DrawRect(text_x - 10, text_y - 20, 250, 180);

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	sprintf_s(display, sizeof(display), "Mouse X: %.0f", CP_Input_GetMouseX());
	CP_Font_DrawText(display, text_x, text_y);

	sprintf_s(display, sizeof(display), "Mouse Y: %.0f", CP_Input_GetMouseY());
	CP_Font_DrawText(display, text_x, text_y + 20);

	sprintf_s(display, sizeof(display), "Total dirt opacity: %d", get_total_opacity());
	CP_Font_DrawText(display, text_x, text_y + 40);

	sprintf_s(display, sizeof(display), "Equipped sponge?: %s", is_SpongeEquipped() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y + 60);

	sprintf_s(display, sizeof(display), "Sponge power: %d", get_SpongePower());
	CP_Font_DrawText(display, text_x, text_y + 80);

	sprintf_s(display, sizeof(display), "Scrubbing?: %s", is_Scrubbing() ? "Yes" : "No");
	CP_Font_DrawText(display, text_x, text_y + 100);

	sprintf_s(display, sizeof(display), "Current Plate value: %d", get_plate_value());
	CP_Font_DrawText(display, text_x, text_y + 120);

	sprintf_s(display, sizeof(display), "Total earnings / score: %d", get_total_earned());
	CP_Font_DrawText(display, text_x, text_y + 140);


	CP_Settings_RectMode(CP_POSITION_CENTER);
}