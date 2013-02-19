/********************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
 /* ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    13/06/2011
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "menu.h"
#include <string.h>
#include <yfuns.h>
#include "includes.h"

#define DLY_100US  1000

Int32U CriticalSecCntr;
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int MyLowLevelGetchar(void);
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize);
size_t __read(int handle, unsigned char * buffer, size_t size);
void Joystick_Init (void);

/* Private functions ---------------------------------------------------------*/
/*-------------SysTickStart---------------------------------------------------*/
void SysTickStart(uint32_t Tick)
{
RCC_ClocksTypeDef Clocks;
volatile uint32_t dummy;

  RCC_GetClocksFreq(&Clocks);

  dummy = SysTick->CTRL;  
  SysTick->LOAD = (Clocks.HCLK_Frequency/8)/Tick;
  
  SysTick->CTRL = 1;
}
/*-------------SysTickStop----------------------------------------------------*/
void SysTickStop(void)
{
    SysTick->CTRL = 0;
}
/*----------------------------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
 int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();
  
  /* GLCD init */
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  DelayResolution100us(5000);

  GLCD_PowerUpInit((pInt8U)STM32_Logo.pPicStream);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_TextSetPos(4,6);
  GLCD_print("STM32F407ZG-SK");
  GLCD_Backlight(BACKLIGHT_ON);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
  /* Menu init */
  Menu_Init();
 
  Joystick_Init();
 
   
  DisplayMenu();
  
   
  /* Infinite loop */
  while (1)
  {
  }	
}

/*******************************************************************************
* Function Name  : Joystick_Init
* Description    : Joystick_Init.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Joystick_Init (void)
{

   /*Init Buttons*/
  STM_EVAL_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);

    

  IntExtOnOffConfig(ENABLE);
}



/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dely)
{
  for(; Dely; Dely--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/*************************************************************************
 * Function Name: __write
 * Parameters: Low Level cahracter output
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
size_t nChars = 0;

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET);
    USART_SendData(EVAL_COM1, * Buf++);
    ++nChars;
  }
  return nChars;
}
/*************************************************************************
 * Function Name: __read
 * Parameters: Low Level cahracter input
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __read(int handle, unsigned char * buffer, size_t size)
{
  int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelGetchar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
}

static int MyLowLevelGetchar(void)
{
int ch;
unsigned int status = EVAL_COM1->SR;

  if(status & USART_FLAG_RXNE)
  {
    ch = USART_ReceiveData(EVAL_COM1);
    if(status & (USART_FLAG_ORE | USART_FLAG_PE | USART_FLAG_FE) )
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff );
  }
  return -1;
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
