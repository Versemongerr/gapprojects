#include "rt/rt_api.h"
#include "ADS1299.h"
#include "USER_SPI.h"
#include "USER_UART.h"

#define GPIO 5

uint8_t DATA_buffer[27] = {0x00};

uint8_t Data_Trans[12 + 24 + 1] = {0xa0, 0x10, 0x21};
uint8_t stop = 0xc0;

// int done =0;

uint8_t helloworld[] = {0x06};
uint8_t rx_buf[1];
float DATA_out[8] = {0x00};

//-------------------------------------------------Interrupt Handler-----------------------------------------------------------
static void gpio_handler(void *arg)
{
    ADS_Read(DATA_buffer);
    for (int i = 0; i < 24; i++)
    {
        Data_Trans[12 + i] = DATA_buffer[i + 3];
        //          printf("0x%02x\r\n", Data_Trans[12 + i]);
    }
    Data_Trans[12 + 24] = stop;
    // done = 1;
    pi_uart_write_async(&uart, (void *)Data_Trans, 12 + 24 + 1, NULL);

    //    done =1;

    // for (int i = 0; i < 9; i++)
    // {
    //     if (i != 0)
    //     {
    //         if (DATA_buffer[i * 3] >> 7 == 1)
    //         {
    //             uint32_t buffer;
    //             buffer = DATA_buffer[i * 3] * 65536 + DATA_buffer[i * 3 + 1] * 256 + DATA_buffer[i * 3 + 2] - 1;
    //             //
    //             //				DATA_buffer[i*3] =~ buffer&0x007fffff>>16;
    //             //				DATA_buffer[i*3] =~ buffer>>8;
    //             //				DATA_buffer[i*3] =~ buffer;
    //             //
    //             DATA_buffer[i * 3] = ~(DATA_buffer[i * 3] << 1) >> 1;
    //             DATA_buffer[i * 3 + 1] = ~DATA_buffer[i * 3 + 1];
    //             DATA_buffer[i * 3 + 2] = ~DATA_buffer[i * 3 + 2];

    //             //HAL_UART_Transmit(&huart1,DATA_buffer+3,3,100);

    //             DATA_out[i - 1] = 0.0f - (DATA_buffer[i * 3] * 65536 + DATA_buffer[i * 3 + 1] * 256 + DATA_buffer[i * 3 + 2] + 1) * 4.5f / 8388607.0f;
    //         }
    //         else
    //         {
    //             //HAL_UART_Transmit(&huart1,DATA_buffer+3,3,100);
    //             DATA_out[i - 1] = (DATA_buffer[i * 3] * 65536 + DATA_buffer[i * 3 + 1] * 256 + DATA_buffer[i * 3 + 2] + 1) * 4.5f / 8388607.0f;
    //         }
    //         //printf("CH%d: %f\r\n",i,DATA_out[i-1]);
    //     }
    // }
    // printf("%f\r\n", DATA_out[8 - 1]);
}

//----------------------------------------------------MAIN Entry------------------------------------------------------------------
int main(void)
{
    printf("entering main controller\n");

    // rt_freq_set(__RT_FREQ_DOMAIN_FC,250000000);
    printf("Soc FC frequency: %d\n", rt_freq_get(__RT_FREQ_DOMAIN_FC));

    //---------------------------------------------------Start Scheduler----------------------------------------------------------
    // rt_event_sched_t *p_sched = rt_event_internal_sched(); //get a pointer to the default scheduler created when the runtime starts

    // if (rt_event_alloc(p_sched, 4))
    //     return -1;

    //---------------------------------------------------INIT SPI&1299-------------------------------------------------------------------

    SPI_GPIO_INIT();
    //  SPI_INIT();

    //rt_event_t *p_event = rt_event_get(NULL,ADS1299_Check,NULL);
    ADS1299_Check();
    //rt_event_push(p_event);
    ADS1299_Init();

    //----------------------------------------------------INIT USART-----------------------------------------------------------------

    usart_init();

    printf("usart open successed\n");

    //---------------------------------------------------INIT GPIO--------------------------------------------------------------
    // rt_padframe_profile_t *profile_gpio = rt_pad_profile_get("hyper_gpio");

    // if (profile_gpio == NULL) {
    //     printf("pad config error\n");
    //     return 1;
    // }
    // rt_padframe_set(profile_gpio);

    // GPIO initialization
    rt_gpio_init(0, GPIO);
    //   rt_gpio_init(0, 13);

    // Configure GPIO as an inpout
    rt_gpio_set_dir(0, 1 << GPIO, RT_GPIO_IS_IN);
    //   rt_gpio_set_dir(0, 1 << 13, RT_GPIO_IS_OUT);

    rt_gpio_set_sensitivity(0, GPIO, RT_GPIO_SENSITIVITY_FALL);
    rt_gpio_set_event(0, GPIO, rt_event_irq_get(gpio_handler, (void *)GPIO));

    while (1)
    {
        //     //ADS1299_Check();
        //     // ADS1299_Init();
        //     //pi_spi_transfer_async(&spi, (void*)helloworld,(void*)rx_buf,8,PI_SPI_CS_AUTO|PI_SPI_LINES_SINGLE,NULL);
        //     //rt_event_execute(p_sched,1);
        rt_event_yield(NULL);
    }

    return (0);
}
