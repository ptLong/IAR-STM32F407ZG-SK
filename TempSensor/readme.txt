########################################################################
#
#                           TempSensor.eww
#
# $Revision: 50541 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STM32F407ZG-SK board. It shows basic use of the 
  parallel I/O,the SysTick timer,the interrupt controller,the interface to 
  Nokia Graphic color LCD module and the I2C temperature sensor.
   Measured/readed temperature is shown on the LCD display below the IAR logo. Each 
  time the temperature is measured/readed, the ReadTemp LED (LED1) lights up 
  for 100ms. The alarm (LED3) activates when temperature is over 31.0°C and deactivates 
  when it drop below 30.0°C. User can swith the measurement units (Celsius/Fahrenheit) 
  alternatively, by pressing USER button at the board.

COMPATIBILITY
=============
   The temperature sensor example project is compatible with IAR-STM32F407ZG-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   
  SRAM Debug - The Progam is loaded to internal RAM.
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F407ZG-SK evaluation board:

   Jumpers:
    PWR_SEL - depending of power source
    B0_1/B0_0 - B0_0
    B1_1/B1_0 - B1_0

  The LCD and the Trace module have shared pins (LCD_RST and TRACED2).
  If you need to use the LCD and the Trace simultaneously please set 
  the Trace port width to 2 in the ETM settings. 

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\ST\
     \STM32F4xx\IAR-STM32F407ZG-SK\TempSensor\TempSensor.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.
 
  5) Press CTRL+D or use Download and Debug button to start a debug session.
