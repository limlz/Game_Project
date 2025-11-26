/*************************************************************************
@file       sponge.h
@Author		Ng Cher Kai Dan cherkaidan.ng@digipen.edu
@Co-authors 
@brief      This file contains the function declaration for the Sponge Mechanics
			in the game, including equipping, scrubbing, and upgrading
			the sponge used to clean dirt off plates.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

void SpongeInit(void);

// Check if sponge is scrubbing
int IsScrubbing(void);

// Check if sponge is equipped (Return 1 if equipped, 0 if not)
int IsSpongeEquipped(void);

//Get the sponge current power (Returns an int based on the power)
int GetSpongePower(void);

// Upgrade the sponge power by 5, max power is 100
void UpgradeSponge(void);

// Check if sponge is upgradeable (Returns 1 if upgradeable, 0 if maxed)
int SpongeUpgradeable(void);

// Reset sponge state for a new game session
void SpongeReset(void);

void UnequipSponge(void);