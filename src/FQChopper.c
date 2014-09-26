/**
  ******************************************************************************
  * @file    FQChopper.c
  * @author  Eugen Bartel
  * @version V1.0
  * @date    17-April-2013
  * @brief   Provides FQChopper's firmware functions
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "FQChopper.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInit_Structure;
TIM_OCInitTypeDef TIM_OCInitTypeDef_Structure;
float FQC_fDutyCycle=0.0;
uint16_t FQC_u16ARR=0;
uint16_t FQC_u16CCR=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Configuration of GPIOs and Timer
 * @note   
 * @param  none
 * @retval none
 **/
void FQC_vInit(void)
{
  /* Activate Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  /* GPIO configuration */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);  //PB3  TIM2_CH2
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);  //PB10  TIM2_CH3
  
  /* Timer configuration */
  TIM_TimeBaseInit_Structure.TIM_Prescaler=(uint16_t)(((SystemCoreClock/2)/FQC_CK_CNT)-1);/* Prescaler = ((SystemCoreClock/2) / TIMx counter clock) - 1 */
  FQC_u16ARR=(FQC_CK_CNT/FQC_SWITCHING_FREQUENCY)-1;/* ARR = (TIMx counter clock / TIMx output clock) - 1 */
  TIM_TimeBaseInit_Structure.TIM_Period=(uint32_t)FQC_u16ARR;
  TIM_TimeBaseInit_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseInit_Structure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInit_Structure);
  
  /* Channel2 configuration */
  FQC_u16CCR=(uint16_t)((FQC_fDutyCycle/100.0f)*(FQC_u16ARR+1));/* DutyCycle = (TIMx_CCR1/ (TIMx_ARR+1))*100 */
  
  TIM_OCInitTypeDef_Structure.TIM_OCMode=TIM_OCMode_PWM1;
  TIM_OCInitTypeDef_Structure.TIM_OutputState=TIM_OutputState_Enable;
  TIM_OCInitTypeDef_Structure.TIM_Pulse=(uint32_t)FQC_u16CCR;
  TIM_OCInitTypeDef_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
  TIM_OC2Init(TIM2,&TIM_OCInitTypeDef_Structure);
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
  
  /* Channel3 configuration */
  TIM_OCInitTypeDef_Structure.TIM_OutputState=TIM_OutputState_Enable;
  TIM_OCInitTypeDef_Structure.TIM_Pulse=(uint32_t)FQC_u16CCR;
  TIM_OC3Init(TIM2,&TIM_OCInitTypeDef_Structure);
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  
  /* Enable counter */
  TIM_Cmd(TIM2,ENABLE);
}

/**
 * @brief  Set PWM duty cycle to rotate in forward direction
 * @param  FQC_fDutyCycle Dutycycle
 * @retval none
 **/

void FQC_vSetDutyCycleForward(float FQC_fDutyCycle)
{
    /*Catch and correct faulty duty cycle values*/
    if(FQC_fDutyCycle>100)
        FQC_fDutyCycle=100;
    if(FQC_fDutyCycle<0)
        FQC_fDutyCycle=0;
    
  FQC_u16CCR=(uint16_t)((FQC_fDutyCycle/100.0f)*(FQC_u16ARR+1));
  TIM_SetCompare2(TIM2, (uint32_t)0);
  TIM_SetCompare3(TIM2, (uint32_t)FQC_u16CCR);
}
/**
 * @brief  Set PWM duty cycle to rotate in backward direction
 * @param  FQC_fDutyCycle Dutycycle
 * @retval none
 **/

void FQC_vSetDutyCycleBackward(float FQC_fDutyCycle)
{
    /*Catch and correct faulty duty cycle values*/
    if(FQC_fDutyCycle>100)
        FQC_fDutyCycle=100;
    if(FQC_fDutyCycle<0)
        FQC_fDutyCycle=0;
    
  FQC_u16CCR=(uint16_t)((FQC_fDutyCycle/100.0f)*(FQC_u16ARR+1));
  TIM_SetCompare2(TIM2, (uint32_t)FQC_u16CCR);
  TIM_SetCompare3(TIM2, (uint32_t)0);
}

/**
 * @brief  Stops the Motor
 * @param  none
 * @retval none
 **/

void FQC_vBreak(void)
{
  TIM_SetCompare2(TIM2, (uint32_t)0);
  TIM_SetCompare3(TIM2, (uint32_t)0);
}

/**
 * @brief  Set dutycycle. Also accept negaitve dutycycle
 * @param FQC_fDutyCycle Positive or negative dutycycle 
 * @retval none
 **/

void FQC_vSetDutyCycle(float FQC_fDutyCycle)
{
    if(FQC_fDutyCycle>0)
    {
       FQC_vSetDutyCycleForward(FQC_fDutyCycle);
    }
    else if(FQC_fDutyCycle<0)
    {
        FQC_vSetDutyCycleBackward((-1)*FQC_fDutyCycle);
    }
}
