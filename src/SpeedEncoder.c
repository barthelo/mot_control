/**
  ******************************************************************************
  * @file    Engine.c
  * @author  Eugen Bartel
  * @version V1.0
  * @date    17-April-2013
  * @brief   This file provides set of firmware functions to manage the Engine
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "SpeedEncoder.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
uint16_t ENC_ui16Cnt1=0;
uint16_t ENC_ui16Cnt2=0;
int16_t ENC_i16dCnt=0;
int16_t ENC_i16dCnt1=0;

/**
  * @brief  Configuration an initialization of an encoder.
  * @param  none
  * @retval none
  */
void ENC_vInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);  //PB6  TIM4_CH1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);  //PB7  TIM4_CH2

  //Setup quadrature encoder and enable timer
  TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI1,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
  //Set autoreload register (ARR) to 2^16;
  TIM_SetAutoreload(TIM4,0xffff);

  TIM_Cmd(TIM4, ENABLE);
  
  //Setting up timer Interrupt
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInit_Structure;
  NVIC_InitTypeDef NVIC_InitStructure;
  //Enable TIM5 Clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  //Configure timer
  TIM_TimeBaseInit_Structure.TIM_Prescaler=((SystemCoreClock/2)/3000000)-1;//Prescaler = ((SystemCoreClock /2) / TIMx counter clock) - 1
  TIM_TimeBaseInit_Structure.TIM_Period=(3000000/ENC_SAMPLE_FREQ)-1;// ARR = (TIMx counter clock / TIMx output clock) - 1
  TIM_TimeBaseInit_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseInit_Structure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInit_Structure);
  
  //Configure Interrupt
  NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
  NVIC_Init(&NVIC_InitStructure);
  
  //Attach timer to interrupt
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  
  //Enable timer
  TIM_Cmd(TIM5, ENABLE);
}

/**
  * @brief  This function handles timer exception. 
  *         Especially calculates delta angle per sample time.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
  
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    //Calculate delta pulses
    ENC_i16dCnt1=ENC_i16dCnt;
    ENC_ui16Cnt1=ENC_ui16Cnt2;
    ENC_ui16Cnt2=ENC_ui16GetCnt();

    if(ENC_ui16Cnt1>ENC_ui16Cnt2)
      ENC_i16dCnt=(ENC_ui16Cnt2+ENC_CNT_MAX-ENC_ui16Cnt1);
    else
      ENC_i16dCnt=(ENC_ui16Cnt2-ENC_ui16Cnt1);

    if(ENC_ui8GetDir())
      ENC_i16dCnt=(-1)*(ENC_CNT_MAX-ENC_i16dCnt);
    
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  }
}

/**
  * @brief  Read encoder's counter value.
  * @param  None
  * @retval ENC_ui16CntValue Encoder's counter value.  
  */
uint16_t ENC_ui16GetCnt(void)
{
  return (uint16_t)TIM_GetCounter(TIM4);
}

/**
  * @brief  Read encoder's direction in regsiter
  * @param  None
  * @retval ENC_i8Dir Encoder's rotating direction.
  */
uint8_t ENC_ui8GetDir(void)
{
  return (uint8_t)(TIM4->CR1 & TIM_CR1_DIR); //Get direction bit (DIR) in TIM4_CR1 Register
}

/**
  * @brief  Calculate encoder's angle in degree (0-360)
  * @param  None
  * @retval ENC_fAngle Encoder's current angle.
  */
float ENC_fGetAngle(void)
{ 
  return (((float)ENC_ui16Cnt2/ENC_CPR)-((uint8_t)(ENC_ui16Cnt2/ENC_CPR)))*360.0;
}

/**
  * @brief  Calculate encoder's velocity in rad/s.
  * @param  None
  * @retval ENC_fVelocity Encoder's current velocity.
  */
float ENC_fGetVelocity(void)
{ 
  return (((float)ENC_i16dCnt/ENC_CPR)*ENC_SAMPLE_FREQ*ENC_PI);
}

/**
  * @brief  Calculate RPM
  * @param  None
  * @retval ENC_fRPM Encoder's current RPM.
  */
float ENC_fGetRPM(void)
{
  return (((float)ENC_i16dCnt/ENC_CPR)*ENC_SAMPLE_FREQ*60.0);
}

/**
* @brief  Calculate Acceleration in rad/s^2
* @param  None
* @retval ENC_fAcceleration Encoder's current acceleration
*/
float ENC_fGetAcceleration(void)
{
  return (((float)(ENC_i16dCnt-ENC_i16dCnt1)/ENC_CPR)*ENC_SAMPLE_FREQ*ENC_SAMPLE_FREQ*ENC_PI);
}


