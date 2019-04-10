/**********************************************
显示数据初始化
输入浮点数，输出各位的ASCII码
温度显示范围-99.99～199.99
湿度显示范围0.00～99.99
VOC显示范围0.00～9999.9
***********************************************/
#include "disp_init.h"

union pfloat{
	float a;
	uint8_t b[4];
};
void disp_conv(uint16_t *in,uint8_t *char4row1,\
	uint8_t *char4row2,uint8_t *char4row3,uint8_t *char4row4)
{
	union pfloat temp2,temp3,temp4;
	float temp;
	uint8_t intt1,intt2,intt3,intt4,frt1,frt2;
	uint16_t inttemp1;
	
	/***Big-endian to little-endian
	modbus is big-endian
	
	stm32 is little-endian:
	b[0] is lowest byte
	b[3] is highest byte*****/
	//VOC
	temp4.b[0] = (*(in+1)) & 0xFF;
	temp4.b[1] = (*(in+1))>>8;
	temp4.b[2] = (*in) & 0xFF;
	temp4.b[3] = (*in)>>8;
	//Temperature
	temp2.b[0] = (*(in+3)) & 0xFF;
	temp2.b[1] = (*(in+3))>>8;
	temp2.b[2] = (*(in+2)) & 0xFF;
	temp2.b[3] = (*(in+2))>>8;
	//humidity
	temp3.b[0] = (*(in+5)) & 0xFF;
	temp3.b[1] = (*(in+5))>>8;
	temp3.b[2] = (*(in+4)) & 0xFF;
	temp3.b[3] = (*(in+4))>>8;
	
	//Temperature
	if((temp2.b[3]&0x80)==0){
		temp = temp2.a;
		
		inttemp1 = (int)temp;
		intt1 = inttemp1/100%10;
		intt2 = inttemp1/10%10;
		intt3 = inttemp1%10;
		
		inttemp1 = (temp-inttemp1)*100;
		frt1 = inttemp1/10%10;
		frt2 = inttemp1%10;
		
		
		if(intt1!=0){
			char4row2[0] = ' ';
			char4row2[1] = intt1;
			char4row2[2] = intt2;
			char4row2[3] = intt3;
			char4row2[4] = '.';
			char4row2[5] = frt1;
			char4row2[6] = frt2;
			
			char4row2[1] += 48;
			char4row2[2] += 48;
			char4row2[3] += 48;
			char4row2[5] += 48;
			char4row2[6] += 48;
			char4row2[7] = '^';
			char4row2[8] = 'C';
		}
		else if(intt2!=0){
			char4row2[0] = ' ';
			char4row2[1] = intt2;
			char4row2[2] = intt3;
			char4row2[3] = '.';
			char4row2[4] = frt1;
			char4row2[5] = frt2;	
			
			char4row2[1] += 48;
			char4row2[2] += 48;	
			char4row2[4] += 48;
			char4row2[5] += 48;
			char4row2[6] = '^';
			char4row2[7] = 'C';
			char4row2[8] = ' ';
		}
		else{
			char4row2[0] = ' ';
			char4row2[1] = ' ';
			char4row2[2] = intt3;
			char4row2[3] = '.';
			char4row2[4] = frt1;
			char4row2[5] = frt2;
			
			char4row2[2] += 48;
			char4row2[4] += 48;
			char4row2[5] += 48;
			char4row2[6] = '^';
			char4row2[7] = 'C';
			char4row2[8] = ' ';
		}
	}
	else
	{
		temp = temp2.a*(-1);
		intt1 = temp/10;
		intt2 = temp-intt1*10;
		temp = temp -intt1*10-intt2;
		frt1 = temp*10;
		frt2 = temp*100-frt1*10;
		char4row2[0] = ' ';
		char4row2[1] = '-';
		char4row2[2] = intt1;
		char4row2[3] = intt2;
		char4row2[4] = '.';
		char4row2[5] = frt1;
		char4row2[6] = frt2;	
		char4row2[2] += 48;
		char4row2[3] += 48;
		char4row2[5] += 48;
		char4row2[6] += 48;
		char4row2[7] = '^';
		char4row2[8] = 'C';
	}
	//humidity
	if((temp3.b[3]&0x80)==0){
		temp = temp3.a;
		
		inttemp1 = (int)temp;
		intt1 = inttemp1/10%10;
		intt2 = inttemp1%10;
		
		inttemp1 = (temp-inttemp1)*100;
		frt1 = inttemp1/10%10;
		frt2 = inttemp1%10;
		
		if(intt1!=0){
			char4row3[0] = ' ';
			char4row3[1] = intt1;
			char4row3[2] = intt2;
			char4row3[3] = '.';
			char4row3[4] = frt1;
			char4row3[5] = frt2;	
			char4row3[1] += 48;
			char4row3[2] += 48;
			char4row3[4] += 48;
			char4row3[5] += 48;
			char4row3[6] = '%';
		}
		else{
			char4row3[0] = ' ';
			char4row3[1] = ' ';
			char4row3[2] = intt2;
			char4row3[3] = '.';
			char4row3[4] = frt1;
			char4row3[5] = frt2;	
			char4row3[1] += 48;
			char4row3[2] += 48;
			char4row3[4] += 48;
			char4row3[5] += 48;
			char4row3[6] = '%';
		}
	}
	//VOC
	if((temp4.b[3]&0x80)==0){
		//判断如果浓度是负数，设置浓度=9999.9
		if(temp4.a<100) 
			temp = temp4.a;
		else 
			temp = 9999.9;
		
		inttemp1 = (int)temp;
		intt1 = inttemp1/1000%10;
		intt2 = inttemp1/100%10;
		intt3 = inttemp1/10%10;
		intt4 = inttemp1%10;
		
		inttemp1 = (temp-inttemp1)*100;
		frt1 = inttemp1/10%10;
		frt2 = inttemp1%10;
		//千位=0
		if(intt1!=0){
			char4row4[0] = intt1;
			char4row4[1] = intt2;
			char4row4[2] = intt3;
			char4row4[3] = intt4;
			char4row4[4] = '.';
			char4row4[5] = frt1;	
			
			char4row4[0] += 48;
			char4row4[1] += 48;
			char4row4[2] += 48;
			char4row4[3] += 48;
			char4row4[5] += 48;
			char4row4[6] = 'p';
			char4row4[7] = 'p';
			char4row4[8] = 'm';
		}
		//百位=0
		else if(intt2!=0){
			char4row4[0] = ' ';
			char4row4[1] = intt2;
			char4row4[2] = intt3;
			char4row4[3] = intt4;
			char4row4[4] = '.';
			char4row4[5] = frt1;	
			
			char4row4[1] += 48;
			char4row4[2] += 48;
			char4row4[3] += 48;
			char4row4[5] += 48;
			char4row4[6] = 'p';
			char4row4[7] = 'p';
			char4row4[8] = 'm';
		}
		//十位=0
		else if(intt3!=0){
			char4row4[0] = ' ';
			char4row4[1] = intt3;
			char4row4[2] = intt4;
			char4row4[3] = '.';
			char4row4[4] = frt1;
			char4row4[5] = frt2;	
			
			char4row4[1] += 48;
			char4row4[2] += 48;
			char4row4[4] += 48;
			char4row4[5] += 48;
			char4row4[6] = 'p';
			char4row4[7] = 'p';
			char4row4[8] = 'm';
		}
		else{
			char4row4[0] = ' ';
			char4row4[1] = ' ';
			char4row4[2] = intt4;
			char4row4[3] = '.';
			char4row4[4] = frt1;
			char4row4[5] = frt2;	
			
			char4row4[2] += 48;
			char4row4[4] += 48;
			char4row4[5] += 48;
			char4row4[6] = 'p';
			char4row4[7] = 'p';
			char4row4[8] = 'm';
		}
	}
}
