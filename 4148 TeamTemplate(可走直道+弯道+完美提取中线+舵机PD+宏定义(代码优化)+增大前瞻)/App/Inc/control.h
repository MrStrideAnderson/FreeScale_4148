#ifndef CONTROL_H
#define CONTROL_H

//¶æ»ú
#define STEER_LEFT_MAX 880
#define STEER_RIGHT_MAX 826
#define STEER_MIDDLE 850

#define SD5_FTM   FTM1
#define SD5_CH    FTM_CH0//PA12
#define SD5_HZ    (100)

//µç»ú
#define MOTOR540_FTM FTM2
#define MOTOR540_CH FTM_CH0 //PA10
#define MOTOR540_HZ (1000)

void SteerInit();
void MotorInit();
void GetError();

#endif