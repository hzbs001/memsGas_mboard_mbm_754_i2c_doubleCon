/*OLED底层驱动
*@采用列行式，分辨率为128*64
*存放格式如下.
*[0]0 1 2 3 ... 127	
*[1]0 1 2 3 ... 127	
*[2]0 1 2 3 ... 127	
*[3]0 1 2 3 ... 127	
*[4]0 1 2 3 ... 127	
*[5]0 1 2 3 ... 127	
*[6]0 1 2 3 ... 127	
*[7]0 1 2 3 ... 127 
*/
#include "OLED.h"



#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 


#define X_WIDTH 128
#define Y_WIDTH 64
//======================================
const uint8_t F6x8[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,   // sp
    0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 ,   // !
    0x00, 0x00, 0x07, 0x00, 0x07, 0x00 ,   // "
    0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 ,   // #
    0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 ,   // $
    0x00, 0x62, 0x64, 0x08, 0x13, 0x23 ,   // %
    0x00, 0x36, 0x49, 0x55, 0x22, 0x50 ,   // &
    0x00, 0x00, 0x05, 0x03, 0x00, 0x00 ,   // '
    0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 ,   // (
    0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 ,   // )
    0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 ,   // *
    0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 ,   // +
    0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 ,   // ,
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08 ,   // -
    0x00, 0x00, 0x60, 0x60, 0x00, 0x00 ,   // .
    0x00, 0x20, 0x10, 0x08, 0x04, 0x02 ,   // /
    0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E ,   // 0
    0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 ,   // 1
    0x00, 0x42, 0x61, 0x51, 0x49, 0x46 ,   // 2
    0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 ,   // 3
    0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 ,   // 4
    0x00, 0x27, 0x45, 0x45, 0x45, 0x39 ,   // 5
    0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 ,   // 6
    0x00, 0x01, 0x71, 0x09, 0x05, 0x03 ,   // 7
    0x00, 0x36, 0x49, 0x49, 0x49, 0x36 ,   // 8
    0x00, 0x06, 0x49, 0x49, 0x29, 0x1E ,   // 9
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00 ,   // :
    0x00, 0x00, 0x56, 0x36, 0x00, 0x00 ,   // ;
    0x00, 0x08, 0x14, 0x22, 0x41, 0x00 ,   // <
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14 ,   // =
    0x00, 0x00, 0x41, 0x22, 0x14, 0x08 ,   // >
    0x00, 0x02, 0x01, 0x51, 0x09, 0x06 ,   // ?
    0x00, 0x32, 0x49, 0x59, 0x51, 0x3E ,   // @
    0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C ,   // A
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 ,   // B
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 ,   // C
    0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C ,   // D
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 ,   // E
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 ,   // F
    0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A ,   // G
    0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F ,   // H
    0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 ,   // I
    0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 ,   // J
    0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 ,   // K
    0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 ,   // L
    0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F ,   // M
    0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F ,   // N
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E ,   // O
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 ,   // P
    0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E ,   // Q
    0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 ,   // R
    0x00, 0x46, 0x49, 0x49, 0x49, 0x31 ,   // S
    0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 ,   // T
    0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F ,   // U
    0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F ,   // V
    0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F ,   // W
    0x00, 0x63, 0x14, 0x08, 0x14, 0x63 ,   // X
    0x00, 0x07, 0x08, 0x70, 0x08, 0x07 ,   // Y
    0x00, 0x61, 0x51, 0x49, 0x45, 0x43 ,   // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 ,   // [
    0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 ,   // 55
    0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 ,   // ]
    0x00, 0x04, 0x02, 0x01, 0x02, 0x04 ,   // ^
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40 ,   // _
    0x00, 0x00, 0x01, 0x02, 0x04, 0x00 ,   // '
    0x00, 0x20, 0x54, 0x54, 0x54, 0x78 ,   // a
    0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 ,   // b
    0x00, 0x38, 0x44, 0x44, 0x44, 0x20 ,   // c
    0x00, 0x38, 0x44, 0x44, 0x48, 0x7F ,   // d
    0x00, 0x38, 0x54, 0x54, 0x54, 0x18 ,   // e
    0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 ,   // f
    0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C ,   // g
    0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 ,   // h
    0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 ,   // i
    0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 ,   // j
    0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 ,   // k
    0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 ,   // l
    0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 ,   // m
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 ,   // n
    0x00, 0x38, 0x44, 0x44, 0x44, 0x38 ,   // o
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 ,   // p
    0x00, 0x18, 0x24, 0x24, 0x18, 0xFC ,   // q
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 ,   // r
    0x00, 0x48, 0x54, 0x54, 0x54, 0x20 ,   // s
    0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 ,   // t
    0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C ,   // u
    0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C ,   // v
    0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C ,   // w
    0x00, 0x44, 0x28, 0x10, 0x28, 0x44 ,   // x
    0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C ,   // y
    0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 ,   // z
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14     // horiz lines
};

