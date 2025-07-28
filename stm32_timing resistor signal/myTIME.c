/**
 ****************************************************************************************************
 * @file        myTIME.c
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

#include "myTIME.h"

TIM_HandleTypeDef mytime_handle; // 定时器句柄

void myTIME_Init(void)
{
    myTIME_CLK_ENABLE(); // 使能定时器时钟

    // TIM2 配置结构体
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    // TIM2 基本配置
    mytime_handle.Instance = myTIME;
    mytime_handle.Init.Prescaler = 71;                         // 预分频系数 (72 MHz / (71 + 1) = 1 MHz)
    mytime_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数
    mytime_handle.Init.Period = 999;                           // 自动重装载值，每增加1 对应 1μs
    mytime_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 不分频，72MHz
    HAL_TIM_Base_Init(&mytime_handle);

    // 配置时钟源使用内部时钟源（可忽略）
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&mytime_handle, &sClockSourceConfig);

    // 配置主输出
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;          // 定时器不发送触发信号
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; // 禁用主从模式，定时器不会作为主定时器生成触发信号，也不会与其他定时器同步
    HAL_TIMEx_MasterConfigSynchronization(&mytime_handle, &sMasterConfig);

    // 启动定时器
    HAL_TIM_Base_Start(&mytime_handle);
}

uint32_t GetElapsedTime(void)
{
    return __HAL_TIM_GET_COUNTER(&mytime_handle); // 获取计数器值
}
