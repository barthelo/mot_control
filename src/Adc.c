/**
  ******************************************************************************
  * @file    Adc.c
  * @author  Eugen Bartel
  * @version V1.0
  * @date    17-April-2013
  * @brief   Provides ADC's firmware functions
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Adc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief ADC and DMA configuration
* @param none
* @retval none
**/
void ADC_vInit(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable ADC1, DMA2 and GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* DMA2 Stream0 channel0 configuration **************************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;//Channelwahl
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//Richtung des DMA Quelle-zu-Ziel
    DMA_InitStructure.DMA_BufferSize = 2;//Anzahl der Daten
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//Erneuter beginn nach Durchlauf von DMA_BufferSize
    
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_u16ConvertedValue;//Speicheradresse im Speicher
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//Datenyp bzw. zu belegende Groesse im Speicher
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_BufferSize mal Druchlauf des Speicher ab DMA_Memory0BaseAddr in DMA_MemoryDataSize_HalfWord Schritten
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//Uebertragung von Daten ohne von Interrupt unterbrochen zu werden
    
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;//Speicherstelle des Inhalts vom periphaeren Geraet
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//zu lesende Datetyp bzw. Groesse
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//inkrementaler Druchlauf beim lesen
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//lesen von Daten ohne von Interrupt unterbrochen zu werden
    
    
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//Prioritaet des Channels
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//Zwischenspeicher aktivieren ja/nein
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//Schwelle fuer Zwischenspeicher
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);//Setzen der Einstellungen
    DMA_Cmd(DMA2_Stream0, ENABLE);//Aktivieren der gesetzten <einstellungen

    /* Configure ADC1 Channel1 - Channel3 pin as analog input ******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC1 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//     ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 2;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel configuration *************************************/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_480Cycles);

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    /* Enable ADC3 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC3 */
    ADC_Cmd(ADC1, ENABLE);
    
}
/**
* @brief After choosing a FQC side its ADC value is returned
* @param ADC_cChopperSide 'l' or 'r' for appropriate side
* @retval ADC_u16ADCValue 12Bit ADC value
**/

uint16_t ADC_u16GetADCValue(char ADC_cChopperSide)
{
    uint16_t ADC_u16ADCValue;
    switch(ADC_cChopperSide)
    {
        case 'l':
            ADC_u16ADCValue=ADC_u16ConvertedValue[0];
            break;
        case 'r':
            ADC_u16ADCValue=ADC_u16ConvertedValue[1];
            break;
    }
    
    return ADC_u16ADCValue;
}

/**
* @brief Calculate voltage out of given ADC value
* @param ADC_u16ADCValue ADC value
* @retval ADC_u16Voltage Calculated volatage
**/

float ADC_fGetVoltage(uint16_t ADC_u16ADCValue)
{
  float ADC_fVoltage;

  /*Calculate voltage out of ADC value*/
  ADC_fVoltage=ADC_u16ADCValue*(ADC_VOLTAGE_REFERENCE/ADC_MAX_RESOLUTION);
  return ADC_fVoltage;
}

/**
* @brief Filtering any kind of signal.
* @note  Only one Signal can be filtered because just two variables to
*        store old values are available.
* @param ADC_fSignal Signal value
* @retval ADC_fFilteredSignal Filtered signal value
**/
float ADC_fFilterSignal(float ADC_fSignal)
{
  static float ADC_fSignal1=0;
  static float ADC_fSignal2=0;
  float ADC_fFilteredSignal;

  /*Filtering signal*/
  ADC_fFilteredSignal=ADC_FILTER_COEFF_1*ADC_fSignal1+ADC_FILTER_COEFF_2*ADC_fSignal2;
  ADC_fSignal1=ADC_fSignal;
  ADC_fSignal2=ADC_fFilteredSignal;

  return ADC_fFilteredSignal;
}
/**
* @brief Calculate current value out of given voltage value
* @param ADC_fVoltage Voltage value
* @retval ADC_fCurrent Current value
**/

float ADC_fGetCurrent(void)
{
  float ADC_fCurrent;
  
  /*Calculate voltage to currentt*/
  ADC_fCurrent=ADC_LOAD_CURRENT_FACTOR*((ADC_fGetVoltage(ADC_u16GetADCValue('l'))-ADC_OFFSET_L)-(ADC_fGetVoltage(ADC_u16GetADCValue('r'))-ADC_OFFSET_R));

  return ADC_fFilterSignal(ADC_fCurrent);
}