const uint8_t F16x16_Idx[] = 
{
	"实验北京工业大学浓度温湿设计出品北森科技"
};

const uint8_t F16x16[] = 
{
	0x10,0x0C,0x04,0x84,0x14,0x64,0x05,0x06,0xF4,0x04,0x04,0x04,0x04,0x14,0x0C,0x00,
0x04,0x84,0x84,0x44,0x47,0x24,0x14,0x0C,0x07,0x0C,0x14,0x24,0x44,0x84,0x04,0x00,/*"?",0*/

0x02,0xFA,0x82,0x82,0xFE,0x80,0x40,0x20,0x50,0x4C,0x43,0x4C,0x50,0x20,0x40,0x00,
0x08,0x18,0x48,0x84,0x44,0x3F,0x40,0x44,0x58,0x41,0x4E,0x60,0x58,0x47,0x40,0x00,/*"?",1*/

	0x00,0x20,0x20,0x20,0x20,0xFF,0x00,0x00,0x00,0xFF,0x40,0x20,0x10,0x08,0x00,0x00,
0x20,0x60,0x20,0x10,0x10,0xFF,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00,/*"?",0*/

0x04,0x04,0x04,0xE4,0x24,0x24,0x25,0x26,0x24,0x24,0x24,0xE4,0x04,0x04,0x04,0x00,
0x00,0x40,0x20,0x1B,0x02,0x42,0x82,0x7E,0x02,0x02,0x02,0x0B,0x10,0x60,0x00,0x00,/*"?",1*/

0x00,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,/*"?",2*/

0x00,0x10,0x60,0x80,0x00,0xFF,0x00,0x00,0x00,0xFF,0x00,0x00,0xC0,0x30,0x00,0x00,
0x40,0x40,0x40,0x43,0x40,0x7F,0x40,0x40,0x40,0x7F,0x42,0x41,0x40,0x40,0x40,0x00,/*"?",3*/

0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00,/*"?",4*/

0x40,0x30,0x11,0x96,0x90,0x90,0x91,0x96,0x90,0x90,0x98,0x14,0x13,0x50,0x30,0x00,
0x04,0x04,0x04,0x04,0x04,0x44,0x84,0x7E,0x06,0x05,0x04,0x04,0x04,0x04,0x04,0x00,/*"?",5*/

0x10,0x60,0x02,0x8C,0x20,0x18,0x08,0xC8,0x38,0xCF,0x08,0x08,0x28,0x98,0x00,0x00,
0x04,0x04,0x7E,0x01,0x04,0x02,0x01,0xFF,0x40,0x21,0x06,0x0A,0x11,0x20,0x40,0x00,/*"?",6*/

0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,/*"?",7*/

0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00,/*"?",8*/

0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00,/*"?",9*/

0x40,0x40,0x42,0xCC,0x00,0x40,0xA0,0x9E,0x82,0x82,0x82,0x9E,0xA0,0x20,0x20,0x00,
0x00,0x00,0x00,0x3F,0x90,0x88,0x40,0x43,0x2C,0x10,0x28,0x46,0x41,0x80,0x80,0x00,/*"?",0*/

0x40,0x40,0x42,0xCC,0x00,0x40,0x40,0x40,0x40,0xFF,0x40,0x40,0x40,0x40,0x40,0x00,
0x00,0x00,0x00,0x7F,0x20,0x10,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,/*"?",1*/

0x00,0x00,0x7C,0x40,0x40,0x40,0x40,0xFF,0x40,0x40,0x40,0x40,0xFC,0x00,0x00,0x00,
0x00,0x7C,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0xFC,0x00,0x00,/*"?",2*/

0x00,0x00,0x00,0x7E,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x7E,0x00,0x00,0x00,0x00,
0x00,0xFE,0x42,0x42,0x42,0xFE,0x00,0x00,0x00,0xFE,0x42,0x42,0x42,0xFE,0x00,0x00,/*"?",3*/

0x00,0x20,0x20,0x20,0x20,0xFF,0x00,0x00,0x00,0xFF,0x40,0x20,0x10,0x08,0x00,0x00,
0x20,0x60,0x20,0x10,0x10,0xFF,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00,/*"?",0*/

0x40,0x44,0x24,0x24,0x94,0x0C,0x04,0x7F,0x04,0x0C,0x94,0x24,0x24,0x44,0x40,0x00,
0x20,0x22,0x12,0x0A,0xFF,0x0A,0x12,0x20,0x12,0x0A,0xFF,0x06,0x0A,0x12,0x20,0x00,/*"?",1*/

0x24,0x24,0xA4,0xFE,0xA3,0x22,0x00,0x22,0xCC,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,
0x08,0x06,0x01,0xFF,0x00,0x01,0x04,0x04,0x04,0x04,0x04,0xFF,0x02,0x02,0x02,0x00,/*"?",2*/

0x10,0x10,0x10,0xFF,0x10,0x90,0x08,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,0x08,0x00,
0x04,0x44,0x82,0x7F,0x01,0x80,0x80,0x40,0x43,0x2C,0x10,0x28,0x46,0x81,0x80,0x00/*"?",3*/

};
//======================================================
// 128X64I液晶底层驱动[8X16]字体库
// 设计者: powerint
// 描  述: [8X16]西文字符的字模数据 (纵向取模,字节倒序)
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//======================================================
const uint8_t F8X16[]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
	0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//!1
	0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//"2
	0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//#3
	0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$4
	0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//%5
	0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//&6
	0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//'7
	0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//(8
	0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//)9
	0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//*10
	0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+11
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//,12
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//-13
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//.14
	0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,///15
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//016
	0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//117
	0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//218
	0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//319
	0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//420
	0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//521
	0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//622
	0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//723
	0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//824
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//925
	0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//:26
	0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//;27
	0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//<28
	0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//=29
	0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//>30
	0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//?31
	0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@32
	0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A33
	0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B34
	0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C35
	0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D36
	0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E37
	0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F38
	0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G39
	0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H40
	0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I41
	0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J42
	0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K43
	0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L44
	0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M45
	0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N46
	0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O47
	0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P48
	0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q49
	0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R50
	0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S51
	0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T52
	0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U53
	0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V54
	0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W55
	0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X56
	0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y57
	0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z58
	0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[59
	0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\60
	0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//]61
	0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^62
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_63
	0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//`64
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a65
	0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b66
	0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c67
	0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d68
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e69
	0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f70
	0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g71
	0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h72
	0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i73
	0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j74
	0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k75
	0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l76
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m77
	0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n78
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o79
	0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p80
	0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q81
	0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r82
	0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s83
	0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t84
	0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//uint8_t5
	0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v86
	0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w87
	0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x88
	0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y89
	0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z90
	0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{91
	0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//|92
	0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//}93
	0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~94

};
/* 生成实心菱形图标的16*16点阵*/
const uint8_t nonside[] =
{
	0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,//◆0,
};

static GPIO_InitTypeDef GPIO_InitStructure;

void LCD_GPIO_cfg(void)
{
	//GPIOB1: CLK
	//GPIOA5: D/C
	//GPIOA6: RST
	//GPIOA6: DAT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void LCD_WrDat(uint8_t data)
{
	uint8_t i=8;
	//LCD_CS=0;;
	LCD_DC_SET();
  LCD_SCL_CLR();
  //asm("nop");    
  while(i--)
  {
    if(data&0x80){LCD_SDA_SET();}
    else{LCD_SDA_CLR();}
    LCD_SCL_SET(); 
    //asm("nop");;
		//asm("nop");            
    LCD_SCL_CLR();    
    data<<=1;    
  }
	//LCD_CS=1;
}
void LCD_WrCmd(uint8_t cmd)
{
	uint8_t i=8;
	
	//LCD_CS=0;;
	LCD_DC_CLR();
  LCD_SCL_CLR();
  //asm("nop");   
  while(i--)
  {
    if(cmd&0x80){LCD_SDA_SET();}
    else{LCD_SDA_CLR();}
    LCD_SCL_SET();
    //asm("nop");;
		//asm("nop");             
    LCD_SCL_CLR();    
    cmd<<=1;;   
  } 	
	//LCD_CS=1;
}

/*设置起始地址
*Y轴是按8格递进的，y轴0~63，只能按8格的倍数显示，
*因为列行式只能按8个字节进行
*/
void LCD_Set_Pos(uint8_t x, uint8_t y)
{ 
  LCD_WrCmd(0xb0+(y>>3));
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x01); 
} 
void LCD_Fill(uint8_t bmp_data)
{
	uint8_t y,x;
	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(bmp_data);
	}
}
void LCD_CLS(void)
{
	uint8_t y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(0);
	}
}
//清除一行0-63
void LCD_CLS_y(char y)
{
	uint8_t x;	
	
	LCD_WrCmd(0xb0+(y>>3));
	LCD_WrCmd(0x01);
	LCD_WrCmd(0x10); 
	for(x=0;x<X_WIDTH;x++)
	{
		LCD_WrDat(0);
	}
	
}

