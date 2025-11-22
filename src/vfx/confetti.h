/*************************************************************************
@file       confetti.h
@Author     Chiu Jun Wen (j.chiu@digipen.edu)
@Co-authors
@brief      This file contains the function declarations for creating
            the confetti particle effect

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/

#pragma once
// function to initialize confetti properties with random values
void InitConfetti(void);

// function to update and draw confetti each frame
void UpdateConfetti(float dt);