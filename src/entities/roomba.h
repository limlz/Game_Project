/*************************************************************************
@file		roomba.h
@Author		lim liang zhou (l.liangzhou@digipen.edu)
@Co-authors	Nur Nadia Natasya Binte Mohd Taha (nurnadianatasya.b@digipen.edu)
@brief		This file contains the function declarations for the Roomba entity,
            including its initialization, purchasing, movement toward dirt,
            and scrubbing functionality.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once
#ifndef ROOMBA_H
#define ROOMBA_H

// Public interface
void InitRoomba(void);
void PurchaseRoomba(void);
void RoombaFunction(void);
int  GetRoombaStrength(void);
void AddRoombaStrength(int add_roomba);
void AddRoombaSpeed(int add_roomba);
void MinusRoombaStrength(int minus_roomba);
int  RoombaPurchase(void);
int  GetClosestDirtIndex(void);
int  GetRoombaCurrentlyScrubbing(void);
float GetRoombaX(void);
float GetRoombaY(void);
float GetRoombaSpeed(void);
void ResetRoomba(void);

// Maximum number of Roombas allowed in the scene
#define MAX_ROOMBA 10

// Internal Roomba representation (also usable by other modules if needed)
typedef struct roomba {
    float position_x;
    float position_y;
    float angle;
    int   current_dirt_index;
    int   currently_scrubbing;
    int   current_frame;
    float anim_timer;
} roomba;

#endif // ROOMBA_H
