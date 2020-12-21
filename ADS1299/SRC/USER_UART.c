#include "USER_UART.h"


void usart_init(void)
{
    struct pi_uart_conf conf;

    pi_uart_conf_init(&conf);

    conf.baudrate_bps = 115200;
    conf.enable_rx = 0;
    conf.enable_tx = 1;

    pi_open_from_conf(&uart, &conf);

    if (pi_uart_open(&uart))
    {
        printf("\n\t open usart failed \n\n");
        pmsis_exit(-1);
    }
}
