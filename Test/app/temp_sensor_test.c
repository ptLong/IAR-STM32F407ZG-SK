/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : Tem_Sensor_Test.c
* Author             : (c) Copyright IAR Systems 2008
* Version            : V1.0.0
* Date               : 13/06/2011
* Description        : Temp_Sensor_Test program body
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
#include "temp_sensor_drv.h"
#include "i2c1_drv.h"


/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Temp_Sensor_Test
* Description    : Temp_Sensor_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Temp_Sensor_Test(void)
{

Flo32 Temp, TempHold = 200.0;
Boolean Alarm;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "Temp Sensor Test\n ");

  /* GPIO Configuration */
  STM_EVAL_LEDInit(LED1);

 

  /* I2C1 init */
  I2C1_Init();

  /* Init Temperature sensor */
  if(   FALSE == TempSensor_Init()
     || FALSE == TempSensorShutdown(FALSE)
     || FALSE == TempSensor_Conf(31.0,30.0,TEMP_SENOSR_COMP_MODE,2))
  {
    /* Initialization fault */
    GLCD_TextSetPos(4,4);
    GLCD_print("Temp.Sensor Init.\r\nFailed\r\n");
  
  }
  while(ReadKey() != User)
    {
      if(TempSensorGetTemp(&Temp,&Alarm))
      {
        if(Alarm)
        {
          /* turn on Status LED1 */
         STM_EVAL_LEDOn(LED1);
        }
        else
        {
          /* turn off Status LED1 */
          STM_EVAL_LEDOff(LED1);
        }
        if(TempHold != Temp)
        {
          TempHold = Temp;
          GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
          GLCD_TextSetPos(1,4);
          GLCD_print("Temperature: %3.1fC",Temp);
        }
     }
      else
      {
        /* Initialization fault */
        GLCD_TextSetPos(4,4);
        GLCD_print("\fTemp.Sensor reading\r\nFailed\r\n");
        
      }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    GLCD_print( "Press User key to exit\n ");
    }



  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);

  /* TransferStatus2 = PASSED, if the specified sector part is erased */
  /* TransferStatus2 = FAILED, if the specified sector part is not well erased  */
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
