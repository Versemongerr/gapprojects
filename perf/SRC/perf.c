
/* PMSIS includes */
#include "pmsis.h"
#include "bsp/bsp.h"

/* User includes */
#include "usr_fft.h"

/* User defines */ 
#define ASYNC 1
#define LOOP_SIZE 65535*128

/* Variables used. */
struct pi_device uart;
PI_L2 uint32_t perf_values[ARCHI_CLUSTER_NB_PE] = {0};
uint32_t count_cl[8];
uint32_t fc_perf ;
uint32_t count = 0;
/* Task executed by cluster cores. */
void cluster_helloworld(void *arg)
{
    pi_perf_conf(1 << PI_PERF_ACTIVE_CYCLES);
    pi_perf_start();

    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();

    for(uint64_t i =0;i<LOOP_SIZE;i++)
    {
       count_cl[core_id] = count_cl[core_id] + core_id;
    }
    pi_cl_team_barrier();
    pi_perf_stop();
    perf_values[core_id] = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
    // printf(".");
}

/* Cluster main entry, executed by core 0. */
void cluster_delegate(void *arg)
{
    printf("Cluster master core entry\n");
    /* Task dispatch to cluster cores. */
    // pi_cl_team_fork(pi_cl_cluster_nb_cores(), cluster_helloworld, arg);
    for(int16_t i =1;i<9;i++){
        pi_cl_team_fork(i, cluster_helloworld, arg);
        // printf("%d\n",i);
    }
    printf("Cluster master core exit\n");
}

/* do some thing */
void calc(void *arg){
    pi_perf_start();
    for(uint64_t i =0;i<LOOP_SIZE;i++)
    {
        count++;
    }
    // printf("%d\n",count);
    pi_perf_stop();
}

void helloworld(void){
    /* Initialize the GPIO for trigger and LED */
    pi_gpio_pin_configure(NULL, PI_GPIO_A3_PAD_15_B1, PI_GPIO_OUTPUT);
    
    pi_perf_conf(1 << PI_PERF_CYCLES | 1 << PI_PERF_ACTIVE_CYCLES);
    printf("Entering main controller\n");
    uint32_t errors = 0;
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();

    struct pi_device cluster_dev = {0};
    struct pi_cluster_conf cl_conf = {0};

    /* Init cluster configuration structure. */
    pi_cluster_conf_init(&cl_conf);
    cl_conf.id = 0;                /* Set cluster ID. */
    /* Configure & open cluster. */
    pi_open_from_conf(&cluster_dev, &cl_conf);
    if (pi_cluster_open(&cluster_dev))
    {
        printf("Cluster open failed !\n");
        pmsis_exit(-2);
    }

    /* Prepare cluster task and send it to cluster. */
    struct pi_cluster_task cl_task = {0};
    cl_task.entry = cluster_delegate;
    cl_task.arg = NULL;

    /* Prepare FC task */
    pi_task_t calc_task = {0};  
    calc_task.id = PI_TASK_NONE_ID;
    calc_task.arg[0] = (uint32_t)(void *)calc;
    calc_task.arg[1] = 0x00;
    calc_task.implem.keep = 1;
    __rt_task_init(&calc_task);

#ifdef ASYNC

    /* Blocking task*/
    pi_task_t wait_task = {0};
    pi_task_block(&wait_task);

    /* Trigger the power test */
    pi_gpio_pin_write(NULL, PI_GPIO_A3_PAD_15_B1, 1);
    // pi_time_wait_us(2000000);

    pi_task_push(&calc_task);
    pi_cluster_send_task_to_cl_async(&cluster_dev, &cl_task, &wait_task);
    
    /* set a barrier to force syncing Cluster and FC */
    pi_task_wait_on(&calc_task);
    pi_task_wait_on(&wait_task);    

    // pi_time_wait_us(2000000);

    // pi_task_push(&calc_task);
    // pi_cluster_send_task_to_cl_async(&cluster_dev, &cl_task, &wait_task);
    
    // /* set a barrier to force syncing Cluster and FC */
    // pi_task_wait_on(&calc_task);
    // pi_task_wait_on(&wait_task);    

   /* Stop the power test */
    pi_gpio_pin_write(NULL, PI_GPIO_A3_PAD_15_B1, 0);

#else
    /* Trigger the power test */
    pi_gpio_pin_write(NULL, PI_GPIO_A3_PAD_15_B1, 1);

    pi_task_push(&calc_task);
    pi_cluster_send_task_to_cl(&cluster_dev, &cl_task);
    
    /* force syncing */
    pi_task_wait_on(&calc_task);

#endif    

    pi_cluster_close(&cluster_dev);

    // pi_perf_stop();

    //  for(int i =0;i<8;i++)
    // {
    //     printf("cout3[%d]:%d\n",i,count_cl[i]);
    // }

    fc_perf = pi_perf_read(PI_PERF_ACTIVE_CYCLES);

    uint32_t cycles = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
    uint32_t tim_cycles = pi_perf_read(PI_PERF_CYCLES);
    printf("Perf : %d cycles Timer : %d cycles\n", cycles, tim_cycles);
    printf("[%d %d] Perf : %d cycles\n", cluster_id, core_id, fc_perf);
    for (uint32_t i = 0; i < (uint32_t) ARCHI_CLUSTER_NB_PE; i++)
    {
        printf("[%d %d] Perf : %d cycles\n", 0, i, perf_values[i]);
    }

    int32_t cur_fc_freq = pi_freq_get(PI_FREQ_DOMAIN_FC);
    int32_t cur_cl_freq = pi_freq_get(PI_FREQ_DOMAIN_CL);

    printf("FC frequency : %ld\nCL frequency : %ld\n", cur_fc_freq, cur_cl_freq);
    
    printf("Test success !\n");

    pmsis_exit(errors);
}

/* Program Entry. */
int main(void)
{
    printf("\n\n\t *** PMSIS HelloWorld ***\n\n");
    return pmsis_kickoff((void *) helloworld);
}

