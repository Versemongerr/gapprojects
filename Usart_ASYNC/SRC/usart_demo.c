
#include "pmsis.h"
#include "usr_usart.h"

char helloworld[] = "hello world\n";

void usart_demo()
{

    usart_init();

    pi_task_t task = {0};

    USR_UART_BUFFER = (char *)pi_l2_malloc((sizeof(char)*(USR_BUFFER_SIZE)));

    pi_task_callback(&task, (void *)usart_tx_call_back, &task);
    pi_uart_write_async(&uart, helloworld, 12, &task);

    while(1)
    {
        pi_yield();
    }

    pmsis_exit(0);
}

/* Program Entry. */
int main(void)
{
    printf("\n\n\t *** Usart Demo ***\n\n");
    return pmsis_kickoff((void *)usart_demo);
}
