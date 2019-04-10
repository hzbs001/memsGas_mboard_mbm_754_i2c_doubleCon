/*****************************************************
IO Emulation I2C  for reading SHT3X
*****************************************************/
#include "i2c_emu.h"
#define I2C2_SCL_CLR	GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define I2C2_SCL_SET	GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define I2C2_SDA_CLR	GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define I2C2_SDA_SET	GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define I2C2_READ_BIT GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

static GPIO_InitTypeDef GPIO_InitStructure;

void i2c2_GPIO_cfg(void)
{
	//GPIOA0: SDA
	//GPIOA1: SCL
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}



void i2c2_start(void)
{
	
	I2C2_SDA_SET;
	I2C2_SCL_SET;
	delay_us(1);
	I2C2_SDA_CLR;
	delay_us(1);
}

void i2c2_stop(void)
{
	
	I2C2_SDA_CLR;
	I2C2_SCL_SET;
	delay_us(1);
	I2C2_SDA_SET;
	delay_us(1);
}

void i2c2_ack(void)
{
	
	I2C2_SCL_CLR;
	delay_us(1);
	I2C2_SDA_CLR;
	delay_us(1);
	I2C2_SCL_SET;
	delay_us(1);
	I2C2_SCL_CLR;
	delay_us(1);
	I2C2_SDA_SET;
}

void i2c2_noack(void)
{
	
	I2C2_SCL_CLR;
	delay_us(1);
	I2C2_SDA_SET;
	delay_us(1);
	I2C2_SCL_SET;
	delay_us(1);
	I2C2_SCL_CLR;
	delay_us(1);
}

uint8_t i2c2_waitack(void)
{
	uint8_t re;
	
	I2C2_SDA_SET;
	delay_us(1);
	I2C2_SCL_SET;
	delay_us(1);
	if(I2C2_READ_BIT)
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C2_SCL_CLR;
	delay_us(1);
	return re;
}

uint8_t i2c2_read_byte(void)
{
	uint8_t i;
	uint8_t value;
	value = 0;
	
	for(i=0;i<8;i++)
	{
		value <<= 1;
		I2C2_SCL_CLR;
		I2C2_SDA_SET;
		delay_us(1);
		I2C2_SCL_SET;	
		if(I2C2_READ_BIT)
		{
			value++;
		}
		delay_us(1);
	}
	return value;
}

void i2c2_write_byte(uint8_t data)
{
	uint8_t i;

	I2C2_SCL_CLR;
	delay_us(1);
	for(i=0;i<8;i++)
	{
		if(data & 0x80)
		{
			I2C2_SDA_SET;
		}
		else
		{
			I2C2_SDA_CLR;
		}
		delay_us(1);
		I2C2_SCL_SET;
		delay_us(1);
		I2C2_SCL_CLR;
		//delay_us(1);
		data <<= 1;
	}
}

