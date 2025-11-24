/*************************************************************************
@file		debug.h
@Author		Lim Liang Zhou l.liangzhou@digipen.edu
@Co-authors	nil
@brief		This file contains the header declarations for the debug
			overlay that displays various game information and
			allows for debug actions.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

int IsCurrentlyDebugging(void);
void StartDebugging(void);
void StopDebugging(void);
void DebugPrint(void);