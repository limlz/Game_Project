#include "game.h"
#include "img_font_init.h"
#include "day.h"
#include "cprocessing.h"
#include "timer.h"
#include "utils.h"
#include "sponge.h"
#include "dirt.h"
#include "faucet.h"
#include "shop.h"
#include "roomba.h"

#define TIME 3.0f

int tutorial_need_answer = 1; //starts game with tutorial question
int start_tutorial = 0; //if tutorial is active, show tutorial screen - first step
int is_tutorial_active = 0; //if sponge tutorial is active, show sponge equip tutorial
int tutorial_step = 8; //which step of the tutorial the player is on
int text_placement = 1;
int standard_textbox_values = 1; //if 1, use standard textbox positions; if 0, use custom positions

float hamsta_x = 180.0f;
float hamsta_y = 570.0f;
float ham_angle = 0.0f;
float width = 350.0f;
float draw_text_x, draw_text_y1, draw_text_y2;
float textbox_x, textbox_y, textbox_x_below, textbox_y_below;

CP_Image pointer_arm;

void TutorialStart(void) {

	if (standard_textbox_values) {
		textbox_x = hamsta_x + (width * 0.5f - 70.0f);
		textbox_y = hamsta_y - 145.0f;
		textbox_x_below = hamsta_x - (width * 0.5f - 70.0f);
		textbox_y_below = hamsta_y + 110.0f;
	}
	else {
		textbox_x = 1450.0f + (width * 0.5f - 70.0f);
		textbox_y = 710.0f - 145.0f;
	}
	
	
	if (text_placement == 1) { //text above hamsta, left-aligned
		draw_text_x = (textbox_x - (width * 0.5f)) + 15.0f;
		draw_text_y1 = textbox_y - 15;
		draw_text_y2 = textbox_y + 15;
	}
	else if (text_placement == 2) { //text below hamsta, right-aligned
		draw_text_x = (textbox_x_below - (width * 0.5f)) + 15.0f;
		draw_text_y1 = textbox_y_below - 15;
		draw_text_y2 = textbox_y_below + 15;
	}
	else if (text_placement == 3) { //text below hamsta, left-aligned
		draw_text_x = (textbox_x - (width * 0.5f)) + 15.0f;
		draw_text_y1 = textbox_y_below - 15;
		draw_text_y2 = textbox_y_below + 15;
	}
	else { //text above hamsta, right-aligned
		draw_text_x = (textbox_x_below - (width * 0.5f)) + 15.0f;
		draw_text_y1 = textbox_y - 15;
		draw_text_y2 = textbox_y + 15;

	}
	

	if (is_tutorial_active) {
		CP_Image_Draw(hamsta, hamsta_x, hamsta_y, 150.0f, 150.0f, 255);
		CP_Image_DrawAdvanced(pointer_arm, hamsta_x + 55.0f, hamsta_y, 180.0f, 150.0f, 255, ham_angle);
		CP_Settings_Fill(white);

		if (text_placement == 1) {
			CP_Graphics_DrawRectAdvanced(textbox_x, textbox_y, width, 100.0f, 0.0f, 20.0f);
		}
		else if (text_placement == 2) {
			CP_Graphics_DrawRectAdvanced(textbox_x_below, textbox_y_below, width, 100.0f, 0.0f, 20.0f);
		}
		else if (text_placement == 3) {
			CP_Graphics_DrawRectAdvanced(textbox_x, textbox_y_below, width, 100.0f, 0.0f, 20.0f);
		}
		else {
			CP_Graphics_DrawRectAdvanced(textbox_x_below, textbox_y, width, 100.0f, 0.0f, 20.0f);
		}

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
		if (tutorial_step == 0) {
			pointer_arm = arm_arrow_right;
			ham_angle = 45.0f;
			CP_Font_DrawText("First, equip the SPONGE", draw_text_x, draw_text_y1);
			CP_Font_DrawText("by pressing E!", draw_text_x, draw_text_y2);
			if (IsSpongeEquipped() == 1) {
				tutorial_step = 1;
			}
		}
		else if (tutorial_step == 1) {
			ham_angle = 0.0f;
			CP_Font_DrawText("Great! Now, let's get to", draw_text_x, draw_text_y1);
			CP_Font_DrawText("scrubbing those plates!", draw_text_x, draw_text_y2);
			if (GetTotalOpacity() < 1) {
				tutorial_step = 2;
			}
		}
		else if (tutorial_step == 2) {
			pointer_arm = arm_arrow_left;
			ham_angle = 315.0f;
			CP_Font_DrawText("Now, witness some magic", draw_text_x, draw_text_y1);
			CP_Font_DrawText("by pressing W!", draw_text_x, draw_text_y2);
			if (Faucet_ReturnStreamOn()) {
				tutorial_step = 3;
			}
		}
		else if (tutorial_step == 3) {
			pointer_arm = arm_arrow_right;
			ham_angle = 0.0f;
			width = 320.0f;
			CP_Font_DrawText("Wow, look at this water!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("So fast!", draw_text_x, draw_text_y2);
			if (!Faucet_ReturnStreamOn()) {
				tutorial_step = 4;
			}
		}
		else if (tutorial_step == 4) {
			pointer_arm = arm_arrow_left;
			ham_angle = 315.0f;
			width = 270.0f;
			CP_Font_DrawText("Ability on cooldown!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Hang tight!", draw_text_x, draw_text_y2);
			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();
			if (timer_for_next_step <= 0.0f) {
				tutorial_step = 5;
			}
		}
		else if (tutorial_step == 5) {
			text_placement = 2;
			pointer_arm = arm_arrow_right;
			hamsta_x = 1440.0f;
			hamsta_y = 260.0f;
			ham_angle = 315.0f;
			width = 350.0f;
			CP_Font_DrawText("Time to polish those skils!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Press F to visit the shop!", draw_text_x, draw_text_y2);
			if (IsShopOpen() == 1) {
				tutorial_step = 6;
			}
		}
		else if (tutorial_step == 6) {
			pointer_arm = arm_arrow_left;
			hamsta_x = 1520.0f;
			hamsta_y = 350.0f;
			ham_angle = 345.0f;
			width = 270.0f;
			CP_Font_DrawText("Upgrage your skills!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Be the very best!", draw_text_x, draw_text_y2);
			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();
			if (timer_for_next_step <= 0.0f) {
				ForceCloseShop();
				tutorial_step = 7;
			}
		}
		else if (tutorial_step == 7) {
			text_placement = 3;
			pointer_arm = arm_arrow_left;
			hamsta_x = 120.0f;
			hamsta_y = 240.0f;
			ham_angle = 45.0f;
			width = 300.0f;
			CP_Font_DrawText("Here's your target for", draw_text_x, draw_text_y1);
			CP_Font_DrawText("the day! Be quick!", draw_text_x, draw_text_y2);
			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();
			if (timer_for_next_step <= 0.0f) {
				ForceCloseShop();
				tutorial_step = 8;
			}
		}
		else if (tutorial_step == 8) {
			text_placement = 4;
			hamsta_x = 1490.0f;
			hamsta_y = 710.0f;
			pointer_arm = arm_arrow_left;
			ham_angle = 315.0f;
			width = 320.0f;
			CP_Font_DrawText("Here's the timer!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Finish before time's up!", draw_text_x, draw_text_y2);
			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();
			if (timer_for_next_step <= 0.0f) {
				tutorial_step = 9;
				TimeReset();
			}
		}
		else if (tutorial_step == 9) {
			hamsta_x = 2000.0f;
			hamsta_y = 2000.0f;
			width = 355.0f;
			standard_textbox_values = 0;
			text_placement = 4;
			PurchaseRoomba();
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
			static float timer_for_next_step = TIME*2;
			timer_for_next_step -= CP_System_GetDt();
			if (timer_for_next_step <= 0.0f) {
				CP_Font_DrawText("Now, get to work!", draw_text_x, draw_text_y1);
				CP_Font_DrawText("These plates should SHINE!", draw_text_x, draw_text_y2);
			}
			else {
				CP_Font_DrawText("You can purchase me too!", draw_text_x, draw_text_y1);
				CP_Font_DrawText("Shiny plates await!", draw_text_x, draw_text_y2);
			}
		}
	}
	
}

	void TutorialDayZero(void) {
		TutorialStart();

		if (start_tutorial) {
			TimeStop();
			float mid_x = CP_System_GetWindowWidth() * 0.5f;
			float mid_y = CP_System_GetWindowHeight() * 0.5f;
			float width = mid_x + 95.0f;
			float height = mid_y / 3.0f;
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 175));
			CP_Graphics_DrawRect(mid_x, mid_y, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());

			CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 155));
			CP_Settings_Fill(CP_Color_Create(230, 230, 230, 255));
			CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 600);
			CP_Settings_Fill(CP_Color_Create(210, 210, 210, 255));
			CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 400);

			CP_Image_Draw(hamstamugshot, mid_x, mid_y - 120.0f, 300.0f, 245.0f, 255);

			CP_Settings_NoStroke();
			CP_Settings_Fill(button_blue);
			CP_Graphics_DrawRectAdvanced(mid_x, mid_y, width, height, 0, 20.0f);
			CP_Font_Set(montserrat_light);
			CP_Settings_Fill(white);
			CP_Settings_TextSize(40.0f);
			CP_Font_DrawText("So, you want to be the best dish-washer in town, huh?", mid_x, mid_y - 30.0f);
			CP_Font_DrawText("Come on over and let me show you around...", mid_x, mid_y + 30.0f);

			//button for user
			CP_Settings_Fill(white);
			CP_Settings_Stroke(button_blue);
			CP_Font_Set(sub_font);
			CP_Graphics_DrawRectAdvanced(mid_x, mid_y + 150.0f, 300.0f, 100.0f, 0, 20.0f);
			CP_Settings_Fill(button_blue);
			CP_Font_DrawText("YES, LET'S GO!", mid_x, mid_y + 150.0f);

			if (CP_Input_MouseClicked() && IsAreaClicked(mid_x, mid_y + 150.0f, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				start_tutorial = 0;
				is_tutorial_active = 1;
			}
		}
}

