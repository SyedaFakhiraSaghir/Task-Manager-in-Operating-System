#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *gpu_usage(void *arg)
{
    printf("\033[43m");
    printf("------------------------GPU USAGE------------------------");
    system("sudo lshw -C display");
    system("sudo lshw -C display > gpu_usage.txt ");
    system("sudo lshw -C display > system_monitor.txt ");
}
