#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define CPU_INFO_FILE "/proc/stat"

//function to read stats from the file
void get_cpu_stats(unsigned long long *user, unsigned long long *nice, unsigned long long *system, unsigned long long *idle) {
    FILE *fp;
    char line[128];
    
    //open file in read mode
    fp = fopen(CPU_INFO_FILE, "r");
    
    //if there is an error in file opening exit
    if (fp == NULL) 
    {
        perror("Error opening CPU info file");
        exit(EXIT_FAILURE);
    }
    //reading from the file
    fgets(line, sizeof(line), fp);
    sscanf(line, "cpu %llu %llu %llu %llu", user, nice, system, idle);
    
    fclose(fp);
}

void *cpu_utilization(void *arg) 
{
    unsigned long long user1, nice1, system1, idle1, user2, nice2, system2, idle2, delta_used, delta_idle, delta_total;
    unsigned int cpu_usage;
    
    FILE *f, *sys_monitor;
    f = fopen("cpu_utilization.txt", "w");
    sys_monitor = fopen("system_monitor.txt", "w");
    if (f == NULL) 
    {
        perror("Error opening CPU info file");
        exit(EXIT_FAILURE);
    }
    if (sys_monitor == NULL) 
    {
        perror("Error opening CPU info file");
        exit(EXIT_FAILURE);
    }
    
    //to get the cpu statistics 
    get_cpu_stats(&user1, &nice1, &system1, &idle1);
    sleep(1);
    
    //calculating cpu usage
    get_cpu_stats(&user2, &nice2, &system2, &idle2);
    delta_used = (user2 - user1) + (nice2 - nice1) + (system2 - system1);
    delta_total = delta_used + (idle2 - idle1);
    cpu_usage = (int)((delta_used * 100) / delta_total);
    char *str="_____________________________________________";
    
    //writing the cpu usage to the file and console
    fprintf(f,"%sCPU USAGE%s\n", str,str);
    fprintf(f,"CPU usage at %d%%\n", cpu_usage);
    fprintf(sys_monitor,"%sCPU USAGE%s\n", str,str);
    fprintf(sys_monitor,"CPU usage at %d%%\n", cpu_usage);
    printf("\033[0;37;44mCPU usage at %d%%\033[0m\n", cpu_usage);
    fclose(f);
    fclose(sys_monitor);
}
