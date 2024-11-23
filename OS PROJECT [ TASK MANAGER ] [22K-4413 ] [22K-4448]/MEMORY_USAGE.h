#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *mem_usage(void *arg) 
{
	//opening file in read mode to enable reading from file
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) 
    {
        perror("Error opening file");
        exit(1);
    }
    double total = 0, free = 0, buffers = 0, cached = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) 
    {
        char key[64];
        double value;
        //compare if key equals total memory, free memory , buffer, cache if match found values are set
        if (sscanf(line, "%s %lf", key, &value) == 2) 
        {
            if (strcmp(key, "MemTotal:") == 0) 
            {
                total = value;
            } 
            else if (strcmp(key, "MemFree:") == 0) 
            {
                free = value;
            } 
            else if (strcmp(key, "Buffers:") == 0) 
            {
                buffers = value;
            } 
            else if (strncmp(key, "Cached:", 6) == 0) 
            {
                cached += value;
            }
        }
    }
    fclose(file);
    FILE *sys_monitor= fopen("system_monitor.txt", "a");
    FILE *m= fopen("memory_usage.txt", "w");
    if (sys_monitor == NULL) {
        perror("Error opening file");
        exit(1);
    }	
    if (m == NULL) {
        perror("Error opening file");
        exit(1);
    }	
    char *x="______________________________";
    
    //calculating memory usage
    float memory_usage= (total - (free + buffers + cached)) / 1000;
    
    //writing calculated isk usage to files and console
    
    printf("\n%sM E M O R Y     U S A G E%s\n",x,x); 
    fprintf(sys_monitor,"%sMemory Usage%s\n", x,x); 
    fprintf(sys_monitor,"Memory usage: %.0f\n", memory_usage); 
    fprintf(m,"%sMemory Usage%s\n", x,x); 
    fprintf(m,"Memory usage: %.0f\nFree Memory: %.0f\nBuffers: %.0f\nCache: %.0f\nTotal Memory: %.0f\n", memory_usage,free,buffers,cached,total);
    printf("Memory usage: %.0f\nFree Memory: %.0f\nBuffers: %.0f\nCache: %.0f\nTotal Memory: %.0f\n", memory_usage,free,buffers,cached,total);
    fprintf(sys_monitor,"Memory usage: %.0f\nFree Memory: %.0f\nBuffers: %.0f\nCache: %.0f\nTotal Memory: %.0f\n", memory_usage,free,buffers,cached,total);
    fclose(sys_monitor);
    fclose(m);
}
