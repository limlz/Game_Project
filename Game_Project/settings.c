#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings.h"
#include "mainmenu.h"
#include "sounds.h"
#include "bubbles.h"
#include "bubblegun.h"

int volume_level = 0;
CP_Font myFont;
CP_Font titleFont;
CP_Font subFont;
CP_Color White, ButtonBlue;
CP_Image arm;
CP_Image armflipped;
CP_Image hamsta;
float mousex = 0, mousey = 0;

void Settings_Init(void)
{
	hamsta = CP_Image_Load("Assets/hambod.gif");
	arm = CP_Image_Load("Assets/hamgun.gif");
	armflipped = CP_Image_Load("Assets/hamgunflip.gif");
	myFont = CP_Font_Load("Assets/MontserratLight.ttf");
	titleFont = CP_Font_Load("Assets/SuperWater.ttf");
	subFont = CP_Font_Load("Assets/MontserratBlackItalic.otf");
	White = CP_Color_Create(255, 255, 255, 255);
	ButtonBlue = CP_Color_Create(123, 183, 220, 255);
	Bubbles_Init();
	Bullets_Init();
}

void Settings_Update(void)
{
	int settings_pop = 0;
	mousex = CP_Input_GetMouseX();
	mousey = CP_Input_GetMouseY();
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	// Draw settings logo
	CP_Font_Set(subFont);
	CP_Settings_Fill(ButtonBlue);
	CP_Settings_TextSize(150.0f);
	CP_Font_DrawText("Settings", CP_System_GetWindowWidth()/2, 130);

	CP_Font_Set(myFont);
	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText("SFX Volume", CP_System_GetWindowWidth() / 2, 260);
	sfx_volume_setting_bar(330);
	CP_Settings_Fill(ButtonBlue);

	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText("Music Volume", CP_System_GetWindowWidth() / 2, 410);
	music_volume_setting_bar(480);
	CP_Settings_Fill(ButtonBlue);


	if (IsAreaClicked(120, CP_System_GetWindowHeight() - 120, 100, 100, mousex, mousey)) {
		settings_pop = 10;
		Bubbles_Manual(mousex, mousey);
		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}


	// Hamster
	CP_Settings_Fill(CP_Color_Create(141, 144, 147, 200));
	CP_Graphics_DrawEllipse(CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() - 80, 200, 30);
	CP_Image_Draw(hamsta, CP_System_GetWindowWidth()/2,CP_System_GetWindowHeight() - 130, 170.0f, 170.0f, 255);

	// Draw settings button
	CP_Settings_Fill(ButtonBlue);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120, 100 + settings_pop, 100 + settings_pop, 0, 20);
	CP_Settings_Fill(White);
	CP_Graphics_DrawCircle(120, CP_System_GetWindowHeight() - 120, 50);
	update_volumes();
	Bubbles_Draw();
	Bullets_UpdateAndDraw();
	// Hamster Pointer

	float mx = CP_Input_GetMouseX();
	float my = CP_Input_GetMouseY();
	CP_Vector hand_origin = CP_Vector_Set(CP_System_GetWindowWidth() / 2 + 60, CP_System_GetWindowHeight() - 130);
	CP_Vector hand_vector = CP_Vector_Subtract(CP_Vector_Set(mx,my), hand_origin);
	CP_Vector up = CP_Vector_Set(0, 1);
	float hand_angle = CP_Vector_AngleCW(up, CP_Vector_Negate(hand_vector));

	// Fire once per click
	if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
		Bullets_Spawn(hand_origin, CP_Vector_Set(mx, my), 400.0f, 10.0f);
	}
	if (hand_angle > 0 && hand_angle < 180) {
		CP_Image_DrawAdvanced(arm, CP_System_GetWindowWidth() / 2 + 55, CP_System_GetWindowHeight() - 130, 80, 80, 255, hand_angle);
	}
	else {
		CP_Image_DrawAdvanced(armflipped, CP_System_GetWindowWidth() / 2 + 55, CP_System_GetWindowHeight() - 130, 60, 80, 255, hand_angle);
	}

	CP_Settings_NoStroke();
	
}

void Settings_Exit(void)
{
	CP_Font_Free(myFont);
	CP_Font_Free(subFont);
	CP_Font_Free(titleFont);
}