/**
 ****************************************************************************************************
 * @file        myLED.c
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

#include "myLED.h"

void myLED_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    myLED0_GPIO_CLK_ENABLE(); /* myLED0时钟使能 */

    gpio_init_struct.Pin = myLED0_GPIO_PIN;             /* myLED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* 推挽输出，不需要上拉，最大电流 25mA */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;       /* 低速 */
    HAL_GPIO_Init(myLED0_GPIO_PORT, &gpio_init_struct); /* 初始化LED0引脚 */

    myLED0(0); /* 引脚接 LED0 阳极，低电平关闭 */
}
