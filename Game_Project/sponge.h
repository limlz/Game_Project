void sponge_init(void);

// Check if sponge is scrubbing
int is_Scrubbing(void);

// Check if sponge is equipped (Return 1 if equipped, 0 if not)
int is_SpongeEquipped(void);

//Get the sponge current power (Returns an int based on the power)
int get_SpongePower(void);

// Upgrade the sponge power by 5, max power is 100
void upgrade_Sponge(void);

// Check if sponge is upgradeable (Returns 1 if upgradeable, 0 if maxed)
int sponge_upgradeable(void);