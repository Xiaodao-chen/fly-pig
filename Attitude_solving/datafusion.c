#include "datafusion.h"
#include "bsp_gy86.h"
#include "ucos_ii.h"

#define deltat 0.001f // sampling period in seconds (shown as 1 ms)
#define gyroMeasError 3.14159265358979 * (25.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define gyroMeasDrift 3.14159265358979 * (0.01f / 180.0f) // gyroscope measurement error in rad/s/s (shown as 0.2f deg/s/s)
#define beta 0.3f // sqrt(3.0f / 4.0f) * gyroMeasError // compute beta
#define zeta -sqrt(3.0f / 4.0f) * gyroMeasDrift // compute zeta


float a_x, a_y, a_z; // accelerometer measurements
float w_x, w_y, w_z; // gyroscope measurements in rad/s
float m_x, m_y, m_z; // magnetometer measurements
float SEq_1 = 1, SEq_2 = 0, SEq_3 = 0, SEq_4 = 0; // estimated orientation quaternion elements with initial conditions
float b_x = 1, b_z = 0; // reference direction of flux in earth frame
float w_bx = 0, w_by = 0, w_bz = 0; // estimate gyroscope biases error


float invSqrt(float number) //开跟分之一
{
    long i;
    float x2, y;
    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (x2 * y * y));
    return y;
}

float Kp = 0.2f;                      
float Ki = 0.01f;                                          
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;  
float exInt = 0, eyInt = 0, ezInt = 0; 
Angle_t angle;                         