//清除一行上的一块区域y=0-63
void LCD_CLS_line_area(uint8_t start_x,uint8_t start_y,uint8_t width)
{
	uint8_t x;	
	
	LCD_WrCmd(0xb0+(start_y>>3));
	LCD_WrCmd(0x01);
	LCD_WrCmd(0x10); 
	for(x=start_x;x<width;x++)
	{
		LCD_WrDat(0);
	}
	
}


void LCD_DLY_ms(uint16_t ms)
{                         
  uint16_t a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
  return;
}

void LCD_Init(void)        
{
  //DDRA=0XFF;
  
	LCD_SCL_SET();
	//LCD_CS=1;	//预制SLK和SS为高电平   	
	
	LCD_RST_CLR();
	LCD_DLY_ms(50);
	LCD_RST_SET();

  LCD_WrCmd(0xae);//--turn off oled panel
  LCD_WrCmd(0x00);//---set low column address
  LCD_WrCmd(0x10);//---set high column address
  LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  LCD_WrCmd(0xa6);//--set normal display
  LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);//--1/64 duty
  LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
  LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x40);//Set VCOM Deselect Level
  LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);//
  LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  LCD_WrCmd(0xaf);//--turn on oled panel
  LCD_Fill(0x00);  //初始清屏
  LCD_Set_Pos(0,0);  
	
} 
//==============================================================
//函数名： void LCD_PutPixel(uint8_t x,uint8_t y)
//功能描述：绘制一个点（x,y）
//参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
//返回：无
//==============================================================
void LCD_PutPixel(uint8_t x,uint8_t y)
{
	uint8_t data1;  //data1当前点的数据 
	 
    //LCD_Set_Pos(x,y); 
	data1 = 0x01<<(y%8); 	
	LCD_WrCmd(0xb0+(y>>3));
	LCD_WrCmd(((x&0xf0)>>4)|0x10);
	LCD_WrCmd((x&0x0f)|0x00);
	LCD_WrDat(data1); 	 	
}
//==============================================================
//函数名： void LCD_Put_Column(uint8_t x,uint8_t y,uint8_t data)
//功能描述：操作一列显示，一列全显示0XFF,清除一列0X00;
//参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
//返回：无
//==============================================================
void LCD_Put_Column(uint8_t x,uint8_t y,uint8_t data)
{	
	LCD_WrCmd(0xb0+(y>>3));
	LCD_WrCmd(((x&0xf0)>>4)|0x10);
	LCD_WrCmd((x&0x0f)|0x00);
	LCD_WrDat(data); 	 	
}


