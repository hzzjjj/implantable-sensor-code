/**
 ****************************************************************************************************
 * @file        myPWM.h
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

#ifndef _MYPWM_H
#define _MYPWM_H
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 通用定时器 定义 */

/* TIMX 中断定义
 * 默认是针对TIM2~TIM5
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM8任意一个定时器.
 */

#define GTIM_TIMX_INT TIM3
#define GTIM_TIMX_INT_IRQn TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()   \
    do                               \
    {                                \
        __HAL_RCC_TIM3_CLK_ENABLE(); \
    } while (0) /* TIM3 时钟使能 */

/******************************************************************************************/
/* TIMX PWM输出定义 */

/* 使用 PA6 作为PWM输出 */
#define myPWM_GPIO_PORT GPIOA
#define myPWM_GPIO_PIN GPIO_PIN_6
#define myPWM_GPIO_CLK_ENABLE()       \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA口时钟使能 */

#define GTIM_TIMX_PWM TIM3
#define GTIM_TIMX_PWM_CHY TIM_CHANNEL_1   /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_PWM_CHY_CCRX TIM3->CCR1 /* 通道Y的输出比较寄存器 */
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE() \
    do                                 \
    {                                  \
        __HAL_RCC_TIM3_CLK_ENABLE();   \
    } while (0) /* TIM3 时钟使能 */

/******************************************************************************************/

void myPWM_init(uint16_t arr, uint16_t psc); /* 通用定时器 PWM初始化函数 */

#endif
