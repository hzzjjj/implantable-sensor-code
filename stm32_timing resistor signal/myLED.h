/**
 ****************************************************************************************************
 * @file        myLED.h
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

#ifndef _MYLED_H
#define _MYLED_H
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ���� ���� */

/* PA6 TIM3_CH1 */
#define myLED0_GPIO_PORT GPIOA
#define myLED0_GPIO_PIN GPIO_PIN_6
#define myLED0_GPIO_CLK_ENABLE()      \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA��ʱ��ʹ�� */

/******************************************************************************************/
/* LED�˿ڶ��� */
#define myLED0(x)                                                                                                                                      \
    do                                                                                                                                                 \
    {                                                                                                                                                  \
        x ? HAL_GPIO_WritePin(myLED0_GPIO_PORT, myLED0_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(myLED0_GPIO_PORT, myLED0_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* LED0��ת */

/* LEDȡ������ */
#define myLED0_TOGGLE()                                        \
    do                                                         \
    {                                                          \
        HAL_GPIO_TogglePin(myLED0_GPIO_PORT, myLED0_GPIO_PIN); \
    } while (0) /* ��תLED0 */

/******************************************************************************************/
/* �ⲿ�ӿں���*/
void myLED_init(void); /* ��ʼ�� */

#endif
