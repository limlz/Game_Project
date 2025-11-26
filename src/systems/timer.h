/*************************************************************************
@file       timer.h
@Author		Ng Cher Kai Dan cherkaidan.ng@digipen.edu
@Co-authors Tan Choon Ming choonming.tan@digipen.edu
@brief      This file contains the function declarations for the entire timer system,
			including starting, stopping, resetting, updating the timer.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

void TimerInit(void);
void TimeReset(void);
void TimeAdd(float add_time);

// Return`s 1 if the game is running, 0 if paused
int CheckGameRunning(void);
void TimeStop(void);
int IsTimerStopped(void);
