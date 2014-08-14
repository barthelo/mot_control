/**
 * *************************************************************************
 * @file     Engine.h
 * @author   Eugen Bartel
 * @version  V1.0
 * @date     17. April 2013
 * @brief    Diese Datei enthält die Deklarationen der Funktionen, der Variablen
 *              und die Präprozessoranweisungen, die bnoetigt werden, um den
 *              Brushless-Motor zu betreiben.
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_H
#define __PWM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* Exported types ------------------------------------------------------------*/
extern uint16_t PWM_u16ARR;
extern uint16_t PWM_u16CCR;

/* Exported constants --------------------------------------------------------*/
#define PWM_CK_CNT 3000000
#define PWM_TIM_OUTPUT_CLK 50

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void PWM_vInit(void);
void PWM_vSetDCycleCh1(float);

#endif /* __PWM_H */
