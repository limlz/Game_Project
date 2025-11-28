/*************************************************************************
@file       settings_options.c
@Author     Lim Liang Zhou (l.liangzhou@digipen.edu)
@Co-authors nil
@brief      Settings options screen: draws/updates volume bars for SFX & music.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "settings_options.h"
#include "utils.h"
#include "bubbles.h"

typedef struct {
    float position_x, height, width;
    CP_Color color;
} Bar;

static Bar Settings_Boxes;   // shared bar style + base position
int sfx_level = 10;          // 0..10
int music_level = 10;        // 0..10
float mx = 0, my = 0;        // cached mouse position on click

static int DrawVolumeBar(float y_value, int level) {
    // compute bar sizing based on window
    Settings_Boxes.width = (float)(CP_System_GetWindowWidth() * 0.75) / 19;
    Settings_Boxes.height = 30;
    Settings_Boxes.position_x = (float)(CP_System_GetWindowWidth() / 7.0);
    Settings_Boxes.color = CP_Color_Create(123, 183, 220, 255);

    // arrow positions
    float left_base_x = (float)(CP_System_GetWindowWidth() / 7.0);
    float right_base_x = left_base_x + (float)((CP_System_GetWindowWidth() * 0.75) / 19) * 18;
    float arrow_y = (float)y_value;

    // draw left/right arrows
    CP_Settings_Fill(Settings_Boxes.color);
    CP_Graphics_DrawTriangle(left_base_x - 75.0f, arrow_y, left_base_x - 50.0f, arrow_y - 20.0f, left_base_x - 50.0f, arrow_y + 20.0f);
    CP_Graphics_DrawTriangle(right_base_x + 75.0f, arrow_y, right_base_x + 50.0f, arrow_y - 20.0f, right_base_x + 50.0f, arrow_y + 20.0f);

    // handle click to increase/decrease level
    if (CP_Input_MouseClicked()) {
        mx = CP_Input_GetMouseX();
        my = CP_Input_GetMouseY();
        if (IsAreaClicked(right_base_x + 62.5f, arrow_y, 20.0f, 20.0f, mx, my)) {
            BubblesManual(Settings_Boxes.position_x + Settings_Boxes.width * (level++ * 2), y_value);
        }
        else if (IsAreaClicked(left_base_x - 62.5f, arrow_y, 20.0f, 20.0f, mx, my)) {
            BubblesManual(Settings_Boxes.position_x + Settings_Boxes.width * (--level * 2), y_value);
        }
    }

    // clamp to valid range
    level = CP_Math_ClampInt(level, 0, 10);

    // draw 10 segments (grey = empty, blue = filled)
    int index = 10;
    while (index > 0) {
        index -= 1;
        CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));
        CP_Graphics_DrawRect(Settings_Boxes.position_x + Settings_Boxes.width * (index * 2), y_value, Settings_Boxes.width, Settings_Boxes.height);

        if (index < level) {
            CP_Settings_Fill(Settings_Boxes.color);
            CP_Graphics_DrawRect(Settings_Boxes.position_x + Settings_Boxes.width * (index * 2), y_value, Settings_Boxes.width, Settings_Boxes.height);
        }
    }
    return level;
}

void SfxVolumeSettingBar(float y_value) {
    // update/draw SFX bar
    sfx_level = DrawVolumeBar(y_value, sfx_level);
}

void MusicVolumeSettingBar(float y_value) {
    // update/draw music bar
    music_level = DrawVolumeBar(y_value, music_level);
}

float GetSfxVolume(void) {
    return (float)sfx_level / 10.0f;   // convert 0..10 to 0.0..1.0
}

float GetMusicVolume(void) {
    return (float)music_level / 10.0f; // convert 0..10 to 0.0..1.0
}
