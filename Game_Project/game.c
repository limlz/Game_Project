#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"

typedef struct {
	float positionX, positionY, angle;
	int direction;
	CP_Color color;
	int isControlledByPlayer;
} Ball;
static Ball npc;
static Ball controller;
static Ball green_ball;
static Ball red_ball;
CP_Color triangle_color;
int speed = 8;
float distance_between_circles, x_diff, y_diff, overlap;
float sprite_width = 60;
float dist_between_balls , x_vector, y_vector;
int isStopped;
void Game_Init(void)
{
	green_ball.color = CP_Color_Create(0, 255, 0, 255);
	red_ball.color = CP_Color_Create(255, 0, 0, 255);
	green_ball.positionX = CP_Math_ClampFloat(25.0,5.0,1595.0);
	green_ball.positionY = CP_Math_ClampFloat(25.0, 5.0, 1595.0);
	green_ball.isControlledByPlayer = 1;
	red_ball.positionX = CP_Math_ClampFloat(700.0, 0.0, 1600.0);
	red_ball.positionY = CP_Math_ClampFloat(700.0, 0.0, 1600.0);
	triangle_color = CP_Color_Create(255, 255, 255, 255);

}


void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(224, 224, 224, 255));
	if (CP_Input_MouseClicked()) {
		if (IsCircleClicked(green_ball.positionX, green_ball.positionY, sprite_width, CP_Input_GetMouseX(), CP_Input_GetMouseY()) &&
			green_ball.isControlledByPlayer == 0) {
			green_ball.isControlledByPlayer = !green_ball.isControlledByPlayer;
			red_ball.isControlledByPlayer = !red_ball.isControlledByPlayer;
		}
		else if (IsCircleClicked(red_ball.positionX, red_ball.positionY, sprite_width, CP_Input_GetMouseX(), CP_Input_GetMouseY()) &&
			red_ball.isControlledByPlayer == 0) {
			green_ball.isControlledByPlayer = !green_ball.isControlledByPlayer;
			red_ball.isControlledByPlayer = !red_ball.isControlledByPlayer;
		}
	}
	if (green_ball.isControlledByPlayer) {
		npc = red_ball;
		controller = green_ball;
	}
	else {
		npc = green_ball;
		controller = red_ball;
	}
	distance_between_circles = CP_Math_Distance(controller.positionX, controller.positionY, npc.positionX, npc.positionY);
	if (CP_Input_KeyDown(KEY_W)) {
		controller.positionY -= speed;
		controller.angle = 0;
		controller.direction = 0;
	}
	else if (CP_Input_KeyDown(KEY_A)) {
		controller.positionX -= speed;
		controller.angle = 270;
		controller.direction = 3;
	}
	else if (CP_Input_KeyDown(KEY_S)) {
		controller.positionY += speed;
		controller.angle = 180;
		controller.direction = 2;
	}
	else if (CP_Input_KeyDown(KEY_D)) {
		controller.positionX += speed;
		controller.angle = 90;
		controller.direction = 1;
	}

	if (npc.positionX - sprite_width / 2 <= 0 ||
		npc.positionY - sprite_width / 2 <= 0 ||
		npc.positionX + sprite_width / 2 >= CP_System_GetWindowWidth() ||
		npc.positionY + sprite_width / 2 >= CP_System_GetWindowHeight() ) {

		npc.direction += 1;
		if (npc.direction > 3 && distance_between_circles > sprite_width) {
			npc.direction = 0;
		}
		else if (distance_between_circles < sprite_width + 20){
			npc.direction = 4;
		}
	} else if (distance_between_circles <= sprite_width + 20) {
		x_diff = npc.positionX - controller.positionX;
		y_diff = npc.positionY - controller.positionY;

		overlap = (sprite_width + 20) - distance_between_circles;

		npc.positionX += (x_diff / distance_between_circles) * overlap;
		npc.positionY += (y_diff / distance_between_circles) * overlap;

		if (x_diff >= y_diff) {
			if (x_diff > 0) {
				npc.direction = 1;
			}
			else {
				npc.direction = 3;
			}
		}
		else {
			if (y_diff > 0) {
				npc.direction = 2;
			}
			else {
				npc.direction = 0;
			}
		}

	}

	if (npc.direction == 0) {
		npc.positionY -= speed;
		npc.angle = 0;
	}
	else if (npc.direction == 1) {
		npc.positionX += speed;
		npc.angle = 90;
	}
	else if (npc.direction == 2) {
		npc.positionY += speed;
		npc.angle = 180;
	}
	else if (npc.direction == 3) {
		npc.positionX -= speed;
		npc.angle = 270;
	}

	npc.positionX = CP_Math_ClampFloat(npc.positionX, 30, 1570);
	npc.positionY = CP_Math_ClampFloat(npc.positionY, 30, 870);
	controller.positionX = CP_Math_ClampFloat(controller.positionX, 30, 1570);
	controller.positionY = CP_Math_ClampFloat(controller.positionY, 30, 870);

	if (green_ball.isControlledByPlayer) {
		red_ball = npc;
		green_ball = controller;
		CP_Settings_Fill(red_ball.color);
		CP_Graphics_DrawCircle(red_ball.positionX, red_ball.positionY, sprite_width);
		CP_Settings_Fill(triangle_color);
		CP_Graphics_DrawTriangleAdvanced((float)(red_ball.positionX), (float)(red_ball.positionY - sprite_width / 2), (float)(red_ball.positionX - sprite_width / 2.0 * 0.866), (float)(red_ball.positionY + sprite_width / 2.0 * 0.5), (float)(red_ball.positionX + sprite_width / 2 * 0.866), (float)(red_ball.positionY + sprite_width / 2 * 0.5), red_ball.angle);
		CP_Settings_Fill(green_ball.color);
		CP_Graphics_DrawCircle(green_ball.positionX, green_ball.positionY, sprite_width);
		CP_Settings_Fill(triangle_color);
		CP_Graphics_DrawTriangleAdvanced((float)(green_ball.positionX), (float)(green_ball.positionY - sprite_width / 2), (float)(green_ball.positionX - sprite_width / 2 * 0.866), (float)(green_ball.positionY + sprite_width / 2 * 0.5), (float)(green_ball.positionX + sprite_width / 2 * 0.866), (float)(green_ball.positionY + sprite_width / 2 * 0.5), green_ball.angle);

	}
	else {
		red_ball = controller;
		green_ball = npc;
		CP_Settings_Fill(green_ball.color);
		CP_Graphics_DrawCircle(green_ball.positionX, green_ball.positionY, sprite_width);
		CP_Settings_Fill(triangle_color);
		CP_Graphics_DrawTriangleAdvanced((float)(green_ball.positionX), (float)(green_ball.positionY - sprite_width / 2), (float)(green_ball.positionX - sprite_width / 2 * 0.866), (float)(green_ball.positionY + sprite_width / 2 * 0.5), (float)(green_ball.positionX + sprite_width / 2 * 0.866), (float)(green_ball.positionY + sprite_width / 2 * 0.5), green_ball.angle);
		CP_Settings_Fill(red_ball.color);
		CP_Graphics_DrawCircle(red_ball.positionX, red_ball.positionY, sprite_width);
		CP_Settings_Fill(triangle_color);
		CP_Graphics_DrawTriangleAdvanced((float)(red_ball.positionX), (float)(red_ball.positionY - sprite_width / 2), (float)(red_ball.positionX - sprite_width / 2.0 * 0.866), (float)(red_ball.positionY + sprite_width / 2.0 * 0.5), (float)(red_ball.positionX + sprite_width / 2 * 0.866), (float)(red_ball.positionY + sprite_width / 2 * 0.5), red_ball.angle);

	}
	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}



}

void Game_Exit(void)
{

}

