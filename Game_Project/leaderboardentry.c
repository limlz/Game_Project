#include "cprocessing.h"
#include "leaderboardentry.h"

CP_Color white;
CP_Color black;
CP_Color background_colour;
CP_Color button_blue;
CP_Font main_font;
CP_Font sub_font;
CP_Font montserrat_light;

void Leaderboard_Entry_Init(void) {
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	background_colour = CP_Color_Create(233, 239, 255, 255);
	button_blue = CP_Color_Create(123, 183, 220, 255);
	main_font = CP_Font_Load("Assets/SuperWater.ttf");
	sub_font = CP_Font_Load("Assets/SuperWater.ttf");
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
}
void Leaderboard_Entry_Update(void) {
	CP_Graphics_ClearBackground(background_colour);
}
void Leaderboard_Entry_Exit(void) {

}