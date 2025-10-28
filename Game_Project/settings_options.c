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


static int DrawVolumeBar(int y_value, int level) {
    Settings_Boxes.width = (float)(CP_System_GetWindowWidth() * 0.75) / 19;
    Settings_Boxes.height = 30;
    Settings_Boxes.position_x = (float)(CP_System_GetWindowWidth() / 7.0);
    Settings_Boxes.color = CP_Color_Create(123, 183, 220, 255);

    float left_base_x = (float)(CP_System_GetWindowWidth() / 7.0);
    float right_base_x = left_base_x + (float)((CP_System_GetWindowWidth() * 0.75) / 19) * 18;
    float arrow_y = (float)y_value;

    CP_Settings_Fill(Settings_Boxes.color);
    CP_Graphics_DrawTriangle(left_base_x - 75.0f, arrow_y, left_base_x - 50.0f, arrow_y - 20.0f, left_base_x - 50.0f, arrow_y + 20.0f);
    CP_Graphics_DrawTriangle(right_base_x + 75.0f, arrow_y, right_base_x + 50.0f, arrow_y - 20.0f, right_base_x + 50.0f, arrow_y + 20.0f);

    if (CP_Input_MouseClicked()) {
        mx = CP_Input_GetMouseX();
        my = CP_Input_GetMouseY();
        if (IsAreaClicked(right_base_x + 62.5, arrow_y, 20, 20, mx, my)) {
            BubblesManual(Settings_Boxes.position_x + Settings_Boxes.width * (level++ * 2), y_value);
        }
        else if (IsAreaClicked(left_base_x - 62.5, arrow_y, 20, 20, mx, my)) {
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

void SfxVolumeSettingBar(int y_value) {
    sfx_level = DrawVolumeBar(y_value, sfx_level);
}

void MusicVolumeSettingBar(int y_value) {
    music_level = DrawVolumeBar(y_value, music_level);
}

float GetSfxVolume(void) {
	return (float)sfx_level / 10.0;
}

float GetMusicVolume(void) {
	return (float)music_level / 10.0;
}
