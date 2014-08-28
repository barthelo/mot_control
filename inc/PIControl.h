/**
 * *************************************************************************
 * @file     PIControl.h
 * @author   Eugen Bartel
 * @date     August-14-2014
 * @brief    Contains declarations and defines for PIController usage
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PIControl_H
#define __PIControl_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct PIC PIC;
struct PIC{
  /*Control constants*/
  float PIC_fKI;
  float PIC_fKP; 
  
  /*AntiWindUp constants*/
  float PIC_fUpperLimit;
  float PIC_fLowerLimit;
  float PIC_fKb;
  
  /*Integration*/
  float PIC_fArea0;
  float PIC_fArea1;
  float PIC_fIntgVal0;
  float PIC_fIntgVal1;
  float PIC_fTs;
  
  /*Internal integration Vars*/
  float PIC_fDiff;
  float PIC_fCommandValue;
  float PIC_fValKPKI;
};
/* Exported functions --------------------------------------------------------*/
void PIC_vConstructor(PIC* Controller,float PIC_fKI, float PIC_fKP, float PIC_fUpperLimit, float PIC_fLowerLimit, float PIC_fKb, float PIC_fTs);
float PIC_fCalcCommand(PIC* Controller,float PIC_fInputValue,float PIC_fFeedbackValue);
void PIC_vCalcIntegrationValue(PIC* Controller);
float PIC_fAntiWindUpBackCalc(PIC* Controller);

void TIMM_vInit(void);

#endif /* __PIControl_H */
