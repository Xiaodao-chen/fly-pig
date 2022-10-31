/** 
 *  @brief	    
 *  @details    
 *  @author     屈子铭
 *  @date       2022/10/26
 *  @version    1.0
 *  @par        日志
*/

#ifndef GY86_AHRS_H
#define GY86_AHRS_H

#include "main.h"
#include "sdk_ano.h"

#define MADGWICK_BETA 0.066f
#define MADGWICK_DELTA_TIME 0.03f

/**
 * madgwick算法
 * @param q 四元数（原始数据及结果数据）
 * @param a 加速度
 * @param g 角速度
 * @param m 磁力计数据
 */
void AHRS_MadgWick(quat_t *q, vector3f_t a, vector3f_t g, vector3f_t m);

/**
 * 将四元数转换为欧拉角
 * @details 按照ZYX顺序转换，绕着坐标轴逆时针旋转为角度的负方向，即右手螺旋方向为角度负方向
 * @param q 四元数
 * @param angle 角度（结果）
 */
void AHRS_ConvertQuatToDegree(quat_t *q, vector3f_t *angle);

#endif //GY86_AHRS_H
