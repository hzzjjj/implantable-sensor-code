/**
 ****************************************************************************************************
 * @file        myEXTI.h
 * @author      ������
 * @version     V2.0
 * @date        2024-09-03
 * @brief       ��紫������Ӧ���ԣ�PWM���ơ�ADC�������жϣ�
 * @license     �й���ҵ��ѧ��ȫ����ѧԺ
 ****************************************************************************************************
 * @attention
 *
 ****************************************************************************************************
 */

#ifndef _MYEXTI_H
#define _MYEXTI_H
#include "./SYSTEM/sys/sys.h"
#include "myPWM.h"

// ��¼ PWM��GPIO���� ��ǰģʽ�ı�־λ
typedef enum
{
    myPWM_GPIO_MODE_PWM,
    myPWM_GPIO_MODE_OUTPUT
} myPWM_GPIO_MODE;

extern myPWM_GPIO_MODE g_mypwm_gpio_mode; // ��ͷ�ļ���������Ҫ�� extern

/******************************************************************************************/
/* ���� �� �жϱ�� & �жϷ����� ���� */
/* KEY0 */
#define KEY0_INT_GPIO_PORT GPIOE
#define KEY0_INT_GPIO_PIN GPIO_PIN_4
#define KEY0_INT_GPIO_CLK_ENABLE()    \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0) /* PE��ʱ��ʹ�� */
#define KEY0_INT_IRQn EXTI4_IRQn
#define KEY0_INT_IRQHandler EXTI4_IRQHandler

/* KEY1 */
#define KEY1_INT_GPIO_PORT GPIOE
#define KEY1_INT_GPIO_PIN GPIO_PIN_3
#define KEY1_INT_GPIO_CLK_ENABLE()    \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0) /* PE��ʱ��ʹ�� */
#define KEY1_INT_IRQn EXTI3_IRQn
#define KEY1_INT_IRQHandler EXTI3_IRQHandler

/* WKUP */
#define WKUP_INT_GPIO_PORT GPIOA
#define WKUP_INT_GPIO_PIN GPIO_PIN_0
#define WKUP_INT_GPIO_CLK_ENABLE()    \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA��ʱ��ʹ�� */
#define WKUP_INT_IRQn EXTI0_IRQn
#define WKUP_INT_IRQHandler EXTI0_IRQHandler

/******************************************************************************************/

void myEXTI_init(void); /* �ⲿ�жϳ�ʼ�� */
void myPWM_GPIO_SetAsOutput(void);
void myPWM_GPIO_SetAsPWM(void);

#endif
