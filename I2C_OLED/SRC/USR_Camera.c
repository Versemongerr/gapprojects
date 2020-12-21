#include "USR_Camera.h"


int open_camera(struct pi_device *device)
{
  printf("Opening Himax camera\n");
  struct pi_himax_conf cam_conf;
  pi_himax_conf_init(&cam_conf);

  cam_conf.format = PI_CAMERA_QQVGA;

  pi_open_from_conf(device, &cam_conf);
  if (pi_camera_open(device))
    return -1;

  return 0;
}