//==============================================================
//函数名： void LCD_Rectangle(uint8_t x1,uint8_t y1,
//                   uint8_t x2,uint8_t y2,uint8_t color,uint8_t gif)
//功能描述：绘制一个实心矩形
//参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）
//      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
//返回：无
//==============================================================
void LCD_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif)
{
	uint8_t n; 
		
	LCD_Set_Pos(x1,y1>>3);
	for(n=x1;n<=x2;n++)
	{
		LCD_WrDat(0x01<<(y1%8)); 			
		if(gif == 1) 	LCD_DLY_ms(50);
	}  
	LCD_Set_Pos(x1,y2>>3);
  for(n=x1;n<=x2;n++)
	{
		LCD_WrDat(0x01<<(y2%8)); 			
		if(gif == 1) 	LCD_DLY_ms(5);
	}
	
}  
//==============================================================
//函数名：LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F6x8)
{
	uint8_t c=0,i=0,j=0;

	while (*(ch+j)!='\0')
	{    
		c =*(ch+j)-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		LCD_Set_Pos(x,y);    
		for(i=0;i<6;i++) 
		{
			LCD_WrDat(*(F6x8+c*6+i));
		}
		x+=6;
		j++;
	}
}
//==============================================================
//函数名：LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～63，要显示的字符串
//返回：无
//==============================================================  
void LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F8x16)
{
  uint8_t c=0,i=0,j=0;
        
  while (*(ch+j)!='\0')
  {    
    c =*(ch+j)-32;
    if(x>120)
	{	
		x=0;
		y++;
	}
    LCD_Set_Pos(x,y); 
  	for(i=0;i<8;i++) 
  	{
  	  	LCD_WrDat(*(F8x16+c*16+i));
  	}
  	LCD_Set_Pos(x,y+8);    
  	for(i=0;i<8;i++)  
  	{
  	  	LCD_WrDat(*(F8x16+c*16+i+8));  
  	}
  	x+=8;
  	j++;
  }
}
//输出汉字字符串
void LCD_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[],const uint8_t *F14x16_Idx,const uint8_t *F14x16)
{
	uint8_t wm=0,ii = 0;
	uint16_t adder=1; 
	
	while(ch[ii] != '\0')
	{
	  	wm = 0;
	  	adder = 1;
	  	while(*(F14x16_Idx+wm) > 127)
	  	{
	  		if(*(F14x16_Idx+wm) == ch[ii])
	  		{
	  			if(*(F14x16_Idx+wm+1) == ch[ii + 1])
	  			{
	  				adder = wm * 14;
	  				break;
	  			}
	  		}
	  		wm += 2;			
	  	}
	  	if(x>118)
		{
			x=0;
			y++;
		}
	  	LCD_Set_Pos(x , y); 
	  	if(adder != 1)// 显示汉字					
	  	{
	  		LCD_Set_Pos(x , y);
	  		for(wm = 0;wm < 14;wm++)               
	  		{
	  			LCD_WrDat(*(F14x16+adder));	
	  			adder += 1;
	  		}      
	  		LCD_Set_Pos(x,y + 1); 
	  		for(wm = 0;wm < 14;wm++)          
	  		{
	  			LCD_WrDat(*(F14x16+adder));
	  			adder += 1;
	  		}   		
	  	}
	  	else			  //显示空白字符			
	  	{
	  		ii += 1;
	      	LCD_Set_Pos(x,y);
	  		for(wm = 0;wm < 16;wm++)
	  		{
	  			LCD_WrDat(0);
	  		}
	  		LCD_Set_Pos(x,y + 1);
	  		for(wm = 0;wm < 16;wm++)
	  		{   		
	  			LCD_WrDat(0);	
	  		}
	  	}
	  	x += 14;
	  	ii += 2;
	}
}
//输出汉字字符串
void LCD_P16x16Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F16x16_Idx,const uint8_t *F16x16)
{
	uint8_t wm=0,ii = 0;
	uint16_t adder=1; 
	
	while(*(ch+ii) != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(*(F16x16_Idx+wm) > 127)
  	{
  		if(*(F16x16_Idx+wm) == *(ch+ii))
  		{
  			if(*(F16x16_Idx+wm + 1) == *(ch+ii + 1))
  			{
  				adder = wm * 16;
  				break;
  			}
  		}
  		wm += 2;			
  	}
  	if(x>118){x=0;y++;}
  	LCD_Set_Pos(x , y); 
  	if(adder != 1)// 显示汉字					
  	{
  		LCD_Set_Pos(x , y);
  		for(wm = 0;wm < 16;wm++)               
  		{
  			LCD_WrDat(*(F16x16+adder));	
  			adder += 1;
  		}      
  		LCD_Set_Pos(x,y + 8); 
  		for(wm = 0;wm < 16;wm++)          
  		{
  			LCD_WrDat(*(F16x16+adder));
  			adder += 1;
  		}   		
  	}
  	else			  //显示空白字符			
  	{
  		ii += 1;
      	LCD_Set_Pos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  			LCD_WrDat(0);
  		}
  		LCD_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{   		
  			LCD_WrDat(0);	
  		}
  	}
  	x += 16;
  	ii += 2;
	}
}


