#include "stdint.h"
#include "i2c_emu.h"
#include "sht20_op.h"

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
uint8_t sht_success_flag = 0;

union pfloat{
	float a;
	uint8_t b[4];
};

void delay_ms(uint16_t time)
{
	uint16_t i = 7999;
	while(time--)
	{
		while(i--);
	}
}

void sht20_cfg(void)
{
	i2c2_start();
	i2c2_write_byte(0x80);
	i2c2_waitack();
	
	i2c2_write_byte(0xFE);
	i2c2_waitack();
	i2c2_stop();
	
	delay_ms(5);
	
	i2c2_start();
	i2c2_write_byte(0x80);
	i2c2_waitack();
	
	//soft reset
	i2c2_write_byte(0xE6);
	i2c2_waitack();
	//12bit 14bit
	i2c2_write_byte(0x02);
	i2c2_waitack();
	i2c2_stop();
}

void sht20_st(void)
{
	static uint8_t sht20cmd = 0xF3;
	
	i2c2_start();
	i2c2_write_byte(0x80);
	i2c2_waitack();
	
	//F3: Triger T measure no hold master
	//F5: Triger RH measure no hold master
	i2c2_write_byte(sht20cmd);
	i2c2_waitack();
	i2c2_stop();
	
	if(sht20cmd==0xF3)
	{
		sht20cmd = 0xF5;
	}
	else
	{
		sht20cmd = 0xF3;
	}
	
}

void sht20_rd(uint16_t *phdc)
{
	union pfloat temp,hum;
	uint16_t temp1,hum1,hdc_cfg,dev_id,mau_id,i;

	i2c2_start();
	i2c2_write_byte(0x81);//read data
	if(i2c2_waitack()==0)
	{
		sht_success_flag = 1;
	temp1 = i2c2_read_byte();
	i2c2_ack();
	temp1 <<= 8;
	
	temp1 |= i2c2_read_byte();
	i2c2_ack();
		
	i2c2_read_byte();
	i2c2_ack();
	i2c2_stop();
	//Read Temperature from sht
	if(temp1&0x0002)
	{
		temp1 = temp1 & 0xFFF0;
		hum.a = temp1*125.0/65536-6;
		//*H = hum.a;

		*(phdc+2) = (hum.b[3]<<8) | hum.b[2];
		*(phdc+3) = (hum.b[1]<<8) | hum.b[0];
		//*phdc = temp1;
	}
	//Read humidity from sht
	else
	{
		temp.a = temp1*175.72/65536-46.85;
		//*T = temp.a;
		
		*phdc++ = (temp.b[3]<<8) | temp.b[2];
		*phdc = (temp.b[1]<<8) | temp.b[0];
		//*phdc = temp1;

	}
	}
}

void sendT_H(uint16_t *p)
{
	if(sht_success_flag==1)
	{
	i2c_start();
	i2c_write_byte(0x60);
	i2c_waitack();
	i2c_write_byte(0x02);
	i2c_waitack();
	//Write Temperature Value
	i2c_write_byte(*p>>8);
	i2c_waitack();
	i2c_write_byte(*p&0xFF);
	i2c_waitack();
	i2c_write_byte(*(p+1)>>8);
	i2c_waitack();
	i2c_write_byte(*(p+1)&0xFF);
	i2c_waitack();
	
	//Write humidity value
	i2c_write_byte(*(p+2)>>8);
	i2c_waitack();
	i2c_write_byte(*(p+2)&0xFF);
	i2c_waitack();
	i2c_write_byte(*(p+3)>>8);
	i2c_waitack();
	i2c_write_byte(*(p+3)&0xFF);
	i2c_waitack();
	i2c_stop();
	sht_success_flag = 0;
	}
	
}

