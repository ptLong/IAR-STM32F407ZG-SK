/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : LED_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : LED_Test program body
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
#include "iar_stm32f407zg_sk.h"
#include "menu.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Led_TypeDef Led[4]={LED1, LED2, LED3, LED4};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : LED_Test
* Description    : LED_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LED_Test(void)
{
  unsigned long n;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "LED Test\n ");

  /* Initialize led status */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Turn On  all LEDs */
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  GLCD_TextSetPos(1,3);
  GLCD_print( "Turn On LEDs \n ");
  DelayResolution100us(Dly);

  for (n = 0; n < 4; n++)
   {
  /* Turn Off one LED  */
    STM_EVAL_LEDOff(Led[n]);
    GLCD_TextSetPos(1,3);
    GLCD_print( "Turn Off each LED\n ");
    DelayResolution100us(Dly-2000);
   }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    GLCD_print( "Press User key to exit\n ");
  while(ReadKey() != User)
    {
    }

  ReturnFunc();

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);

}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
