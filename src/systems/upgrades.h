/*************************************************************************
@file
@Author
@Co-authors
@brief Public interface for all shop-related upgrade operations:
       sponge upgrades, soap refill and drain upgrades, roomba upgrades,
       and faucet (stream) power/cooldown upgrades.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once
#ifndef UPGRADES_H
#define UPGRADES_H

/* Sponge upgrades */
int  UpgradesGetSpongeCost(void);          /* Returns cost of the next sponge upgrade. */
void UpgradesAttemptSpongeUpgrade(void);   /* Attempts to buy a sponge upgrade. */
void ResetSpongeUpgradeCost(void);         /* Resets sponge upgrade cost to its initial values. */

/* Soap (refill + saver) upgrades */
int  UpgradesGetSoapCost(void);            /* Returns the cost to refill soap to max. */
void UpgradesAttemptSoapPurchase(void);    /* Attempts to buy a full soap refill. */

int  UpgradesGetSoapDrainCost(void);       /* Returns the cost of the next soap drain upgrade. */
void UpgradesAttemptSoapDrainUpgrade(void);/* Attempts to buy a soap drain (Soap Saver) upgrade. */

/* Roomba (cleaning robot) upgrade */
int  UpgradesGetRoombaCost(void);          /* Returns cost of the next roomba upgrade. */
void UpgradesAttemptRoombaUpgrade(void);   /* Attempts to upgrade roomba strength and speed. */

/* Faucet (stream) power upgrades */
int  UpgradesGetFaucetPowerCost(void);         /* Returns cost of the next faucet power upgrade. */
void UpgradesAttemptFaucetPowerUpgrade(void);  /* Attempts to upgrade faucet power. */

/* Faucet (stream) cooldown upgrades */
int  UpgradesGetFaucetCooldownCost(void);      /* Returns cost of the next faucet cooldown upgrade. */
void UpgradesAttemptFaucetCooldownUpgrade(void);/* Attempts to upgrade faucet cooldown. */

/* Backwards-compatibility macros for older underscore-style names. */
#define Upgrades_GetSpongeCost           UpgradesGetSpongeCost
#define Upgrades_AttemptSpongeUpgrade    UpgradesAttemptSpongeUpgrade
#define Reset_SpongeUpgradeCost          ResetSpongeUpgradeCost

#define Upgrades_GetSoapCost             UpgradesGetSoapCost
#define Upgrades_AttemptSoapPurchase     UpgradesAttemptSoapPurchase

#define Upgrades_GetSoapDrainCost        UpgradesGetSoapDrainCost
#define Upgrades_AttemptSoapDrainUpgrade UpgradesAttemptSoapDrainUpgrade

#define Upgrades_GetRoombaCost           UpgradesGetRoombaCost
#define Upgrades_AttemptRoombaUpgrade    UpgradesAttemptRoombaUpgrade

#define Upgrades_GetFaucetPowerCost           UpgradesGetFaucetPowerCost
#define Upgrades_AttemptFaucetPowerUpgrade    UpgradesAttemptFaucetPowerUpgrade

#define Upgrades_GetFaucetCooldownCost        UpgradesGetFaucetCooldownCost
#define Upgrades_AttemptFaucetCooldownUpgrade UpgradesAttemptFaucetCooldownUpgrade

#endif // UPGRADES_H
