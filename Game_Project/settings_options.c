#include "cprocessing.h"
#include "utils.h"

typedef struct {
	float positionX, height, width;
	CP_Color color;
} Bar;
static Bar Settings_Boxes;
int setting_level = 10;
float mx = 0, my = 0;


int setting_bar(int y_value) {
	Settings_Boxes.width = (float)(CP_System_GetWindowWidth() * 0.75) / 19;
	Settings_Boxes.height = 30;
	Settings_Boxes.positionX = (float)(CP_System_GetWindowWidth() / 7.0);
	Settings_Boxes.color = CP_Color_Create(123, 183, 220, 255);
	int boxes = setting_level;

	float leftBaseX = (float)(CP_System_GetWindowWidth() / 7.0);
	float rightBaseX = (float)(CP_System_GetWindowWidth() / 7.0) + (float)((CP_System_GetWindowWidth() * 0.75) / 19) * 18;
	float arrowY = (float)y_value;
	CP_Settings_Fill(Settings_Boxes.color);
	CP_Graphics_DrawTriangle(
		leftBaseX - 75.0f, arrowY,             
		leftBaseX - 50.0f, arrowY - 20.0f,       
		leftBaseX - 50.0f, arrowY + 20.0f);
	CP_Graphics_DrawTriangle(
		rightBaseX + 75.0f, arrowY,
		rightBaseX + 50.0f, arrowY - 20.0f,
		rightBaseX + 50.0f, arrowY + 20.0f);
	if (CP_Input_MouseClicked()) {
		mx = CP_Input_GetMouseX();
		my = CP_Input_GetMouseY();
		if (IsAreaClicked(rightBaseX + 62.5, arrowY, 20, 20, mx, my)) {
			setting_level++;
		}
		else if (IsAreaClicked(leftBaseX - 62.5, arrowY, 20, 20, mx, my)) {
			setting_level--;
		}
	}
	setting_level = CP_Math_ClampInt(setting_level, 0, 10);
	while (boxes > 0) {
		boxes -= 1;
		CP_Graphics_DrawRect(Settings_Boxes.positionX + Settings_Boxes.width * (boxes * 2), y_value, Settings_Boxes.width, Settings_Boxes.height);
	}
	return setting_level;
}
