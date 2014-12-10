#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "Adc.h"
#include "SpeedEncoder.h"
#include "FQChopper.h"
#include "Usart.h"
#include "PIControl.h"

void BTN_vInit(void);
PIC CurrentController;
PIC SpeedController;

float velo_set=0;
float velo_value=0;
float current_set=0;
float voltage_value=0;
float current_value=0;
float dutycycle=0;
float signal=0;


void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {   
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

    velo_value=ENC_fGetVelocity();
    current_value=ADC_fGetCurrent();

    /*Calculating current to set*/
    current_set=PIC_fCalcCommandIdealForm(&SpeedController,velo_set,velo_value);
    /*Calculating duty cycle to set*/
    dutycycle=PIC_fCalcCommandIdealForm(&CurrentController,current_set,current_value);
    /*Convert values to valid duty cylce*/
    FQC_vSetDutyCycle(dutycycle);
    
    USART_vSendFloatAsString(velo_value);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}


int main(void)
{
  SystemInit();
  ENC_vInit();
  USART_vInit();
  FQC_vInit();
  BTN_vInit();
  ADC_vInit();
  TIMM_vInit();

  /*Current controller*/
  PIC_vConstructor(&CurrentController,21.17,28.34,100,-100,21.17,1/100.0);
  /*Speed controller*/
  PIC_vConstructor(&SpeedController,17.3,0.006,10,-10,2.3,1/100.0);

  while(1)
  {
    /*FQC_vSetDutyCycle(50);*/
    /*User button pushed*/
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
    {
      velo_set=-150.0;
      /*current_set=1.95;*/
      //FQC_vSetDutyCycle(-80);
      /*signal=4.52;*/
      GPIO_SetBits(GPIOD, GPIO_Pin_15);
    }
    else
    {
      /*User button released*/
      velo_set=15.0;
      /*current_set=0.28;*/
      //FQC_vSetDutyCycle(80);
      /*signal=2.01;*/
      GPIO_ResetBits(GPIOD, GPIO_Pin_15);
    }
  }
}

void BTN_vInit(void)
{
  /* Structure declaration*/
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
