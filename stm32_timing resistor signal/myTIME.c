/**
 ****************************************************************************************************
 * @file        myTIME.c
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

#include "myTIME.h"

TIM_HandleTypeDef mytime_handle; // ��ʱ�����

void myTIME_Init(void)
{
    myTIME_CLK_ENABLE(); // ʹ�ܶ�ʱ��ʱ��

    // TIM2 ���ýṹ��
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    // TIM2 ��������
    mytime_handle.Instance = myTIME;
    mytime_handle.Init.Prescaler = 71;                         // Ԥ��Ƶϵ�� (72 MHz / (71 + 1) = 1 MHz)
    mytime_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       // ���ϼ���
    mytime_handle.Init.Period = 999;                           // �Զ���װ��ֵ��ÿ����1 ��Ӧ 1��s
    mytime_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // ����Ƶ��72MHz
    HAL_TIM_Base_Init(&mytime_handle);

    // ����ʱ��Դʹ���ڲ�ʱ��Դ���ɺ��ԣ�
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&mytime_handle, &sClockSourceConfig);

    // ���������
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;          // ��ʱ�������ʹ����ź�
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; // ��������ģʽ����ʱ��������Ϊ����ʱ�����ɴ����źţ�Ҳ������������ʱ��ͬ��
    HAL_TIMEx_MasterConfigSynchronization(&mytime_handle, &sMasterConfig);

    // ������ʱ��
    HAL_TIM_Base_Start(&mytime_handle);
}

uint32_t GetElapsedTime(void)
{
    return __HAL_TIM_GET_COUNTER(&mytime_handle); // ��ȡ������ֵ
}
