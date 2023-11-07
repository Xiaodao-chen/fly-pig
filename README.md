# fly-pig

这是一个无人机四轴项目，使用stm32 作为主控，使用固件库编程，移植ucos ii 操作系统，最终实现飞机可以起飞的完整项目

## 所使用的硬件：

1. STM32 f401 开发版
2. ESP8266-01s  wifi 模块
3. OLED ssd1305 
4. GY86模块
5. XXD 新西达 A2212/13T KV1000强磁无刷电机 4个
6. T8FB 2.4G 8通道  + R8FM接收机
7. F450 机架 
8. 好盈电调航模无刷电调 20A

## 需要完成：

1. ucos ii的移植
2. 姿态解算算法的实现
3. PID算法实现
4. 串口通信 与上位机的连接进行数据传递
5. I2C与OLED通信
6. 主控对于接收机PPM的处理
7. 主控对于电调PWM的输出
8. 一体板 PCB设计 制作

## 文件结构

```
+---DebugConfig
+---ESP8266
+---f401
|   +---CMSIS
|   +---FWLIB
|   \---STARTUP
+---I2C
+---LED
+---Listings
+---Objects
+---OLED
+---RTE
|   +---Device
|   |   \---STM32F401RETx
|   \---_Target_1
+---SYSTICK
+---TIM
+---USART
\---USER
```



....

