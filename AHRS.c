/** 
 *  @brief	    
 *  @details    
 *  @author     屈子铭
 *  @date       2022/10/26
 *  @version    1.0
 *  @par        日志
*/

#include "AHRS.h"
#include "sdk_time.h"
#include "sdk_math.h"

uint32_t lastMadgwickTick = 0; //用于madgwick算法中记录上次计算时间


void AHRS_MadgWick(quat_t *q, vector3f_t a, vector3f_t g, vector3f_t m) {

    uint32_t tickStart = sdk_time_GetMicroSecondsTick();

    float q0 = q->a, q1 = q->b, q2 = q->c, q3 = q->d;

    float beta = MADGWICK_BETA;
    float detaT = MADGWICK_DELTA_TIME;


    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0, _2q1, _2q2, _2q3;
    float _2q0q1, _2q0q2, _2q0q3, _2q1q1, _2q1q2, _2q1q3, _2q2q2, _2q2q3, _2q3q3;
    float bx, bz;

    float f[6];
    float Q_E2S[3][3], Q_S2E[3][3];

    // ½Q*Wₛ
    qDot1 = 0.5f * (-q1 * g.x - q2 * g.y - q3 * g.z);
    qDot2 = 0.5f * (q0 * g.x + q2 * g.z - q3 * g.y);
    qDot3 = 0.5f * (q0 * g.y - q1 * g.z + q3 * g.x);
    qDot4 = 0.5f * (q0 * g.z + q1 * g.y - q2 * g.x);

    if (!((a.x == 0.0f) && (a.y == 0.0f) && (a.z == 0.0f))) {

        recipNorm = invSqrt(a.x * a.x + a.y * a.y + a.z * a.z);
        a.x *= recipNorm;
        a.y *= recipNorm;
        a.z *= recipNorm;

        recipNorm = invSqrt(m.x * m.x + m.y * m.y + m.z * m.z);
        m.x *= recipNorm;
        m.y *= recipNorm;
        m.z *= recipNorm;

        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;

        _2q0q1 = _2q0 * q1;
        _2q0q2 = _2q0 * q2;
        _2q0q3 = _2q0 * q3;
        _2q1q1 = _2q1 * q1;
        _2q1q2 = _2q1 * q2;
        _2q1q3 = _2q1 * q3;
        _2q2q2 = _2q2 * q2;
        _2q2q3 = _2q2 * q3;
        _2q3q3 = _2q3 * q3;

        Q_E2S[0][0] = Q_S2E[0][0] = 1 - _2q2q2 - _2q3q3;
        Q_E2S[0][1] = Q_S2E[1][0] = _2q0q3 + _2q1q2;
        Q_E2S[0][2] = Q_S2E[2][0] = _2q1q3 - _2q0q2;
        Q_E2S[1][0] = Q_S2E[0][1] = -_2q0q3 + _2q1q2;
        Q_E2S[1][1] = Q_S2E[1][1] = 1 - _2q1q1 - _2q3q3;
        Q_E2S[1][2] = Q_S2E[2][1] = _2q0q1 + _2q2q3;
        Q_E2S[2][0] = Q_S2E[0][2] = _2q1q3 + _2q0q2;
        Q_E2S[2][1] = Q_S2E[1][2] = -_2q0q1 + _2q2q3;
        Q_E2S[2][2] = Q_S2E[2][2] = 1 - _2q1q1 - _2q2q2;

        // 磁偏校正
        hx = Q_S2E[0][0] * m.x + Q_S2E[0][1] * m.y + Q_S2E[0][2] * m.z;
        hy = Q_S2E[1][0] * m.x + Q_S2E[1][1] * m.y + Q_S2E[1][2] * m.z;
        bx = sqrtf(hx * hx + hy * hy);
        bz = Q_S2E[2][0] * m.x + Q_S2E[2][1] * m.y + Q_S2E[2][2] * m.z;


        //cal F
        f[0] = Q_E2S[0][2] - a.x;
        f[1] = Q_E2S[1][2] - a.y;
        f[2] = Q_E2S[2][2] - a.z;
        f[3] = Q_E2S[0][0] * bx + Q_E2S[0][2] * bz - m.x;
        f[4] = Q_E2S[1][0] * bx + Q_E2S[1][2] * bz - m.y;
        f[5] = Q_E2S[2][0] * bx + Q_E2S[2][2] * bz - m.z;

//        printf("%.2f %2.f %2.f %2.f %.2f %.2f\n", f[0], f[1], f[2], f[3], f[4], f[5]);

        //cal ▽S(Q₀) = JᵀF
        s0 = -q2 * f[0] + q1 * f[1] - q2 * bz * f[3] + (-q3 * bx + q1 * bz) * f[4] + q2 * bx * f[5];
        s1 = q3 * f[0] + q0 * f[1] - _2q1 * f[2] + (q3 * bx - _2q1 * bz) * f[5];
        s2 = -q0 * f[0] + q3 * f[1] - _2q2 * f[2] + (-_2q2 * bx - q0 * bz) * f[3] +
             (q1 * bx + q3 * bz) * f[4] + (q0 * bx - _2q2 * bz) * f[5];
        s3 = q1 * f[0] + q2 * f[1] + (-_2q3 * bx + q1 * bz) * f[3] + (-q0 * bx + q2 * bz) * f[4] + q1 * bx * f[5];

        //cal ▽S(Q₀) / |▽S(Q₀)|
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;
//        printf("invSqrt:%.2f\n", recipNorm);

        // β * ▽S(Q₀) / |▽S(Q₀)|
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    if (lastMadgwickTick) {
        detaT = (float) (HAL_GetTick() - lastMadgwickTick) / 1000.0f;
//        printf("%.2f\n", detaT);
    }
    lastMadgwickTick = HAL_GetTick();

    // Q=Q₀+(½Q * Wₛ - β * ▽S(Q₀) / |▽S(Q₀)|)Δt
    q0 += qDot1 * detaT;
    q1 += qDot2 * detaT;
    q2 += qDot3 * detaT;
    q3 += qDot4 * detaT;


    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    q->a = q0;
    q->b = q1;
    q->c = q2;
    q->d = q3;

    uint32_t tickEnd = sdk_time_GetMicroSecondsTick();
    printf("usedTime:%lu us\n", tickEnd - tickStart);
}

void AHRS_ConvertQuatToDegree(quat_t *q, vector3f_t *angle) {
    /*
     * matlab代码
     * tmp = qb.*qd.*the2 - qa.*qc.*the2;
        tmp(tmp > the1) = the1;
        tmp(tmp < -the1) = -the1;
        tolA = cast(0.5.*pi - 10.*eps(the1), 'like', tmp);
        tolB = cast(-0.5.*pi + 10.*eps(the1), 'like', tmp);
        for ii=1:numel(tmp)
            b(ii) = -asin(tmp(ii)); //Y
            if b(ii) >=  tolA
                a(ii) = -2.*atan2(qb(ii),qa(ii));
                c(ii) = 0;
            elseif b(ii) <= tolB
                a(ii) = 2.*atan2(qb(ii),qa(ii));
                c(ii) = 0;
            else
                a(ii) = atan2((qa(ii).*qd(ii).*the2 + qb(ii).*qc(ii).*the2),(qa(ii).^2.*the2 - the1 + qb(ii).^2.*the2)); //Z
                c(ii) = atan2((qa(ii).*qb(ii).*the2 + qc(ii).*qd(ii).*the2),(qa(ii).^2.*the2 - the1 + qd(ii).^2.*the2)); //X
            end
        end
     */

    //TODO 待实机测试，未验证
    float tmp = 2.0f * q->b * q->d - 2.0f * q->a * q->c;
    LIMIT(tmp, -1.0f, 1.0f);
    angle->y = asinf(tmp);
    angle->x = -atan2f(2.0f * q->a * q->b + 2.0f * q->c * q->d, 2.0f * SQR(q->a) - 1 + 2.0f * SQR(q->d));
    angle->z = -atan2f(2.0f * q->a * q->d + 2.0f * q->b * q->c, 2.0f * SQR(q->a) - 1 + 2.0f * SQR(q->b));
}