#include "cprocessing.h"
#include "utils.h"
#include "bubbles.h"

typedef struct {
	float position_x, height, width;
	CP_Color color;
} Bar;
static Bar Settings_Boxes;
int sfx_level = 10;
int music_level = 10;
float mx = 0, my = 0;


static int draw_volume_bar(int y_value, int level) {
    Settings_Boxes.width = (float)(CP_System_GetWindowWidth() * 0.75) / 19;
    Settings_Boxes.height = 30;
    Settings_Boxes.position_x = (float)(CP_System_GetWindowWidth() / 7.0);
    Settings_Boxes.color = CP_Color_Create(123, 183, 220, 255);

    float leftBaseX = (float)(CP_System_GetWindowWidth() / 7.0);
    float rightBaseX = leftBaseX + (float)((CP_System_GetWindowWidth() * 0.75) / 19) * 18;
    float arrowY = (float)y_value;

    CP_Settings_Fill(Settings_Boxes.color);
    CP_Graphics_DrawTriangle(leftBaseX - 75.0f, arrowY, leftBaseX - 50.0f, arrowY - 20.0f, leftBaseX - 50.0f, arrowY + 20.0f);
    CP_Graphics_DrawTriangle(rightBaseX + 75.0f, arrowY, rightBaseX + 50.0f, arrowY - 20.0f, rightBaseX + 50.0f, arrowY + 20.0f);

    if (CP_Input_MouseClicked()) {
        mx = CP_Input_GetMouseX();
        my = CP_Input_GetMouseY();
        if (IsAreaClicked(rightBaseX + 62.5, arrowY, 20, 20, mx, my)) {
            BubblesManual(Settings_Boxes.position_x + Settings_Boxes.width * (level++ * 2), y_value);
        }
        else if (IsAreaClicked(leftBaseX - 62.5, arrowY, 20, 20, mx, my)) {
            BubblesManual(Settings_Boxes.position_x + Settings_Boxes.width * (--level * 2), y_value);
        }
    }

    level = CP_Math_ClampInt(level, 0, 10);

    int index = 10;
    while (index > 0) {
        index -= 1;
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Graphics_DrawRect(Settings_Boxes.position_x + Settings_Boxes.width * (index * 2), y_value, Settings_Boxes.width, Settings_Boxes.height);

        if (index < level) {
            CP_Settings_Fill(Settings_Boxes.color);
            CP_Graphics_DrawRect(Settings_Boxes.position_x + Settings_Boxes.width * (index * 2), y_value, Settings_Boxes.width, Settings_Boxes.height);
        }
    }
    return level;
}

void sfx_volume_setting_bar(int y_value) {
    sfx_level = draw_volume_bar(y_value, sfx_level);
}

void music_volume_setting_bar(int y_value) {
    music_level = draw_volume_bar(y_value, music_level);
}

float get_sfx_volume(void) {
	return (float)sfx_level / 10.0;
}

float get_music_volume(void) {
	return (float)music_level / 10.0;
}
