#ifndef _ANO_H
#define _ANO_H




#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )




void ANO_Angle_Transform(short roll, short pitch, short yaw);
void ANO_PWN_Transform(void);
void ANO_IMU_Transform(short AX,short AY,short AZ,short GX,short GY,short GZ);
void ANO_HMC_Transform(short MX,short MY,short MZ);
void ANO_PID_Transform(short P,short I,short D);
void ANO_Quant_Transform(short q0,short q1, short q2,short q3);
#endif

