/*************************************************************************
@file day.h
@Author Tan Choon Ming (choonming.tan@digipen.edu)
@Co-authors NIL
@brief Public interface for managing day progression.
       Handles:
       - Day counters
       - Plate-cleaning goals
       - Day completion popup
       - Transition control into next day and shop state

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

/* Resets all day variables when the game begins or restarts. */
void DayInit(void);

/* Starts the current day (clears popup, resets cleaned count). */
void DayStartCurrentDay(void);

/* Called whenever the player fully cleans a plate. */
void DayOnPlateCleaned(void);

/* Draws the HUD showing day number and plate progress. */
void DayDrawHUD(float x, float y);

/* Draws the day completion popup and auto-transition timer. */
void DayDrawPopup(void);

/* Returns 1 when the player is actively washing dishes (normal gameplay). */
int DayIsInGameplay(void);

/* Getters for basic day statistics. */
int DayGetDay(void);
int DayGetCleaned(void);
int DayGetGoal(void);

/* Returns 1 if the shop should show “Next Day” button. */
int DayIsReadyForNextDay(void);

/* Clears the “ready for next day” flag. */
void DayClearReadyForNextDay(void);

/* Forces a day to end immediately (used in special conditions). */
void DayForceEndDay(void);
