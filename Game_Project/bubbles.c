#include "cprocessing.h"
#include "sponge.h"
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
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    float radius;
    float lifeRemaining;
    float lifeMax;
    int   isAlive;
} Bubble;

static Bubble bubbleList[MAX_BUBBLES];
static float  spawnCooldownTimer = 0.0f;

static float RandomRange(float a, float b) {
    return CP_Random_RangeFloat(a, b);
}

// Spawn one bubble near the mouse
static void SpawnBubble(float mouseX, float mouseY) {
    for (int i = 0; i < MAX_BUBBLES; ++i) {
        if (!bubbleList[i].isAlive) {

            float angle = RandomRange(0.0f, 6.2831853f);  // 0 to 2*pi
            float spawnOffset = RandomRange(0.0f, 18.0f);
            float speed = RandomRange(SPEED_MIN, SPEED_MAX);
            float radius = RandomRange(BASE_RADIUS_MIN, BASE_RADIUS_MAX);
            float lifetime = RandomRange(LIFETIME_MIN, LIFETIME_MAX);

            bubbleList[i].positionX = mouseX + cosf(angle) * spawnOffset;
            bubbleList[i].positionY = mouseY + sinf(angle) * spawnOffset;
            bubbleList[i].velocityX = cosf(angle) * speed * 0.35f;
            bubbleList[i].velocityY = -fabsf(sinf(angle)) * speed * 0.65f;
            bubbleList[i].radius = radius;
            bubbleList[i].lifeRemaining = lifetime;
            bubbleList[i].lifeMax = lifetime;
            bubbleList[i].isAlive = 1;

            return;
        }
    }
}

// Initialize random number and bubbles
void Bubbles_Init(void) {
    CP_Random_Seed((int)CP_System_GetSeconds());
    for (int i = 0; i < MAX_BUBBLES; ++i) {
        bubbleList[i].isAlive = 0;
    }

    CP_Settings_AntiAlias(1);
    CP_Settings_NoStroke();
}

// Update and draw bubbles 
void Bubbles_UpdateAndDraw(void) {
    float mouseX = CP_Input_GetMouseX();
    float mouseY = CP_Input_GetMouseY();
    float deltaTime = CP_System_GetDt();
    spawnCooldownTimer -= deltaTime;

    // Spawn new bubbles if mouse is dragged and cooldown is done
    if (CP_Input_MouseDragged(MOUSE_BUTTON_LEFT) && spawnCooldownTimer <= 0.0f && is_SpongeEquipped()) {
        spawnCooldownTimer = SPAWN_COOLDOWN;
        for (int i = 0; i < BUBBLES_PER_SPAWN; ++i) {
            SpawnBubble(mouseX, mouseY);
        }
    }

    CP_Settings_BlendMode(CP_BLEND_ALPHA);

    for (int i = 0; i < MAX_BUBBLES; ++i) {
        Bubble* bubble = &bubbleList[i];
        if (!bubble->isAlive) {
            continue;
        }

        // apply upward force
        bubble->velocityY -= UPWARD_FORCE * deltaTime;

        // apply side-to-side wobble
        bubble->velocityX += RandomRange(-WOBBLE_STRENGTH, WOBBLE_STRENGTH) * deltaTime;

        // apply drag
        bubble->velocityX *= powf(DRAG, deltaTime);
        bubble->velocityY *= powf(DRAG, deltaTime);

        // move the bubble
        bubble->positionX += bubble->velocityX * deltaTime;
        bubble->positionY += bubble->velocityY * deltaTime;

        // update life
        bubble->lifeRemaining -= deltaTime;
        float lifeRatio = (bubble->lifeRemaining <= 0.0f)
            ? 0.0f
            : (bubble->lifeRemaining / bubble->lifeMax);

        // fade and shrink
        float alpha = CP_Math_ClampFloat(lifeRatio * 255.0f, 0.0f, 255.0f);
        float drawRadius = bubble->radius * (0.6f + 0.4f * lifeRatio);

        // draw bubble
        CP_Color bubbleColor = CP_Color_Create(135, 206, 235, (int)alpha);
        CP_Settings_Fill(bubbleColor);
        CP_Settings_Stroke(CP_Color_Create(145, 216, 245, 200));
        CP_Graphics_DrawEllipse(
            bubble->positionX,
            bubble->positionY,
            drawRadius * 2.0f,
            drawRadius * 2.0f
        );
        CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

        // remove bubble if done
        if (bubble->lifeRemaining <= 0.0f ||
            bubble->positionX < -50 ||
            bubble->positionX > CP_System_GetWindowWidth() + 50 ||
            bubble->positionY < -100) {
            bubble->isAlive = 0;
        }
    }
}