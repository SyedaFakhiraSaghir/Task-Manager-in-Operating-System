#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *network_usage(void *arg)
{
	// Command to execute nload with parameters -u k -t eth0
	const char *command2 = "nload -m";//to show multiple devices at a time; no traffic graphs.
	const char *command3 = "nload -t";//Determines the refresh interval of the display in milliseconds. Default is 500.
	// Execute the command using system function
	int status3=system(command3);
	int status2=system(command2);
	// Check if the command executed successfully
    	// Check if the command executed successfully
	    if (status2 == -1 || status3==-1) 
	    {
		// Handle error
		perror("Error executing command");
	    }
	    sleep(1);
	    
	    system("clear");
}
