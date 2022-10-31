#include "PID.h"
#include "math.h"
#include "bsp_gy86.h"
#include <stdlib.h>
#include "bsp_tim.h"
#include  "datafusion.h"
#include "bsp_ppm.h"

//自动初始化为 0
PID_t rollCore, rollShell, pitchCore, pitchShell, yawCore, thrShell; //六个环的pid结构体
float expRoll, expPitch, expYaw, expHeight;                 //期望值
float motor1, motor2, motor3, motor4;                                //四个电机速度


extern Angle_t angle;                    										//姿态解算-角度值


float rollShellKp = 8.0f;  //外环Kp 8.0
float rollShellKi = 0.0f; //外环Ki
float rollShellKd = 0.0f;
float rollCoreKp = 0.0f;  //内环Kp 0.13
float rollCoreKi = 0.0f;  //环Ti 500
float rollCoreKd = 0.0f; //内环Td 0.005

float pitchShellKp = 0.0f;
float pitchShellKi = 0.00f;
float pitchCoreKp = 0.0f;
float pitchCoreKi = 0.00f;
float pitchCoreKd = 0.0f;

float yawCoreKp = 0.0f;   // 0.5f
float yawCoreKd = 0.0f; // 0.005f
float yawCoreKi = 0.000f;


void PID_Init(void){
    // roll
    rollShell.Kp = rollShellKp;
    rollShell.Ki = rollShellKi;
    rollCore.Kp = rollCoreKp;
    rollCore.Ki = rollCoreKi;
    rollCore.Kd = rollCoreKd;

    // pitch
    pitchShell.Kp = pitchShellKp;
    pitchShell.Ki = pitchShellKi;
    pitchCore.Kp = pitchCoreKp;
    pitchCore.Ki = pitchCoreKi;
    pitchCore.Kd = pitchCoreKd;

    // yaw

    yawCore.Kp = yawCoreKp;
    yawCore.Ki = yawCoreKi;
    yawCore.Kd = yawCoreKd;
}


// 输出值 
void ChangeToExpert(void){ // 上25°
	expRoll=(TIM1_DataBuf[0]-300)*0.25f;
	expPitch=-(TIM1_DataBuf[1]-300)*0.25f;
}


/*
PID = kp*error + ki*(之前积分+现在误差*time)+kd (err-errl)/time
*/
float PID_Count(PID_t* pid_t){
	float result=0.0f;
	result=(pid_t->eK)*pid_t->Kp+pid_t->Ki*pid_t->eSum+pid_t->Kd*pid_t->eRate;
 	return result;
}



/*
就很矛盾，我希望一个函数实现所有的东西，但是需要传入的就是，先不管细节，就直接调用。
Shell_PID_Pitch 函数 参数是 time 

作用 执行PID 运算，将PID运算保存到对应的PID结构体中
	
*/
void Shell_PID_Pitch(float time){
	
	pitchShell.Time=time;
	pitchShell.eK=expPitch-angle.pitch;
	float eSum;
	eSum=pitchShell.eSum+pitchShell.eK*time;
	pitchShell.eSum=LimitSum(eSum);
	pitchShell.eRate=(pitchShell.eK-pitchShell.eK_1)/pitchShell.Time;	
	pitchShell.output=PID_Count(&pitchShell);
	//将ek_1赋值给ek
	pitchShell.eK_1=pitchShell.eK;
}

void Shell_PID_Roll(float time){
	rollShell.Time=time;
	rollShell.eK=expPitch-angle.pitch;
	float eSum;
	eSum=rollShell.eSum+rollShell.eK*time;
	rollShell.eSum=LimitSum(eSum);
	rollShell.eRate=(rollShell.eK-rollShell.eK_1)/rollShell.Time;
	rollShell.output=PID_Count(&rollShell);
	rollShell.eK_1=rollShell.eK;
}

float LimitSum(float eSum){
	if(eSum>50){
			eSum=50;
	}else if(eSum<-50){
		eSum=-50;
	}
	return eSum;
}


/*
header 
		|
		|
CH3   CH1
	\___/
	/		\
CH4   CH2

buf[0] 是左右方向
buf[1] 是前后方向
buf[2] 是油门方向
buf[3] 是左旋转和右旋转方向

	
		  CH1 CH2
		  	\/
header- []  --pitch pitch为正，则应该右偏，pitch 为负，则应该左偏
				/\          前进pitch 为负，后退pitch为正
			CH3 CH4


*/

//控制油门
void Power_Change(void){
	T1=5*TIM1_DataBuf[2];
	T2=5*TIM1_DataBuf[2];
	T3=5*TIM1_DataBuf[2];
	T4=5*TIM1_DataBuf[2];
}

//在控制油门之后 ，再根据pitchShell进行改变他的值
void pitch_Change(void){
	T1=T1+pitchShell.output-rollShell.output;
	T3=T3+pitchShell.output+rollShell.output;
	T2=T2-pitchShell.output-rollShell.output;
	T4=T4-pitchShell.output+rollShell.output;
}

//一般的	起飞油门是310*5=1550
void ChangeMotor(void){
	TIM_SetCompare1(TIM3,T1);
	TIM_SetCompare2(TIM3,T2);
	TIM_SetCompare3(TIM3,T3);
	TIM_SetCompare4(TIM3,T4);
}




