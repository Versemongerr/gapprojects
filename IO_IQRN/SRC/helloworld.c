#include "rt/rt_api.h"

#define GPIO 0

int n;

static void gpio_handler(void *arg)
{
    n++;
    if(n%2==0)
    {rt_gpio_set_pin_value(0,3,1);}
    else
    {
        //rt_gpio_set_pin_value(0,3,0);
    }
    
    
}

int main(void)
{
    printf("entering main controller\n\n");

    // rt_padframe_profile_t *profile_gpio = rt_pad_profile_get("hyper_gpio");

    // if (profile_gpio == NULL) {
    //     printf("pad config error\n");
    //     return 1;
    // }
    // rt_padframe_set(profile_gpio);

    // GPIO initialization
    rt_gpio_init(0, GPIO);
    rt_gpio_init(0, 3);

    // Configure GPIO as an inpout
    rt_gpio_set_dir(0, 1<<GPIO, RT_GPIO_IS_IN);
    rt_gpio_set_dir(0, 1<<3, RT_GPIO_IS_OUT);

    rt_gpio_set_sensitivity(0, GPIO, RT_GPIO_SENSITIVITY_RISE);

    rt_gpio_set_event(0, GPIO, rt_event_irq_get(gpio_handler, (void *)GPIO));


    while(1)
    {
        rt_event_yield(NULL);
    }


    
    return (0);
}
