/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

#ifndef SOAP_H
#define SOAP_H

void SoapInit(void);
void SoapUpdate(void);
void SoapConsumeOnScrub(void);
int SoapCanScrub(void);
int SoapIsFull(void);
void SoapRefill(void);
void SoapUpgradeDrain(void);
int SoapCanUpgradeDrain(void);
int SoapGetDrainUpgradeLevel(void);

#endif
