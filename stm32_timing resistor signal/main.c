/**
 ****************************************************************************************************
 * @file        main.c
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

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "myLED.h"
#include "myPWM.h"
#include "myEXTI.h"
#include "myADC.h"
#include "myTIME.h"

uint8_t len;        // ���͵����ݳ��ȣ���λ�ֽڣ�Դ����δ����ʼ��
uint32_t adc_value; // ��� ADC ��ȡ��ֵ
float voltage;      // ת����ĵ�ѹֵ����λV
float R;            // ���������ֵ����λM��

#define myADC_DMA_BUF_SIZE 100              /* ADC DMA�ɼ� BUF��С */
uint16_t g_adc_dma_buf[myADC_DMA_BUF_SIZE]; /* ADC DMA BUF */
extern uint8_t g_adc_dma_start;             /* DMA����״̬��־, 0,δ���; 1, ����� */

int main(void)
	{
    HAL_Init();                         /* ��ʼ�� HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʼ�� ��ʱ */
    usart_init(115200);                 /* ��ʼ�� ���ڣ������ʺ�ʵʱ��¼��ADCƵ�ʳ����� */
    led_init();                         /* ��ʼ�� �������ϵ�LED */

    myTIME_Init();                            /* ��ʼ�� ��ʱ����ʱ(1��s) */
    myPWM_init(100 - 1, 72 - 1);              /* ��ʼ�� PWM */
    myEXTI_init();                            /* ��ʼ�� �ж� */
    myADC_DMA_init((uint32_t)&g_adc_dma_buf); /* ��ʼ�� myADC_DMA */
    myADC_DMA_enable(myADC_DMA_BUF_SIZE);     /* ����һ��ADC DMA�ɼ� */

    // uint32_t current_time_ms = HAL_GetTick(); // ��ȡ��ǰʱ�䣬��λms
    // uint32_t current_time_us = GetElapsedTime();

    while (1)
    {
        // �ȴ�DMA�������
        if (g_adc_dma_start == 1)
        {
            // ���ݴ���
            adc_value = 0;
            for (uint8_t i = 0; i < myADC_DMA_BUF_SIZE; i++)
            {
                adc_value += g_adc_dma_buf[i];
            }
            adc_value /= myADC_DMA_BUF_SIZE; // ���������ֵ

            voltage = (float)adc_value * (3.3f / 4096);
            R = (3.26 - voltage) * 4.96 / voltage;

            printf("%.4f\n", R);
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)g_usart_rx_buf, len, 1000); /* ���ͽ��յ������� */
            while (__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_TC) != SET)
                ; /* �ȴ����ͽ��� */

            g_adc_dma_start = 0;                  /* ���DMA�ɼ����״̬��־ */
            myADC_DMA_enable(myADC_DMA_BUF_SIZE); /* ������һ��ADC DMA�ɼ� */
        }

        LED0_TOGGLE();
        delay_ms(1000); // ÿ��30�뿪ʼһ�βɼ�
    }
}
