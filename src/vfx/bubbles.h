/*************************************************************************
@file		bubbles.h
@Author		Lim Liang Zhou l.liangzhou@digipen.edu
@Co-authors	nil
@brief		This file contains the header declarations for spawning, updating,
			and rendering bubble visual effects when the sponge is used and when
			bubbles are manually spawned.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
void SpawnBubble(float mx, float my);
void BubblesInit(void);
void Bubbles_Draw(void);
void BubblesManual(float manualX, float manualY);
