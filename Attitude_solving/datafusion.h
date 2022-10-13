#ifndef __DATAFUSION_H
#define __DATAFUSION_H


#include "math.h"
#include "bsp_gy86.h"
#define RAD_TO_ANGLE 57.2957795f //»¡¶È×ª½Ç¶È
#define PI 3.1415927f

typedef struct
{
    float yaw;
    float roll;
    float pitch;
} Angle_t;
float invSqrt( float number );
void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void Quat_Init(void);
#endif
