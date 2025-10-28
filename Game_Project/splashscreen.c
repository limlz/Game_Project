#include "cprocessing.h"
#include "mainmenu.h"

CP_Font montserrat_light;
CP_Image logo;
float timer = 0.0f;

void Splash_Screen_Init(void)
{
	logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
}

void Splash_Screen_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_DrawAdvanced(logo, (CP_System_GetWindowWidth() / 2), (CP_System_GetWindowHeight() / 2), CP_Image_GetWidth(logo), CP_Image_GetHeight(logo), 255, 0);

	timer += CP_System_GetDt();
	if (timer > 3.0) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void Splash_Screen_Exit(void)
{
	void CP_Image_Free(logo);
}