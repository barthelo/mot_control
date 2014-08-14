/**
 * *************************************************************************
 * @file     SpeedEncoder.h
 * @author   Eugen Bartel
 * @version  V1.0
 * @date     May-4-2013
 * @brief    Contains declarations and defines for encoder (HEDS-5540) usage
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_SPEED_GET_H
#define __MOTOR_SPEED_GET_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
/* Private define ------------------------------------------------------------*/
#define	ENC_CPR 1024 //CPR=cycles per revolution
#define ENC_SAMPLE_FREQ 100
#define ENC_CNT_MAX 0xffff
#define ENC_PI 3.14159265359
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void ENC_vInit(void);
uint8_t ENC_ui8GetDir(void);
uint16_t ENC_ui16GetCnt(void);
float ENC_fGetAngle(void);
float ENC_fGetVelocity(void);
float ENC_fGetRPM(void);
float ENC_fGetAcceleration(void);
#endif /* __MOTOR_SPEED_GET_H */
