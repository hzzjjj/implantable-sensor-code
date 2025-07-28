/**
 ****************************************************************************************************
 * @file        myEXTI.c
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

#include "./SYSTEM/delay/delay.h"
#include "myEXTI.h"
#include "myPWM.h"
#include "myLED.h"

extern TIM_HandleTypeDef mygtimx_pwm_chy_handle; /* 来自 myPWM.h 的定时器x句柄 */
myPWM_GPIO_MODE g_mypwm_gpio_mode;               /* PWM的GPIO引脚 工作模式 */

/**
 * GPIO与按键映射：PA0 WK_UP; PE4 KEY0; PE3 KEY1
 * KEY0，KEY1接地，需要上拉电阻，输入信号是下降沿
 * 在 stm32f1xx_hal.c 中已设置中断分组为2：HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
 */

/**
 * @brief       KEY0 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN); /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 然后在里边调用用户自定义的中断回调函数 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN); /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       KEY1 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY1_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN); /* 调用中断处理公用函数 清除KEY1所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN); /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       WK_UP 外部中断服务程序
 * @param       无
 * @retval      无
 */
void WKUP_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN); /* 调用中断处理公用函数 清除KEY_UP所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
    __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN); /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       外部中断初始化程序
 * @param       无
 * @retval      无
 */
void myEXTI_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    KEY0_INT_GPIO_CLK_ENABLE(); /* KEY0时钟使能 */
    KEY1_INT_GPIO_CLK_ENABLE(); /* KEY1时钟使能 */
    WKUP_INT_GPIO_CLK_ENABLE(); /* WKUP时钟使能 */

    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;         /* 中断模式：下降沿触发（HAL库内部完成引脚和对应EXTI的映射） */
    gpio_init_struct.Pull = GPIO_PULLUP;                  /* 上拉电阻 */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct); /* KEY0配置为下降沿触发中断 */

    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING; /* 下升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct); /* KEY1配置为下降沿触发中断 */

    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING; /* 上升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(WKUP_INT_GPIO_PORT, &gpio_init_struct); /* WKUP配置为上升沿触发中断 */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2); /* 抢占0，子(响应)优先级2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);         /* 使能中断线4 */

    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2); /* 抢占1，子优先级2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);         /* 使能中断线3 */

    HAL_NVIC_SetPriority(WKUP_INT_IRQn, 2, 2); /* 抢占2，子优先级2 */
    HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);         /* 使能中断线0 */

    myPWM_GPIO_SetAsOutput(); // 初始化为推挽输出模式
    g_mypwm_gpio_mode = myPWM_GPIO_MODE_OUTPUT;
}

/**
 * @brief       用户重定义的回调函数
 *              中断服务程序中需要做的事情
 *              在HAL库中所有的外部中断服务函数都会调用此函数
 * @param       GPIO_Pin:中断引脚号
 * @retval      无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20); /* 消抖 */

    switch (GPIO_Pin)
    {
    case KEY0_INT_GPIO_PIN:
        // KEY0 产生的中断信号
        if (HAL_GPIO_ReadPin(KEY0_INT_GPIO_PORT, KEY0_INT_GPIO_PIN) == 0) // 检查是否为低电平（确保按键正常按下）
        {
            if (g_mypwm_gpio_mode != myPWM_GPIO_MODE_OUTPUT)
            {
                myPWM_GPIO_SetAsOutput();                   // 配置PWM引脚为 推挽输出
                g_mypwm_gpio_mode = myPWM_GPIO_MODE_OUTPUT; // 更新模式状态
            }

            myLED0_TOGGLE();
        }
        break;

    case KEY1_INT_GPIO_PIN:
        // KEY1 产生的中断信号
        if (HAL_GPIO_ReadPin(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == 0)
        {
            if (g_mypwm_gpio_mode != myPWM_GPIO_MODE_PWM)
            {
                myPWM_GPIO_SetAsPWM();                   // 配置PWM引脚为 PWM 输出
                g_mypwm_gpio_mode = myPWM_GPIO_MODE_PWM; // 更新模式状态
            }

            /* 修改 PWM 频率为10kHz（直接更新寄存器） */
            __HAL_TIM_DISABLE(&mygtimx_pwm_chy_handle); // 暂停定时器

            __HAL_TIM_SET_PRESCALER(&mygtimx_pwm_chy_handle, 72 - 1);              // 更新分频系数 psc
            __HAL_TIM_SET_AUTORELOAD(&mygtimx_pwm_chy_handle, 100 - 1);            // 更新重装载值 arr
            __HAL_TIM_SET_COMPARE(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 50); // 重新设置占空比 (arr+1)/2

            __HAL_TIM_ENABLE(&mygtimx_pwm_chy_handle); // 重新启动定时器
        }
        break;

    case WKUP_INT_GPIO_PIN:
        // WKUP 产生的中断信号
        if (HAL_GPIO_ReadPin(WKUP_INT_GPIO_PORT, WKUP_INT_GPIO_PIN) == 1) // 检查是否为高电平（确保按键正常按下）
        {
            if (g_mypwm_gpio_mode != myPWM_GPIO_MODE_PWM)
            {
                myPWM_GPIO_SetAsPWM();                   // 配置PWM引脚为 PWM 输出
                g_mypwm_gpio_mode = myPWM_GPIO_MODE_PWM; // 更新模式状态
            }

            /* 修改 PWM 频率为5kHz（直接更新寄存器） */
            __HAL_TIM_DISABLE(&mygtimx_pwm_chy_handle); // 暂停定时器

            __HAL_TIM_SET_PRESCALER(&mygtimx_pwm_chy_handle, 72 - 1);               // 更新分频系数 psc
            __HAL_TIM_SET_AUTORELOAD(&mygtimx_pwm_chy_handle, 200 - 1);             // 更新重装载值 arr
            __HAL_TIM_SET_COMPARE(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 100); // 重新设置占空比 (arr+1)/2

            __HAL_TIM_ENABLE(&mygtimx_pwm_chy_handle); // 重新启动定时器
        }
        break;
    }
}

// 配置PWM引脚为 推挽输出
void myPWM_GPIO_SetAsOutput(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    // 禁用引脚
    HAL_GPIO_DeInit(myPWM_GPIO_PORT, myPWM_GPIO_PIN);

    // 配置为推挽输出
    gpio_init_struct.Pin = myPWM_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(myPWM_GPIO_PORT, &gpio_init_struct);

    // 关闭 PWM 输出
    HAL_TIM_PWM_Stop(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);
}

// 配置PWM引脚为 PWM 输出
void myPWM_GPIO_SetAsPWM(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    // 禁用引脚
    HAL_GPIO_DeInit(myPWM_GPIO_PORT, myPWM_GPIO_PIN);

    // 配置为 PWM 输出（推挽式复用功能）
    gpio_init_struct.Pin = myPWM_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(myPWM_GPIO_PORT, &gpio_init_struct);

    // 启用 PWM 输出
    HAL_TIM_PWM_Start(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);
}
