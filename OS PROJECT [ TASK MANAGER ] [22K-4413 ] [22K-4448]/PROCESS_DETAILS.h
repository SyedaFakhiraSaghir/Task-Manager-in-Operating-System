#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
void *process_details(void *arg)
{
    FILE *f, *sys_monitor;
    f = fopen("process_details.txt", "w");
    sys_monitor = fopen("system_monitor.txt", "a");
    if (f == NULL) 
    {
        perror("Error opening process details info file");
        exit(EXIT_FAILURE);
    }
    if (sys_monitor == NULL) 
    {
        perror("Error opening process details info file");
        exit(EXIT_FAILURE);
    }
    system("ps > file1.txt");
    system("/usr/bin/htop");
    system("/usr/bin/htop");
    const char* command = "ps aux";
	FILE* outputFile = fopen("data.txt", "w");
	//error in file opening
	if (outputFile == NULL)
	{
	    	perror("Error opening output file");
	}
	FILE* pipe = popen(command, "r");
	if (pipe == NULL) 
	{
	    	perror("Error executing command");
	    	fclose(outputFile);
	}
	char buffer[4096];
	while (fgets(buffer, sizeof(buffer), pipe) != NULL) 
	{
    		fprintf(outputFile, "%s", buffer);
	}
	pclose(pipe);
	fclose(outputFile);
    fclose(f);
    fclose(sys_monitor);

}
