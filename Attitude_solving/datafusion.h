#ifndef __DATAFUSION_H
#define __DATAFUSION_H


#include "math.h"
#include "bsp_gy86.h"
#define RAD_TO_ANGLE 57.2957795f //»¡¶È×ª½Ç¶È

#define PI 3.1415927f
//#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
//#define beta 0.013f//invSqrt(3.0f / 4.0f) * gyroMeasError // compute beta


typedef struct
{
    float yaw;
    float roll;
    float pitch;
} Angle_t;

extern Angle_t angle;
extern float q0 , q1 , q2 , q3 ; 

float invSqrt( float number );



void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz,float Time);
void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz,float time);
void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z, float m_x, float m_y, float m_z,float halfT);
void Accelerometer_Update(float Time);
void Quat_Init(void);
void Gyroscope_Update(float Time);
void Accelerometer_Update(float Time);
void Magenic_Update(float Time);
void yaw_show(void);
#endif
