/*************************************************************************
@file		money.c
@Author		Lim Liang Zhou (l.liangzhou@digipen.edu)
@Co-authors	nil
@brief		This file contains the function declarations for managing
			the in-game money system, including tracking current money,
			total earnings, plate value, and displaying money on screen.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
void MoneyDisplay(void);
int GetCurrentMoney(void);
int GetTotalEarned(void);
void IncrementMoney(int money_change);
void DecrementMoney(int money_change);
void SetPlateValue(int plate_change);
int GetPlateValue(void);
int ResetMoney(void);