#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"


#define CORE_INT_SEP_MAX 300.0f //内环积分分离幅值
#define CORE_INT_MAX 100.0f //内环积分幅值
#define SHELL_INT_MAX 50.0f //外环积分幅值
#define PID_OUT_MAX 50.0f //PID输出幅值
#define MOTOR_OUT_MAX 100.0f //PWM输出幅值最大值
#define MOTOR_OUT_MIN 0.0f //PWM输出幅值最小值

void PID_Init(void);

//PID算法的数据结构 


typedef struct {
    float eK; //本次误差
    float eK_1; //上次误差
    float eSum; //积分项
		float eRate; //变化斜率
    float Kp; //比例系数
	  float Ki;
	  float Kd;
		float Time ; //进行一次PID所需要的时间
    float output; //pid输出
} PID_t;


extern PID_t pitchShell;
extern float pitchShellKp ;
extern float pitchShellKi;
extern float pitchCoreKp;



float PID_Count(PID_t* pid_t);
void pitch_Change(void);
void Power_Change(void);
void Shell_PID_Pitch(float time);
void ChangeToExpert(void);
float LimitSum(float eSum);
void PID_Init(void);
#endif