void TutorialYesorNo(void) {

	float mid_x = CP_System_GetWindowWidth() * 0.5f;
	float mid_y = CP_System_GetWindowHeight() * 0.5f;

	if (tutorial_need_answer) {
		TimeStop();
		CP_Graphics_ClearBackground(blue_chalk);
		CP_Settings_NoStroke();
		CP_Settings_Fill(white);
		CP_Graphics_DrawRectAdvanced(mid_x, mid_y - 85.0f, 700.0f, 75.0f, 0.0f, 20.0f);
		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(40.0f);
		CP_Font_DrawText("Hey there! Would you like a tutorial?", mid_x, mid_y - 85.0f);

		// two buttons yes and n0
		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(mid_x - 120.0f, mid_y, 150.0f, 70.0f, 0, 15.0f);
		CP_Graphics_DrawRectAdvanced(mid_x + 120.0f, mid_y, 150.0f, 70.0f, 0, 15.0f);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(40.0f);
		CP_Font_DrawText("Yes", mid_x - 120.0f, mid_y);
		CP_Font_DrawText("No", mid_x + 120.0f, mid_y);
	}

	if (CP_Input_MouseClicked() && IsAreaClicked(mid_x - 120.0f, mid_y, 150.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		tutorial_need_answer = 0;
		start_tutorial = 1;
	}
	else if (!start_tutorial && CP_Input_MouseClicked() && IsAreaClicked(mid_x + 120.0f, mid_y, 150.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		Day_ForceEndDay();
	}

}

int TutorialIsActive(void) {
	return is_tutorial_active;
}

