/*************************************************************************
@file soap.h
@Author Tan Choon Ming (choonming.tan@digipen.edu)
@Co-authors NIL
@brief Public interface for the soap stamina system. This module controls
       how soap is drained, upgraded, and displayed in the UI.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

#ifndef SOAP_H
#define SOAP_H

/* Initializes soap stamina and upgrade levels to default values. */
void SoapInit(void);

/* Updates the on-screen soap bar and related status text each frame. */
void SoapUpdate(void);

/* Drains soap stamina while the player is actively scrubbing. */
void SoapConsumeOnScrub(void);

/* Returns non-zero if there is enough soap to continue scrubbing. */
int  SoapCanScrub(void);

/* Returns non-zero if soap stamina is at maximum. */
int  SoapIsFull(void);

/* Instantly refills soap stamina to maximum. */
void SoapRefill(void);

/* Increases the soap saver upgrade level (reduces drain rate). */
void SoapUpgradeDrain(void);

/* Returns non-zero if the soap saver upgrade can still be increased. */
int  SoapCanUpgradeDrain(void);

/* Returns the current number of soap saver upgrades the player has. */
int  SoapGetDrainUpgradeLevel(void);

/* Resets the soap saver upgrade level back to 0 (used when restarting runs). */
void SoapResetDrainUpgradeLevel(void);

#endif

