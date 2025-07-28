/**
 ****************************************************************************************************
 * @file        myADC.h
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

#ifndef _MYADC_H
#define _MYADC_H
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "myTIME.h"

/******************************************************************************************/
/* ADC及引脚 定义 */

#define myADC_GPIO_PORT GPIOA
#define myADC_GPIO_PIN GPIO_PIN_5
#define myADC_GPIO_CLK_ENABLE()       \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PA口时钟使能 */

#define myADC_ADCX ADC1
#define myADC_ADCX_CHY ADC_CHANNEL_5 /* 通道Y,  0 <= Y <= 17 */
#define myADC_ADCX_CHY_CLK_ENABLE()  \
    do                               \
    {                                \
        __HAL_RCC_ADC1_CLK_ENABLE(); \
    } while (0) /* ADC1 时钟使能 */

/* ADC单通道/多通道 DMA采集 DMA及通道 定义
 * 注意: ADC1的DMA通道只能是: DMA1_Channel1, 因此只要是ADC1, 这里是不能改动的
 *       ADC2不支持DMA采集
 *       ADC3的DMA通道只能是: DMA2_Channel5, 因此如果使用 ADC3 则需要修改
 */
#define myADC_ADCX_DMACx DMA1_Channel1
#define myADC_ADCX_DMACx_IRQn DMA1_Channel1_IRQn
#define myADC_ADCX_DMACx_IRQHandler DMA1_Channel1_IRQHandler

#define myADC_ADCX_DMACx_IS_TC() (DMA1->ISR & (1 << 1)) /* 判断 DMA1_Channel1 传输完成标志, 这是一个假函数形式, \
                                                         * 不能当函数使用, 只能用在if等语句里面                  \
                                                         */
#define myADC_ADCX_DMACx_CLR_TC() \
    do                            \
    {                             \
        DMA1->IFCR |= 1 << 1;     \
    } while (0) /* 清除 DMA1_Channel1 传输完成标志 */

/******************************************************************************************/
/* 外部接口函数*/

void myADC_DMA_init(uint32_t mar);     // ADC DMA 初始化
void myADC_DMA_enable(uint16_t cndtr); // 使能一次ADC DMA采集传输

#endif
