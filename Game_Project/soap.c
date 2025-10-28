#include "cprocessing.h"
#include <stdio.h>
#include "soap.h"

static const float kSoapMaxStamina = 100.0f;
static const float kSoapDrainPerSecond = 2.0f;
static float soapStamina = 0.0f;

static float Soap_GetNormalized(void)
{
    return soapStamina / kSoapMaxStamina;
}

void Soap_Init(void)
{
    soapStamina = kSoapMaxStamina;
}

void Soap_Update(void)
{
    float barCenterX = 320.0f;
    float barCenterY = 750.0f;
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
    CP_Settings_Fill(CP_Color_Create(40, 40, 40, 180));
    CP_Graphics_DrawRect(barCenterX, barCenterY, barWidth, barHeight);

    float fillWidth = barWidth * normalized;
    float fillCenterX = barCenterX - (barWidth - fillWidth) * 0.5f;
    CP_Settings_Fill(CP_Color_Create(255, 214, 10, 255));
    CP_Graphics_DrawRect(fillCenterX, barCenterY, fillWidth, barHeight - 6.0f);

    CP_Settings_StrokeWeight(2.0f);
    CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
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

    if (!Soap_CanScrub()) {
        CP_Settings_TextSize(20.0f);
        CP_Font_DrawText("Out of soap! Buy more in the shop.", barCenterX, barCenterY + 42.0f);
    }
}

void Soap_ConsumeOnScrub(void)
{
    if (soapStamina <= 0.0f) {
        soapStamina = 0.0f;
        return;
    }

    soapStamina -= kSoapDrainPerSecond * CP_System_GetDt();
    if (soapStamina < 0.0f) {
        soapStamina = 0.0f;
    }
}

int Soap_CanScrub(void)
{
    return soapStamina > 0.0f;
}

int Soap_IsFull(void)
{
    return soapStamina >= kSoapMaxStamina;
}

void Soap_Refill(void)
{
    soapStamina = kSoapMaxStamina;
}