/*************************************************************************
@file       shop.h
@Author		Ng Cher Kai Dan cherkaidan.ng@digipen.edu
@Co-authors Lim Liang Zhou l.liangzhou@digipen.edu
            Nur Nadia Natasya Binte Mohd Taha nurnadianatasya.b@digipen.edu
            Tan Choon Ming choonming.tan@digipen.edu
@brief      This file contains the function declarations for initialising the shop
			system, checking if the shop is open, and force closing shop menu.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#pragma once

void shop_init();
int IsShopOpen(void);
void ForceCloseShop(void);