void init_dirt(void);
void draw_dirt(int new_game);
void dirt_scrubbed (int equipped, int sponge_power);
int dirt_removed(void);
void set_number_of_dirt(int num);
int get_number_of_dirt(void);

typedef struct dirt {
    float positionX;
    float positionY;
    int opacity;
} dirt;

extern dirt dirtList[];
