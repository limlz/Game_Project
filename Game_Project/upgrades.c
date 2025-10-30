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

int Upgrades_GetSpongeCost(void) {
    return spongeUpgradeCost;
}

void Upgrades_AttemptSpongeUpgrade(void) {
    if (GetCurrentMoney() < spongeUpgradeCost) {
        return;
    }
    DecrementMoney(spongeUpgradeCost);
    upgrade_Sponge();

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