#include <math.h>
#include "cprocessing.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	float area_width_half = area_width / 2;
	float area_height_half = area_height / 2;
	if (click_x < area_center_x + area_width_half &&
		click_x > area_center_x - area_width_half &&
		click_y < area_center_y + area_height_half &&
		click_y > area_center_y - area_height_half){
		return 1;
	}
	else {
		return 0;
	}
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	float radius = diameter / 2.0;
	float vect_x = click_x - circle_center_x;
	float vect_y = click_x - circle_center_x;
	float dist_between_click_and_center = vect_x * vect_x + vect_y * vect_y;
	return dist_between_click_and_center < radius*radius;
}

