/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include <stdio.h>
#include "soap.h"
#include "img_font_init.h"

static const float SoapMaxStamina = 100.0f;
static const float SoapDrainPerSecond = 2.0f;
static const float SoapDrainReductionPerUpgrade = 0.001f;   // 0.1%
static const int SoapDrainUpgradeMaxLevel = 10;             // cap at 10 levels
static const float SoapMinDrainMultiplier = 0.1f;
static float soapStamina = 0.0f;

static int soapDrainUpgradeLevel = 0;

static float Soap_GetDrainMultiplier(void) {
    float reduction = 1.0f - (float)soapDrainUpgradeLevel * SoapDrainReductionPerUpgrade;
    if (reduction < SoapMinDrainMultiplier) {
        reduction = SoapMinDrainMultiplier;
    }
    return reduction;
}

static float Soap_GetDrainRate(void) {
    return SoapDrainPerSecond * Soap_GetDrainMultiplier();
}

static float Soap_GetNormalized(void) {
    return soapStamina / SoapMaxStamina;
}

void Soap_Init(void) {
    soapStamina = SoapMaxStamina;
    soapDrainUpgradeLevel = 0;
}

void Soap_Update(void) {
    float barCenterX = 335.0f;
    float barCenterY = 730.0f;
    float barWidth = 250.0f;
    float barHeight = 28.0f;

    float normalized = Soap_GetNormalized();
    if (normalized < 0.0f) {
        normalized = 0.0f;
    }
    else if (normalized > 1.0f) {
        normalized = 1.0f;
    }

    CP_Settings_NoStroke();
    CP_Settings_Fill(dark_grey);
    CP_Graphics_DrawRect(barCenterX, barCenterY, barWidth, barHeight);

    float fillWidth = barWidth * normalized;
    float fillCenterX = barCenterX - (barWidth - fillWidth) * 0.5f;
    CP_Settings_Fill(yellow);
    CP_Graphics_DrawRect(fillCenterX, barCenterY, fillWidth, barHeight - 6.0f);

    CP_Settings_StrokeWeight(2.0f);
    CP_Settings_Stroke(black);
    CP_Settings_NoFill();
    CP_Graphics_DrawRect(barCenterX, barCenterY, barWidth, barHeight);

    /*
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

    CP_Settings_TextSize(26.0f);
    CP_Font_DrawText("Soap Stamina", barCenterX, barCenterY - 30.0f);
    */
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

    char staminaText[32];
    sprintf_s(staminaText, sizeof(staminaText), "%3.0f%%", normalized * 100.0f);
    CP_Settings_TextSize(22.0f);
    CP_Font_DrawText(staminaText, barCenterX, barCenterY);

    float reductionPercent = (float)soapDrainUpgradeLevel * SoapDrainReductionPerUpgrade * 100.0f;
    float multiplierPercent = Soap_GetDrainMultiplier() * 100.0f;

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Settings_TextSize(20.0f);
    if (!Soap_CanScrub()) {
        CP_Font_DrawText("Out of soap! Buy more in the shop.", barCenterX, barCenterY + 30.0f);
    }
    char upgradeText[64];
    CP_Settings_TextSize(20.0f);
    sprintf_s(upgradeText, sizeof(upgradeText), "Soap Saver Lv %d", soapDrainUpgradeLevel);
    CP_Font_DrawText(upgradeText, barCenterX, barCenterY + 30.0f);

    CP_Settings_TextSize(18.0f);
    sprintf_s(upgradeText, sizeof(upgradeText), "Drain Rate: %.2f/s (%.1f%% of base, -%.1f%%)",
        Soap_GetDrainRate(), multiplierPercent, reductionPercent);
    CP_Font_DrawText(upgradeText, barCenterX, barCenterY + 50.0f);
}


void Soap_ConsumeOnScrub(void) {
    if (soapStamina <= 0.0f) {
        soapStamina = 0.0f;
        return;
    }

    soapStamina -= Soap_GetDrainRate() * CP_System_GetDt();
    if (soapStamina < 0.0f) {
        soapStamina = 0.0f;
    }
}

int Soap_CanScrub(void) {
    return soapStamina > 0.0f;
}

int Soap_IsFull(void) {
    return soapStamina >= SoapMaxStamina;
}

void Soap_Refill(void) {
    soapStamina = SoapMaxStamina;
}

void Soap_UpgradeDrain(void) {
    if (Soap_CanUpgradeDrain()) {
        soapDrainUpgradeLevel += 1;
    }
}

int Soap_CanUpgradeDrain(void) {
    return soapDrainUpgradeLevel < SoapDrainUpgradeMaxLevel;
}

int Soap_GetDrainUpgradeLevel(void) {
    return soapDrainUpgradeLevel;
}