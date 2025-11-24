/*************************************************************************
@file		faucet.h
@Author		Nur Nadia Natasya Binte Mohd Taha nurnadianatasya.b@digipen.edu
@Co-authors	
@brief		This file contains the header declarations for the faucet entity,
			including stream drawing and updating, as well as faucet drawing.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

void DrawFaucet(void);
void DrawStream(void);
void DrawStreamTimer(void);

void InitStream(void);
void UpdateStream(void);
void AOEStream(void);
void StopStream(void);
void CleanDirtWithStream(void);

void Faucet_ReduceCooldown(float reduction);
void Faucet_ResetCooldown(void);
void Faucet_UpgradeCooldown(void);
void Faucet_UpgradePower(void);
int Faucet_CanUpgradePower(void);
int Faucet_GetPowerLevel(void);
int Faucet_CanUpgradeCooldown(void);
int Faucet_GetCooldownLevel(void);
float Faucet_GetCooldownBase(void);
float Faucet_GetPowerMultiplier(void);

float Faucet_ReturnCooldown(void);
float Faucet_ReturnAOETimeLeft(void);
int Faucet_ReturnIsAttackReady(void);
int Faucet_ReturnStreamOn(void);