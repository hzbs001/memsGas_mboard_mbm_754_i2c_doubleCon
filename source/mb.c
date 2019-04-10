/***************Function code have been done*********************
03: read holding register
06: write single holding register
16: write multiple holding resister
--by lwang @home,2018/03/11
*****************************************************************/

#include "stdint.h"
#include "mb.h"
#include "mbport.h"
#include "mbcrc.h"

//global variable

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Used by modbus
static uint8_t MB_RX_BUFF[256];//receive buffer
static uint16_t MB_RX_CNT=0;
static uint16_t MB_FrameFlag=0;
static uint8_t MB_TX_BUFF[256];//tx buffer
//static uint16_t MB_TX_CNT=0;
//static uint16_t MB_TX_EN=0;

static uint8_t MB_Addr;//slave address

void mbRcvComplete(void)
{
	//MB_TX_EN=1;//stop receive for rs485
  MB_FrameFlag=1;//��λ֡�������
}
void mbRcvProcess(void)
{
	//uint8_t res;
	if(MB_RX_CNT < MB_FRAME_SIZE_MAX)
      {
				mb_receiveData(&MB_RX_BUFF[MB_RX_CNT]);
        //MB_RX_BUFF[MB_RX_CNT]=res;
        MB_RX_CNT++;
      }
}

void setMBAddr(uint8_t addr)
{
	MB_Addr = addr;
}

/////////////////////////////////////////////////////////////////////////////////////
//RS485����������ڴ�����յ�������(������������ѭ������)
uint16_t startRegAddr;
uint16_t RegNum;
uint16_t calCRC;

void mb_Service(void)
{
  uint16_t recCRC;
  if(MB_FrameFlag==1)
  {
      if(MB_RX_BUFF[0]==MB_Addr)//��ַ��ȷ
      {
          //if((MB_RX_BUFF[1]==01)||(MB_RX_BUFF[1]==02)||(MB_RX_BUFF[1]==03)||(MB_RX_BUFF[1]==05)||(MB_RX_BUFF[1]==06)||(MB_RX_BUFF[1]==15)||(MB_RX_BUFF[1]==16))//��������ȷ
					if((MB_RX_BUFF[1]==03)||(MB_RX_BUFF[1]==06)||(MB_RX_BUFF[1]==16))//Func Code correct
          {
              startRegAddr=(((uint16_t)MB_RX_BUFF[2])<<8)|MB_RX_BUFF[3];//Get register start address
              if(startRegAddr<REG_HOLDING_NREGS)//�Ĵ�����ַ�ڷ�Χ��
              {
                  calCRC=getCRC16(MB_RX_BUFF,MB_RX_CNT-2);//�������������ݵ�CRC
                  recCRC=MB_RX_BUFF[MB_RX_CNT-2]|(((uint16_t)MB_RX_BUFF[MB_RX_CNT-1])<<8);//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
                  if(calCRC==recCRC)//CRCУ����ȷ
                  {
                      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                      switch(MB_RX_BUFF[1])//���ݲ�ͬ�Ĺ�������д���
                      {         
                          case 03: //������Ĵ���
                          {
                              Modbus_03_Solve();
                              break;
                          }
                                
                          case 06: //д�����Ĵ���
                          {
                              Modbus_06_Solve();
                              break;
                          }
                                
                          case 16: //д����Ĵ���
                          {
                              Modbus_16_Solve();
                              break;
                          }
                                            
                      }
                      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  }
                  else//CRCУ�����
                  {
                      MB_TX_BUFF[0]=MB_RX_BUFF[0];
                      MB_TX_BUFF[1]=MB_RX_BUFF[1]|0x80;
                      MB_TX_BUFF[2]=0x04; //�쳣��
                      calCRC=getCRC16(MB_TX_BUFF,3);
			
			                MB_TX_BUFF[3]=calCRC&0xFF;
			                MB_TX_BUFF[4]=(calCRC>>8)&0xFF;
                      mb_SendData(MB_TX_BUFF,5);
                  }    
              }
              else//�Ĵ�����ַ������Χ
              {
                  MB_TX_BUFF[0]=MB_RX_BUFF[0];
                  MB_TX_BUFF[1]=MB_RX_BUFF[1]|0x80;
                  MB_TX_BUFF[2]=0x02; //�쳣��
                  calCRC=getCRC16(MB_TX_BUFF,3);
			
			            MB_TX_BUFF[3]=calCRC&0xFF;
			            MB_TX_BUFF[4]=(calCRC>>8)&0xFF;
                  mb_SendData(MB_TX_BUFF,5);
              }                        
          }
          else//���������
          {
              MB_TX_BUFF[0]=MB_RX_BUFF[0];
              MB_TX_BUFF[1]=MB_RX_BUFF[1]|0x80;
              MB_TX_BUFF[2]=0x01; //�쳣��
              
							calCRC=getCRC16(MB_TX_BUFF,3);
			
							MB_TX_BUFF[3]=calCRC&0xFF;
							MB_TX_BUFF[4]=(calCRC>>8)&0xFF;
							mb_SendData(MB_TX_BUFF,5);
          }
    }
                
      MB_FrameFlag=0;	//��λ֡������־
      MB_RX_CNT=0;		//���ռ���������
      //MB_TX_EN=0;			//��������ģʽ        
  }        
}

