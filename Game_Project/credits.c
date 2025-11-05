#include "credits.h"
#include "cprocessing.h"
#include <stdio.h>
#include "mainmenu.h"

#define MAX_LINES 200

typedef struct 
{
	CP_Image image;
	float width;
	float height;
}image;

static image prassana;

CP_Font montserrat_light;
FILE *creditsFile;
CP_Color black;
char *creditsLines[MAX_LINES];
int totalLines;
float scrollY;
float speed;
char buffer[256];

void credits_Init(void) {

	black = CP_Color_Create(0, 0, 0, 255);

	// load image
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	prassana.image = CP_Image_Load("Assets/Prasanna Ghali.jpg");
	prassana.height = CP_Image_GetHeight(prassana.image) * 3;
	prassana.width = CP_Image_GetWidth(prassana.image) * 7;

	// load font
	montserrat_light = CP_Font_Load("Assets/Exo2-Regular.ttf");

	// load credits file
	errno_t err = fopen_s(&creditsFile, "Assets/credits.txt", "r");
	if (err != 0 || creditsFile == NULL) {
		return;
	}
	totalLines = 0;
	while (fgets(buffer, sizeof(buffer), creditsFile) && totalLines < MAX_LINES) {

		buffer[strcspn(buffer, "\r\n")] = 0; //removes newline

		creditsLines[totalLines] = _strdup(buffer);
		totalLines++;
	}

	// unload credits file
	fclose(creditsFile);

	scrollY = CP_System_GetWindowHeight();
	speed = 200.0f;

}
void credits_Update(void) {
	float deltaTime = CP_System_GetDt();

	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	CP_Image_Draw(prassana.image, CP_System_GetWindowWidth() / 2.0f, scrollY, prassana.width, prassana.height, 255);
	// Draw credits
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(black);
	CP_Settings_TextSize(30.0f);

	for (int i = 0; i < totalLines; i++) {
		CP_Font_DrawText(creditsLines[i], CP_System_GetWindowWidth() / 2.0f, scrollY + prassana.height/2.0f + 15.0f + i * 15.0f);
	}

	// scroll upwards
	scrollY -= speed * deltaTime;

	// go back main menu (temporary)
	if (CP_Input_KeyTriggered(KEY_C)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}
void credits_Exit(void) {
	CP_Font_Free(montserrat_light);
	CP_Image_Free(prassana.image);
} 