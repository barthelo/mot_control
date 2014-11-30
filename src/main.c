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

float RPM;
float current_set;
float dutycycle;
uint16_t adc_value;
float voltage_value;
float current_value;

float rpm=0;
float signal=0;

void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {   
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
    /*Get voltage value*/
    /*voltage_value=(ADC_fGetVoltage(ADC_u16GetADCValue('l'))-ADC_OFFSET_L)-(ADC_fGetVoltage(ADC_u16GetADCValue('r'))-ADC_OFFSET_R);*/
    /*current_value=(ADC_fGetCurrent());*/
    /*float speed=ENC_fGetRPM();*/
    float current=ADC_fGetCurrent();

    /*Calculating current to set*/
    /*current_set=PIC_fCalcCommand(&SpeedController,RPM,speed);*/
    /*Calculating duty cycle to set*/
    dutycycle=PIC_fCalcCommandIdealForm(&CurrentController,current_set,current);

    /*Convert values to valid duty cylce*/
    FQC_vSetDutyCycle(dutycycle);
    
    USART_vSendFloatAsString(current);

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

  /* Start ADC cenversion (Position is variable in source code)*/
  ADC_SoftwareStartConv(ADC1);

  /*Current controller*/
  /*PIC_vConstructor(&CurrentController,1.59545708416707,0.353526079900942,1,-1,1,1/100.0);*/
  /*DC 0 to 100*/
  /*PIC_vConstructor(&CurrentController,198.795121348507,6.18792004994483,100,-100,198.795121348507,1/100.0);*/
  /*slow*/
  /*PIC_vConstructor(&CurrentController,0.90295848990433,0.06169815417594,1,-1,1,1/100.0);*/
  /*agressive*/
  //PIC_vConstructor(&CurrentController,6.24238455222794,1.72290111327225,1,-1,1,1/100.0);
  /*Betragsoptimum*/
  PIC_vConstructor(&CurrentController,38.0228,0.08291,100,-100,38.0228,1/100.0);

  /*Speed controller*/
  //PIC_vConstructor(&SpeedController,0,0.0911764179885756,20,-20,1,1/100.0);
  /*PIC_vConstructor(&SpeedController,0.00859,0.00117,10,-10,1,1/100.0);*/
  /*TOP PPIC_vConstructor(&SpeedController,0.003101,0.0008224,20,-20,1,1/100.0);*/

  
  while(1)
  {
    /*FQC_vSetDutyCycle(50);*/
    /*User button pushed*/
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
    {
      /*RPM=-1500.0;*/
      current_set=0.58;
      /*FQC_vSetDutyCycle(-50);*/
      GPIO_SetBits(GPIOD, GPIO_Pin_15);
      /*signal=4.52;*/
    }
    else
    {
      /*User button released*/
      /*RPM=500.0;*/
      current_set=0.35;
      /*FQC_vSetDutyCycleBackward(0);*/
      GPIO_ResetBits(GPIOD, GPIO_Pin_15);
      /*signal=2.01;*/
    }
    /*adc_value=ADC_u16GetADCValue('l');*/
    /*voltage_value=ADC_fGetVoltage(adc_value);*/
    /*current_value=ADC_fGetCurrent(voltage_value);*/
    /*rpm=ENC_fGetRPM();*/
    /*voltage_value=ADC_fGetVoltage(ADC_u16GetADCValue('l'));*/
    /*USART_vSendFloatAsString(ADC_fGetCurrentSum());*/
    /*USART_vSendFloatAsString(rpm);*/
    /*USART_vSendFloatAsString(voltage_value);*/
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
