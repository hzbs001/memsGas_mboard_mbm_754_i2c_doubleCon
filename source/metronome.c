/**********************************************
Ω⁄≈ƒøÿ÷∆≈‰÷√
***********************************************/
#include "stm32f0xx.h"
#include "metronome.h"

static GPIO_InitTypeDef GPIO_InitStructure;
static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

static uint8_t l2tell = 0;


uint8_t time2tell(void)
{
	if(l2tell==1)
	{
		l2tell = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void mtn_TIM17_cfg(void)
{
	TIM_TimeBaseStructure.TIM_Prescaler = 5999; //1199; //12MHz/12000= 1KHz
	TIM_TimeBaseStructure.TIM_Period = 1999; //1.0 second
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM17,&TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIM17,ENABLE);
	TIM_ITConfig(TIM17,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM17,ENABLE);
}

void TIM17_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM17,TIM_IT_Update)== SET){
      TIM_ClearITPendingBit(TIM17,TIM_IT_Update);//clear interupt mark
			l2tell = 1;
	}
}
