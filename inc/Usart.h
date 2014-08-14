/**
 * *************************************************************************
 * @file     Usart.h
 * @author   Eugen Bartel
 * @version  V1.0
 * @date     24. April 2014
 * @brief    Diese Datei enthält die Deklarationen der Funktionen, der Variablen
 *              und die Praeprozessoranweisungen, die bnoetigt werden, um den
 *              USART zu betreiben.
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h" 
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void USART_vInit(void);
void USART_vSendChar(uint8_t ch);
void USART_vSendString(volatile char *s);
void USART_vSendFloatAsString(float value);

#endif /* __USART_H */