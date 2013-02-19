/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    1. Date        : September 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    2. Date        : October 2011
 *       Author      : Atanas Uzunov
 *       Description : Ported to STM32F407ZG-SK board
 *
 *   This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F407ZG-SK board. It is an
 * embedded Web Server based on lwIP TCP/IP stack v1.3.2 and features
 * three HTML pages that form a compact and interactive Web Server to
 * interact with IAR-STM32F407ZG-SK board.
 *  You will be able to address the IAR-STM32F407ZG-SK board as a web 
 * page using your web browser, as well as controlling the four LEDs on 
 * the board through the browser and finally you will be able to get 
 * continuously, each 1 second, the ADC1 Channel 10 (onboard potentiometer)
 * converted value displayed  on a status bar also in the web page of 
 * your browser. The project can be configured to use static or dynamic 
 * IP (see main.h). The address that is selected will be shown on the 
 * onboard LCD display. Default IP for the project is 192.168.0.200
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

#define DLY_100US             1000

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

// variable for critical section entry control
uint32_t CriticalSecCntr;

volatile uint32_t timeCounter=0;

// ETHERNET STATUS 
#define ETHERNET_SUCCESS    0
#define ETHERNET_PHY_ERROR  1
#define ETHERNET_INIT_ERROR 2

/*************************************************************************
 * Function Name: Ethernet_Configure
 * Parameters: void
 * Return: uint32_t (configuration result)
 *
 * Description: Configures the Ethernet Module
 *		
 *************************************************************************/
uint32_t Ethernet_Configure(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
ETH_InitTypeDef ETH_InitStructure;

  // ETHERNET pins configuration
  // AF Output Push Pull:
  //- ETH_RMII_MDIO: PA2
  //- ETH_RMII_MDC: PC1
  //- ETH_RMII_MDINT: PA3
  //- ETH_RMII_TX_EN: PB11
  //- ETH_RMII_TXD0: PG13
  //- ETH_RMII_TXD1: PG14
  //- ETH_RMII_PPS_OUT: PB5    
  //- ETH_RMII_REF_CLK: PA1
  //- ETH_RMII_CRS_DV: PA7
  //- ETH_RMII_RXD0: PC4
  //- ETH_RMII_RXD1: PC5

  // Configure PA1, PA2, PA3, PA7 as alternate function
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

  // Configure PB11 as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);

  // Configure PG13, PG14 as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
  
  // Configure PC1, PC4, PC5 as alternate function
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
  
  // Enable SYSCFG clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  // Set RMII mode
  SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
  
  // Reset ETHERNET on AHB Bus
  ETH_DeInit();
  
  // Software reset
  ETH_SoftwareReset();
  
  // Wait for software reset
  while(ETH_GetSoftwareResetStatus()==SET); 
  
  // ETHERNET Configuration
  // Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter
  ETH_StructInit(&ETH_InitStructure);
  
  // Fill ETH_InitStructure parametrs
  //------------------------   MAC   -----------------------------------
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable  ;                                       
  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;                                                                                               
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;                                                                                  
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;                                                                                                                                                                        
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;                                                                                                       
  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;      
  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;                                                             
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;      
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;                        
  ETH_InitStructure.ETH_Speed = ETH_Speed_100M;                        
#ifdef CHECKSUM_BY_HARDWARE
  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif
  //------------------------   DMA   -----------------------------------  
  // When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
  // the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
  // if the checksum is OK the DMA can handle the frame otherwise the frame is dropped 
  
  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;         
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;       
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;   
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;      
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;          
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
  
  // Find PHY Address  
  unsigned int PhyAddr;
  for(PhyAddr = 1; 32 >= PhyAddr; PhyAddr++)
  { 
    union {
      Int32U    HI_LO;
      struct
      {
        Int16U  LO;
        Int16U  HI;
      };
    } PHYID;
    
    PHYID.HI = ETH_ReadPHYRegister(PhyAddr,2);
    PHYID.LO = ETH_ReadPHYRegister(PhyAddr,3) & 0xFFF0;
    
    if((0x00061c50 == PHYID.HI_LO) || (0x02038460 == PHYID.HI_LO)) break;
  }
  
  // if PHY address not found
  if(32 < PhyAddr)
  {  
    return ETHERNET_PHY_ERROR;
  }
  
  // Initialize Ethernet
  if(0 == ETH_Init(&ETH_InitStructure, PhyAddr))
  {
    return ETHERNET_INIT_ERROR;    
  }        
  return ETHERNET_SUCCESS;
}

/*************************************************************************
 * Function Name: ADC_Configure
 * Parameters: void
 * Return: void
 *
 * Description: Configures the ADC Module
 *		
 *************************************************************************/
void ADC_Configure(void)
{
ADC_InitTypeDef       ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
GPIO_InitTypeDef      GPIO_InitStructure;
  
  // Enable ADC1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
  
  // Configure PC.0 (ADC Channel10) as analog input 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  // ADC Common Initialization
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; 
  ADC_CommonInit(&ADC_CommonInitStructure); 

  // ADC1 Configuration
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // ADC1 Regular Channel Configuration
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_84Cycles);

  // Enable ADC1
  ADC_Cmd(ADC1, ENABLE);

  // ADC1 Software Start Conversion 
  ADC_SoftwareStartConv(ADC1);
}

/*************************************************************************
 * Function Name: SysTickHandler
 * Parameters: void
 * Return: void
 *
 * Description: SysTick interrupt handler
 *		
 *************************************************************************/
void SysTickHandler(void)
{
  timeCounter+=10;
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
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
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
void main(void)
{
uint32_t ethResult;
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  

  ENTR_CRT_SECTION();
              
  // Enable ETHERNET and GPIOs clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                         RCC_AHB1Periph_ETH_MAC_Rx | RCC_AHB1Periph_ETH_MAC_PTP | 
                         RCC_AHB1Periph_GPIOA |	
                         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

  
  // SysTick end of count event each 0.01s with input clock equal to 15MHz (HCLK/8, default)
  SysTick_Config(150000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  
  EXT_CRT_SECTION();
  
  // Status LEDs port init
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4); 

  // Turn off LEDs
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
          
  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(9,105,131,131);
            
  // ADC configuration
  ADC_Configure();
  
  // Ethernet configuration
  ethResult = Ethernet_Configure(); 
  GLCD_TextSetPos(0,0);  
  switch(ethResult) 
  {
    case ETHERNET_SUCCESS : 
      GLCD_print("\flwIP WEB Server\n\r");           
      break;
      
    case ETHERNET_PHY_ERROR :
      GLCD_print("\fEth PHY Error\r");      
      while(1);;
    
    case ETHERNET_INIT_ERROR :
      GLCD_print("\fEth Init Error\r");      
      while(1);
      
    default : break;  
  }
    
  // Initilaize the LwIP stack
  LwIP_Init();  
  
#ifndef USE_DHCP  
  // Show the IP address on LCD  
char s[20];
  sprintf(s,"ip:%d.%d.%d.%d", \
           IP_ADDR0, IP_ADDR1,\
           IP_ADDR2, IP_ADDR3);
  GLCD_print(s);    
#else      
  GLCD_print("Obtaining IP...");
#endif
  
  // Http webserver init
  httpd_init(); 
  
  // Main loop
  while (1)
  {  
    // check if any packet received
    if (ETH_CheckFrameReceived())
    { 
      // process received ethernet packet
      LwIP_Pkt_Handle();
    }
    // handle periodic timers for LwIP
    LwIP_Periodic_Handle(timeCounter);
  }
}
 