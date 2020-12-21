/* PMSIS includes */
#include "pmsis.h"

/* Variables used. */
struct pi_device uart;
uint32_t count_cl[8];
/* Task executed by cluster cores. */
void cluster_helloworld(void *arg)
{
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    for(int i =0;i<200;i++)
    {
       count_cl[pi_core_id()] = count_cl[pi_core_id()] + pi_core_id();
    }
}

/* Cluster main entry, executed by core 0. */
void cluster_delegate(void *arg)
{
    printf("Cluster master core entry\n");
    /* Task dispatch to cluster cores. */
    pi_cl_team_fork(pi_cl_cluster_nb_cores(), cluster_helloworld, arg);
    printf("Cluster master core exit\n");
}

void calc(void *arg){
    uint32_t count = 0;
    for(int i =0;i<65535;i++)
    {
        count++;
    }
    printf("%d\n",count);
}

void helloworld(void)
{
    printf("Entering main controller\n");
    uint32_t errors = 0;
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("[%d %d] Hello World!\n", cluster_id, core_id);

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
        pmsis_exit(-1);
    }

    /* Prepare cluster task and send it to cluster. */
    struct pi_cluster_task cl_task = {0};
    cl_task.entry = cluster_delegate;
    cl_task.arg = (void*) 0x01;

    pi_task_t wait_task = {0};  //task to block the core until cluster finishes task

    pi_task_t calc_task = {0};  
    calc_task.id = PI_TASK_NONE_ID;
    calc_task.arg[0] = (uint32_t)(void *)calc;
    calc_task.arg[1] = 0x00;
    calc_task.implem.keep = 1;
    __rt_task_init(&calc_task);

    pi_task_block(&wait_task);
    pi_cluster_send_task_to_cl_async(&cluster_dev, &cl_task, &wait_task);
    pi_task_push(&calc_task);
  
    pi_task_wait_on(&calc_task);
    pi_task_wait_on(&wait_task);

    pi_cluster_close(&cluster_dev);
    
    for(int i =0;i<8;i++)
    {
        printf("cout3[%d]:%d\n",i,count_cl[i]);
    }

    printf("Test success !\n");

    pmsis_exit(errors);
}

/* Program Entry. */
int main(void)
{
    printf("\n\n\t *** PMSIS HelloWorld ***\n\n");
    return pmsis_kickoff((void *) helloworld);
}
