#pragma once

void draw_faucet(void);
void draw_stream(void);
void stream_init(void);
void update_stream(void);
void AOE_stream(float);
void stop_stream(void);
void clean_dirt_with_stream(void);
void draw_stream_timer(void);
void reduce_cooldown(float reduction);
void reset_cooldown(void);
float return_cooldown(void);
float return_aoe_time_left(void);
int return_is_attack_ready(void);
int return_stream_on(void);
void Faucet_UpgradePower(void);
int Faucet_CanUpgradePower(void);
int Faucet_GetPowerLevel(void);
float Faucet_GetPowerMultiplier(void);
void Faucet_UpgradeCooldown(void);
int Faucet_CanUpgradeCooldown(void);
int Faucet_GetCooldownLevel(void);
float Faucet_GetCooldownBase(void);