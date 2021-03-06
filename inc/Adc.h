/**
 * *************************************************************************
 * @file     Adc.h
 * @author   Eugen Bartel
 * @version  V1.0
 * @date     17. April 2013
 * @brief    This file provides set of firmware functions to manage the AD
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h" 
#include "stm32f4xx_adc.h"
#include "SpeedEncoder.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ADC_SAMPLES 105
#define ADC_FILTER_COEFF_1 0.09516
#define ADC_FILTER_COEFF_2 0.9048
#define ADC_VOLTAGE_REFERENCE 2.98
#define ADC_MAX_RESOLUTION 4095.0
#define ADC_LOAD_CURRENT_FACTOR 17.25
#define ADC_OFFSET_L 0.159
#define ADC_OFFSET_R 0.123

#define  ADC1_DR_ADDRESS ((uint32_t)0x4001204C) //ADC Regular Data Register of ADC1 (RM0090 p. 51, 247, 242)
#define  ADC2_DR_ADDRESS ((uint32_t)0x4001214C) //ADC Regular Data Register of ADC2 (RM0090 p. 51, 247, 242)
#define  ADC_CDR_ADDRESS ((uint32_t)0x40012308)

volatile uint16_t ADC_u16ConvertedValueLeft;
volatile uint16_t ADC_u16ConvertedValueRight;
volatile float ADC_fSumLeft;
volatile float ADC_fSumRight;
volatile float ADC_fMeanLeft;
volatile float ADC_fMeanRight;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void ADC_vInit(void);
uint16_t ADC_u16GetADCValue(char ADC_cChopperSide);
float ADC_fGetVoltage(uint16_t ADC_u16ADCValue);
float ADC_fGetCurrent(void);
float ADC_fFilterSignal(float ADC_fSignal);

#endif /* __ADC_H */
