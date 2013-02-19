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
 * to develop code for the IAR-STM32F407ZG-SK board. It shows basic use of
 * the I/O, the timer, the interrupt controller and the LDC module for graphic
 * and text drawing.
 *   IAR logo is displayed on the LCD and after 3 second a test screen is drawn.
 * 
 *   Controls:
 *  AN_TR                    - contrast / backlight
 *  Left Joystik position    - adjust backlight
 *  Right Joystik position   - adjust LCD contrast
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

/*************************************************************************
 * Function Name: DrawTable
 * Parameters: none
 *
 * Return: none
 *
 * Description: Show color table on the GCLCD
 *
 *************************************************************************/
void DrawTable (void)
{
Int32U x,y,j,k;

// xxxxRRRRGGGGBBBB
const Int32U TableColor [] =
{
  0xFFF, 0xAAA, 0x888, 0x666, 0x444, 0x000,   // Grey
  0x00F, 0x00A, 0x008, 0x006, 0x004, 0x001,   // Red
  0x0F0, 0x0A0, 0x080, 0x060, 0x040, 0x010,   // Green
  0xF00, 0xA00, 0x800, 0x600, 0x400, 0x100,   // Blue
};

  for(y = 0; y < 4; ++y)
  {
    for(x = 0; x < 6; ++x)
    {
      // set rectangle
      k = (x*22) | ((((x+1)*22)-1)<<8);
      GLCD_SendCmd(CASET,(pInt8U)&k,0);
      k = (y*29) | ((((y+1)*29)-1)<<8);
      GLCD_SendCmd(RASET,(pInt8U)&k,0);
      // fill rectangle
      for(j = 0; j < (22*29); ++j)
      {
        GLCD_SendCmd(RAMWR,(pInt8U)&TableColor[x+(y*6)],2);
      }
    }
  }
}

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
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
Boolean CntrSel = 0;
Int32U AdcData;
int cntr;

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
  
  /*Init LEDs*/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /*Joystick Buttons init*/
  STM_EVAL_PBInit(BUTTON_RIGHT,BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT,BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP,BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN,BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL,BUTTON_MODE_GPIO);
  
  /*TRIMER  Init*/
  RCC_AHB1PeriphClockCmd(TRIMER_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin = TRIMER_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TRIMER_PORT, &GPIO_InitStructure);
  /*ADC Init*/
  /*Enable ADC Clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

 // ADC_CommonStructInit(&ADC_CommonInitStruct);
 // ADC_CommonInit(&ADC_CommonInitStruct);                         
  /*ADC2 configuration*/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_6b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC2, &ADC_InitStructure);

  /*ADC2 regular TRIMER_CHANNEL configuration*/
  ADC_RegularChannelConfig(ADC2, TRIMER_CHANNEL, 1, ADC_SampleTime_56Cycles);
  /*Disable ADC2 DMA*/
  ADC_DMACmd(ADC2, DISABLE);
  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);
  /*Software Start ADC2 Conversion*/
  ADC_SoftwareStartConv(ADC2);

  /* GLCD init */
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);

  GLCD_Backlight(BACKLIGHT_ON);
  /*3 seconds delay*/
  DelayResolution100us(30000);
  DrawTable();

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,116,131,131);
  GLCD_TextSetPos(0,0);

  if(CntrSel)
  {
    GLCD_print("\fContrast adj.\r");
  }
  else
  {
    GLCD_print("\fBacklight adj.\r");
  }

  while(1)
  {
    DelayResolution100us(1000);
    
    STM_EVAL_LEDOff((Led_TypeDef)(cntr & 0x03));

    STM_EVAL_LEDOn((Led_TypeDef)(++cntr & 0x03));

    if(Bit_SET == STM_EVAL_PBGetState(BUTTON_RIGHT))
    {
      CntrSel = TRUE;
      GLCD_print("\fContrast adj.\r");
    }
    else if (Bit_SET == STM_EVAL_PBGetState(BUTTON_LEFT))
    {
      CntrSel = FALSE;
      GLCD_print("\fBacklight adj.\r");
    }

    AdcData = ADC_GetConversionValue(ADC2)>>1;
    ADC_SoftwareStartConv(ADC2);

    if(TRUE == CntrSel)
    {
        // Contract adj
        AdcData += 0x18;
        GLCD_SendCmd(SETCON,(pInt8U)&AdcData,0);
    }
    else
    {
        // Backlight adj
        AdcData += 0x40;
        GLCD_Backlight(AdcData);
    }
  }
}
