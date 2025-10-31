#include "cprocessing.h"
#include "img_font_init.h"

void InitImagesFontsColors(void) {
	// Load images
	sponge_cat = CP_Image_Load("Assets/sponge_cat.gif");
	soap_bottle = CP_Image_Load("Assets/soap_bottle.gif");
	hamsta = CP_Image_Load("Assets/hambod.gif");
	hamstamugshot = CP_Image_Load("Assets/hamstermugshot.gif");
	arm = CP_Image_Load("Assets/hamgun.gif");
	arm_flipped = CP_Image_Load("Assets/hamgunflip.gif");
	cart = CP_Image_Load("Assets/cart.gif");
	front = CP_Image_Load("Assets/hamface.gif");
	jiggle1 = CP_Image_Load("Assets/jiggle1.gif");
	jiggle2 = CP_Image_Load("Assets/jiggle2.gif");
	jiggle3 = CP_Image_Load("Assets/jiggle3.gif");

	// Load fonts
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	title_font = CP_Font_Load("Assets/SuperWater.ttf");
	sub_font = CP_Font_Load("Assets/MontserratBlackItalic.otf");

	// Create colors
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	yellow = CP_Color_Create(255, 214, 10, 255);
	dark_grey = CP_Color_Create(40, 40, 40, 180);
	blue_chalk = CP_Color_Create(233, 239, 255, 255);
	blue_grey = CP_Color_Create(186, 191, 197, 255);
	green = CP_Color_Create(0, 200, 0, 140);
	red = CP_Color_Create(244, 3, 48, 255);
	grey = CP_Color_Create(150, 150, 150, 160);
	button_blue = CP_Color_Create(123, 183, 220, 255);
	plate_outer = CP_Color_Create(230, 230, 230, 255);
	plate_inner = CP_Color_Create(210, 210, 210, 255);
	roomba_color = CP_Color_Create(255, 255, 255, 200);
}

void FreeImagesFonts(void) {
	// Free images
	CP_Image_Free(sponge_cat);
	CP_Image_Free(soap_bottle);
	CP_Image_Free(hamsta);
	CP_Image_Free(arm);
	CP_Image_Free(arm_flipped);

	// Free fonts
	CP_Font_Free(montserrat_light);
	CP_Font_Free(title_font);
	CP_Font_Free(sub_font);

	// No need to free colors as they are simple structs
}