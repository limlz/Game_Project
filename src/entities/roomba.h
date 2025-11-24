/*************************************************************************
@file		roomba.c
@Author		lim liang zhou (l.liangzhou@digipen.edu)
@Co-authors	Nur Nadia Natasya Binte Mohd Taha (nurnadianatasya.b@digipen.edu)
@brief		This file contains the function declarations for the Roomba entity,
			including its initialization, purchasing, movement toward dirt,
			and scrubbing functionality.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
void InitRoomba(void);
void PurchaseRoomba(void);
void RoombaFunction(void);
int GetRoombaStrength(void);
void AddRoombaStrength(int add_roomba);
void AddRoombaSpeed(int AddRoombaSpeed);
void MinusRoombaStrength(int minus_roomba);
int RoombaPurchase(void);
int GetClosestDirtIndex(void);
int GetRoombaCurrentlyScrubbing(void);
float GetRoombaX(void); 
float GetRoombaY(void);
float GetRoombaSpeed(void);
void ResetRoomba(void);