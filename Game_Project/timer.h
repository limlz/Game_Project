void TimerInit(void);
void TimeReset(void);
void TimeAdd(float add_time);

// Return`s 1 if the game is running, 0 if paused
int CheckGameRunning(void);
void TimeStop(void);
int IsTimerStopped(void);
