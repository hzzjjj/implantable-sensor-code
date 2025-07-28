/**
 ****************************************************************************************************
 * @file        myLED.h
 * @author      夏雨祺
 * @version     V2.0
 * @date        2024-09-03
 * @brief       光电传感器响应测试（PWM控制、ADC、按键中断）
 * @license     中国矿业大学安全工程学院
 ****************************************************************************************************
 * @attention
 *
 ****************************************************************************************************
 */

#ifndef _MYLED_H
#define _MYLED_H
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 引脚 定义 */

/* PA6 TIM3_CH1 */
#define myLED0_GPIO_PORT GPIOA
#define myLED0_GPIO_PIN GPIO_PIN_6
#define myLED0_GPIO_CLK_ENABLE()      \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA口时钟使能 */

/******************************************************************************************/
/* LED端口定义 */
#define myLED0(x)                                                                                                                                      \
    do                                                                                                                                                 \
    {                                                                                                                                                  \
        x ? HAL_GPIO_WritePin(myLED0_GPIO_PORT, myLED0_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(myLED0_GPIO_PORT, myLED0_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* LED0翻转 */

/* LED取反定义 */
#define myLED0_TOGGLE()                                        \
    do                                                         \
    {                                                          \
        HAL_GPIO_TogglePin(myLED0_GPIO_PORT, myLED0_GPIO_PIN); \
    } while (0) /* 翻转LED0 */

/******************************************************************************************/
/* 外部接口函数*/
void myLED_init(void); /* 初始化 */

#endif
