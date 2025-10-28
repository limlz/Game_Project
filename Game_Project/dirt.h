void InitDirt(void);
void DrawDirt(void);
void DirtScrubbed (int equipped, int sponge_power);
int DirtRemoved(void);
void SetNumberOfDirt(int num);
int GetNumberOfDirt(void);
void ReduceAllDirtOpacity(int);
int GetTotalOpacity(void);
int GetSpongeRadius(void);

typedef struct dirt {
    float position_x;
    float position_y;
    int opacity;
} dirt;

extern dirt dirt_list[];
