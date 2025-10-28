#pragma once
#ifndef SOAP_H
#define SOAP_H

void Soap_Init(void);
void Soap_Update(void);
void Soap_ConsumeOnScrub(void);
int Soap_CanScrub(void);
int Soap_IsFull(void);
void Soap_Refill(void);
void Soap_UpgradeDrain(void);
int Soap_CanUpgradeDrain(void);
int Soap_GetDrainUpgradeLevel(void);

#endif
