/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings_options.h"

CP_Sound scrubbing_sound = NULL,background_music = NULL;
int sound_playing = 0;
int music_playing = 0;

void InitBackgrounMusic(void) {
	background_music = CP_Sound_LoadMusic("Assets/jazzy.mp3");
	if (!music_playing) {
		CP_Sound_PlayMusic(background_music);
		music_playing = 1;
	}
}

void InitScrubbingSounds(void) {
	scrubbing_sound = CP_Sound_Load("Assets/scrubbing.wav");
	CP_Sound_PlayAdvanced(scrubbing_sound, 1.0f, 1.0f, TRUE, CP_SOUND_GROUP_SFX);
	CP_Sound_PauseGroup(CP_SOUND_GROUP_SFX);
}

void UpdateVolume(void) {
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, GetSfxVolume());
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, GetMusicVolume());
}

void StartScrubbingSounds(void) {
	if (!sound_playing) {
		CP_Sound_ResumeGroup(CP_SOUND_GROUP_SFX);
		sound_playing = 1;
	}
}

void StopScrubbingSounds(void) {
	if (sound_playing) {
		CP_Sound_PauseGroup(CP_SOUND_GROUP_SFX);
		sound_playing = 0;
	}
}

void ClearSounds(void) {
	CP_Sound_Free(scrubbing_sound);
}
