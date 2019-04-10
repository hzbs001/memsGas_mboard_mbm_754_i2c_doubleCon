/**********************************************
通过i2C读取模块的温度，湿度，传感器电阻
通过I2C向模块传输温度和湿度
***********************************************/
#include "i2c_emu.h"
#include "commu.h"
union pfloat{
	float a;
	uint8_t b[4];
};

void read_module(uint16_t *pbuf)
{
	i2c_start();
	i2c_write_byte(0x60);
	i2c_waitack();
	i2c_write_byte(0x00);
	i2c_waitack();
	i2c_stop();
	
	i2c_start();
	i2c_write_byte(0x61);//read data
	if(i2c_waitack()==0)
	{
		*pbuf = i2c_read_byte();
		i2c_ack();
		/****read temperature*****/
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_ack();
		/****read humidity*****/
		pbuf++;
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_ack();
		/****read VOC*****/
		pbuf++;
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_ack();
		
		pbuf++;
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_ack();
		
		pbuf++;
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_ack();
		
		pbuf++;
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_ack();
		//RS realtime
		pbuf++;
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_ack();
		//AdcValue
		pbuf++;
		*pbuf = i2c_read_byte();
		i2c_ack();
		*pbuf <<= 8;
		*pbuf |= i2c_read_byte();
		i2c_noack();
		//*pbuf=0x11;
	}
	i2c_stop();
}
//Write Temperature and humidity to module
void send_T_H(uint16_t *pbuf)
{
	i2c_start();
	i2c_write_byte(0x60);
	i2c_waitack();
	i2c_write_byte(0x02);
	i2c_waitack();
	
	i2c_write_byte((*pbuf>>8)&0xFF);
	i2c_waitack();
	i2c_write_byte(*pbuf&0xFF);
	i2c_waitack();
		
	i2c_write_byte((*(pbuf+1)>>8)&0xFF);
	i2c_waitack();
	i2c_write_byte(*(pbuf+1)&0xFF);
	i2c_waitack();
		
	i2c_write_byte((*(pbuf+2)>>8)&0xFF);
	i2c_waitack();
	i2c_write_byte(*(pbuf+2)&0xFF);
	i2c_waitack();
	
	i2c_write_byte((*(pbuf+3)>>8)&0xFF);
	i2c_waitack();
	i2c_write_byte(*(pbuf+3)&0xFF);
	i2c_waitack();
	
	i2c_stop();
		
}
