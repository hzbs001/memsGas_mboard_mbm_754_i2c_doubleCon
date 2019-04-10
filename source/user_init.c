/**********************************************
≥ı ºªØ ±÷”£¨ø™∆ÙÕ‚…Ë ±÷”£¨∫Õ÷–∂≈‰÷œ
***********************************************/
#include "stm32f0xx.h"
#include "user_init.h"

static NVIC_InitTypeDef NVIC_InitStructure;

void RCC_cfg()
{
	/*******Using HSI = 8MHz and PLL***********/
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_6);//HSI=8MHz,HSI/2*6
	RCC_HSEConfig(RCC_HSE_OFF);
	
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //sysclk=24MHz
	
	RCC_HCLKConfig(RCC_SYSCLK_Div2); 	//Hclk=12MHz
	RCC_PCLKConfig(RCC_HCLK_Div2); 		//Pclk=6MHz
	//peripheral clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | \
												 RCC_APB2Periph_TIM16 | RCC_APB2Periph_TIM17 | \
												 RCC_APB2Periph_SYSCFG,ENABLE);//| RCC_APB2Periph_ADC1

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | \
	                      RCC_AHBPeriph_DMA1,ENABLE);
	
}
void NVIC_cfg()
{
	/*************For infrared receive************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	/*************For modbus***********************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	/*************For Led***********************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

