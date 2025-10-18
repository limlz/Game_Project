#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings_options.h"

CP_Sound scrubbing_sound = NULL,background_music = NULL;
int sound_playing = 0;
int music_playing = 0;

void init_background_music(void) {
	background_music = CP_Sound_LoadMusic("Assets/jazzy.mp3");
	if (!music_playing) {
		CP_Sound_PlayMusic(background_music);
		music_playing = 1;
	}
}

void init_scrubbing_sounds(void) {
	scrubbing_sound = CP_Sound_Load("Assets/scrubbing.wav");
	CP_Sound_PlayAdvanced(scrubbing_sound, 1.0f, 1.0f, TRUE, CP_SOUND_GROUP_SFX);
	CP_Sound_PauseGroup(CP_SOUND_GROUP_SFX);
}

void update_volumes(void) {
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, get_sfx_volume());
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, get_music_volume());
}

void start_scrubbing_sound(void) {
	if (!sound_playing) {
		CP_Sound_ResumeGroup(CP_SOUND_GROUP_SFX);
		sound_playing = 1;
	}
}

void stop_scrubbing_sound(void) {
	if (sound_playing) {
		CP_Sound_PauseGroup(CP_SOUND_GROUP_SFX);
		sound_playing = 0;
	}
}

void clear_sounds(void) {
	CP_Sound_Free(scrubbing_sound);
}
