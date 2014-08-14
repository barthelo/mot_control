/**
 * *************************************************************************
 * @file     Adc.h
 * @author   Eugen Bartel
 * @version  V1.0
 * @date     17. April 2013
 * @brief    Diese Datei enthält die Deklarationen der Funktionen, der Variablen
 *              und die Präprozessoranweisungen, die bnoetigt werden, um den
 *              ADC betreiben.
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h" 
#include "stm32f4xx_adc.h"
/* Exported types ------------------------------------------------------------*/
volatile uint16_t ADC_u16ConvertedValue[2];
/* Exported constants --------------------------------------------------------*/
#define ADC1_DR_ADDRESS 0x4001204C //ADC Regular Data Register of ADC1 (RM0090 p. 51, 247, 242)
#define ADC_VOLTAGE_REFERENCE 2.9583
#define ADC_MAX_RESOLUTION 4095.0
#define ADC_LOAD_CURRENT_FACTOR 1.75
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void ADC_vInit(void);
uint16_t ADC_u16GetADCValue(char ADC_cChopperSide);
float ADC_fGetVoltage(uint16_t ADC_u16ADCValue);
float ADC_fGetCurrent(float ADC_fVoltage);

#endif /* __ADC_H */