/**
 ****************************************************************************************************
 * @file        myPWM.h
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

#ifndef _MYPWM_H
#define _MYPWM_H
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ͨ�ö�ʱ�� ���� */

/* TIMX �ж϶���
 * Ĭ�������TIM2~TIM5
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM8����һ����ʱ��.
 */

#define GTIM_TIMX_INT TIM3
#define GTIM_TIMX_INT_IRQn TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()   \
    do                               \
    {                                \
        __HAL_RCC_TIM3_CLK_ENABLE(); \
    } while (0) /* TIM3 ʱ��ʹ�� */

/******************************************************************************************/
/* TIMX PWM������� */

/* ʹ�� PA6 ��ΪPWM��� */
#define myPWM_GPIO_PORT GPIOA
#define myPWM_GPIO_PIN GPIO_PIN_6
#define myPWM_GPIO_CLK_ENABLE()       \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA��ʱ��ʹ�� */

#define GTIM_TIMX_PWM TIM3
#define GTIM_TIMX_PWM_CHY TIM_CHANNEL_1   /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_PWM_CHY_CCRX TIM3->CCR1 /* ͨ��Y������ȽϼĴ��� */
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE() \
    do                                 \
    {                                  \
        __HAL_RCC_TIM3_CLK_ENABLE();   \
    } while (0) /* TIM3 ʱ��ʹ�� */

/******************************************************************************************/

void myPWM_init(uint16_t arr, uint16_t psc); /* ͨ�ö�ʱ�� PWM��ʼ������ */

#endif
