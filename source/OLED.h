#ifndef _LQOLED_H
#define _LQOLED_H
#include "stm32f0xx.h"

//汉字大小，英文数字大小
#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3

//-----------------OLED端口定义----------------  					   

#define LCD_SCL_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LCD_SCL_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define LCD_SDA_CLR()	GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LCD_SDA_SET()	GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define LCD_RST_CLR()	GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LCD_RST_SET()	GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LCD_DC_CLR()	GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LCD_DC_SET()	GPIO_SetBits(GPIOA,GPIO_Pin_5)

void LCD_GPIO_cfg(void);
void LCD_Init(void);
void LCD_CLS(void);
void LCD_CLS_y(char y);
void LCD_CLS_line_area(uint8_t start_x,uint8_t start_y,uint8_t width);
void LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F6x8);
void LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F8x16);
void LCD_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[],const uint8_t *F14x16_Idx,const uint8_t *F14x16);
void LCD_P16x16Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F16x16_Idx,const uint8_t *F16x16);
void LCD_PutPixel(uint8_t x,uint8_t y);
void LCD_Print(uint8_t x, uint8_t y, uint8_t *ch,uint8_t char_size, uint8_t ascii_size);
void LCD_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
void Draw_BMP(uint8_t x,uint8_t y,const uint8_t *bmp); 
void LCD_Fill(uint8_t dat);
#endif

