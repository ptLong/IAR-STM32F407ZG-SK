/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : Buzzer_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 13/06/2011
* Description        : Main program body
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
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/
extern void SysTickStart(uint32_t Tick);
extern void SysTickStop(void);
/*******************************************************************************
* Function Name  : Buzzer_Test
* Description    : Buzzer_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Buzzer_Test(void)
{

GPIO_InitTypeDef GPIO_InitStructure;
int cntr = 0;

 /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "Buzzer Test\n ");
  /* Enable the BUTTON Clock */
  RCC_AHB1PeriphClockCmd(BUZZER_GPIO_CLK, ENABLE);

  /* Configure Button pin as input */
  GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);

  SysTickStart(2000);

  while(1)
  {
    if( SysTick->CTRL & (1<<16))
    {
      if(2*2000 <= cntr++) break;
      BUZZER_GPIO_PORT->ODR ^= BUZZER_GPIO_PIN; 
    }
    
  }
 
  SysTickStop();

  /**/
 
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    GLCD_print( "Press User key to exit\n ");

  while(ReadKey() != User)
    {
    }

    ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
