#include "pmsis.h"
#include "OLED.h"
#include "USR_Camera.h"

static unsigned char *img_buf;
// static unsigned char img_buf[];

void OLED_Test(void)
{
    printf("entering main controller\n");

    img_buf = pmsis_l2_malloc(164 * 124);

    pi_i2c_conf_t i2c_conf;
    pi_i2c_conf_init(&i2c_conf);

    i2c_conf.itf = 0;
    i2c_conf.max_baudrate = 100000;
    i2c_conf.cs = (uint32_t)OLED_ADDRESS;

    pi_open_from_conf(&I2C, &i2c_conf);

    if (pi_i2c_open(&I2C))
    {
        printf("Failed to open i2c\n");
        pmsis_exit(-1);
    }

    OLED_Init();

    OLED_Fill(0xff);
    rt_time_wait_us(10000);
    OLED_CLS();


    
    // OLED_DrawLine(1,30,30,30);
    if (open_camera(&camera))
    {
        printf("Failed to open camera\n");
        pmsis_exit(-1);
    }
    else
        printf("Camera opened\n");


    int count = 0;
    // int i;
    OLED_DrawPoint(64,32,1);
    OLED_DrawPoint(0,0,1);
    OLED_Refresh_Gram();
    // OLED_Fill_Block(64,0,127,63,1);
    // while(i<128)
    // {
    //     OLED_DrawPoint(i,50,1);
    //     i++;
    // }
    // OLED_Refresh_Gram();
    // for (int i; i < 164 * 124; i++)
    // {
    while (count < 10)
    {
        pi_camera_control(&camera, PI_CAMERA_CMD_START, 0);

        pi_camera_capture_async(&camera, (void *)img_buf, 164 * 124, NULL);
        printf("0x%02x\n", img_buf[164 * 61 + 82]);
        pi_camera_control(&camera, PI_CAMERA_CMD_STOP, 0);

        // OLED_DrawBMP(0, 0, 128, 64, img_buf);
        // OLED_CLS();
        count++;

        // for(int i=0;i<128*64;i++)
        // {

        // }
    }

    return 0;
    // }
}

void main(void)
{
    printf("\n\n\t *** I2C OLED Demo ***\n\n");
    return pmsis_kickoff((void *)OLED_Test);
}
