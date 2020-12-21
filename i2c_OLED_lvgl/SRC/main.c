#include "pmsis.h"
#include "rt/rt_api.h"
#include "OLED.h"
#include "lvgl.h"

struct pi_device gpio;
pi_gpio_e gpio_in = PI_GPIO_A0_PAD_12_A3;


static void arc_loader(lv_task_t *t)
{
    static int16_t a = 0;

    a += 30;
    if (a >= 359)
        a = 359;

    if (a < 180)
        lv_arc_set_angles(t->user_data, 180 - a, 180);
    else
        lv_arc_set_angles(t->user_data, 540 - a, 180);

    if (a == 359)
    {
        lv_arc_set_angles(t->user_data, 0, 0);
        a = 0;
        // lv_task_del(t);
        return;
    }
}

void helloworld_lvgl()
{
    lv_obj_t *ta1, *arc, *bar1, *btn1,*label,*preload,*gauge,*label2;

    ta1 = lv_ta_create(lv_scr_act(), NULL);
    lv_obj_set_size(ta1, 64, 40);
    lv_obj_align(ta1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_ta_set_cursor_type(ta1, LV_CURSOR_NONE);
    lv_ta_set_text(ta1, "Helloworld LvGL"); /*Set an initial text*/

    arc = lv_arc_create(lv_scr_act(), NULL);
    lv_arc_set_angles(arc, 0, 5);
    lv_obj_set_size(arc, 16, 16);
    lv_obj_align(arc, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

    lv_task_create(arc_loader, 1, LV_TASK_PRIO_LOWEST, arc);

    bar1 = lv_bar_create(lv_scr_act(), NULL);
    lv_obj_set_size(bar1, 64, 16);
    lv_obj_align(bar1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_bar_set_anim_time(bar1, 10000);
    lv_bar_set_value(bar1, 100, LV_ANIM_ON);

    btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_size(btn1, 60, 20);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Button");
    
    // lv_btn_set_toggle(btn1,true);
    // lv_btn_set_fit2(btn1,LV_FIT_NONE,LV_FIT_TIGHT);


    preload = lv_preload_create(lv_scr_act(),NULL);
    lv_obj_set_size(preload,16,16);
    lv_obj_align(preload,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,0);
    lv_preload_set_spin_time(preload,3000);
    lv_preload_set_arc_length(preload,270);
    lv_preload_set_type(preload,LV_PRELOAD_TYPE_FILLSPIN_ARC);


    label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC);     /*Circular scroll*/
    lv_obj_set_width(label2, 60);
    lv_label_set_text(label2, "This is a scrolling text in LittlevGL. Created by Versemonger @ 2020.5.13 21:35 ");
    lv_obj_align(label2, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, -25);

    // gauge = lv_gauge_create(lv_scr_act(),NULL);
    // lv_obj_set_size(gauge, 60, 24);
    // lv_obj_align(gauge, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, -23);
    // lv_gauge_set_value(gauge,0,30);

    // lv_anim_t a;
    // lv_anim_set_exec_cb(&a, btn1, lv_bar_set_value); /*Set the animator function and variable to animate*/
    // lv_anim_set_time(&a, 10000, 10);
    // lv_anim_set_values(&a, 0, 100);           /*Set start and end values. E.g. 0, 150*/
    // lv_anim_set_path_cb(&a, lv_anim_path_linear); /*Set path from `lv_anim_path_...` functions or a custom one.*/
    // lv_anim_set_playback(&a, 10);          /*Enable playback of teh animation with `wait_time` delay*/
    // lv_anim_set_repeat(&a, 10);            /*Enable repeat of teh animation with `wait_time` delay. Can be compiled with playback*/

    // lv_anim_create(&a); /*Start the animation*/
}

static void timer_handle(void *arg)
{

    lv_tick_inc(1);
    //   printf("[%d] Entered user handler\n", rt_time_get_us());
    //   nb_ticks++;
}


bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t *data)
{
    static uint32_t last_btn = 0;   /*Store the last pressed button*/
    int btn_pr ; 
    pi_gpio_pin_read(&gpio, gpio_in, &btn_pr);;     /*Get the ID (0,1,2...) of the pressed button*/
    if(btn_pr >= 0) {               /*Is there a button press? (E.g. -1 indicated no button was pressed)*/
       last_btn = btn_pr;           /*Save the ID of the pressed button*/
       data->state = LV_INDEV_STATE_PR;  /*Set the pressed state*/
    } else {
       data->state = LV_INDEV_STATE_REL; /*Set the released state*/
    }

    data->btn_id = last_btn;            /*Save the last button*/

    return false;                    /*No buffering now so no more data read*/
}

void my_disp_flush(lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    int32_t x, y;
    for (y = area->y1; y <= area->y2; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            // set_pixel(x, y, *color_p);  /* Put a pixel to the display.*/
            // uint8_t OLED_PIX = color_p->full;
            OLED_DrawPoint(x, y, lv_color_to1(*color_p));
            color_p++;
        }
    }
    OLED_Refresh_Gram();
    lv_disp_flush_ready(disp); /* Indicate you are ready with the flushing*/
}

void OLED_Test(void)
{
    printf("entering main controller\n");



    uint32_t value = 0;

    rt_freq_set(__RT_FREQ_DOMAIN_FC, 250000000);

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
    OLED_DrawPoint(0, 0, 1);
    OLED_Refresh_Gram();
    rt_time_wait_us(10000);
    OLED_CLS();

    struct pi_gpio_conf gpio_conf = {0};
    pi_gpio_conf_init(&gpio_conf);
    pi_open_from_conf(&gpio, &gpio_conf);
    pi_gpio_open(&gpio);
    pi_gpio_flags_e cfg_flags = PI_GPIO_INPUT | PI_GPIO_PULL_DISABLE | PI_GPIO_DRIVE_STRENGTH_LOW;
    pi_gpio_pin_configure(&gpio, gpio_in, cfg_flags);
    value = pi_gpio_pin_read(&gpio, gpio_in, &value);
    printf("GPIO opened, in val: %d\n", value);

    rt_timer_t timer;

    // Create a periodic timer
    if (rt_timer_create(&timer, RT_TIMER_PERIODIC, rt_event_get(NULL, timer_handle, (void *)&timer)))
        return -1;

    lv_init();

    static lv_disp_buf_t disp_buf;
    static lv_color_t buf[LV_HOR_RES_MAX * 10];
    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    lv_disp_drv_t disp_drv;            /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);       /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush; /*Set your driver function*/
    disp_drv.buffer = &disp_buf;       /*Assign the buffer to the display*/
    lv_disp_drv_register(&disp_drv);   /*Finally register the driver*/


    

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);           /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_BUTTON;                     /*See below.*/
    indev_drv.read_cb = my_input_read; /*See below.*/
        /*Register the driver in LittlevGL and save the created input device object*/
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv);

    const lv_point_t points_array[] =  { {98,50}};
    lv_indev_set_button_points(my_indev,points_array);


    // Start it. This will call the function we provided everytime the
    // specified period is reached.
    rt_timer_start(&timer, 1000);
    lv_theme_mono_init(0, &lv_font_unscii_8);
    lv_theme_set_current(lv_theme_get_mono());

    // lv_ex_calendar_1();

    helloworld_lvgl();

    while (1)
    {
        lv_task_handler();
        rt_time_wait_us(5000);
        pi_yield();
    }

    return 0;
    // }
}

void main(void)
{
    printf("\n\n\t *** I2C OLED lvgl Demo ***\n\n");
    return pmsis_kickoff((void *)OLED_Test);
}
