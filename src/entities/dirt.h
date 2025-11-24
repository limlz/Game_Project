/*************************************************************************
@file       dirt.h
@Author     Lim Liang Zhou l.liangzhou@digipen.edu
@Co-authors nil
@brief      THis file contains the header declarations for managing dirt
            entities, including their initialization, rendering, opacity
			reduction when scrubbed, and checking for removal conditions.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
void InitDirt(void);
void DrawDirt(void);
void DirtScrubbed (int equipped, int sponge_power);
int DirtRemoved(void);
void SetNumberOfDirt(int num);
int GetNumberOfDirt(void);
void ReduceAllDirtOpacity(int);
int GetTotalOpacity(void);
int GetSpongeRadius(void);
void DirtPointer(void);

typedef struct dirt {
    float position_x;
    float position_y;
    int opacity;
    int size;
} dirt;

extern dirt dirt_list[];
