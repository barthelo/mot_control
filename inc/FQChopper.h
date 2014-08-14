/**
 * *************************************************************************
 * @file     FQChopper.h
 * @author   Eugen Bartel
 * @version  V1.0
 * @date     17. April 2013
 * @brief    Diese Datei enthält die Deklarationen der Funktionen, der Variablen
 *		und die Präprozessoranweisungen, die bnoetigt werden, um den
 * 		Brushless-Motor zu betreiben.
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FQChopper_H
#define __FQChopper_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FQC_SWITCHING_FREQUENCY 20000
#define FQC_CK_CNT 3000000
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void FQC_vInit(void);
void FQC_vSetDutyCycleForward(float FQC_fDutyCycle);
void FQC_vSetDutyCycleBackward(float FQC_fDutyCycle);
void FQC_vBreak(void);

#endif /* __FQChopper_H */

