/**
 ****************************************************************************************************
 * @file        main.c
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

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "myLED.h"
#include "myPWM.h"
#include "myEXTI.h"
#include "myADC.h"
#include "myTIME.h"

uint8_t len;        // 发送的数据长度，单位字节，源码中未被初始化
uint32_t adc_value; // 存放 ADC 获取的值
float voltage;      // 转换后的电压值，单位V
float R;            // 待测电阻阻值，单位MΩ

#define myADC_DMA_BUF_SIZE 100              /* ADC DMA采集 BUF大小 */
uint16_t g_adc_dma_buf[myADC_DMA_BUF_SIZE]; /* ADC DMA BUF */
extern uint8_t g_adc_dma_start;             /* DMA传输状态标志, 0,未完成; 1, 已完成 */

int main(void)
	{
    HAL_Init();                         /* 初始化 HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 初始化 延时 */
    usart_init(115200);                 /* 初始化 串口，波特率和实时记录的ADC频率成正比 */
    led_init();                         /* 初始化 开发板上的LED */

    myTIME_Init();                            /* 初始化 定时器计时(1μs) */
    myPWM_init(100 - 1, 72 - 1);              /* 初始化 PWM */
    myEXTI_init();                            /* 初始化 中断 */
    myADC_DMA_init((uint32_t)&g_adc_dma_buf); /* 初始化 myADC_DMA */
    myADC_DMA_enable(myADC_DMA_BUF_SIZE);     /* 启动一次ADC DMA采集 */

    // uint32_t current_time_ms = HAL_GetTick(); // 获取当前时间，单位ms
    // uint32_t current_time_us = GetElapsedTime();

    while (1)
    {
        // 等待DMA传输完成
        if (g_adc_dma_start == 1)
        {
            // 数据处理
            adc_value = 0;
            for (uint8_t i = 0; i < myADC_DMA_BUF_SIZE; i++)
            {
                adc_value += g_adc_dma_buf[i];
            }
            adc_value /= myADC_DMA_BUF_SIZE; // 计算采样均值

            voltage = (float)adc_value * (3.3f / 4096);
            R = (3.26 - voltage) * 4.96 / voltage;

            printf("%.4f\n", R);
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)g_usart_rx_buf, len, 1000); /* 发送接收到的数据 */
            while (__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_TC) != SET)
                ; /* 等待发送结束 */

            g_adc_dma_start = 0;                  /* 清除DMA采集完成状态标志 */
            myADC_DMA_enable(myADC_DMA_BUF_SIZE); /* 启动下一次ADC DMA采集 */
        }

        LED0_TOGGLE();
        delay_ms(1000); // 每隔30秒开始一次采集
    }
}
