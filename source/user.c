#include "stm32f0xx.h"
#include "user_init.h"

#include "mbport.h"
#include "mb.h"

#include "i2c_emu.h"
#include "commu.h"

#include "sht3x.h"

#include "metronome.h"
#include "OLED.h"

#include "disp_init.h"

/* ----------------------- Defines ------------------------------------------*/
const uint8_t REG_HOLDING_START = 0;
/* ----------------------- Static variables ---------------------------------*/
static uint16_t   usRegHoldingStart = REG_HOLDING_START;
static uint16_t   usRegHoldingBuf[REG_HOLDING_NREGS];
/* ----------------------- Start implementation -----------------------------*/
void saveT_H(uint16_t *p);
float sht_buf[4];
int
main( void )
{
	static uint8_t slvAddr = 88;
	
	//用于显示
	uint8_t charHZ[16] = "北森科技出品";
	uint8_t charT[] = "TEMP:";
	uint8_t charTV[16];
	uint8_t charH[] = " HUM:";
	uint8_t charHV[16];
	uint8_t charV[] = " VOC:";
	uint8_t charVV[16];
	
	//clock settings and interrupt management
	RCC_cfg();
	NVIC_cfg();
	//For Modbus
	usart1_GPIO_cfg();
	mb_USART_cfg();
	mb_TIM16_cfg();
	//For led toggle
	mtn_TIM17_cfg();
	//led_GPIO_cfg();
	
	i2c_GPIO_cfg();
	i2c2_GPIO_cfg();
	
	LCD_GPIO_cfg();
	LCD_Init();
	setMBAddr(slvAddr);
	
	SHT3X_SetI2cAdr(0x44);
	delay_us(5000);

	SHT3X_ReadStatus(&usRegHoldingBuf[9]);
	
  while(1)
  {
		mb_Service();		//modbus service
		if(time2tell()==1)
		{
			read_module(usRegHoldingBuf);
			//Read temperature and humidity from SHT3X
			usRegHoldingBuf[8] = SHT3X_GetTempAndHumi(&sht_buf[0],&sht_buf[1],\
                           REPEATAB_MEDIUM,MODE_POLLING,30);
			
			if(usRegHoldingBuf[8]==NO_ERROR)
			{
				//Save Temperature and humidity to usRegHoldingBuf[0:3]
				saveT_H(&usRegHoldingBuf[10]);
				//Write Temperature and humidity to module
				send_T_H(&usRegHoldingBuf[10]);
			}

			disp_conv(usRegHoldingBuf,charHZ,charTV,charHV,charVV);

      LCD_Print(16,0,charHZ,2,2);
			
			LCD_Print(0,16,charT,1,1);
			LCD_Print(40,16,charTV,1,1);
			
			LCD_Print(0,32,charH,1,1);
			LCD_Print(40,32,charHV,1,1);
			
			LCD_Print(0,48,charV,1,1);
			LCD_Print(40,48,charVV,1,1);
			//mbmRdReg(88,0,16);
		}
  }
}

void mbRegHolding(uint8_t *p,uint16_t regAddr,uint16_t regNum,RD_WR mode)
{
	uint16_t i;
	uint16_t iRegIndex;
	if((regAddr >= REG_HOLDING_START) && ((regAddr+regNum) <= (REG_HOLDING_START+REG_HOLDING_NREGS)))
	{
		iRegIndex = (uint16_t)(regAddr-usRegHoldingStart);
		
		switch(mode)
		{
			case READ:
			{
				for(i=0;i<regNum;i++)
				{
					*p++ = (uint8_t)(usRegHoldingBuf[iRegIndex+i] >> 8 );  //High byte first
					*p++ = (uint8_t)(usRegHoldingBuf[iRegIndex+i] & 0xFF); //low byte first
				}
			}
			break;
			case WRITE:
			{
				for(i=0;i<regNum;i++)
				{
						*(usRegHoldingBuf+iRegIndex+i)= ((*p++))<<8; 	//High byte
						*(usRegHoldingBuf+iRegIndex+i)|= *(p++); 			//Low byte
				}

			}	
		}
	}
}

void saveT_H(uint16_t *p)
{
	union pfloat{
	float a;
	uint8_t b[4];
	};
	union pfloat t1,h1;
	t1.a = sht_buf[0];
	h1.a = sht_buf[1];
	*(p) = (t1.b[3]<<8) | t1.b[2];
	*(p+1) = (t1.b[1]<<8) | t1.b[0];
	
	*(p+2) = (h1.b[3]<<8) | h1.b[2];
	*(p+3) = (h1.b[1]<<8) | h1.b[0];
	
}
void SystemInit()
{
	
}
