/**
 ****************************************************************************************************
 * @file        myPWM.c
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

#include "myPWM.h"

TIM_HandleTypeDef mygtimx_pwm_chy_handle;

/**
 * @brief       通用定时器 PWM 输出初始化函数（使用PWM模式2）
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void myPWM_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0}; /* 定时器PWM输出配置 */

    mygtimx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;              // PA6 复用功能 TIM3_CH1
    mygtimx_pwm_chy_handle.Init.Prescaler = psc;                  // 分频系数，最大65535
    mygtimx_pwm_chy_handle.Init.Period = arr;                     // 自动重装载值，最大65535
    mygtimx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP; // 向上计数
    HAL_TIM_PWM_Init(&mygtimx_pwm_chy_handle);                    // 初始化 PWM

    timx_oc_pwm_chy.OCPolarity = TIM_OCNPOLARITY_HIGH; // 输出极性高，高电平有效
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM2;          // 输出比较模式PWM2，CNT>=CRR时输出有效电平
    timx_oc_pwm_chy.Pulse = arr / 2;                   // 初始占空比50%

    HAL_TIM_PWM_ConfigChannel(&mygtimx_pwm_chy_handle, &timx_oc_pwm_chy, GTIM_TIMX_PWM_CHY); // 配置PWM的比较值、占空比等

    // 使能输出并启动计数器 // 让其按下KEY0后才启用PWM输出
    HAL_TIM_PWM_Start(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);
}

/**
 * @brief       定时器输出 PWM MSP初始化函数
 *              定时器底层驱动，时钟使能，引脚配置
 *              此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    // 根据基地址判断使用的是哪个时钟
    if (htim->Instance == GTIM_TIMX_PWM) // PA6 复用功能 TIM3_CH1
    {
        GPIO_InitTypeDef gpio_init_struct;

        myPWM_GPIO_CLK_ENABLE();        // GPIOA 时钟使能 // 已在 myLED_init() 中使能了，且必须先在那配置GPIO才能正常使用PWM
        GTIM_TIMX_PWM_CHY_CLK_ENABLE(); // TIM3 时钟使能

        gpio_init_struct.Pin = myPWM_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;      // 复用推挽输出
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW; // 低速
        gpio_init_struct.Pull = GPIO_NOPULL;          // 推挽输出不需要拉
        HAL_GPIO_Init(myPWM_GPIO_PORT, &gpio_init_struct);
    }
}
