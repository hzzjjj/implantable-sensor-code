/**
 ****************************************************************************************************
 * @file        myEXTI.h
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

#ifndef _MYEXTI_H
#define _MYEXTI_H
#include "./SYSTEM/sys/sys.h"
#include "myPWM.h"

// 记录 PWM的GPIO引脚 当前模式的标志位
typedef enum
{
    myPWM_GPIO_MODE_PWM,
    myPWM_GPIO_MODE_OUTPUT
} myPWM_GPIO_MODE;

extern myPWM_GPIO_MODE g_mypwm_gpio_mode; // 在头文件中声明需要加 extern

/******************************************************************************************/
/* 引脚 和 中断编号 & 中断服务函数 定义 */
/* KEY0 */
#define KEY0_INT_GPIO_PORT GPIOE
#define KEY0_INT_GPIO_PIN GPIO_PIN_4
#define KEY0_INT_GPIO_CLK_ENABLE()    \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0) /* PE口时钟使能 */
#define KEY0_INT_IRQn EXTI4_IRQn
#define KEY0_INT_IRQHandler EXTI4_IRQHandler

/* KEY1 */
#define KEY1_INT_GPIO_PORT GPIOE
#define KEY1_INT_GPIO_PIN GPIO_PIN_3
#define KEY1_INT_GPIO_CLK_ENABLE()    \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0) /* PE口时钟使能 */
#define KEY1_INT_IRQn EXTI3_IRQn
#define KEY1_INT_IRQHandler EXTI3_IRQHandler

/* WKUP */
#define WKUP_INT_GPIO_PORT GPIOA
#define WKUP_INT_GPIO_PIN GPIO_PIN_0
#define WKUP_INT_GPIO_CLK_ENABLE()    \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA口时钟使能 */
#define WKUP_INT_IRQn EXTI0_IRQn
#define WKUP_INT_IRQHandler EXTI0_IRQHandler

/******************************************************************************************/

void myEXTI_init(void); /* 外部中断初始化 */
void myPWM_GPIO_SetAsOutput(void);
void myPWM_GPIO_SetAsPWM(void);

#endif
