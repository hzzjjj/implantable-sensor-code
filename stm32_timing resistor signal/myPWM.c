/**
 ****************************************************************************************************
 * @file        myPWM.c
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

#include "myPWM.h"

TIM_HandleTypeDef mygtimx_pwm_chy_handle;

/**
 * @brief       ͨ�ö�ʱ�� PWM �����ʼ��������ʹ��PWMģʽ2��
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void myPWM_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0}; /* ��ʱ��PWM������� */

    mygtimx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;              // PA6 ���ù��� TIM3_CH1
    mygtimx_pwm_chy_handle.Init.Prescaler = psc;                  // ��Ƶϵ�������65535
    mygtimx_pwm_chy_handle.Init.Period = arr;                     // �Զ���װ��ֵ�����65535
    mygtimx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP; // ���ϼ���
    HAL_TIM_PWM_Init(&mygtimx_pwm_chy_handle);                    // ��ʼ�� PWM

    timx_oc_pwm_chy.OCPolarity = TIM_OCNPOLARITY_HIGH; // ������Ըߣ��ߵ�ƽ��Ч
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM2;          // ����Ƚ�ģʽPWM2��CNT>=CRRʱ�����Ч��ƽ
    timx_oc_pwm_chy.Pulse = arr / 2;                   // ��ʼռ�ձ�50%

    HAL_TIM_PWM_ConfigChannel(&mygtimx_pwm_chy_handle, &timx_oc_pwm_chy, GTIM_TIMX_PWM_CHY); // ����PWM�ıȽ�ֵ��ռ�ձȵ�

    // ʹ����������������� // ���䰴��KEY0�������PWM���
    HAL_TIM_PWM_Start(&mygtimx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);
}

/**
 * @brief       ��ʱ����� PWM MSP��ʼ������
 *              ��ʱ���ײ�������ʱ��ʹ�ܣ���������
 *              �˺����ᱻHAL_TIM_PWM_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    // ���ݻ���ַ�ж�ʹ�õ����ĸ�ʱ��
    if (htim->Instance == GTIM_TIMX_PWM) // PA6 ���ù��� TIM3_CH1
    {
        GPIO_InitTypeDef gpio_init_struct;

        myPWM_GPIO_CLK_ENABLE();        // GPIOA ʱ��ʹ�� // ���� myLED_init() ��ʹ���ˣ��ұ�������������GPIO��������ʹ��PWM
        GTIM_TIMX_PWM_CHY_CLK_ENABLE(); // TIM3 ʱ��ʹ��

        gpio_init_struct.Pin = myPWM_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;      // �����������
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW; // ����
        gpio_init_struct.Pull = GPIO_NOPULL;          // �����������Ҫ��
        HAL_GPIO_Init(myPWM_GPIO_PORT, &gpio_init_struct);
    }
}
