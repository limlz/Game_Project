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