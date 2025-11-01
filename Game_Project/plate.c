#include "cprocessing.h"
#include "money.h"
#include "dirt.h"
#include "img_font_init.h"

int random_num = 0;

// function for when player decides to upgrade/change plates
void ChangePlate(void) {
	random_num = CP_Random_RangeInt(1, 100);
	if (random_num >= 90 && random_num <= 100) {
		plate_outer = CP_Color_Create(224, 160, 220, 255);
		plate_inner = CP_Color_Create(227, 199, 225, 255);
		SetNumberOfDirt(30);
		SetPlateValue(10);
	}
	else if (random_num >= 70 && random_num <= 90) {
		plate_outer = CP_Color_Create(140, 242, 245, 255);
		plate_inner = CP_Color_Create(166, 245, 247, 255);
		SetNumberOfDirt(20);
		SetPlateValue(5);
	}
	else if (random_num >= 40 && random_num <= 70) {
		plate_outer = CP_Color_Create(255, 158, 170, 255);
		plate_inner = CP_Color_Create(252, 192, 199, 255);
		SetNumberOfDirt(10);
		SetPlateValue(2);
	}
	else if (random_num >= 0 && random_num <= 40) {
		plate_outer = CP_Color_Create(230, 230, 230, 255);
		plate_inner = CP_Color_Create(210, 210, 210, 255);
		SetNumberOfDirt(5);
		SetPlateValue(1);
	}
}

// function to draw plate
void DrawPlate(void) {
	if (random_num == 0) {
		plate_outer = CP_Color_Create(230, 230, 230, 255);
		plate_inner = CP_Color_Create(210, 210, 210, 255);
	}
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Fill(plate_outer);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 600);
	CP_Settings_Fill(plate_inner);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 400);
}

