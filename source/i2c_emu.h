#include "stm32f0xx.h"
typedef enum{
  ACK  = 0,
  NACK = 1,
}etI2cAck;

void i2c_GPIO_cfg(void);
void i2c_SDA_IN(void);
void i2c_SDA_OUT(void);
void delay_us(uint16_t time);
void i2c_start(void);
void i2c_stop(void);
void i2c_ack(void);
void i2c_noack(void);
uint8_t i2c_waitack(void);
uint8_t i2c_read_byte(void);
void i2c_write_byte(uint8_t);

void i2c2_GPIO_cfg(void);
void i2c2_SDA_IN(void);
void i2c2_SDA_OUT(void);
void delay_us(uint16_t time);
void i2c2_start(void);
void i2c2_stop(void);
void i2c2_ack(void);
void i2c2_noack(void);
uint8_t i2c2_waitack(void);
uint8_t i2c2_read_byte(void);
void i2c2_write_byte(uint8_t);
