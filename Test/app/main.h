/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : MAIN.H
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Main header file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "menu.h"
#include "includes.h"

/* Define */
/* Setup functions */
extern void Temp_Sensor_Test(void);
extern void Ext_SRAM_Test(void);
extern void LCD_Test(void);
extern void Buzzer_Test(void);
extern void Button_Test(void);
extern void LED_Test(void);
extern void Accelerometer_Test(void);
extern void Stepper_Motor_Test(void);
extern void SD_Card_Test(void);
extern void Potentiometer_Test(void);
extern void USB_Test(void);
extern void RS232_1_Test(void);
extern void CAN_Test(void);
extern void USB_Mouse(void);
extern void Audio_Test(void);
extern void USBOTG_Test(void);
#endif /* __MAIN_H */
