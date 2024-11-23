#include <stdio.h>
#include <stdlib.h>
#define MAX_SERVICE_NAME_LENGTH 100
#include <string.h>
#include <unistd.h>

void *service_management(void *arg)
{
    char service[MAX_SERVICE_NAME_LENGTH];
    
    printf("\nD I S P L A Y I N G    S E R V I C E S\n"); 
    system("systemctl list-unit-files --type=service");
    printf("Enter the name of the service: ");
    fflush(stdin);
    scanf("%s", service);
    int choice, error=0;
    err:
    printf("\nEnter \n1.To start the service\n2.To get status of the service\n3.To stop the service\n"); 
    scanf("%d", &choice);
    if(choice==1)
    {
	    // Start the service
	    char startCommand[256];
	    snprintf(startCommand, sizeof(startCommand), "sudo systemctl start %s", service);
	    system(startCommand);
	    printf("service started");
    }
    else if(choice==2)
    {
	    // Check the status of the service
	    char statusCommand[256];
	    snprintf(statusCommand, sizeof(statusCommand), "sudo systemctl status %s", service);
	    system(statusCommand);
	    printf("service status");
    }
    else if(choice==3)
    {
	    // Stop the service
	    char stopCommand[256];
	    snprintf(stopCommand, sizeof(stopCommand), "sudo systemctl stop %s", service);
	    system(stopCommand);
	    printf("service stopped");
    }
    else
    {
	    printf("Error!! enter again. you only have 3 tries to enter correct service\n");
	    error++;
	    goto err;
    }
}
