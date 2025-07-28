/**
 ****************************************************************************************************
 * @file        myADC.c
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

#include "myADC.h"

/***************************************单通道ADC采集(DMA读取)代码*****************************************/

DMA_HandleTypeDef g_dma_adc_handle = {0}; // DMA句柄
ADC_HandleTypeDef g_adc_dma_handle = {0}; // ADC句柄
uint8_t g_adc_dma_start = 0;              // DMA传输状态标志, 0,未完成; 1, 已完成

/**
 * @brief       ADC DMA读取 初始化函数
 *   @note      本函数还是使用adc_init对ADC进行大部分配置,有差异的地方再单独配置
 * @param       par         : 外设地址
 * @param       mar         : 存储器地址
 * @retval      无
 */
void myADC_DMA_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    myADC_ADCX_CHY_CLK_ENABLE();    /* 使能ADCx时钟 */
    myADC_GPIO_CLK_ENABLE();        /* 开启GPIO时钟 */

    if ((uint32_t)myADC_ADCX_DMACx > (uint32_t)DMA1_Channel7) /* 大于DMA1_Channel7, 则为DMA2的通道了 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE(); /* DMA2时钟使能 */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE(); /* DMA1时钟使能 */
    }

    /* 设置ADC时钟 */
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC; /* ADC外设时钟 */
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;    /* 分频因子6时钟为72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);              /* 设置ADC时钟 */

    /* 设置AD采集通道对应IO引脚工作模式 */
    gpio_init_struct.Pin = myADC_GPIO_PIN;    /* ADC通道对应的IO引脚 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG; /* 模拟 */
    HAL_GPIO_Init(myADC_GPIO_PORT, &gpio_init_struct);

    /* 初始化DMA */
    g_dma_adc_handle.Instance = myADC_ADCX_DMACx;                        /* 设置DMA通道 */
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              /* 从外设到存储器模式 */
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                  /* 外设非增量模式 */
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                      /* 存储器增量模式 */
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* 外设数据长度:16位 */
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* 存储器数据长度:16位 */
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;                             /* 外设流控模式 */
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                /* 中等优先级 */
    HAL_DMA_Init(&g_dma_adc_handle);

    __HAL_LINKDMA(&g_adc_dma_handle, DMA_Handle, g_dma_adc_handle); /* 将DMA与adc联系起来 */

    g_adc_dma_handle.Instance = myADC_ADCX;                      /* 选择哪个ADC */
    g_adc_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;       /* 数据对齐方式：右对齐 */
    g_adc_dma_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;       /* 非扫描模式，仅用到一个通道 */
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;           /* 使能连续转换模式 */
    g_adc_dma_handle.Init.NbrOfConversion = 1;                   /* 赋值范围是1~16，本实验用到1个规则通道序列 */
    g_adc_dma_handle.Init.DiscontinuousConvMode = DISABLE;       /* 禁止规则通道组间断模式 */
    g_adc_dma_handle.Init.NbrOfDiscConversion = 0;               /* 配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略 */
    g_adc_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START; /* 触发转换方式：软件触发 */
    HAL_ADC_Init(&g_adc_dma_handle);                             /* 初始化 */

    HAL_ADCEx_Calibration_Start(&g_adc_dma_handle); /* 校准ADC */

    /* 配置ADC通道 */
    adc_ch_conf.Channel = myADC_ADCX_CHY;                   /* 通道 */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                  /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;  /* 采样时间，设置最大采样周期:239.5个ADC周期 */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf); /* 通道配置 */

    /* 配置DMA数据流请求中断优先级 */
    HAL_NVIC_SetPriority(myADC_ADCX_DMACx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(myADC_ADCX_DMACx_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0); /* 启动DMA，并开启中断 */
    HAL_ADC_Start_DMA(&g_adc_dma_handle, &mar, 0);                    /* 开启ADC，通过DMA传输结果 */
}

/**
 * @brief       使能一次ADC DMA传输
 *   @note      该函数用寄存器来操作，防止用HAL库操作对其他参数有修改,也为了兼容性
 * @param       cndtr: DMA传输的次数
 * @retval      无
 */
void myADC_DMA_enable(uint16_t cndtr)
{
    // 用寄存器操作
    myADC_ADCX->CR2 &= ~(1 << 0); // 关闭 ADCX

    myADC_ADCX_DMACx->CCR &= ~(1 << 0); // 关闭 DMA 传输
    while (myADC_ADCX_DMACx->CCR & (1 << 0))
        ;                            // 等待 DMA 通道x 完全失能，确保DMA可以被设置
    myADC_ADCX_DMACx->CNDTR = cndtr; // 设置 DMA 的数据传输数量（通过形参 cndtr 传递）
    myADC_ADCX_DMACx->CCR |= 1 << 0; // 开启 DMA 传输

    myADC_ADCX->CR2 |= 1 << 0;  // 重新启用 ADC
    myADC_ADCX->CR2 |= 1 << 22; // 启动规则转换通道
}

/**
 * @brief       ADC DMA采集中断服务函数
 * @param       无
 * @retval      无
 */
void myADC_ADCX_DMACx_IRQHandler(void)
{
    if (myADC_ADCX_DMACx_IS_TC())
    {
        g_adc_dma_start = 1;       // 标记DMA传输完成
        myADC_ADCX_DMACx_CLR_TC(); // 清除中断标志位，IFCR寄存器对应位 置1
    }
}
