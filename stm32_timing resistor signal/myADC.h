/**
 ****************************************************************************************************
 * @file        myADC.h
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

#ifndef _MYADC_H
#define _MYADC_H
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "myTIME.h"

/******************************************************************************************/
/* ADC������ ���� */

#define myADC_GPIO_PORT GPIOA
#define myADC_GPIO_PIN GPIO_PIN_5
#define myADC_GPIO_CLK_ENABLE()       \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA��ʱ��ʹ�� */

#define myADC_ADCX ADC1
#define myADC_ADCX_CHY ADC_CHANNEL_5 /* ͨ��Y,  0 <= Y <= 17 */
#define myADC_ADCX_CHY_CLK_ENABLE()  \
    do                               \
    {                                \
        __HAL_RCC_ADC1_CLK_ENABLE(); \
    } while (0) /* ADC1 ʱ��ʹ�� */

/* ADC��ͨ��/��ͨ�� DMA�ɼ� DMA��ͨ�� ����
 * ע��: ADC1��DMAͨ��ֻ����: DMA1_Channel1, ���ֻҪ��ADC1, �����ǲ��ܸĶ���
 *       ADC2��֧��DMA�ɼ�
 *       ADC3��DMAͨ��ֻ����: DMA2_Channel5, ������ʹ�� ADC3 ����Ҫ�޸�
 */
#define myADC_ADCX_DMACx DMA1_Channel1
#define myADC_ADCX_DMACx_IRQn DMA1_Channel1_IRQn
#define myADC_ADCX_DMACx_IRQHandler DMA1_Channel1_IRQHandler

#define myADC_ADCX_DMACx_IS_TC() (DMA1->ISR & (1 << 1)) /* �ж� DMA1_Channel1 ������ɱ�־, ����һ���ٺ�����ʽ, \
                                                         * ���ܵ�����ʹ��, ֻ������if���������                  \
                                                         */
#define myADC_ADCX_DMACx_CLR_TC() \
    do                            \
    {                             \
        DMA1->IFCR |= 1 << 1;     \
    } while (0) /* ��� DMA1_Channel1 ������ɱ�־ */

/******************************************************************************************/
/* �ⲿ�ӿں���*/

void myADC_DMA_init(uint32_t mar);     // ADC DMA ��ʼ��
void myADC_DMA_enable(uint16_t cndtr); // ʹ��һ��ADC DMA�ɼ�����

#endif
