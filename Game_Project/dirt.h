void init_dirt(void);
void draw_dirt(void);
void dirt_scrubbed (int equipped, int sponge_power);
int dirt_removed(void);
void set_number_of_dirt(int num);
int get_number_of_dirt(void);
void reduce_all_dirt_opacity(int);
int get_total_opacity(void);
int get_sponge_radius(void);

typedef struct dirt {
    float positionX;
    float positionY;
    int opacity;
} dirt;

extern dirt dirtList[];
