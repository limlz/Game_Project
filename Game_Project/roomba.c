#include "cprocessing.h"
#include "utils.h"
#include "bubbles.h"
#include "dirt.h"

extern int opacities[] ;
extern float dirt_x[] ;
extern float dirt_y[] ;

// ROOMBA
float roomba_x = 0, roomba_y = 0, roomba_angle = 0;
int roomba_width = 50;
int closest_dirt = 0;
int roomba_strength = 3;
float roomba_speed = 100.0f;
CP_Color roomba_color;

int get_roomba_strength(void) {
	return roomba_strength;
}

void add_roomba_strength(int add_roomba) {
	roomba_strength += add_roomba;
}

void minus_roomba_strength(int minus_roomba) {
	roomba_strength -= minus_roomba;
}

void add_roomba_speed(int add_roomba) {
	roomba_speed += add_roomba;
}

void minus_roomba_speed(int minus_roomba) {
	roomba_speed -= minus_roomba;
}

void init_roomba(void) {
	roomba_x = CP_System_GetWindowWidth() / 2;
	roomba_y = CP_System_GetWindowHeight() / 2;
	roomba_color = CP_Color_Create(255, 255, 255, 200);
}
void roomba(void) {
	// target and position vectors
	CP_Vector roomba_v = CP_Vector_Set(roomba_x, roomba_y);
	CP_Vector dirt_v;
	closest_dirt = -1;
	float closestDist = 999999.0f;
	for (int i = 0; i < 5; i++) { // 5 dirt spots: 0–4
		if (opacities[i] > 0) { // only consider visible dirt
			float d = CP_Math_Distance(roomba_x, roomba_y, dirt_x[i], dirt_y[i]);
			if (d < closestDist) {
				closestDist = d;
				closest_dirt = i;
			}
		}
	}
	dirt_v = CP_Vector_Set(dirt_x[closest_dirt], dirt_y[closest_dirt]);

	// compute direction (from roomba -> dirt)
	CP_Vector dir = CP_Vector_Subtract(dirt_v, roomba_v);

	// compute facing angle
	CP_Vector up = CP_Vector_Set(0, 1);
	roomba_angle = CP_Vector_AngleCW(up, CP_Vector_Negate(dir));

	// move directly toward dirt if not close enough
	if (CP_Vector_Distance(dirt_v, roomba_v) > roomba_width) {
		dir = CP_Vector_Normalize(dir);
		float speed = roomba_speed * CP_System_GetDt();
		CP_Vector move = CP_Vector_Scale(dir, speed);
		roomba_v = CP_Vector_Add(roomba_v, move);

		// update position
		roomba_x = roomba_v.x;
		roomba_y = roomba_v.y;
	}
	else {
		opacities[closest_dirt] -= roomba_strength;
		opacities[closest_dirt] = CP_Math_ClampInt(opacities[closest_dirt], 0, 200);
		SpawnBubble(dirt_x[closest_dirt], dirt_y[closest_dirt]);
	}

	// draw da roomba
	CP_Settings_Fill(roomba_color);
	CP_Graphics_DrawCircle(roomba_x, roomba_y, roomba_width);
	CP_Graphics_DrawTriangleAdvanced(roomba_x, roomba_y - roomba_width / 2, roomba_x - roomba_width / 2 * 0.866f, roomba_y + roomba_width / 2 * 0.5f, roomba_x + roomba_width / 2 * 0.866f, roomba_y + roomba_width / 2 * 0.5f, roomba_angle);

}
