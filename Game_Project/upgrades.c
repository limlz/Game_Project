#include "upgrades.h"
#include "money.h"
#include "sponge.h"
#include "soap.h"
#include "roomba.h"
#include "faucet.h"

static int spongeUpgradeCost = 3;
static int spongeIncrement = 1;

static int soapCost = 2;

static int soapDrainUpgradeCost = 6;
static int soapDrainIncrement = 2;

static int roombaUpgradeCost = 20;
static int roombaIncrement = 5;

static const int FaucetPowerBaseCost = 15;
static const int FaucetPowerCostIncrement = 5;
static const int FaucetCooldownUpgradeCost = 20;

static int faucetPowerLevel = 0;
static int faucetCooldownLevel = 0;

static const float FaucetPowerIncreasePerLevel = 0.05f;
static const float FaucetPowerMaxMultiplier = 1.5f;
static const int FaucetPowerMaxLevel = 10;
static const int FaucetCooldownMaxLevel = 10;
static const float FaucetBaseCooldownValue = 20.0f;

int Upgrades_GetSpongeCost(void) {
    return spongeUpgradeCost;
}

void Upgrades_AttemptSpongeUpgrade(void) {
    if (GetCurrentMoney() < spongeUpgradeCost) {
        return;
    }
    DecrementMoney(spongeUpgradeCost);
    UpgradeSponge();

    spongeUpgradeCost += spongeIncrement;
    spongeIncrement++;
}

int Upgrades_GetSoapCost(void) {
    return soapCost;
}

void Upgrades_AttemptSoapPurchase(void) {
    if (Soap_IsFull() || GetCurrentMoney() < soapCost) {
        return;
    }
    DecrementMoney(soapCost);
    Soap_Refill();
}

int Upgrades_GetSoapDrainCost(void) {
    return soapDrainUpgradeCost;
}

void Upgrades_AttemptSoapDrainUpgrade(void) {
    if (!Soap_CanUpgradeDrain() || GetCurrentMoney() < soapDrainUpgradeCost) {
        return;
    }
    DecrementMoney(soapDrainUpgradeCost);
    Soap_UpgradeDrain();

    soapDrainUpgradeCost += soapDrainIncrement;
    soapDrainIncrement++;
}

int Upgrades_GetRoombaCost(void) {
    return roombaUpgradeCost;
}

void Upgrades_AttemptRoombaUpgrade(void) {
    if (GetCurrentMoney() < roombaUpgradeCost) {
        return;
    }
    DecrementMoney(roombaUpgradeCost);
    AddRoombaStrength(2);
    AddRoombaSpeed(20.0f);

    roombaUpgradeCost += roombaIncrement;
    roombaIncrement++;
}

int Upgrades_GetFaucetPowerCost(void) {
    return FaucetPowerBaseCost + Faucet_GetPowerLevel() * FaucetPowerCostIncrement;
}

void Upgrades_AttemptFaucetPowerUpgrade(void) {
    if (!Faucet_CanUpgradePower()) {
        return;
    }
    int cost = Upgrades_GetFaucetPowerCost();
    if (GetCurrentMoney() < cost) {
        return;
    }
    DecrementMoney(cost);
    Faucet_UpgradePower();
}

int Upgrades_GetFaucetCooldownCost(void) {
    return FaucetCooldownUpgradeCost;
}

void Upgrades_AttemptFaucetCooldownUpgrade(void) {
    if (!Faucet_CanUpgradeCooldown() || GetCurrentMoney() < FaucetCooldownUpgradeCost) {
        return;
    }
    DecrementMoney(FaucetCooldownUpgradeCost);
    Faucet_UpgradeCooldown();
}

void Faucet_UpgradePower(void) {
    if (Faucet_CanUpgradePower()) {
        faucetPowerLevel++;
        if (faucetPowerLevel > FaucetPowerMaxLevel) {
            faucetPowerLevel = FaucetPowerMaxLevel;
        }
    }
}

int Faucet_CanUpgradePower(void) {
    return faucetPowerLevel < FaucetPowerMaxLevel;
}

int Faucet_GetPowerLevel(void) {
    return faucetPowerLevel;
}

float Faucet_GetPowerMultiplier(void) {
    float multiplier = 1.0f + (float)faucetPowerLevel * FaucetPowerIncreasePerLevel;
    if (multiplier > FaucetPowerMaxMultiplier) {
        multiplier = FaucetPowerMaxMultiplier;
    }
    return multiplier;
}

void Faucet_UpgradeCooldown(void) {
    if (Faucet_CanUpgradeCooldown()) {
        faucetCooldownLevel++;
        if (faucetCooldownLevel > FaucetCooldownMaxLevel) {
            faucetCooldownLevel = FaucetCooldownMaxLevel;
        }
        reset_cooldown();
    }
}

int Faucet_CanUpgradeCooldown(void) {
    return faucetCooldownLevel < FaucetCooldownMaxLevel;
}

int Faucet_GetCooldownLevel(void) {
    return faucetCooldownLevel;
}

float Faucet_GetCooldownBase(void) {
    return FaucetBaseCooldownValue;
}