/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : RS232-1_test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 13/06/2011
* Description        : RS232_1_Test program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "menu.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : USART1_Test
* Description    : USART1_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RS232_1_Test(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 USART_InitTypeDef USART_InitStructure;

  /* Disable the JoyStick interrupts */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "RS232-1 Test\n ");
  
   /*Confuture COM1 TX Pins */  
  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_TX_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);
  
  /*Confuture COM1 RX Pins */
  
  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(EVAL_COM1_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);

  
  /* Print messages on GLCD*/
  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  GLCD_TextSetPos(1,2);
  GLCD_print ("-Hyperterminal config:\n");
  GLCD_TextSetPos(1,3);
  GLCD_print ("-BaudRate=115200 baud\n");
  GLCD_TextSetPos(1,4);
  GLCD_print ("-Word Length = 8 Bits\n");
  GLCD_TextSetPos(1,5);
  GLCD_print ("-One Stop Bit \n");
  GLCD_TextSetPos(1,6);
  GLCD_print ("-No parity\n");
  GLCD_TextSetPos(1,7);
  GLCD_print ("Insert RS232 cable\n");
  GLCD_TextSetPos(0,8);
  GLCD_print ("Print A in Hyperterminal\n");
  GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);

  ser_printf(USART6,'A');
  
  
  if (ser_scanf(USART6) != 'A')
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(1,9);
    GLCD_print ("RS232-1 Error!       ");
  }
  else

  {
    GLCD_SetFont(&Terminal_9_12_6,0x10,0xFFF);
    GLCD_TextSetPos(1,9);
    GLCD_print ("RS232-1 OK!         ");
  }

  GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
  GLCD_TextSetPos(0,10);
  GLCD_print( "Press User key to exit\n ");

  while(ReadKey() != User)
    {
    }

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
