/**
  ******************************************************************************
  * @file    Usart.c
  * @author  Eugen Bartel
  * @version V1.0
  * @date    24-April-2014
  * @brief   This file provides set of firmware functions USART
  ******************************************************************************/
#include "Usart.h"
volatile char StringLoop[] = "The quick brown fox jumps over the lazy dog\r\n";
/**
 * @brief  Initialisiert den USART und den dazugehoerigen Ports
 * @param  none
 * @retval none
 **/
void USART_vInit(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  // Initialisierung der GPIOs
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

 USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  
  // USART Configuration
  USART_InitStructure.USART_BaudRate=115200;
  USART_InitStructure.USART_WordLength=USART_WordLength_8b;
  USART_InitStructure.USART_StopBits= USART_StopBits_1;
  USART_InitStructure.USART_Parity= USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode= USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART3, &USART_InitStructure);
  
  // NVIC Configuration
  NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_Init(&NVIC_InitStructure);
  
  //Activate USART
  USART_Cmd(USART3, ENABLE);
  
}

void USART_vSendChar(uint8_t ch)
{
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
  USART_SendData(USART3, ch);
}

void USART_vSendString(volatile char *s)
{
  while(*s)
  {
    // wait until data register is empty
    while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
    USART_SendData(USART3, *s);
    *s++;
  }
}

void USART_vSendFloatAsString(float value)
{
  char valuestring[20];
  sprintf(valuestring,"%f",value);
  USART_vSendString(valuestring);
  USART_vSendString("\r\n");
}

void USART3_IRQHandler(void)
{
  static int tx_index = 0;
  static int rx_index = 0;
 
  if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) // Transmit the string in a loop
  {
    USART_SendData(USART3, StringLoop[tx_index++]);
    if (tx_index >= (sizeof(StringLoop) - 1))
      tx_index = 0;
  }
 
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // Received characters modify string
  {
    StringLoop[rx_index++] = USART_ReceiveData(USART3);
 
    if (rx_index >= (sizeof(StringLoop) - 1))
      rx_index = 0;
  }
}

