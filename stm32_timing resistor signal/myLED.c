/**
 ****************************************************************************************************
 * @file        myLED.c
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

#include "myLED.h"

void myLED_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    myLED0_GPIO_CLK_ENABLE(); /* myLED0ʱ��ʹ�� */

    gpio_init_struct.Pin = myLED0_GPIO_PIN;             /* myLED0���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* �������������Ҫ������������ 25mA */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;       /* ���� */
    HAL_GPIO_Init(myLED0_GPIO_PORT, &gpio_init_struct); /* ��ʼ��LED0���� */

    myLED0(0); /* ���Ž� LED0 �������͵�ƽ�ر� */
}