//Modbus Func code 03
//Read holding register
void Modbus_03_Solve(void)
{
    RegNum= (((uint16_t)MB_RX_BUFF[4])<<8)|MB_RX_BUFF[5];//Get reg number
    if((startRegAddr+RegNum) <= REG_HOLDING_NREGS)//Addr+regNum locate at our range
    {
      MB_TX_BUFF[0]=MB_RX_BUFF[0];
      MB_TX_BUFF[1]=MB_RX_BUFF[1];
      MB_TX_BUFF[2]=RegNum*2;
      
			mbRegHolding(&MB_TX_BUFF[3],startRegAddr,RegNum,READ);
			
      calCRC=getCRC16(MB_TX_BUFF,RegNum*2+3);
      MB_TX_BUFF[RegNum*2+3]=calCRC&0xFF;
      MB_TX_BUFF[RegNum*2+4]=(calCRC>>8)&0xFF;
      mb_SendData(MB_TX_BUFF,RegNum*2+5);
    }
    else//Address+number overrange
    {
        MB_TX_BUFF[0]=MB_RX_BUFF[0];
        MB_TX_BUFF[1]=MB_RX_BUFF[1]|0x80;
        MB_TX_BUFF[2]=0x02; //Exception
			
        calCRC=getCRC16(MB_TX_BUFF,3);
			
			  MB_TX_BUFF[3]=calCRC&0xFF;
			  MB_TX_BUFF[4]=(calCRC>>8)&0xFF;
        mb_SendData(MB_TX_BUFF,5);
    }
}


//Modbus Func code 06
//write single register
void Modbus_06_Solve(void)
{
		if(startRegAddr < REG_HOLDING_NREGS)
		{
			mbRegHolding(&MB_RX_BUFF[4],startRegAddr,1,WRITE);
		
			MB_TX_BUFF[0]=MB_RX_BUFF[0];
			MB_TX_BUFF[1]=MB_RX_BUFF[1];
			MB_TX_BUFF[2]=MB_RX_BUFF[2];
			MB_TX_BUFF[3]=MB_RX_BUFF[3];
			MB_TX_BUFF[4]=MB_RX_BUFF[4];
			MB_TX_BUFF[5]=MB_RX_BUFF[5];
			
			calCRC=getCRC16(MB_TX_BUFF,6);
			MB_TX_BUFF[6]=calCRC&0xFF;
			MB_TX_BUFF[7]=(calCRC>>8)&0xFF;
			mb_SendData(MB_TX_BUFF,8);
		}
		else
		{
			MB_TX_BUFF[0]=MB_RX_BUFF[0];
      MB_TX_BUFF[1]=MB_RX_BUFF[1]|0x80;
      MB_TX_BUFF[2]=0x02; //Exception
			
			calCRC=getCRC16(MB_TX_BUFF,3);
			
			MB_TX_BUFF[3]=calCRC&0xFF;
			MB_TX_BUFF[4]=(calCRC>>8)&0xFF;
      mb_SendData(MB_TX_BUFF,5);
		}
}

//Modbus func 16
//Read multiple register
void Modbus_16_Solve(void)
{
    RegNum= (((uint16_t)MB_RX_BUFF[4])<<8)|MB_RX_BUFF[5];//Get reg number
    if((startRegAddr+RegNum) <= REG_HOLDING_NREGS)//Addr+regNum locate at our range
    {
			 mbRegHolding(&MB_RX_BUFF[7],startRegAddr,RegNum,WRITE);
       MB_TX_BUFF[0]=MB_RX_BUFF[0];
       MB_TX_BUFF[1]=MB_RX_BUFF[1];
       MB_TX_BUFF[2]=MB_RX_BUFF[2];
       MB_TX_BUFF[3]=MB_RX_BUFF[3];
       MB_TX_BUFF[4]=MB_RX_BUFF[4];
       MB_TX_BUFF[5]=MB_RX_BUFF[5];
        
       calCRC=getCRC16(MB_TX_BUFF,6);
       MB_TX_BUFF[6]=calCRC&0xFF;
       MB_TX_BUFF[7]=(calCRC>>8)&0xFF;
       mb_SendData(MB_TX_BUFF,8);
    }
    else//Address+number overrange
    {
       MB_TX_BUFF[0]=MB_RX_BUFF[0];
       MB_TX_BUFF[1]=MB_RX_BUFF[1]|0x80;
       MB_TX_BUFF[2]=0x02; //Exception
			 calCRC=getCRC16(MB_TX_BUFF,3);
			
			 MB_TX_BUFF[3]=calCRC&0xFF;
			 MB_TX_BUFF[4]=(calCRC>>8)&0xFF;
       mb_SendData(MB_TX_BUFF,5);
    }
}
