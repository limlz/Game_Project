#include "upgrades.h"
#include "money.h"
#include "sponge.h"
#include "soap.h"
#include "roomba.h"
#include "faucet.h"

static int s_spongeUpgradeCost = 3;
static int s_spongeIncrement = 1;

static int s_soapCost = 2;

static int s_soapDrainUpgradeCost = 6;
static int s_soapDrainIncrement = 2;

static int s_roombaUpgradeCost = 20;
static int s_roombaIncrement = 5;

static const int kFaucetPowerBaseCost = 15;
static const int kFaucetPowerCostIncrement = 5;
static const int kFaucetCooldownUpgradeCost = 20;

int Upgrades_GetSpongeCost(void) {
    return s_spongeUpgradeCost;
}

void Upgrades_AttemptSpongeUpgrade(void) {
    if (GetCurrentMoney() < s_spongeUpgradeCost) {
        return;
    }
    DecrementMoney(s_spongeUpgradeCost);
    upgrade_Sponge();

    s_spongeUpgradeCost += s_spongeIncrement;
    s_spongeIncrement++;
}

int Upgrades_GetSoapCost(void) {
    return s_soapCost;
}

void Upgrades_AttemptSoapPurchase(void) {
    if (Soap_IsFull() || GetCurrentMoney() < s_soapCost) {
        return;
    }
    DecrementMoney(s_soapCost);
    Soap_Refill();
}

int Upgrades_GetSoapDrainCost(void) {
    return s_soapDrainUpgradeCost;
}

void Upgrades_AttemptSoapDrainUpgrade(void) {
    if (!Soap_CanUpgradeDrain() || GetCurrentMoney() < s_soapDrainUpgradeCost) {
        return;
    }
    DecrementMoney(s_soapDrainUpgradeCost);
    Soap_UpgradeDrain();

    s_soapDrainUpgradeCost += s_soapDrainIncrement;
    s_soapDrainIncrement++;
}

int Upgrades_GetRoombaCost(void) {
    return s_roombaUpgradeCost;
}

void Upgrades_AttemptRoombaUpgrade(void) {
    if (GetCurrentMoney() < s_roombaUpgradeCost) {
        return;
    }
    DecrementMoney(s_roombaUpgradeCost);
    AddRoombaStrength(2);
    AddRoombaSpeed(20.0f);

    s_roombaUpgradeCost += s_roombaIncrement;
    s_roombaIncrement++;
}

int Upgrades_GetFaucetPowerCost(void) {
    return kFaucetPowerBaseCost + Faucet_GetPowerLevel() * kFaucetPowerCostIncrement;
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
    return kFaucetCooldownUpgradeCost;
}

void Upgrades_AttemptFaucetCooldownUpgrade(void) {
    if (!Faucet_CanUpgradeCooldown() || GetCurrentMoney() < kFaucetCooldownUpgradeCost) {
        return;
    }
    DecrementMoney(kFaucetCooldownUpgradeCost);
    Faucet_UpgradeCooldown();
}