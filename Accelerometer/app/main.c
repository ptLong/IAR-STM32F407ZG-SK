/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : 
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F407ZG-SK board. It shows basic use of the I/O,
 * the timer, the interrupt controllers and the LDC module and the on board
 * accelerometer
 *
 *    The IAR logo and a cursor are displayed on the LCD . The cursor moves as
 *  the board position is changed.
 *
 *  SRAM Debug - The Progam is loaded to internal RAM.
 *  Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 *  IAR-STM32F407ZG-SK evaluation board:
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *
 *    $Revision: 48478 $
 **************************************************************************/
#include "includes.h"

#define TICK_PER_SEC          25

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

static volatile Int32U TimingDelay;

/*variable for clitical section entry control*/
Int32U CriticalSecCntr;


/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{ 
  TimingDelay = Dly;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#define CURSOR_H_SIZE 12
#define CURSOR_V_SIZE 12
/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
Int16U Cursor[CURSOR_H_SIZE*CURSOR_V_SIZE];
int cursor_x = (GLCD_HORIZONTAL_SIZE - CURSOR_H_SIZE)/2, cursor_y = (GLCD_VERTICAL_SIZE - CURSOR_V_SIZE)/2;
Int32U m;
Int16S X, Y, Z;

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  

  ENTR_CRT_SECTION();
  /* SysTick Config*/
  if(SysTick_Config(SystemCoreClock/10000))
  { 
    /* Capture error */ 
    while (1);
  }
  
  EXT_CRT_SECTION();

  //GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  /*Turn on Backlight*/
  GLCD_Backlight(BACKLIGHT_ON);
  /*Accelerometer init*/
  I2C1_Init();
  if(FALSE == Accl_Init())
  {
    /* Capture error */ 
    while(1);
  }
  /*Read ID*/
  /*Set cursor position*/
  m = (cursor_y+2)|((cursor_y+2+CURSOR_V_SIZE-1)<<8);
  GLCD_SendCmd(RASET,(pInt8U)&m,0);
  m = (cursor_x)|((cursor_x+CURSOR_H_SIZE-1)<<8) ;
  GLCD_SendCmd(CASET,(pInt8U)&m,0);

  STM_EVAL_LEDInit(LED1);
  
  while(1)
  {
    /*Delay*/
    DelayResolution100us(10000/TICK_PER_SEC);
    /*toggle led 1*/
    STM_EVAL_LEDToggle(LED1);
    /*Read data from accelerometer*/
    Accl_Get(&X,&Y,&Z);
    /*Clear cursor*/
    for(int i = 0; i < CURSOR_V_SIZE; i++ )
    {
      GLCD_SendCmd(RAMWR,(pInt8U)((pInt16U)IAR_Logo.pPicStream+(cursor_y+i)*GLCD_HORIZONTAL_SIZE+cursor_x),CURSOR_H_SIZE*2);
    }

    /*Calculate the new cursor position*/
    cursor_x -= X>>6;
    cursor_y += Y>>6;
    /*X boundary correction*/
    if((GLCD_HORIZONTAL_SIZE - CURSOR_H_SIZE ) < cursor_x)
    {
      cursor_x = GLCD_HORIZONTAL_SIZE - CURSOR_H_SIZE;
    }

    if(0 > cursor_x)
    {
      cursor_x = 0;
    }
    /*Y boundary correction*/
    if((GLCD_VERTICAL_SIZE - CURSOR_V_SIZE) < cursor_y)
    {
      cursor_y = (GLCD_VERTICAL_SIZE - CURSOR_V_SIZE);
    }

    if(0 > cursor_y)
    {
      cursor_y = 0;
    }

    /*Draw cursor*/
    for(int i = 0; CURSOR_V_SIZE > i ; i++)
    {
      for(int k = 0; CURSOR_H_SIZE > k ; k++)
      {
        Cursor[i*CURSOR_H_SIZE+k] = ((pInt16U)IAR_Logo.pPicStream)[(cursor_y+i)*GLCD_HORIZONTAL_SIZE+cursor_x+k]^0xFFFF;
      }
    }
    /*Set cursor position*/
    m = (cursor_y+2)|((cursor_y+2+CURSOR_V_SIZE-1)<<8) ;
    GLCD_SendCmd(RASET,(pInt8U)&m,0);
    m = (cursor_x)|((cursor_x+CURSOR_H_SIZE-1)<<8) ;
    GLCD_SendCmd(CASET,(pInt8U)&m,0);
    /*Display cursor*/
    for(int i = 0; i < CURSOR_V_SIZE; i++ )
    {
      GLCD_SendCmd(RAMWR,(pInt8U)(Cursor+i*CURSOR_H_SIZE),CURSOR_H_SIZE*2);
    }
  }
}
