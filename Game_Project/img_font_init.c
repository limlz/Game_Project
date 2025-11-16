#include "cprocessing.h"
#include "img_font_init.h"
#include "plate.h"

void InitImagesFontsColors(void) {
	ChangePlate();
	// Load images
	sponge_cat = CP_Image_Load("Assets/sponge_cat.gif");
	soap_bottle = CP_Image_Load("Assets/soap_bottle.gif");
	hamsta = CP_Image_Load("Assets/hambod.gif");
	hamstamugshot = CP_Image_Load("Assets/hamstermugshot.gif");
	arm = CP_Image_Load("Assets/hamgun.gif");
	arm_flipped = CP_Image_Load("Assets/hamgunflip.gif");
	arm_arrow_left = CP_Image_Load("Assets/hamarrorlef.gif");
	arm_arrow_right = CP_Image_Load("Assets/hamarrorit.gif");
	cart = CP_Image_Load("Assets/cart.png");
	front = CP_Image_Load("Assets/hamface.gif");
	background_image = CP_Image_Load("Assets/background.png");
	title_image = CP_Image_Load("Assets/title_soap.png");
	settings_icon = CP_Image_Load("Assets/settings.png");
	leaderboard_icon = CP_Image_Load("Assets/leader.png");
	settings_background = CP_Image_Load("Assets/settings_title_background.png");
	tutorial_towel = CP_Image_Load("Assets/tutorial_towel.png");
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
	roomba_color = CP_Color_Create(255, 255, 255, 200);
}

void FreeImagesFonts(void) {
	// Free images
	CP_Image_Free(sponge_cat);
	CP_Image_Free(soap_bottle);
	CP_Image_Free(hamsta);
	CP_Image_Free(arm);
	CP_Image_Free(arm_flipped);
	CP_Image_Free(hamstamugshot);
	CP_Image_Free(arm_arrow_left);
	CP_Image_Free(arm_arrow_right);
	CP_Image_Free(jiggle1);
	CP_Image_Free(jiggle2);
	CP_Image_Free(jiggle3);
	CP_Image_Free(front);
	CP_Image_Free(cart);

	// Free fonts
	CP_Font_Free(montserrat_light);
	CP_Font_Free(title_font);
	CP_Font_Free(sub_font);

	// No need to free colors as they are simple structs
}