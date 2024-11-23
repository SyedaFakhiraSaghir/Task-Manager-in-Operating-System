#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/statvfs.h>

void *disk_usage(void *arg)
{
	FILE *f, *sys_monitor;
	f = fopen("disk_usage.txt", "w");
	sys_monitor = fopen("system_monitor.txt", "a");
	if (f == NULL) 
	{
		perror("Error opening Disk Usage info file");
		exit(EXIT_FAILURE);
	}
	if (sys_monitor == NULL) 
	{
		perror("Error opening Disk Usage info file");
		exit(EXIT_FAILURE);
	}
	struct statvfs vfs;
	    if (statvfs("/", &vfs) != -1) 
	    {
		unsigned long long total = (unsigned long long)vfs.f_blocks * (unsigned long long)vfs.f_frsize;
		unsigned long long available = (unsigned long long)vfs.f_bavail * (unsigned long long)vfs.f_frsize;
		unsigned long long used = total - available;
		fprintf(f,"\033[0;37;45m______________________D I S K   U S A G E______________________\n\033[0m\n");
		fprintf(f,"Total disk space: %llu bytes\n", total);
		fprintf(f,"Used disk space: %llu bytes\n", used);
		fprintf(f,"Available disk space: %llu bytes\n", available);
		fprintf(sys_monitor,"\033[0;37;45m______________________D I S K   U S A G E______________________\n\033[0m\n");
		fprintf(sys_monitor,"Total disk space: %llu bytes\n", total);
		fprintf(sys_monitor,"Used disk space: %llu bytes\n", used);
		fprintf(sys_monitor,"Available disk space: %llu bytes\n", available);
		printf("\033[0;37;45m______________________D I S K   U S A G E______________________\n\033[0m\n");
		printf("\033[0;37;45mTotal disk space: %llu bytes\n\033[0m\n", total);
		printf("\033[0;37;45mUsed disk space: %llu bytes\n\033[0m\n", used);
		printf("\033[0;37;45mAvailable disk space: %llu bytes\n\033[0m\n", available);
	    } 
	    else 
	    {
		perror("Error getting disk usage");
	    }
	fclose(f);
	fclose(sys_monitor);
}