/*输出汉字和字符混合字符串
*Y轴是按8格递进的，y轴0~63，只能按8格的倍数显示，
*因为列行式只能按8个字节进行
*
*/

void LCD_Print(uint8_t x, uint8_t y, uint8_t *ch,uint8_t char_size, uint8_t ascii_size)
{
	uint8_t ch2[3];
	uint8_t ii=0;        
	while(*(ch+ii) != '\0')
	{
		if(*(ch+ii) > 127)//大于127为中文，小于等于127为ASCII
		{
			ch2[0] = *(ch+ii);
	 		ch2[1] = *(ch+ii+1);
			ch2[2] = '\0';			//汉字为两个字节
			LCD_P16x16Str(x , y, ch2,F16x16_Idx,F16x16);	//显示汉字
			x += 16;
			ii += 2;
		}
		else
		{
			ch2[0] = *(ch+ii);	
			ch2[1] = '\0';			//字母占一个字节
			if(TYPE8X16==ascii_size)
			{
				LCD_P8x16Str(x , y ,ch2,F8X16);	//显示字母
				x += 8;
			}
			else if(TYPE6X8==ascii_size)
			{
				LCD_P6x8Str(x , y ,ch2,F6x8);	//显示字母
				x += 6;
			}
			
			ii+= 1;
		}
	}
} 

//==============================================================
//函数名： void Draw_BMP(uint8_t x,uint8_t y)
//功能描述：显示BMP图片128×64
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～63
//返回：无
//==============================================================
void Draw_BMP(uint8_t x,uint8_t y,const uint8_t *bmp)
{ 	
	uint8_t wm=0;
	uint8_t adder=0;
  
  	LCD_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)               
	{
		LCD_WrDat(*(bmp+adder));	
		adder += 1;
	}      
	LCD_Set_Pos(x,y + 8); 
	for(wm = 0;wm < 16;wm++)          
	{
		LCD_WrDat(*(bmp+adder));
		adder += 1;
	} 
 /* u32 ii=0;
  uint8_t x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
		LCD_Set_Pos(x0,y);				
    for(x=x0;x<x1;x++)
	    {      
	    	LCD_WrDat(bmp[ii++]);	    	
	    }
	}	*/
}


