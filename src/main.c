#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "Pwm.h"
#include "Adc.h"
#include "SpeedEncoder.h"
#include "FQChopper.h"
#include "Usart.h"
#include "PIControl.h"


void BTN_vInit(void);
PIC CurrentController;

float go; 

void TIM3_IRQHandler(void)
{
  float current=ADC_fGetCurrent(ADC_fGetVoltage(ADC_u16GetADCValue('l')));
  float command;
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {    
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
    command=PIC_fCalcCommand(&CurrentController,go,current);
    command=command*100;
    command<0?FQC_vSetDutyCycleBackward(command*-1.0):FQC_vSetDutyCycleForward(command);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

int main(void)
{
  TIMM_vInit();
  SystemInit();
  ENC_vInit();
  USART_vInit();
  FQC_vInit();
  BTN_vInit();
  ADC_vInit();
  
  /* Start ADC cenversion (Position is variable in source code)*/
  ADC_SoftwareStartConv(ADC1);
  
  
  PIC_vConstructor(&CurrentController,0.90295848990433,0.06169815417594,1,-1,1,1/100.0);
  uint16_t adc_value;
  float voltage_value;
  float current_value;
  
  
  float rpm=0;
  float signal=0;
  
  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
    {
//       FQC_vSetDutyCycleBackward(20);
      go=0.5;
      GPIO_SetBits(GPIOD, GPIO_Pin_15);
      signal=1;
    }
    else
    {
//       FQC_vSetDutyCycleForward(20);
      go=-0.5;
      GPIO_ResetBits(GPIOD, GPIO_Pin_15);
      signal=0;
    }
    adc_value=ADC_u16GetADCValue('l');
    voltage_value=ADC_fGetVoltage(adc_value);
    current_value=ADC_fGetCurrent(voltage_value);
    rpm=ENC_fGetRPM();
//     USART_vSendFloatAsString(signal);
//     USART_vSendFloatAsString(rpm);
    USART_vSendFloatAsString(current_value);
  }
}

void BTN_vInit(void)
{
  /* Declare structures*/
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Activate clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD,ENABLE);
  
  /* GPIOA configuration */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* GPIOD configuration */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}
