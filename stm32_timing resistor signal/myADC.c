/**
 ****************************************************************************************************
 * @file        myADC.c
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

#include "myADC.h"

/***************************************��ͨ��ADC�ɼ�(DMA��ȡ)����*****************************************/

DMA_HandleTypeDef g_dma_adc_handle = {0}; // DMA���
ADC_HandleTypeDef g_adc_dma_handle = {0}; // ADC���
uint8_t g_adc_dma_start = 0;              // DMA����״̬��־, 0,δ���; 1, �����

/**
 * @brief       ADC DMA��ȡ ��ʼ������
 *   @note      ����������ʹ��adc_init��ADC���д󲿷�����,�в���ĵط��ٵ�������
 * @param       par         : �����ַ
 * @param       mar         : �洢����ַ
 * @retval      ��
 */
void myADC_DMA_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    myADC_ADCX_CHY_CLK_ENABLE();    /* ʹ��ADCxʱ�� */
    myADC_GPIO_CLK_ENABLE();        /* ����GPIOʱ�� */

    if ((uint32_t)myADC_ADCX_DMACx > (uint32_t)DMA1_Channel7) /* ����DMA1_Channel7, ��ΪDMA2��ͨ���� */
    {
        __HAL_RCC_DMA2_CLK_ENABLE(); /* DMA2ʱ��ʹ�� */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE(); /* DMA1ʱ��ʹ�� */
    }

    /* ����ADCʱ�� */
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC; /* ADC����ʱ�� */
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;    /* ��Ƶ����6ʱ��Ϊ72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);              /* ����ADCʱ�� */

    /* ����AD�ɼ�ͨ����ӦIO���Ź���ģʽ */
    gpio_init_struct.Pin = myADC_GPIO_PIN;    /* ADCͨ����Ӧ��IO���� */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG; /* ģ�� */
    HAL_GPIO_Init(myADC_GPIO_PORT, &gpio_init_struct);

    /* ��ʼ��DMA */
    g_dma_adc_handle.Instance = myADC_ADCX_DMACx;                        /* ����DMAͨ�� */
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              /* �����赽�洢��ģʽ */
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                  /* ���������ģʽ */
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                      /* �洢������ģʽ */
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* �������ݳ���:16λ */
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* �洢�����ݳ���:16λ */
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;                             /* ��������ģʽ */
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                /* �е����ȼ� */
    HAL_DMA_Init(&g_dma_adc_handle);

    __HAL_LINKDMA(&g_adc_dma_handle, DMA_Handle, g_dma_adc_handle); /* ��DMA��adc��ϵ���� */

    g_adc_dma_handle.Instance = myADC_ADCX;                      /* ѡ���ĸ�ADC */
    g_adc_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;       /* ���ݶ��뷽ʽ���Ҷ��� */
    g_adc_dma_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;       /* ��ɨ��ģʽ�����õ�һ��ͨ�� */
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;           /* ʹ������ת��ģʽ */
    g_adc_dma_handle.Init.NbrOfConversion = 1;                   /* ��ֵ��Χ��1~16����ʵ���õ�1������ͨ������ */
    g_adc_dma_handle.Init.DiscontinuousConvMode = DISABLE;       /* ��ֹ����ͨ������ģʽ */
    g_adc_dma_handle.Init.NbrOfDiscConversion = 0;               /* ���ü��ģʽ�Ĺ���ͨ����������ֹ����ͨ������ģʽ�󣬴˲������� */
    g_adc_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START; /* ����ת����ʽ��������� */
    HAL_ADC_Init(&g_adc_dma_handle);                             /* ��ʼ�� */

    HAL_ADCEx_Calibration_Start(&g_adc_dma_handle); /* У׼ADC */

    /* ����ADCͨ�� */
    adc_ch_conf.Channel = myADC_ADCX_CHY;                   /* ͨ�� */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                  /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;  /* ����ʱ�䣬��������������:239.5��ADC���� */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf); /* ͨ������ */

    /* ����DMA�����������ж����ȼ� */
    HAL_NVIC_SetPriority(myADC_ADCX_DMACx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(myADC_ADCX_DMACx_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0); /* ����DMA���������ж� */
    HAL_ADC_Start_DMA(&g_adc_dma_handle, &mar, 0);                    /* ����ADC��ͨ��DMA������ */
}

/**
 * @brief       ʹ��һ��ADC DMA����
 *   @note      �ú����üĴ�������������ֹ��HAL������������������޸�,ҲΪ�˼�����
 * @param       cndtr: DMA����Ĵ���
 * @retval      ��
 */
void myADC_DMA_enable(uint16_t cndtr)
{
    // �üĴ�������
    myADC_ADCX->CR2 &= ~(1 << 0); // �ر� ADCX

    myADC_ADCX_DMACx->CCR &= ~(1 << 0); // �ر� DMA ����
    while (myADC_ADCX_DMACx->CCR & (1 << 0))
        ;                            // �ȴ� DMA ͨ��x ��ȫʧ�ܣ�ȷ��DMA���Ա�����
    myADC_ADCX_DMACx->CNDTR = cndtr; // ���� DMA �����ݴ���������ͨ���β� cndtr ���ݣ�
    myADC_ADCX_DMACx->CCR |= 1 << 0; // ���� DMA ����

    myADC_ADCX->CR2 |= 1 << 0;  // �������� ADC
    myADC_ADCX->CR2 |= 1 << 22; // ��������ת��ͨ��
}

/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       ��
 * @retval      ��
 */
void myADC_ADCX_DMACx_IRQHandler(void)
{
    if (myADC_ADCX_DMACx_IS_TC())
    {
        g_adc_dma_start = 1;       // ���DMA�������
        myADC_ADCX_DMACx_CLR_TC(); // ����жϱ�־λ��IFCR�Ĵ�����Ӧλ ��1
    }
}
