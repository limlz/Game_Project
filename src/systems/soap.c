/*************************************************************************
@file soap.c
@Author Tan Choon Ming choonming.tan@digipen.edu
@Co-authors NIL
@brief this file contains the function definitions for the soap system,
       including soap stamina management, draining over time, and upgrades.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include <stdio.h>
#include "soap.h"
#include "img_font_init.h"

/* --------------------------------------------------------------------------
   SOAP SYSTEM CONSTANTS
   These values control the soap stamina system, including the maximum amount,
   drain behavior per second, upgrade effects, and minimum allowed drain rate.
   -------------------------------------------------------------------------- */
static const float soap_max_stamina = 100.0f;               // Total "soap stamina" the player can have
static const float soap_drain_per_second = 2.0f;            // Base drain rate while scrubbing
static const float soap_drain_reduction_per_upgrade = 0.001f; // Each upgrade reduces drain multiplier by 0.1%
static const float soap_min_drain_multiplier = 0.1f;        // Drain multiplier never becomes lower than 10%

/* Current runtime value for soap stamina (upgrade level is now in upgrades.c). */
static float soap_stamina = 0.0f;

/* Returns the drain multiplier after upgrades, using the current upgrade level
   queried from the upgrades system. */
static float SoapGetDrainMultiplier(void) {
    float reduction = 1.0f - (float)SoapGetDrainUpgradeLevel() * soap_drain_reduction_per_upgrade;
    if (reduction < soap_min_drain_multiplier) {
        reduction = soap_min_drain_multiplier;   // Clamp to minimum allowed drain speed
    }
    return reduction;
}

/* Computes the final drain rate per second (base drain × multiplier). */
static float SoapGetDrainRate(void) {
    return soap_drain_per_second * SoapGetDrainMultiplier();
}

/* Returns normalized stamina in [0.0f, 1.0f] for UI drawing. */
static float SoapGetNormalized(void) {
    return soap_stamina / soap_max_stamina;
}

/* --------------------------------------------------------------------------
   PUBLIC SOAP SYSTEM FUNCTIONS
   These control stamina behaviour, upgrades, and UI drawing.
   -------------------------------------------------------------------------- */

   /* Initializes soap stamina at the start of a level/game, restoring full soap
      and resetting all soap drain upgrades. */
void SoapInit(void) {
    soap_stamina = soap_max_stamina;
    SoapResetDrainUpgradeLevel();
}

/* Updates the soap stamina UI bar each frame.
   Also displays the current drain rate and upgrade level. */
void SoapUpdate(void) {
    float bar_center_x = 335.0f;
    float bar_center_y = 730.0f;
    float bar_width = 250.0f;
    float bar_height = 28.0f;

    /* Get clamped normalized value for UI */
    float normalized = SoapGetNormalized();
    if (normalized < 0.0f) {
        normalized = 0.0f;
    }
    else if (normalized > 1.0f) {
        normalized = 1.0f;
    }

    /* Draw soap bar background */
    CP_Settings_NoStroke();
    CP_Settings_Fill(dark_grey);
    CP_Graphics_DrawRect(bar_center_x, bar_center_y, bar_width, bar_height);

    /* Draw soap fill */
    float fill_width = bar_width * normalized;
    float fillCenterX = bar_center_x - (bar_width - fill_width) * 0.5f;
    CP_Settings_Fill(yellow);
    CP_Graphics_DrawRect(fillCenterX, bar_center_y, fill_width, bar_height - 6.0f);

    /* Draw border */
    CP_Settings_StrokeWeight(2.0f);
    CP_Settings_Stroke(black);
    CP_Settings_NoFill();
    CP_Graphics_DrawRect(bar_center_x, bar_center_y, bar_width, bar_height);

    /* Draw percent text */
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    char staminaText[32];
    sprintf_s(staminaText, sizeof(staminaText), "%3.0f%%", normalized * 100.0f);
    CP_Settings_TextSize(22.0f);
    CP_Font_DrawText(staminaText, bar_center_x, bar_center_y);

    /* Compute and display soap saver upgrade info */
    int   level = SoapGetDrainUpgradeLevel();
    float reductionPercent = (float)level * soap_drain_reduction_per_upgrade * 100.0f;
    float multiplierPercent = SoapGetDrainMultiplier() * 100.0f;

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Settings_TextSize(20.0f);

    if (!SoapCanScrub()) {
        CP_Font_DrawText("Out of soap! Buy more in the shop.", bar_center_x, bar_center_y + 30.0f);
    }

    char upgradeText[64];
    CP_Settings_TextSize(20.0f);
    sprintf_s(upgradeText, sizeof(upgradeText), "Soap Saver Lv %d", level);
    CP_Font_DrawText(upgradeText, bar_center_x, bar_center_y + 30.0f);

    CP_Settings_TextSize(18.0f);
    sprintf_s(upgradeText, sizeof(upgradeText),
        "Drain Rate: %.2f/s (%.1f%% of base, -%.1f%%)",
        SoapGetDrainRate(), multiplierPercent, reductionPercent);
    CP_Font_DrawText(upgradeText, bar_center_x, bar_center_y + 50.0f);
}

/* Reduces soap stamina over time when the player is actively scrubbing. */
void SoapConsumeOnScrub(void) {
    if (soap_stamina <= 0.0f) {
        soap_stamina = 0.0f;   // Already empty, prevent negatives
        return;
    }

    /* Drain soap using delta time and upgrade-adjusted drain rate */
    soap_stamina -= SoapGetDrainRate() * CP_System_GetDt();

    if (soap_stamina < 0.0f) {
        soap_stamina = 0.0f;
    }
}

/* Returns non-zero if there is still soap left for scrubbing. */
int SoapCanScrub(void) {
    return soap_stamina > 0.0f;
}

/* Returns non-zero if the soap stamina is currently full. */
int SoapIsFull(void) {
    return soap_stamina >= soap_max_stamina;
}

/* Fully refills the soap stamina. */
void SoapRefill(void) {
    soap_stamina = soap_max_stamina;
}
