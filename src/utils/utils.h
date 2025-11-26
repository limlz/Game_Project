/*************************************************************************
@file		utils.c
@Author		EVERYONE
@Co-authors	EVERYONE
@brief		utils functions for various checks like area click, circle click,
			and circle intersection from our solo projects

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
int AreCirclesIntersecting(float c1_center_x, float c1_center_y, float c1_radius, float c2_center_x, float c2_center_y, float c2_center_radius);
