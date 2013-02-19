/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : Potentiometer_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 13/06/2011
* Description        : Potentiometer_Test program body
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
ADC_InitTypeDef   ADC_InitStructure;
DMA_InitTypeDef   DMA_InitStructure;
vu16 ADC_RegularConvertedValueTab[64], ADC_InjectedConvertedValueTab[32];
volatile unsigned long ADCConvertedValue;
volatile Boolean CntrSel = FALSE;

/* Private function prototypes -----------------------------------------------*/
void DrawTable (void);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Potentiometer_Test
* Description    : Potentiometer_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Potentiometer_Test(void)
{

ADC_CommonInitTypeDef ADC_CommonInitStruct;
ADC_InitTypeDef ADC_InitStructure;
Boolean SelHold;
u32 AdcData;
GPIO_InitTypeDef GPIO_InitStructure;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);


  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "Potentiometer Test\n ");
  
  // GPIO enable clock and release Reset
   RCC_AHB1PeriphClockCmd(  TRIMER_CLK , ENABLE);

  GPIO_InitStructure.GPIO_Pin =  TRIMER_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TRIMER_PORT, &GPIO_InitStructure);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

  ADC_CommonStructInit(&ADC_CommonInitStruct);
  ADC_CommonInit(&ADC_CommonInitStruct);                         
  // ADC2 configuration
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC2, &ADC_InitStructure);

  // ADC2 regular channel10 configuration
  ADC_RegularChannelConfig(ADC2, TRIMER_CHANNEL, 1, ADC_SampleTime_56Cycles);

  // Disable ADC2 DMA
  ADC_DMACmd(ADC2, DISABLE);

  // Enable ADC2
  ADC_Cmd(ADC2, ENABLE);

   /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConv(ADC2);
  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  GLCD_TextSetPos(1,3);
  GLCD_print("Move the potentiometer");
  GLCD_TextSetPos(1,4);
  GLCD_print ("to adjust the contrast.");

  DrawTable();

  while(ReadKey() != User)
    {
      /* Get new sample of the ADC */
      AdcData = ADC_GetConversionValue(ADC2) >> 6;
      ADC_SoftwareStartConv(ADC2);
      if(SelHold)
      {
        /* Contract adj */
        AdcData += 0x38;
        GLCD_SendCmd(SETCON,(pInt8U)&AdcData,0);
      }
      else
      {
        /* Backlight adj */
        AdcData += 0x40;
        GLCD_Backlight(AdcData);
      }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    GLCD_print( "Press User key to exit\n ");
    }

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
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
u32 x,y,j,k;

// xxxxRRRRGGGGBBBB
const u32 TableColor [] =
{
  0xFFF, 0xAAA, 0x888, 0x666, 0x444, 0x000,   // Grey
  0x00F, 0x00A, 0x008, 0x006, 0x004, 0x001,   // Blue
  0xF00, 0xA00, 0x800, 0x600, 0x400, 0x100,   // Red
  0x0F0, 0x0A0, 0x080, 0x060, 0x040, 0x010,   // Green

};

//  for(y = 0; y < 4; ++y)
//  {

y=3;
    for(x = 0; x < 6; ++x)
    {
      // set rectangle
      k = (x*22) | ((((x+1)*22)-1)<<8);
      GLCD_SendCmd(CASET,(pInt8U)&k,0);
      k = (y*29) | ((((y+1)*29)-1)<<8);
      GLCD_SendCmd(RASET,(pInt8U)&k,0);
      // fill rectangle
      for(j = 0; j < (22*16); ++j)
      {
        GLCD_SendCmd(RAMWR,(pInt8U)&TableColor[x+(y*6)],2);
      }
//    }
  }
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
