/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once
#ifndef UPGRADES_H
#define UPGRADES_H

int Upgrades_GetSpongeCost(void);
void Upgrades_AttemptSpongeUpgrade(void);

int Upgrades_GetSoapCost(void);
void Upgrades_AttemptSoapPurchase(void);

int Upgrades_GetSoapDrainCost(void);
void Upgrades_AttemptSoapDrainUpgrade(void);

int Upgrades_GetRoombaCost(void);
void Upgrades_AttemptRoombaUpgrade(void);

int Upgrades_GetFaucetPowerCost(void);
void Upgrades_AttemptFaucetPowerUpgrade(void);

int Upgrades_GetFaucetCooldownCost(void);
void Upgrades_AttemptFaucetCooldownUpgrade(void);

#endif // UPGRADES_H
