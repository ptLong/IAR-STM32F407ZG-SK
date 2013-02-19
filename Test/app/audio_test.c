/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : audio_test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 13/06/2011
* Description        : audio_test program body
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
#define PI (float)3.14159
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void SysTickStart(uint32_t Tick);
extern void SysTickStop(void);
/*******************************************************************************
* Function Name  : Audio_Test
* Description    : Button_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Audio_Test(void)
{
I2S_InitTypeDef I2S_InitStructure; 
GPIO_InitTypeDef GPIO_InitStructure;

int cntr = 0;
int index = 0;
int data;
int volume_r, volume_l;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

   /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(6,1);
  GLCD_print( "Audio Test\n ");
  GLCD_TextSetPos(1,3);
  GLCD_print( "Please connect your\n ");
  GLCD_TextSetPos(3,4);
  GLCD_print( "Headphone \n ");
  
  /*PLLI2S configure*/
  RCC_PLLI2SConfig(271,2);
  /*Enable PLLI2S*/
  RCC_PLLI2SCmd(ENABLE);
  /*Wait PLLI2S Lock*/
  while(RESET == RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY));
  /*PLLI2S is I2S clock source*/
  RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
  /* Enable I2S3 APB1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  /* Deinitialize SPI2 peripheral */
  SPI_I2S_DeInit(SPI3); 

	/* I2S2 peripheral configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_24b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_44k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;  
  I2S_Init(SPI3, &I2S_InitStructure);

  /* Disable the I2S2 TXE Interrupt */ 
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, DISABLE);
 
  /*Pin Configure*/
  /* Enable GPIOs clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* Configure PA15*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure PA15*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_SPI3);

  /* Configure PB3 and PB5*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Connect PB3 and PB5 I2S3 module*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
  /* Configure PC7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Connect PC7 I2S3 module*/
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
  /* Enable the SPI2/I2S2 peripheral */
  I2S_Cmd(SPI3, ENABLE);
  /*Systick start*/
  SysTickStart(20);
  
  GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
  GLCD_TextSetPos(0,8);
  GLCD_print( "                      \n ");
  GLCD_TextSetPos(0,9);
  GLCD_print( "Press User key to exit\n ");

 while(ReadKey() != User)
  {
    if(SysTick->CTRL & (1<<16))
    {
      cntr++;
      if(20 > cntr)
      {
        volume_r = ((1<<13)-1);
        volume_l = 0;
      }
      else
      {
        if(25 > cntr)
        {
          volume_l = 0;
          volume_r = 0;
        }
        else
        {
          if(45 > cntr)
          {
            volume_l = ((1<<13)-1);
            volume_r = 0;
          }
          else
          {
            if(50 > cntr)
            {
               volume_l = 0;
               volume_r = 0;
            }
            else cntr = 0;
          }
        }
          
      }
    }
    index+= SAMPLES_NUM * 1000/I2S_AudioFreq_44k;
    data = Sin_Table[(SAMPLES_NUM - 1) & index]*volume_l;
    while(RESET == SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI3,(data>>8) & 0xFFFF);
    while(RESET == SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI3,(data<<8) & 0xFFFF);
    data = Sin_Table[(SAMPLES_NUM - 1) & (index+SAMPLES_NUM/4)]*volume_r;
    while(RESET == SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI3,(data>>8) & 0xFFFF);
    while(RESET == SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI3,(data<<8) & 0xFFFF);   
  }

  ReturnFunc();
  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 20011 STMicroelectronics *****END OF FILE****/