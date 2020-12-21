
#include "usr_usart.h"

void usart_init(void)
{
    struct pi_uart_conf conf;

    pi_uart_conf_init(&conf);

    conf.baudrate_bps = 115200;
    conf.enable_rx = 1;
    conf.enable_tx = 1;

    pi_open_from_conf(&uart, &conf);

    if (pi_uart_open(&uart))
    {
        printf("\n\t open usart failed \n\n");
        pmsis_exit(-1);
    }
}

void usart_rx_call_back(void *arg)
{
    printf("\n\t task  rx in \n\n");

    pi_task_t *task = (pi_task_t *)arg;
    pi_task_callback(task, (void *)usart_tx_call_back, task);
    if (pi_uart_write_async(&uart, USR_UART_BUFFER, USR_BUFFER_SIZE, task))
    {
        printf("\n\t error write \n");
        pmsis_exit(-1);
    }
}

void usart_tx_call_back(void *arg)
{
    printf("\n\t task  tx in \n\n");

    pi_task_t *task = (pi_task_t *)arg;
    pi_task_callback(task, (void *)usart_rx_call_back, task);
    if (pi_uart_read_async(&uart, USR_UART_BUFFER, USR_BUFFER_SIZE, task))
    {
        printf("\n\t error read \n");
        pmsis_exit(-1);
    }
    else
    {
        printf("%s", USR_UART_BUFFER);
    }
}
