#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
void *own_process(void *arg)
{
	int status;
	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		printf("Error in creating child");
	}
	else if(pid==0)
	{
		printf("\nChild created successfully!!");
		printf("\nPid of child process:%d",getpid());
	}
	else
	{
		wait(&status);
		printf("\nPid of parent process:%d",getppid());
	}
	printf("\nList down all processes\n");
	system("ps aux");
	printf("\n_________________PROCESS CREATED_________________\n");
}
