void mb_SendData(uint8_t *buff,uint8_t len);
void mb_receiveData(uint8_t *buff);

void mb_TIM16_cfg(void);
void mb_USART_cfg(void);
void usart1_GPIO_cfg(void);

void TIM16_IRQHandler(void);
void USART1_IRQHandler(void);
