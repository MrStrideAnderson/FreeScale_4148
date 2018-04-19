#ifndef ROAD_H
#define ROAR_H

#define ROW 60
#define COL 80
#define MID 40
//注意了，解压后的是0xFF和0x00两个值！！！
#define WhitePoint 0xFF
#define BlackPoint 0x00

extern uint8 Middle[60];
void GetCenter();
void RoadInit();

#endif