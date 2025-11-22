/*************************************************************************
@file       leaderboard.h
@Author     Chiu Jun Wen (j.chiu@digipen.edu)
@Co-authors
@brief      This file contains the function declaration for the
			leaderboard screen

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

int GetLowestScore(void);
int GetHighestScore(void);
void Leaderboard_Init(void);
void Leaderboard_Update(void);
void Leaderboard_Exit(void);