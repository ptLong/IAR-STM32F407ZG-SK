########################################################################
#
#                           lwipWebserver.eww
#
# $Revision: 50541 $
#
########################################################################

DESCRIPTION
===========  
   This example project shows how to use the IAR Embedded Workbench
 for ARM to develop code for the IAR-STM32F407ZG-SK board. It is an
 embedded Web Server based on lwIP TCP/IP stack v1.3.2 and features
 three HTML pages that form a compact and interactive Web Server to
 interact with IAR-STM32F407ZG-SK board.
  You will be able to address the IAR-STM32F407ZG-SK board as a web 
 page using your web browser, as well as controlling the four LEDs on 
 the board through the browser and finally you will be able to get 
 continuously, each 1second, the ADC Channel 10 converted value displayed 
 on a status bar also in the web page of your browser. The project can be
 configured to use Static IP or DHCP. The address that is selected will 
 be shown on the onboard LCD display. Default IP for the project is
 192.168.0.200

COMPATIBILITY
=============
    The example project is compatible with IAR-STM32F407ZG-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
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
     \STM32F4xx\IAR-STM32F407ZG-SK\Webserver\lwipWebserver.eww

  3) Power the board and connect the J-Link.
 
  4) Press CTRL+D or use Download and Debug button to start a debug session.
