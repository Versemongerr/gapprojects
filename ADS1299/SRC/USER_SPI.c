#include "USER_SPI.h"
#include "pmsis.h"

/*
*	启动硬件SPI
*/
void SPI_INIT(void)
{
    struct pi_spi_conf conf;
    pi_spi_conf_init(&conf);
    conf.max_baudrate = 10000000;
    conf.cs = 0;
    conf.itf = 1;
    conf.wordsize = PI_SPI_CTRL_WORDSIZE_8;
    conf.phase = 1;
    conf.polarity = 0;

    pi_open_from_conf(&spi, &conf);
    pi_spi_open(&spi);
    printf("spi open success\n");
}

//--------------------------------------------OBSELETE----------------------------------------------

void SPI_GPIO_INIT()
{
    rt_gpio_init(0, SCLK);
    rt_gpio_init(0, MOSI);
    rt_gpio_init(0, MISO);
    rt_gpio_init(0, CS);

    rt_gpio_set_dir(0, 1 << MISO, RT_GPIO_IS_IN);

    rt_gpio_set_dir(0, 1 << SCLK, RT_GPIO_IS_OUT);
    rt_gpio_set_dir(0, 1 << MOSI, RT_GPIO_IS_OUT);
    rt_gpio_set_dir(0, 1 << CS, RT_GPIO_IS_OUT);

    SPI_CS_HIGH;
    //    SPI_SCLK_HIGH;
}

uint8_t ADS_SPI(uint8_t data)
{
    SPI_CS_LOW;
    uint8_t i;
    SPI_SCLK_LOW; //先将时钟线拉低
                  //   rt_time_wait_us(1);
    for (i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0x80) //从高位发送
        {
            SPI_MOSI_HIGH;
            //           rt_time_wait_us(1);
        }
        else
        {
            SPI_MOSI_LOW;
            //           rt_time_wait_us(1);
        }

        SPI_SCLK_HIGH; //将时钟线拉高，在时钟上升沿，数据发送到从设备
                       //       rt_time_wait_us(1);
        data <<= 1;

        if (SPI_MISO_READ) //读取从设备发射的数据
        {
            data |= 0x01;
            //          rt_time_wait_us(1);
        }
        SPI_SCLK_LOW; //在下降沿数据被读取到主机
                      //       rt_time_wait_us(1);
    }
    // SPI_CS_HIGH;
    return data; //返回读取到的数据
}

//---------------------------------------------------------------OBSELETE-------------------------------------------------------