void Gyroscope_Update(float Time){
	  float q0_last, q1_last, q2_last, q3_last;
    float norm;
		q0_last = q0;
    q1_last = q1;
    q2_last = q2;
    q3_last = q3;
		q0 = q0_last + (-q1_last * mpu.GYR_X - q2_last * mpu.GYR_Y - q3_last * mpu.GYR_Z) * Time;
    q1 = q1_last + (q0_last * mpu.GYR_X + q2_last * mpu.GYR_Z - q3_last * mpu.GYR_Y) * Time;
    q2 = q2_last + (q0_last * mpu.GYR_Y - q1_last * mpu.GYR_Z + q3_last * mpu.GYR_X) * Time;
    q3 = q3_last + (q0_last * mpu.GYR_Z + q1_last * mpu.GYR_Y - q2_last * mpu.GYR_X) * Time;
		
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);//归一化
    q0 = q0 * norm; // w
    q1 = q1 * norm; // x
    q2 = q2 * norm; // y
    q3 = q3 * norm; // z
		
    angle.roll = asin(2.0f * (q0 * q2 - q1 * q3)) * RAD_TO_ANGLE;
    angle.pitch =- atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * RAD_TO_ANGLE;
    angle.yaw = atan2(2.0f * (q0 * q3 + q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * RAD_TO_ANGLE;
}

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz,float Time)
{
    float recipNorm;
    float s0, s1, s2, s3;
    double qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth锟斤拷s magnetic field
        hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy)*2;
        _2bz =2* (-_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3);
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;

    // Integrate rate of change of quaternion to yield quaternion
		q0 += qDot1  *2* Time;
    q1 += qDot2  *2* Time;
    q2 += qDot3  *2* Time;
    q3 += qDot4  *2* Time;

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
    angle.roll = -asin(2.0f * (q0 * q2 - q1 * q3)) * RAD_TO_ANGLE;
    angle.pitch= -atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * RAD_TO_ANGLE;
    angle.yaw  = -atan2(2.0f * (q0 * q3 + q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * RAD_TO_ANGLE;
}



void Magenic_Update(float Time)
{
    float recipNorm;
    float s0, s1, s2, s3;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
		float q0_last, q1_last, q2_last, q3_last;
			float ax,az,ay,mx,my,mz;

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)

        // Normalise accelerometer measurement
        recipNorm = invSqrt(mpu.ACC_X * mpu.ACC_X + mpu.ACC_Y * mpu.ACC_Y + mpu.ACC_Z * mpu.ACC_Z);
        ax =mpu.ACC_X *recipNorm;
        ay = recipNorm*mpu.ACC_Y;
        az = recipNorm*mpu.ACC_Z;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(hmc.X * hmc.X + hmc.Y * hmc.Y + hmc.Z * hmc.Z);
        mx = recipNorm*hmc.X;
        my = recipNorm*hmc.Y;
        mz = recipNorm*hmc.Z;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth锟斤拷s magnetic field
        hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy)*2;
        _2bz =2* (-_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3);
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        
				s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        
				s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        
				s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)+ (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        
				s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
    
			
		q0_last=q0;
		q1_last=q1;
		q2_last=q2;
		q3_last=q3;
		
    // Integrate rate of change of quaternion to yield quaternion
		q0=q0_last -Time*s0*2;
		q1=q1_last -Time*s1*2;
		q2=q2_last -Time*s2*2;
		q3=q3_last -Time*s3*2;
		
    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
    angle.roll = asin(2.0f * (q0 * q2 - q1 * q3)) * RAD_TO_ANGLE;
    angle.pitch =-atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * RAD_TO_ANGLE;
    angle.yaw = atan2(2.0f * (q0 * q3 + q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * RAD_TO_ANGLE;
}



void Quat_Init(void) //四元数初始化
{
    int16_t initMx, initMy, initMz;
    float initYaw, initPitch=0, initRoll=0;

	
    HMC_Get();//获取磁力数据
    initMx = hmc.X;
    initMy = hmc.Y;
    initMz = hmc.Z;
	
    // Complement processing and unit conversion
    if (initMx > 0x7fff)
        initMx -= 0xffff;
    if (initMy > 0x7fff)
        initMy -= 0xffff;
    if (initMz > 0x7fff)
        initMz -= 0xffff;
		initYaw=atan2(initMy,initMx);
//	if(hmc.X>=0 && hmc.Y>=0){
//			initYaw = atan2(initMy,initMx);//磁力计是分布到 z和x
//		}else  if(hmc.X>=0 && hmc.Y<=0){
//			initYaw =-atan2(-initMy,initMx);
//		}else if(hmc.X>0 && hmc.Y<=0){
//			initYaw =PI-atan2(hmc.Y,-hmc.X);
//		}else{
//			initYaw = -PI +atan2(-hmc.Y,-hmc.X);
//		}
		initYaw=-initYaw;

    q0 = cos(0.5f * initRoll) * cos(0.5f * initPitch) * cos(0.5f * initYaw) + sin(0.5f * initRoll) * sin(0.5f * initPitch) * sin(0.5f * initYaw); // w
    q1 = cos(0.5f * initRoll) * sin(0.5f * initPitch) * cos(0.5f * initYaw) - sin(0.5f * initRoll) * cos(0.5f * initPitch) * sin(0.5f * initYaw); // x Pitch
    q2 = sin(0.5f * initRoll) * cos(0.5f * initPitch) * cos(0.5f * initYaw) + cos(0.5f * initRoll) * sin(0.5f * initPitch) * sin(0.5f * initYaw); // y Roll
    q3 = cos(0.5f * initRoll) * cos(0.5f * initPitch) * sin(0.5f * initYaw) - sin(0.5f * initRoll) * sin(0.5f * initPitch) * cos(0.5f * initYaw); // z Yaw
}


/*

void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz,float time)
{
    float norm;
    float hx, hy, hz, bz, by;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;
    float q0_last, q1_last, q2_last, q3_last;
		//time=time/2000;

    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    norm = invSqrt(ax * ax + ay * ay + az * az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;

    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    norm = invSqrt(mx * mx + my * my + mz * mz);
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;

    hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
    hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
    hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);

    by = sqrtf((hx * hx) + (hy * hy));
    bz = hz;
    
    wx = 2 * by * (q1q2 + q0q3) + 2 * bz * (q1q3 - q0q2);
    wy = 2 * by * (0.5f - q1q1 - q3q3) + 2 * bz * (q0q1 + q2q3);
    wz = 2 * by * (q2q3 - q0q1) + 2 * bz * (0.5f - q1q1 - q2q2);
		
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

    if (ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
     
        exInt += ex * Ki * time;
        eyInt += ey * Ki * time;
        ezInt += ez * Ki * time;

        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    }

    q0_last = q0;
    q1_last = q1;
    q2_last = q2;
    q3_last = q3;

    q0 = q0_last + (-q1_last * gx - q2_last * gy - q3_last * gz) * time;
    q1 = q1_last + (q0_last * gx + q2_last * gz - q3_last * gy) * time;
    q2 = q2_last + (q0_last * gy - q1_last * gz + q3_last * gx) * time;
    q3 = q3_last + (q0_last * gz + q1_last * gy - q2_last * gx) * time;

    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm; // w
    q1 = q1 * norm; // x
    q2 = q2 * norm; // y
    q3 = q3 * norm; // z

    angle.roll = asin(2.0f * (q0 * q2 - q1 * q3)) * RAD_TO_ANGLE;
    angle.pitch = atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * RAD_TO_ANGLE;
    angle.yaw = atan2(2.0f * (q0 * q3 + q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * RAD_TO_ANGLE;
}
*/



