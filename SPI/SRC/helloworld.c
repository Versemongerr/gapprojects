//#include "rt/rt_api.h"
#include "pmsis.h"

struct pi_device spi;

uint8_t helloworld[]={0x09,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
uint8_t rx_buf[8];
 
void user_spi()
{
    printf("entering main controller\n");

    struct pi_spi_conf conf;

    pi_spi_conf_init(&conf);
//    conf.max_baudrate = 5000000;
    conf.cs = 0;
    conf.itf = 1;
    conf.wordsize = PI_SPI_CTRL_WORDSIZE_8;

    pi_open_from_conf(&spi, &conf);
    pi_spi_open(&spi);

    
    pi_spi_transfer_async(&spi, (void*)helloworld,(void*)rx_buf,8*8,PI_SPI_CS_AUTO|PI_SPI_LINES_SINGLE,NULL);
    while(1)
    {
        //pi_spi_transfer_async(&spi, (void*)helloworld,(void*)rx_buf,8*8,PI_SPI_CS_AUTO|PI_SPI_LINES_SINGLE,NULL);
    }

    pmsis_exit(0);
}

int main(void)
{
    printf("\n\n\t *** spi Demo ***\n\n");
    return pmsis_kickoff((void *)user_spi);
}
