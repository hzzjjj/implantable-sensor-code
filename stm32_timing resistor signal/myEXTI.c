/**
 ****************************************************************************************************
 * @file        myEXTI.c
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

#include "./SYSTEM/delay/delay.h"
#include "myEXTI.h"
#include "myPWM.h"
#include "myLED.h"

extern TIM_HandleTypeDef mygtimx_pwm_chy_handle; /* ���� myPWM.h �Ķ�ʱ��x��� */
myPWM_GPIO_MODE g_mypwm_gpio_mode;               /* PWM��GPIO���� ����ģʽ */

/**
 * GPIO�밴��ӳ�䣺PA0 WK_UP; PE4 KEY0; PE3 KEY1
 * KEY0��KEY1�ӵأ���Ҫ�������裬�����ź����½���
 * �� stm32f1xx_hal.c ���������жϷ���Ϊ2��HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
 */

/**
 * @brief       KEY0 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN); /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ Ȼ������ߵ����û��Զ�����жϻص����� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN); /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       KEY1 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY1_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN); /* �����жϴ����ú��� ���KEY1�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN); /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       WK_UP �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void WKUP_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN); /* �����жϴ����ú��� ���KEY_UP�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN); /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       �ⲿ�жϳ�ʼ������
 * @param       ��
 * @retval      ��
 */
void myEXTI_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    KEY0_INT_GPIO_CLK_ENABLE(); /* KEY0ʱ��ʹ�� */
    KEY1_INT_GPIO_CLK_ENABLE(); /* KEY1ʱ��ʹ�� */
    WKUP_INT_GPIO_CLK_ENABLE(); /* WKUPʱ��ʹ�� */

    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;         /* �ж�ģʽ���½��ش�����HAL���ڲ�������źͶ�ӦEXTI��ӳ�䣩 */
    gpio_init_struct.Pull = GPIO_PULLUP;                  /* �������� */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct); /* KEY0����Ϊ�½��ش����ж� */

    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING; /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct); /* KEY1����Ϊ�½��ش����ж� */

    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING; /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(WKUP_INT_GPIO_PORT, &gpio_init_struct); /* WKUP����Ϊ�����ش����ж� */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2); /* ��ռ0����(��Ӧ)���ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);         /* ʹ���ж���4 */

    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2); /* ��ռ1�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);         /* ʹ���ж���3 */

    HAL_NVIC_SetPriority(WKUP_INT_IRQn, 2, 2); /* ��ռ2�������ȼ�2 */
    HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);         /* ʹ���ж���0 */

    myPWM_GPIO_SetAsOutput(); // ��ʼ��Ϊ�������ģʽ
    g_mypwm_gpio_mode = myPWM_GPIO_MODE_OUTPUT;
}

/**
 * @brief       �û��ض���Ļص�����
 *              �жϷ����������Ҫ��������
 *              ��HAL�������е��ⲿ�жϷ�����������ô˺���
 * @param       GPIO_Pin:�ж����ź�
 * @retval      ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20); /* ���� */

    switch (GPIO_Pin)
    {
    case KEY0_INT_GPIO_PIN:
        // KEY0 �������ж��ź�
        if (HAL_GPIO_ReadPin(KEY0_INT_GPIO_PORT, KEY0_INT_GPIO_PIN) == 0) // ����Ƿ�Ϊ�͵�ƽ��ȷ�������������£�
        {
            if (g_mypwm_gpio_mode != myPWM_GPIO_MODE_OUTPUT)
            {
                myPWM_GPIO_SetAsOutput();                   // ����PWM����Ϊ �������
                g_mypwm_gpio_mode = myPWM_GPIO_MODE_OUTPUT; // ����ģʽ״̬
            }

            myLED0_TOGGLE();
        }
        break;

    case KEY1_INT_GPIO_PIN:
        // KEY1 �������ж��ź�
        if (HAL_GPIO_ReadPin(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == 0)
        {
            if (g_mypwm_gpio_mode != myPWM_GPIO_MODE_PWM)
            {
                myPWM_GPIO_SetAsPWM();                   // ����PWM����Ϊ PWM ���
                g_mypwm_gpio_mode = myPWM_GPIO_MODE_PWM; // ����ģʽ״̬
            }

            /* �޸� PWM Ƶ��Ϊ10kHz��ֱ�Ӹ��¼Ĵ����� */
            __HAL_TIM_DISABLE(&mygtimx_pwm_chy_handle); // ��ͣ��ʱ��

            __HAL_TIM_SET_PRESCALER(&mygtimx_pwm_chy_handle, 72 - 1);              // ���·�Ƶϵ�� psc
            __HAL_TIM_SET_AUTORELOAD(&mygtimx_pwm_chy_handle, 100 - 1);            // ������װ��ֵ arr
            __HAL_TIM_SET_COMPARE(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 50); // ��������ռ�ձ� (arr+1)/2

            __HAL_TIM_ENABLE(&mygtimx_pwm_chy_handle); // ����������ʱ��
        }
        break;

    case WKUP_INT_GPIO_PIN:
        // WKUP �������ж��ź�
        if (HAL_GPIO_ReadPin(WKUP_INT_GPIO_PORT, WKUP_INT_GPIO_PIN) == 1) // ����Ƿ�Ϊ�ߵ�ƽ��ȷ�������������£�
        {
            if (g_mypwm_gpio_mode != myPWM_GPIO_MODE_PWM)
            {
                myPWM_GPIO_SetAsPWM();                   // ����PWM����Ϊ PWM ���
                g_mypwm_gpio_mode = myPWM_GPIO_MODE_PWM; // ����ģʽ״̬
            }

            /* �޸� PWM Ƶ��Ϊ5kHz��ֱ�Ӹ��¼Ĵ����� */
            __HAL_TIM_DISABLE(&mygtimx_pwm_chy_handle); // ��ͣ��ʱ��

            __HAL_TIM_SET_PRESCALER(&mygtimx_pwm_chy_handle, 72 - 1);               // ���·�Ƶϵ�� psc
            __HAL_TIM_SET_AUTORELOAD(&mygtimx_pwm_chy_handle, 200 - 1);             // ������װ��ֵ arr
            __HAL_TIM_SET_COMPARE(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 100); // ��������ռ�ձ� (arr+1)/2

            __HAL_TIM_ENABLE(&mygtimx_pwm_chy_handle); // ����������ʱ��
        }
        break;
    }
}

// ����PWM����Ϊ �������
void myPWM_GPIO_SetAsOutput(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    // ��������
    HAL_GPIO_DeInit(myPWM_GPIO_PORT, myPWM_GPIO_PIN);

    // ����Ϊ�������
    gpio_init_struct.Pin = myPWM_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(myPWM_GPIO_PORT, &gpio_init_struct);

    // �ر� PWM ���
    HAL_TIM_PWM_Stop(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);
}

// ����PWM����Ϊ PWM ���
void myPWM_GPIO_SetAsPWM(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    // ��������
    HAL_GPIO_DeInit(myPWM_GPIO_PORT, myPWM_GPIO_PIN);

    // ����Ϊ PWM ���������ʽ���ù��ܣ�
    gpio_init_struct.Pin = myPWM_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(myPWM_GPIO_PORT, &gpio_init_struct);

    // ���� PWM ���
    HAL_TIM_PWM_Start(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);
}
