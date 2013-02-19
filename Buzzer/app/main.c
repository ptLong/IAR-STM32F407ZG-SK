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
 * the timer, the interrupt controllers and the LDC module
 *
 *    Press TAMPER button and the Buzzer will make a sound.
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

#define BUZZER_FREQ   (2000)

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
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
RCC_ClocksTypeDef RCC_Clocks;
int pwm_state = 0;

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

  /*TAMPER button init*/
  STM_EVAL_PBInit(BUTTON_TAMPER,BUTTON_MODE_GPIO);
  
  /* Enable the BUZZER_GPIO Clock */
  RCC_AHB1PeriphClockCmd(BUZZER_GPIO_CLK, ENABLE);

  /* Configure Buzzer pin*/
  GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
  /*Select AF*/
  GPIO_PinAFConfig(BUZZER_GPIO_PORT,BUZZER_PIN_SOURCE,BUZZER_PIN_AF);
  /* Buzzer Timer enable*/
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_BUZZER_TIM,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2PERIPH_BUZZER_TIM,DISABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_BUZZER_TIM, ENABLE);

  RCC_GetClocksFreq(&RCC_Clocks);
  /*Time base configuration*/
  TIM_TimeBaseStructure.TIM_Period = 0xFF;          /*8 bit resolution*/
  TIM_TimeBaseStructure.TIM_Prescaler = RCC_Clocks.HCLK_Frequency/(256*BUZZER_FREQ); /*2kHz PWM periotd*/
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(BUZZER_TIM,&TIM_TimeBaseStructure);

  /*Output Compare init*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  /*50%*/
  TIM_OCInitStructure.TIM_Pulse = 0x80;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC3Init(BUZZER_TIM,&TIM_OCInitStructure);
  TIM_ARRPreloadConfig(BUZZER_TIM,ENABLE);
  /*Timer counter enable*/
  TIM_Cmd(BUZZER_TIM,ENABLE);
  /*Enables the TIM peripheral Main Outputs*/
  TIM_CtrlPWMOutputs(BUZZER_TIM,ENABLE);
  while(1)
  {
    if (Bit_RESET == STM_EVAL_PBGetState(BUTTON_TAMPER))
    {
      if(0 == pwm_state)
      {
        pwm_state = 1;
        /*Turn off LCD Backlight*/
        GLCD_Backlight(BACKLIGHT_OFF);
        /*Enabel PWM Output*/
        TIM_CCxCmd(BUZZER_TIM,TIM_Channel_3,TIM_CCx_Enable);
      }
    }
    else
    {
      if(1 == pwm_state)
      {
        pwm_state = 0;
        /*Turn On LCD Backlight*/
        GLCD_Backlight(BACKLIGHT_ON);
        /*Delay*/
        TIM_CCxCmd(BUZZER_TIM,TIM_Channel_3,TIM_CCx_Disable);
      }
    }
  }
}
