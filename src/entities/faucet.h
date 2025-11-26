/*************************************************************************
@file		faucet.h
@Author		Nur Nadia Natasya Binte Mohd Taha nurnadianatasya.b@digipen.edu
@Co-authors
@brief		This file contains the header declarations for the faucet entity,
			including stream drawing and updating, as well as faucet drawing.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

/* Drawing and main stream control */
void DrawFaucet(void);
void DrawStream(void);
void DrawStreamTimer(void);

void InitStream(void);
void UpdateStream(void);
void AOEStream(void);
void StopStream(void);
void CleanDirtWithStream(void);

/* Cooldown reduction helpers implemented in faucet.c */
void  Faucet_ReduceCooldown(float reduction);
void  Faucet_ResetCooldown(void);

/* Upgrade-related functions implemented in upgrades.c */
void  FaucetUpgradeCooldown(void);
void  FaucetUpgradePower(void);
int   FaucetCanUpgradePower(void);
int   FaucetGetPowerLevel(void);
int   FaucetCanUpgradeCooldown(void);
int   FaucetGetCooldownLevel(void);
float FaucetGetCooldownBase(void);
float FaucetGetPowerMultiplier(void);

/* Backwards-compatibility macros for older underscore-style names. */
#define Faucet_UpgradeCooldown     FaucetUpgradeCooldown
#define Faucet_UpgradePower        FaucetUpgradePower
#define Faucet_CanUpgradePower     FaucetCanUpgradePower
#define Faucet_GetPowerLevel       FaucetGetPowerLevel
#define Faucet_CanUpgradeCooldown  FaucetCanUpgradeCooldown
#define Faucet_GetCooldownLevel    FaucetGetCooldownLevel
#define Faucet_GetCooldownBase     FaucetGetCooldownBase
#define Faucet_GetPowerMultiplier  FaucetGetPowerMultiplier

/* Query functions for current cooldown/stream state (implemented in faucet.c). */
float Faucet_ReturnCooldown(void);
float Faucet_ReturnAOETimeLeft(void);
int   Faucet_ReturnIsAttackReady(void);
int   Faucet_ReturnStreamOn(void);
