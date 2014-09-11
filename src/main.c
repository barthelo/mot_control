#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "Adc.h"
#include "SpeedEncoder.h"
#include "FQChopper.h"
#include "Usart.h"
#include "PIControl.h"


#define GEAR_RATIO_TO_SHAFT 2.77 /*big_gear=61, small_gear=22*/
void BTN_vInit(void);
PIC CurrentController;
PIC SpeedController;

float RPM=0;
float current_set=0;
float dutycycle;
void TIM3_IRQHandler(void)
{
  float speed=ENC_fGetRPM()/GEAR_RATIO_TO_SHAFT;
  float current=ADC_fGetCurrent(ADC_fGetVoltage(ADC_u16GetADCValue('l')));
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {   
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

    /*Calculating current to set*/
    current_set=PIC_fCalcCommand(&SpeedController,RPM,speed);
    /*Calculating duty cycle to set*/
    dutycycle=PIC_fCalcCommand(&CurrentController,current_set,current);

    /*Convert values to valid duty cylce*/
    dutycycle=dutycycle*100;
    if(dutycycle<0)
    FQC_vSetDutyCycleBackward(dutycycle*-1.0);
    else
    FQC_vSetDutyCycleForward(dutycycle);

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

  /*Speed controller*/
  PIC_vConstructor(&SpeedController,0.0,0.007,10,-10,1,1/100);

  /*Current controller*/
  PIC_vConstructor(&CurrentController,1.59545708416707,0.353526079900942,1,-1,1,1/100.0);
  /*slow*//*PIC_vConstructor(&CurrentController,0.90295848990433,0.06169815417594,1,-1,1,1/100.0);*/
  /*agressive*//*PIC_vConstructor(&CurrentController,6.24238455222794,1.72290111327225,1,-1,1,1/100.0);*/

  /*Speed controller*/
  PIC_vConstructor(&SpeedController,0,0.0911764179885756,20,-20,1,1/100.0);

  uint16_t adc_value;
  float voltage_value;
  float current_value;
    
  float rpm=0;
  float signal=0;
  
  while(1)
  {/*User button pushed*/
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
    {
      /*RPM=2000.0;*/
      /*current_set=0.88;*/
      FQC_vSetDutyCycleForward(90);
      GPIO_SetBits(GPIOD, GPIO_Pin_15);
      signal=4.52;
    }
    else
    {/*User button released*/
      /*RPM=500.0;*/
      /*current_set=0.36;*/
      FQC_vSetDutyCycleForward(35);
      GPIO_ResetBits(GPIOD, GPIO_Pin_15);
      signal=2.01;
    }
    adc_value=ADC_u16GetADCValue('l');
    voltage_value=ADC_fGetVoltage(adc_value);
    current_value=ADC_fGetCurrent(voltage_value);
    rpm=ENC_fGetRPM();
    /*USART_vSendFloatAsString(signal);*/
    /*USART_vSendFloatAsString(rpm);*/
    USART_vSendFloatAsString(voltage_value);
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
