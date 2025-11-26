/*************************************************************************
@file upgrades.c
@Author Tan Choon Ming (choonming.tan@digipen.edu)
@Co-authors
@brief This file contains the implementations of the various upgrades:
       sponge upgrade, soap purchase, soap drain upgrade, roomba upgrade,
       and faucet (stream) power/cooldown upgrades.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "upgrades.h"
#include "money.h"
#include "sponge.h"
#include "soap.h"
#include "roomba.h"
#include "faucet.h"

/* Sponge upgrade state */
static int sponge_upgrade_cost = 3;
static int sponge_increment = 1;

/* Soap refill state */
static int soap_cost = 2;

/* Soap drain (Soap Saver) upgrade state */
static int soap_drain_upgrade_cost = 6;
static int soap_drain_increment = 2;

/* Roomba (cleaning robot) upgrade state */
static int roomba_upgrade_cost = 20;
static int roomba_increment = 5;

/* Faucet (stream) upgrade cost tuning */
static const int faucet_power_base_cost = 15;
static const int faucet_power_cost_increment = 5;
static const int faucet_cooldown_upgrade_cost = 20;

/* Faucet upgrade levels */
static int faucet_power_level = 0;
static int faucet_cooldown_level = 0;

/* Faucet power behaviour */
static const float faucet_power_increase_per_level = 0.05f;
static const float faucet_power_max_multiplier = 1.5f;
static const int faucet_power_max_level = 10;

/* Faucet cooldown behaviour */
static const int faucet_cooldown_max_level = 10;
static const float faucet_base_cooldown_value = 20.0f;

/* Returns the current money cost of the next sponge power upgrade. */
int UpgradesGetSpongeCost(void) {
    return sponge_upgrade_cost;
}

/* Resets sponge upgrade cost and increment back to their starting values (used when restarting a run). */
void ResetSpongeUpgradeCost(void) {
    sponge_upgrade_cost = 3;
    sponge_increment = 1;
}

/* Tries to buy a sponge power upgrade if the player has enough money. */
void UpgradesAttemptSpongeUpgrade(void) {
    if (GetCurrentMoney() < sponge_upgrade_cost) {
        return;
    }

    DecrementMoney(sponge_upgrade_cost);
    UpgradeSponge();

    /* Increase cost more and more each time. */
    sponge_upgrade_cost += sponge_increment;
    sponge_increment++;
}

/* Returns the current money cost to refill soap to maximum. */
int UpgradesGetSoapCost(void) {
    return soap_cost;
}

/* Tries to purchase a full soap refill if there is room and enough money. */
void UpgradesAttemptSoapPurchase(void) {
    if (SoapIsFull() || GetCurrentMoney() < soap_cost) {
        return;
    }

    DecrementMoney(soap_cost);
    SoapRefill();
}

/* Returns the current cost to upgrade the soap saver (slower soap drain). */
int UpgradesGetSoapDrainCost(void) {
    return soap_drain_upgrade_cost;
}

/* Tries to upgrade the soap saver so soap drains more slowly. */
void UpgradesAttemptSoapDrainUpgrade(void) {
    if (!SoapCanUpgradeDrain() || GetCurrentMoney() < soap_drain_upgrade_cost) {
        return;
    }

    DecrementMoney(soap_drain_upgrade_cost);
    SoapUpgradeDrain();

    /* Increase future upgrade cost and scaling. */
    soap_drain_upgrade_cost += soap_drain_increment;
    soap_drain_increment++;
}

/* Returns the current cost to upgrade the cleaning robot (Roomba). */
int UpgradesGetRoombaCost(void) {
    return roomba_upgrade_cost;
}

/* Tries to upgrade the robot's cleaning strength and speed. */
void UpgradesAttemptRoombaUpgrade(void) {
    if (GetCurrentMoney() < roomba_upgrade_cost) {
        return;
    }

    DecrementMoney(roomba_upgrade_cost);
    AddRoombaStrength(2);
    AddRoombaSpeed(20.0f);

    roomba_upgrade_cost += roomba_increment;
    roomba_increment++;
}

/* Returns the cost for the next faucet stream power upgrade. */
int UpgradesGetFaucetPowerCost(void) {
    return faucet_power_base_cost + FaucetGetPowerLevel() * faucet_power_cost_increment;
}

/* Tries to upgrade how strong the water stream is (cleaning multiplier). */
void UpgradesAttemptFaucetPowerUpgrade(void) {
    if (!FaucetCanUpgradePower()) {
        return;
    }

    int cost = UpgradesGetFaucetPowerCost();
    if (GetCurrentMoney() < cost) {
        return;
    }

    DecrementMoney(cost);
    FaucetUpgradePower();
}

/* Returns the fixed cost for each faucet cooldown upgrade. */
int UpgradesGetFaucetCooldownCost(void) {
    return faucet_cooldown_upgrade_cost;
}

/* Tries to upgrade the faucet cooldown so the stream can be used more often. */
void UpgradesAttemptFaucetCooldownUpgrade(void) {
    if (!FaucetCanUpgradeCooldown() || GetCurrentMoney() < faucet_cooldown_upgrade_cost) {
        return;
    }

    DecrementMoney(faucet_cooldown_upgrade_cost);
    FaucetUpgradeCooldown();
}

/* Increases internal faucet power level, clamped to the maximum level. */
void FaucetUpgradePower(void) {
    if (FaucetCanUpgradePower()) {
        faucet_power_level++;
        if (faucet_power_level > faucet_power_max_level) {
            faucet_power_level = faucet_power_max_level;
        }
    }
}

/* Returns non-zero if the faucet power can still be upgraded. */
int FaucetCanUpgradePower(void) {
    return faucet_power_level < faucet_power_max_level;
}

/* Returns the current faucet power level. */
int FaucetGetPowerLevel(void) {
    return faucet_power_level;
}

/* Returns a multiplier applied to stream cleaning speed based on power level. */
float FaucetGetPowerMultiplier(void) {
    float multiplier = 1.0f + (float)faucet_power_level * faucet_power_increase_per_level;
    if (multiplier > faucet_power_max_multiplier) {
        multiplier = faucet_power_max_multiplier;
    }
    return multiplier;
}

/* Increases faucet cooldown level, clamps it, and resets the active cooldown timer. */
void FaucetUpgradeCooldown(void) {
    if (FaucetCanUpgradeCooldown()) {
        faucet_cooldown_level++;
        if (faucet_cooldown_level > faucet_cooldown_max_level) {
            faucet_cooldown_level = faucet_cooldown_max_level;
        }

        /* Reset the faucet's internal cooldown timer after an upgrade. */
        Faucet_ResetCooldown();
    }
}

/* Returns non-zero if faucet cooldown can still be upgraded. */
int FaucetCanUpgradeCooldown(void) {
    return faucet_cooldown_level < faucet_cooldown_max_level;
}

/* Returns the current faucet cooldown upgrade level. */
int FaucetGetCooldownLevel(void) {
    return faucet_cooldown_level;
}

/* Returns the base cooldown value before any upgrades are applied. */
float FaucetGetCooldownBase(void) {
    return faucet_base_cooldown_value;
}
