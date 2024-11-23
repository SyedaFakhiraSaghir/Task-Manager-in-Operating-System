#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h> 

void *system_information(void *arg) 
{
	struct sysinfo info;
	if (sysinfo(&info) != 0)
	{
	    perror("sysinfo");
	    exit(EXIT_FAILURE);
	}
	FILE *f, *f1;
	f = fopen("system_info.txt", "a"); // Opening file for appending
	if (f == NULL) 
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	f1 = fopen("system_info.txt", "w"); // Opening file for appending
	if (f1 == NULL) 
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	fprintf(f, "Uptime: %ld seconds\n", info.uptime);
	fprintf(f, "1 minute load average: %lu\n", info.loads[0]);
	fprintf(f, "5 minute load average: %lu\n", info.loads[1]);
	fprintf(f, "15 minute load average: %lu\n", info.loads[2]);
	fprintf(f, "Total RAM: %lu bytes\n", info.totalram);
	fprintf(f, "Free RAM: %lu bytes\n", info.freeram);
	fprintf(f, "Shared RAM: %lu bytes\n", info.sharedram);
	fprintf(f, "Buffer RAM: %lu bytes\n", info.bufferram);
	fprintf(f, "Total swap space: %lu bytes\n", info.totalswap);
	fprintf(f, "Free swap space: %lu bytes\n", info.freeswap);
	fprintf(f, "Number of processes: %u\n", info.procs);
	fprintf(f, "Total high memory size: %lu bytes\n", info.totalhigh);
	fprintf(f, "Available high memory size: %lu bytes\n", info.freehigh);
	fprintf(f, "Memory unit size: %u bytes\n", info.mem_unit);
	//output in main file
	fprintf(f1, "Uptime: %ld seconds\n", info.uptime);
	fprintf(f1, "1 minute load average: %lu\n", info.loads[0]);
	fprintf(f1, "5 minute load average: %lu\n", info.loads[1]);
	fprintf(f1, "15 minute load average: %lu\n", info.loads[2]);
	fprintf(f1, "Total RAM: %lu bytes\n", info.totalram);
	fprintf(f1, "Free RAM: %lu bytes\n", info.freeram);
	fprintf(f1, "Shared RAM: %lu bytes\n", info.sharedram);
	fprintf(f1, "Buffer RAM: %lu bytes\n", info.bufferram);
	fprintf(f1, "Total swap space: %lu bytes\n", info.totalswap);
	fprintf(f1, "Free swap space: %lu bytes\n", info.freeswap);
	fprintf(f1, "Number of processes: %u\n", info.procs);
	fprintf(f1, "Total high memory size: %lu bytes\n", info.totalhigh);
	fprintf(f1, "Available high memory size: %lu bytes\n", info.freehigh);
	fprintf(f1, "Memory unit size: %u bytes\n", info.mem_unit);
	//output on console
	printf("Uptime: %ld seconds\n", info.uptime);
	printf("1 minute load average: %lu\n", info.loads[0]);
	printf("5 minute load average: %lu\n", info.loads[1]);
	printf("15 minute load average: %lu\n", info.loads[2]);
	printf("Total RAM: %lu bytes\n", info.totalram);
	printf("Free RAM: %lu bytes\n", info.freeram);
	printf("Shared RAM: %lu bytes\n", info.sharedram);
	printf("Buffer RAM: %lu bytes\n", info.bufferram);
	printf("Total swap space: %lu bytes\n", info.totalswap);
	printf("Free swap space: %lu bytes\n", info.freeswap);
	printf("Number of processes: %u\n", info.procs);
	printf("Total high memory size: %lu bytes\n", info.totalhigh);
	printf("Available high memory size: %lu bytes\n", info.freehigh);
	printf("Memory unit size: %u bytes\n", info.mem_unit);
	fclose(f1);
	fclose(f);
}
