/**
  ******************************************************************************
  * @file    PIControl.c
  * @author  Eugen Bartel
  * @date    17-April-2013
  * @brief   Provides PI-Controllers firmware functions
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PIControl.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Setting up the structure's constants
 * @param  *Conrtoller Pointer to structure
 * @param  PIC_fKI Integration multiplier
 * @param  PIC_fKP Proportianal multiplier
 * @param  PIC_fUpperLimit Upper limit for saturation
 * @param  PIC_fLowerLimit Lower limit for saturation
 * @param  PIC_fKb Multiplier for back calculation within AntiWindUp feature
 * @param  PIC_fTs Sampling time for integration
 * @retval none
 **/
void PIC_vConnstructor(PIC* Controller,float PIC_fKI, float PIC_fKP, float PIC_fUpperLimit, float PIC_fLowerLimit, float PIC_fKb, float PIC_fTs)
{
  Controller->PIC_fKI=PIC_fKI;
  Controller->PIC_fKP=PIC_fKP;
  Controller->PIC_fUpperLimit=PIC_fUpperLimit;
  Controller->PIC_fLowerLimit=PIC_fLowerLimit;
  Controller->PIC_fKb=PIC_fKb;
  Controller->PIC_fTs=PIC_fTs;
  
  Controller->PIC_fArea0=0;
  Controller->PIC_fArea1=0;
  Controller->PIC_fIntgVal0=0;
  Controller->PIC_fIntgVal1=0;
  Controller->PIC_fDiff=0;
  Controller->PIC_fCommandValue=0;
}

/**
 * @brief  Setting up the structure's constants
 * @param  *Conrtoller Pointer to structure
 * @retval none
 **/
float PIC_fCalcCommand(PIC* Controller, float PIC_fInputValue, float PIC_fFeedbackValue)
{
  /*Calculatin diff*/
  Controller->PIC_fDiff=PIC_fInputValue-PIC_fFeedbackValue;
  /*Integral's input*/
  Controller->PIC_fIntgVal0=Controller->PIC_fDiff*Controller->PIC_fKI+PIC_fAntiWindUpBackCalc(Controller);
  /*Integration*/
  PIC_vCalcIntegrationValue(Controller);
  /*Pre Command Value*/
  Controller->PIC_fValKPKI=Controller->PIC_fDiff*Controller->PIC_fKP+Controller->PIC_fArea1;
  /*Calculate command value*/
  if(Controller->PIC_fValKPKI>Controller->PIC_fUpperLimit)
  {
    Controller->PIC_fCommandValue=Controller->PIC_fUpperLimit;
  }
  else if(Controller->PIC_fValKPKI<Controller->PIC_fUpperLimit)
  {
    Controller->PIC_fCommandValue=Controller->PIC_fLowerLimit;
  }
  else
  {
    Controller->PIC_fCommandValue=Controller->PIC_fValKPKI;
  }
  
  return Controller->PIC_fCommandValue;
}

/**
 * @brief  Integration procedure
 * @param  *Conrtoller Pointer to structure
 * @param  none
 * @retval none
 **/
void PIC_vCalcIntegrationValue(PIC* Controller)
{
  Controller->PIC_fArea0=(Controller->PIC_fIntgVal0+Controller->PIC_fIntgVal1)*Controller->PIC_fTs*0.5;
  Controller->PIC_fIntgVal1=Controller->PIC_fIntgVal0;
  
  Controller->PIC_fArea1=Controller->PIC_fArea0+Controller->PIC_fArea1;
}

/**
 * @brief  Compute back calculation value
 * @param  *Conrtoller Pointer to structure
 * @retval PIC_fBackCalcDiff AntiWindUp's back calculation value
 **/
float PIC_fAntiWindUpBackCalc(PIC* Controller)
{
  return (Controller->PIC_fCommandValue-Controller->PIC_fValKPKI)*Controller->PIC_fKb;

}
