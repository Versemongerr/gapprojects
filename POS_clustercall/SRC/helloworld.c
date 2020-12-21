#include "rt/rt_api.h"

#define STACK_SIZE 2048
#define CID 0

int done = 0;

static void testtask1(void *arg)
{
    printf("task1 is running in cluser 0x%2x ,core %d, param %d\n", rt_cluster_id(), rt_core_id(), arg);
    done++;
}

static void testtask2(void *arg)
{
    printf("task2 is running in cluser 0x%2x ,core %d, param %d\n\n", rt_cluster_id(), rt_core_id(),arg);
}

static void cluster_entry(void *arg)
{
    printf("cluster 0x%2x core 0 called\n\n", rt_cluster_id());
    rt_team_fork(4, testtask2, (void *)0x1);
}

int main(void)
{
    printf("entering main controller\n\n");
    rt_event_sched_t *p_sched = rt_event_internal_sched(); //get a pointer to the default scheduler created when the runtime starts

    if (rt_event_alloc(p_sched, 4))
        return -1;

    rt_event_t *p_event = rt_event_get(NULL, testtask1, (void *)CID);
    
    rt_cluster_mount(1, CID, 0, NULL);

    void *stacks = rt_alloc(RT_ALLOC_CL_DATA, STACK_SIZE * rt_nb_pe());

    if (stacks == NULL)
        return -1;


    rt_cluster_call(NULL, CID, cluster_entry, NULL, NULL, 0, 0, rt_nb_pe(), p_event);

    while (done<2)
    
        rt_event_execute(p_sched, 1);

    rt_cluster_mount(0, CID, 0, NULL);

    return (0);
}
