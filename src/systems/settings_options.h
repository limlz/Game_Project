/*************************************************************************
@file       settings_options.h
@Author     Lim Liang Zhou (l.liangzhou@digipen.edu)
@Co-authors nil
@brief      This file contains the function declarations for the settings
			options screen, specifically for drawing and managing volume
			bars for sound effects and music.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
void SfxVolumeSettingBar(int y_value);
void MusicVolumeSettingBar(int y_value);
float GetSfxVolume(void);
float GetMusicVolume(void);
