#include "cprocessing.h"
#include "utils.h"
#include "bubbles.h"
#include "dirt.h"
#include "timer.h"

// ROOMBA
float roomba_x = 0, roomba_y = 0, roomba_angle = 0;
int roomba_width = 50;
int closest_dirt = 0;
int roomba_strength = 3;
float roomba_speed = 100.0f;
int current_frame = 0;
float anitimer = 0.0f;
float frame_duration = 0.1f; // seconds per frame
CP_Color roomba_color;
CP_Image jiggle1, jiggle2, jiggle3;

int get_roomba_strength(void) {
	return roomba_strength;
}

void add_roomba_strength(int add_roomba) {
	roomba_strength += add_roomba;
}

void minus_roomba_strength(int minus_roomba) {
	roomba_strength -= minus_roomba;
}

void add_roomba_speed(int add_roomba_speed) {
	roomba_speed += add_roomba_speed;
}

void minus_roomba_speed(int minus_roomba_speed) {
	roomba_speed -= minus_roomba_speed;
}

void init_roomba(void) {
	roomba_x = CP_System_GetWindowWidth() / 2;
	roomba_y = CP_System_GetWindowHeight() / 2;
	roomba_color = CP_Color_Create(255, 255, 255, 200);
	jiggle1 = CP_Image_Load("Assets/jiggle1.gif");
	jiggle2 = CP_Image_Load("Assets/jiggle2.gif");
	jiggle3 = CP_Image_Load("Assets/jiggle3.gif");
}
void roomba(void) {
	// target and position vectors
	CP_Vector roomba_v = CP_Vector_Set(roomba_x, roomba_y);
	CP_Vector dirt_v;
	closest_dirt = -1;
	float closestDist = 999999.0f;
	for (int i = 0; i < get_number_of_dirt(); i++) { // 5 dirt spots: 0–4
		if (dirtList[i].opacity > 0) { // only consider visible dirt
			float d = CP_Math_Distance(roomba_x, roomba_y, dirtList[i].positionX, dirtList[i].positionY);
			if (d < closestDist) {
				closestDist = d;
				closest_dirt = i;
			}
		}
	}
	dirt_v = CP_Vector_Set(dirtList[closest_dirt].positionX, dirtList[closest_dirt].positionY);

	// compute direction (from roomba -> dirt)
	CP_Vector dir = CP_Vector_Subtract(dirt_v, roomba_v);

	// compute facing angle
	CP_Vector up = CP_Vector_Set(0, 1);
	roomba_angle = CP_Vector_AngleCW(up, CP_Vector_Negate(dir));

	// move directly toward dirt if not close enough
	if (checkGameRunning()) {
		if (CP_Vector_Distance(dirt_v, roomba_v) > 1) {
			dir = CP_Vector_Normalize(dir);
			float speed = roomba_speed * CP_System_GetDt();
			CP_Vector move = CP_Vector_Scale(dir, speed);
			roomba_v = CP_Vector_Add(roomba_v, move);

			// update position
			roomba_x = roomba_v.x;
			roomba_y = roomba_v.y;
		}
		else {
			dirtList[closest_dirt].opacity -= roomba_strength;
			dirtList[closest_dirt].opacity = CP_Math_ClampInt(dirtList[closest_dirt].opacity, 0, 200);
			SpawnBubble(dirtList[closest_dirt].positionX, dirtList[closest_dirt].positionY);
		}
	}


	// draw da roomba
	anitimer += CP_System_GetDt();

    // Switch frames when time passes threshold
	if (anitimer >= frame_duration)
	{
		anitimer = 0;
		current_frame = (current_frame + 1) % 3;
	}


	float x = CP_System_GetWindowWidth() / 2.0f;
	float y = CP_System_GetWindowHeight() / 2.0f;

	// Display the current image
	switch (current_frame)
	{
	case 0:
		CP_Image_Draw(jiggle1, roomba_x - 50, roomba_y + 30, 150, 150, 255);
		break;
	case 1:
		CP_Image_Draw(jiggle2, roomba_x - 50, roomba_y + 30, 150, 150, 255);
		break;
	case 2:
		CP_Image_Draw(jiggle3, roomba_x - 50, roomba_y + 30, 150, 150, 255);
		break;
	}
	//CP_Settings_Fill(roomba_color);
	//CP_Graphics_DrawCircle(roomba_x, roomba_y, roomba_width);
	//CP_Graphics_DrawTriangleAdvanced(roomba_x, roomba_y - roomba_width / 2, roomba_x - roomba_width / 2 * 0.866f, roomba_y + roomba_width / 2 * 0.5f, roomba_x + roomba_width / 2 * 0.866f, roomba_y + roomba_width / 2 * 0.5f, roomba_angle);
}
