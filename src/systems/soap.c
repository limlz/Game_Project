/*************************************************************************
@file soap.c
@Author Tan Choon Ming choonming.tan@digipen.edu
@Co-authors
@brief this file contains the function definitions for the soap system,
	   including soap stamina management, draining over time, and upgrades.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include <stdio.h>
#include "soap.h"
#include "img_font_init.h"

static const float soap_max_stamina = 100.0f;
static const float soap_drain_per_second = 2.0f;
static const float soap_drain_reduction_per_upgrade = 0.001f;   // 0.1%
static const int soap_drain_upgrade_max_level = 10;             // cap at 10 levels
static const float soap_min_drain_multiplier = 0.1f;
static float soap_stamina = 0.0f;

static int soap_drain_upgrade_level = 0;

static float SoapGetDrainMultiplier(void) {
    float reduction = 1.0f - (float)soap_drain_upgrade_level * soap_drain_reduction_per_upgrade;
    if (reduction < soap_min_drain_multiplier) {
        reduction = soap_min_drain_multiplier;
    }
    return reduction;
}

static float SoapGetDrainRate(void) {
    return soap_drain_per_second * SoapGetDrainMultiplier();
}

static float SoapGetNormalized(void) {
    return soap_stamina / soap_max_stamina;
}

void SoapInit(void) {
    soap_stamina = soap_max_stamina;
    soap_drain_upgrade_level = 0;
}

void SoapUpdate(void) {
    float bar_center_x = 335.0f;
    float bar_center_y = 730.0f;
    float bar_width = 250.0f;
    float bar_height = 28.0f;

    float normalized = SoapGetNormalized();
    if (normalized < 0.0f) {
        normalized = 0.0f;
    }
    else if (normalized > 1.0f) {
        normalized = 1.0f;
    }

    CP_Settings_NoStroke();
    CP_Settings_Fill(dark_grey);
    CP_Graphics_DrawRect(bar_center_x, bar_center_y, bar_width, bar_height);

    float fill_width = bar_width * normalized;
    float fillCenterX = bar_center_x - (bar_width - fill_width) * 0.5f;
    CP_Settings_Fill(yellow);
    CP_Graphics_DrawRect(fillCenterX, bar_center_y, fill_width, bar_height - 6.0f);

    CP_Settings_StrokeWeight(2.0f);
    CP_Settings_Stroke(black);
    CP_Settings_NoFill();
    CP_Graphics_DrawRect(bar_center_x, bar_center_y, bar_width, bar_height);

    /*
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

    CP_Settings_TextSize(26.0f);
    CP_Font_DrawText("Soap Stamina", barCenterX, barCenterY - 30.0f);
    */
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

    char staminaText[32];
    sprintf_s(staminaText, sizeof(staminaText), "%3.0f%%", normalized * 100.0f);
    CP_Settings_TextSize(22.0f);
    CP_Font_DrawText(staminaText, bar_center_x, bar_center_y);

    float reductionPercent = (float)soap_drain_upgrade_level* soap_drain_reduction_per_upgrade * 100.0f;
    float multiplierPercent = SoapGetDrainMultiplier() * 100.0f;

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Settings_TextSize(20.0f);
    if (!SoapCanScrub()) {
        CP_Font_DrawText("Out of soap! Buy more in the shop.", bar_center_x, bar_center_y + 30.0f);
    }
    char upgradeText[64];
    CP_Settings_TextSize(20.0f);
    sprintf_s(upgradeText, sizeof(upgradeText), "Soap Saver Lv %d", soap_drain_upgrade_level);
    CP_Font_DrawText(upgradeText, bar_center_x, bar_center_y + 30.0f);

    CP_Settings_TextSize(18.0f);
    sprintf_s(upgradeText, sizeof(upgradeText), "Drain Rate: %.2f/s (%.1f%% of base, -%.1f%%)",
        SoapGetDrainRate(), multiplierPercent, reductionPercent);
    CP_Font_DrawText(upgradeText, bar_center_x, bar_center_y + 50.0f);
}


void SoapConsumeOnScrub(void) {
    if (soap_stamina <= 0.0f) {
        soap_stamina = 0.0f;
        return;
    }

    soap_stamina -= SoapGetDrainRate() * CP_System_GetDt();
    if (soap_stamina < 0.0f) {
        soap_stamina = 0.0f;
    }
}

int SoapCanScrub(void) {
    return soap_stamina > 0.0f;
}

int SoapIsFull(void) {
    return soap_stamina >= soap_max_stamina;
}

void SoapRefill(void) {
    soap_stamina = soap_max_stamina;
}

void SoapUpgradeDrain(void) {
    if (SoapCanUpgradeDrain()) {
        soap_drain_upgrade_level += 1;
    }
}

int SoapCanUpgradeDrain(void) {
    return soap_drain_upgrade_level < soap_drain_upgrade_max_level;
}

int SoapGetDrainUpgradeLevel(void) {
    return soap_drain_upgrade_level;
}