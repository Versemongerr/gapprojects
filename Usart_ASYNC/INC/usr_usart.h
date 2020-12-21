
#include "pmsis.h"

#define USR_BUFFER_SIZE 10

struct pi_device uart;
char *USR_UART_BUFFER;

void usart_init(void);
void usart_rx_call_back(void *arg);
void usart_tx_call_back(void *arg);

