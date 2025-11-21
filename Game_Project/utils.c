/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
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


// Function to check whether circles are intersecting
// Inputs are as followed :
// - The radius, center x and y of the player circle
// - The radius, center x and y of the "zonal" circle
int AreCirclesIntersecting(float c1_center_x, float c1_center_y, float c1_radius, float c2_center_x, float c2_center_y, float c2_center_radius)
{
	float total_radius = c1_radius + c2_center_radius;
	float pointsDist = CP_Math_Distance(c1_center_x, c1_center_y, c2_center_x, c2_center_y);
	return (pointsDist <= total_radius) ? 1 : 0;
}
