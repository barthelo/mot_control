/**
  ******************************************************************************
  * @file    PWM.c
  * @author  Eugen Bartel
  * @version V1.0
  * @date    17-April-2013
  * @brief   This file provides set of firmware functions to manage the PWM
  ******************************************************************************/

#include "Pwm.h"

float PWM_fDCycle=7.5;//Angabe in Prozent (7.5% entsprechen 1.5ms)

uint16_t PWM_u16ARR=0;
uint16_t PWM_u16CCR=0;

/**
 * @brief  Initialisiert die Ports an dennen der Motor und der Servo angeschlossen ist.
*              Ausserdem werden PWM Cycle eingestellt und der neutrale DutyCyle von 
*              1.5ms (Millisekunden gesetzt).
 * @param  none
 * @retval none
 **/
void PWM_vInit(void)
{
    TIM_TimeBaseInitTypeDef myTIM;
    GPIO_InitTypeDef myGPIO_TIM;
    TIM_OCInitTypeDef myTIM_OC;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

    //**********GPIO's konfigurieren
    myGPIO_TIM.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
    myGPIO_TIM.GPIO_Mode=GPIO_Mode_AF;
    myGPIO_TIM.GPIO_Speed=GPIO_Speed_100MHz;
    myGPIO_TIM.GPIO_OType=GPIO_OType_PP;
    myGPIO_TIM.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&myGPIO_TIM);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);

    //TIMER KONFIGURIEREN
    //Prescaler = ((SystemCoreClock /2) / TIM3 counter clock) - 1
    myTIM.TIM_Prescaler=(uint16_t)(((168000000/2)/PWM_CK_CNT)-1);
    // ARR = (TIM3 counter clock / TIM3 output clock) - 1
    PWM_u16ARR=(PWM_CK_CNT/PWM_TIM_OUTPUT_CLK)-1;
    myTIM.TIM_Period=(uint32_t)PWM_u16ARR;
    myTIM.TIM_ClockDivision=TIM_CKD_DIV1;
    myTIM.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&myTIM);

    //1 KANAL KONFIGURIEREN
    //DutyCycle = (TIM3_CCR1/ (TIM3_ARR+1))* 100
    PWM_u16CCR=(uint16_t)((PWM_fDCycle/100.0f)*(PWM_u16ARR+1));
    
    myTIM_OC.TIM_OCMode=TIM_OCMode_PWM1;
    myTIM_OC.TIM_OutputState=TIM_OutputState_Enable;
    myTIM_OC.TIM_Pulse=PWM_u16CCR;
    myTIM_OC.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC1Init(TIM3,&myTIM_OC);
    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
    
    //2 KANAL KONFIGURIEREN
    myTIM_OC.TIM_OutputState=TIM_OutputState_Enable;
    myTIM_OC.TIM_Pulse=PWM_u16CCR;
    TIM_OC2Init(TIM3,&myTIM_OC);
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
    
    
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
}

void PWM_vSetDCycleCh1(float PWM_fDCycle)
{
  PWM_u16CCR=(uint16_t)((PWM_fDCycle/100.0f)*(PWM_u16ARR+1));
}

