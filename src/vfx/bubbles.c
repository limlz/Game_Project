/*************************************************************************
@file       bubbles.c
@Author     Lim Liang Zhou l.liangzhou@digipen.edu
@Co-authors nil
@brief      This file contains the function definitions for spawning, updating,
			and rendering bubble visual effects when the sponge is used and when
			bubbles are manually spawned.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "sponge.h"
#include "debug.h"
#include <math.h>


#define MAX_BUBBLES         400
#define BUBBLES_PER_SPAWN   6
#define BASE_RADIUS_MIN     4.0f
#define BASE_RADIUS_MAX     10.0f
#define SPEED_MIN           20.0f
#define SPEED_MAX           80.0f
#define DRAG                0.98f
#define UPWARD_FORCE        60.0f
#define LIFETIME_MIN        0.8f
#define LIFETIME_MAX        1.8f
#define SPAWN_COOLDOWN      0.016f  // about 60 FPS
#define WOBBLE_STRENGTH     20.0f


typedef struct Bubble {
    float position_x;
    float position_y;
    float velocity_x;
    float velocity_y;
    float radius;
    float life_remaining;
    float life_max;
    int   is_alive;
} Bubble;

static Bubble bubble_list[MAX_BUBBLES];
static float  spawn_cooldown_timer = 0.0f;



// Spawn one bubble near the mouse
void SpawnBubble(float mouseX, float mouseY) {
    for (int i = 0; i < MAX_BUBBLES; ++i) {
        if (!bubble_list[i].is_alive) {

            float angle = CP_Random_RangeFloat(0.0f, 6.2831853f);  // 0 to 2*pi
            float spawn_offset = CP_Random_RangeFloat(0.0f, 18.0f);
            float speed = CP_Random_RangeFloat(SPEED_MIN, SPEED_MAX);
            float radius = CP_Random_RangeFloat(BASE_RADIUS_MIN, BASE_RADIUS_MAX);
            float lifetime = CP_Random_RangeFloat(LIFETIME_MIN, LIFETIME_MAX);

            bubble_list[i].position_x = mouseX + cosf(angle) * spawn_offset;
            bubble_list[i].position_y = mouseY + sinf(angle) * spawn_offset;
            bubble_list[i].velocity_x = cosf(angle) * speed * 0.35f;
            bubble_list[i].velocity_y = -fabsf(sinf(angle)) * speed * 0.65f;
            bubble_list[i].radius = radius;
            bubble_list[i].life_remaining = lifetime;
            bubble_list[i].life_max = lifetime;
            bubble_list[i].is_alive = 1;

            return;
        }
    }
}

// Initialize random number and bubbles
void BubblesInit(void) {
    CP_Random_Seed((int)CP_System_GetSeconds());
    for (int i = 0; i < MAX_BUBBLES; ++i) {
        bubble_list[i].is_alive = 0;
    }

    CP_Settings_AntiAlias(1);
    CP_Settings_NoStroke();
}

void BubblesUpdate(void) {
    float mouse_x = CP_Input_GetMouseX();
    float mouse_y = CP_Input_GetMouseY();
    float delta_time = CP_System_GetDt();
    spawn_cooldown_timer -= delta_time;

    // Spawn new bubbles if mouse is dragged and cooldown is done
    if (CP_Input_MouseDragged(MOUSE_BUTTON_LEFT) && spawn_cooldown_timer <= 0.0f && IsSpongeEquipped()) {
        spawn_cooldown_timer = SPAWN_COOLDOWN;
        for (int i = 0; i < BUBBLES_PER_SPAWN; ++i) {
            SpawnBubble(mouse_x, mouse_y);
        }
    }
}

void BubblesManual(float manualX, float manualY) {
    float delta_time = CP_System_GetDt();
    spawn_cooldown_timer -= delta_time;

    // Spawn new bubbles if mouse is dragged and cooldown is done
    if (spawn_cooldown_timer <= 0.0f) {
        spawn_cooldown_timer = 0.07f;
        for (int i = 0; i < 2; ++i) {
            SpawnBubble(manualX, manualY);
        }
    }
}

// Update and draw bubbles 
void Bubbles_Draw(void) {

    float delta_time = CP_System_GetDt();
    BubblesUpdate();

    CP_Settings_BlendMode(CP_BLEND_ALPHA);

    for (int i = 0; i < MAX_BUBBLES; ++i) {
        Bubble* bubble = &bubble_list[i];
        if (!bubble->is_alive) {
            continue;
        }

        // apply upward force
        bubble->velocity_y -= UPWARD_FORCE * delta_time;

        // apply side-to-side wobble
        bubble->velocity_x += CP_Random_RangeFloat(-WOBBLE_STRENGTH, WOBBLE_STRENGTH) * delta_time;

        // apply drag
        bubble->velocity_x *= powf(DRAG, delta_time);
        bubble->velocity_y *= powf(DRAG, delta_time);

        // move the bubble
        bubble->position_x += bubble->velocity_x * delta_time;
        bubble->position_y += bubble->velocity_y * delta_time;

        // update life
        bubble->life_remaining -= delta_time;
        float life_ratio = (bubble->life_remaining <= 0.0f)
            ? 0.0f
            : (bubble->life_remaining / bubble->life_max);

        // fade and shrink
        float alpha = CP_Math_ClampFloat(life_ratio * 255.0f, 0.0f, 255.0f);
        float draw_radius = bubble->radius * (0.6f + 0.4f * life_ratio);

        // draw bubble

        CP_Color bubbleColor = CP_Color_Create(135, 206, 235, (int)alpha);
        if (!IsCurrentlyDebugging()) {
            CP_Settings_Fill(bubbleColor);
            CP_Settings_Stroke(CP_Color_Create(145, 216, 245, 200));
            CP_Graphics_DrawEllipse(
                bubble->position_x,
                bubble->position_y,
                draw_radius * 2.0f,
                draw_radius * 2.0f
            );
        }
        CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

        // remove bubble if done
        if (bubble->life_remaining <= 0.0f ||
            bubble->position_x < -50 ||
            bubble->position_x > CP_System_GetWindowWidth() + 50 ||
            bubble->position_y < -100) {
            bubble->is_alive = 0;
        }
    }
}