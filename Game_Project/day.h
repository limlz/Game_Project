#pragma once

void Day_Init(void);
void Day_StartCurrentDay(void);
void Day_OnPlateCleaned(void);
void Day_DrawHUD(float x, float y);

int Day_IsInGameplay(void);
int Day_GetDay(void);
int Day_GetCleaned(void);
int Day_GetGoal(void);

int Day_IsReadyForNextDay(void);
void Day_ClearReadyForNextDay(void);
