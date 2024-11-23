
//COMPILE WITH COMMANDS:
// gcc mainfile.c -o output `pkg-config --cflags --libs gtk+-3.0` 
// ./output

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu_utilization.h"
#include "SYSTEM_INFORMATION.h"
#include "MEMORY_USAGE.h"
#include "SERVICE_MANAGEMENT.h"
#include "PROCESS_DETAILS.h"
#include "NETWORK_USAGE.h"
#include "GPU_USAGE.h"
#include "DISK_USAGE.h"
#include"process.h"
#include<pthread.h>
#include<errno.h>
#include<signal.h>
#include<sys/types.h>
#include<string.h>
#include<time.h>
pthread_t pids[12];
#define size 1024
int priority[size];
int pid;
int totalP=0;//total processes
pid_t processes[size];//storing ids
int startT[size];
int burstT[size];
int completionT[size];
typedef struct{
int totalProcesses;
}taskManager;

//terminate a process
void killProcess(int signal)
{
    printf("\nEnter id of process:");
    scanf("%d",&pid);
    printf("\nProcess terminated.\n");
    kill(pid,SIGKILL);
}
//stop a process
void stopProcess(int signal){
    printf("\nEnter id of process:");
    scanf("%d",&pid);
    printf("\nProcess stopped.\n");
    kill(pid,SIGSTOP);
}
//resume a process
void ResumeProcess(int signal){
    printf("\nEnter id of process:");
    scanf("%d",&pid);
    printf("\nProcess resumed.\n");
    kill(pid,SIGCONT);

}

//storing current processes
void *storeCurrentProcess(void *arg)
{
	FILE *pipe = popen("ps aux", "r");
	if(pipe == NULL)
	{
    		perror("popen");
	}
	char* linee="********List all processes********\nPID:";
	char line[256];
	FILE *outputFile = fopen("processes.txt", "w");
	fprintf(outputFile, "%s\n", linee);
	fgets(line,sizeof(line),pipe);//this is to ignore the header line
	while(fgets(line,sizeof(line), pipe)!=NULL)
	{//start from the second line
	    	// Extract PID from the line
	    	char *token=strtok(line," ");
	    	int count=0;
	    	 
	    	while(token!=NULL)
	    	{
			if(count==1)
			{ //first column is for names and second is for ids
			    	if(atoi(token)!=0)
			    	{
					processes[totalP]=atoi(token);
					fprintf(outputFile, "%s\n", token);
					totalP=totalP+1;
					break;
			    	}
			}
			token=strtok(NULL," ");
			count++;
	    	}
	}
	int i=0;
	while(i<totalP)
	{
	    	priority[i]=rand()%totalP;
	    	i++;
	}
	printf("\033[44m");
	printf("\ncurrent process stored\n");
	fclose(outputFile);
}

//calculation of process times
void *calcABCTime(void *arg)
{
    printf("\037[44m");
    FILE *pipe=popen("ps aux", "r");//open pipe in read mode
    if (pipe==NULL) 
    {
   	 perror("popen");
    }
    int i=0;
    clock_t start_t, end_t;
    char line[256];
    fgets(line,sizeof(line),pipe);//this is to ignore the header line
    while(fgets(line,sizeof(line), pipe)!=NULL)
    {
	    start_t=clock();
	    startT[i]=start_t;
	    //process done
	    end_t=clock();
	    completionT[i]=end_t;
	    //burst time
	    burstT[i]= completionT[i]-startT[i];
	    i++;
    }
    fclose(pipe);
    char* linee="\nID\tS.Time\tC.Time\tB.Time\n";
    FILE *outputFile = fopen("ABC.txt", "w");
    FILE *outputFile1 = fopen("system_monitor.txt", "w");
    fprintf(outputFile, "%s\n", linee);
    fprintf(outputFile1, "%s\n", linee);
    //WRITE CLOUMN NAMES TO CONSOLE
    printf("%s\n", linee);
    i=0;
    while(i<totalP)
    {
    	    fprintf(outputFile1,"%d\t%d\t%d\t%d\n",processes[i],startT[i],completionT[i],burstT[i]);
	    fprintf(outputFile,"%d\t%d\t%d\t%d\n",processes[i],startT[i],completionT[i],burstT[i]);
	    i++;
    }
    fprintf(outputFile1,"%d\t%d\t%d\t%d\n",processes[i],startT[i],completionT[i],burstT[i]);
    fprintf(outputFile,"%d\t%d\t%d\t%d\n",processes[i],startT[i],completionT[i],burstT[i]);
    printf("%d\t%d\t%d\t%d\n",processes[i],startT[i],completionT[i],burstT[i]);
    fclose(outputFile1);  
    fclose(outputFile);
}

void *scheduling_algo(void *arg) 
{
	int tq = 5;
	int current_time = 0;
	int remaining_processes = totalP;
	int remainingTime[totalP];
	int startTime[totalP];
	int completionTime[totalP];

	// Initialize remaining time for all processes
	for(int i = 0; i < totalP; ++i) 
	{
    		remainingTime[i] = burstT[i];
	}

	while(remaining_processes > 0) 
	{
    		// Traverse all processes one by one repeatedly
	    	for(int i = 0; i < totalP; ++i) 
	    	{
			// If the process is still not finished
			if(remainingTime[i] > 0) 
			{
			    	// A time quantum of tq is given to the current process
			    	startTime[i] = current_time;
			    	// Execute process for time quantum or remaining time, whichever is smaller
			    	int execute_time = (remainingTime[i] > tq) ? tq : remainingTime[i];
			    	remainingTime[i] -= execute_time;
			    	current_time += execute_time;
			    	// If the process is completed
			    	if (remainingTime[i] == 0) 
			    	{
					remaining_processes--;
					completionTime[i] = current_time;
			    	}
			}
	    	}	
	}
	FILE* f = fopen("text.txt","a");
	fprintf(f, "Process\tCompletion time\n");
	for(int i = 0; i < totalP; ++i)
	{
      		fprintf(f, "%d\t%d\n", processes[i], completionTime[i]);
      	}
    	printf("\nprocesses scheduled\n"); 
	fclose(f);
}


// Functions to execute when buttons are clicked
void on_cpu_button_clicked(GtkWidget *widget, gpointer data)
{
	pthread_create(&pids[0],NULL,cpu_utilization,NULL);
	//cpu_utilization();//function in cpu_utilization.h
}

void on_fork_button_clicked(GtkWidget *widget, gpointer data)
{
	pthread_create(&pids[1],NULL,own_process,NULL);
	//own_process();//function in process.h
}

void on_memory_button_clicked(GtkWidget *widget, gpointer data)
{
	pthread_create(&pids[2],NULL,mem_usage,NULL);
	//mem_usage();//function in MEMORY_USAGE.h
}

void on_disk_button_clicked(GtkWidget *widget, gpointer data)
{
	pthread_create(&pids[3],NULL,disk_usage,NULL);
	//disk_usage();//function in disk_usage.h
}

void on_network_button_clicked(GtkWidget *widget, gpointer data)
{
        printf("------------------------NETWORK USAGE------------------------");
	pthread_create(&pids[4],NULL,network_usage,NULL);
	//network_usage();// FUNCTION IN NETWORK_USAGE.h
	printf("\n\033[44m_________________T A S K   M A N A G E R_________________\n");
}

void on_gpu_button_clicked(GtkWidget *widget, gpointer data)
{
	pthread_create(&pids[5],NULL,gpu_usage,NULL);
	//gpu_usage();
}

void on_process_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n_________________ DISPLAYING PROCESS DETAILS _________________\n");
	system("clear");
	pthread_create(&pids[6],NULL,process_details,NULL);
	printf("\n\033[44m_________________T A S K   M A N A G E R_________________\n");
	//process_details();// function to give process details, top, atop htop commands
}

void on_service_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n_________________Service Management_________________\n");
	pthread_create(&pids[7],NULL,service_management,NULL);
	//service_management();// function in SERVICE_MANAGEMENT.h
}

void on_system_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n_________________system information_________________\n");
	pthread_create(&pids[8],NULL,system_information,NULL);
	//system_information();//to display system information
}

void on_suspend_process_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n_________________Suspend a Process_________________\n");
	signal(SIGUSR1, stopProcess);
	if(signal(SIGUSR1,stopProcess)==SIG_ERR)
	{
	    printf("Error setting up stop process for signal\n");
	    return ;
	}
	else
	{
		stopProcess(SIGUSR1);
	}
}

void on_resume_process_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n_________________Resume a Process_________________\n");
	signal(SIGUSR1, ResumeProcess);
	if(signal(SIGUSR1,ResumeProcess)==SIG_ERR){
    printf("Error setting up stop process for signal\n");
    return ;}
    else{
	ResumeProcess(SIGUSR1);
 }
}

void on_kill_process_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n_________________kill a process_________________\n");
	signal(SIGUSR1, killProcess);
	if(signal(SIGUSR1,stopProcess)==SIG_ERR){
    printf("Error setting up stop process for signal\n");
    return;}
    else
    {
	killProcess(SIGUSR1);
	}
}

//functionality of store current process button
void on_store_current_process_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n\033[45m_________________Storing Current Process_________________\n");
	pthread_create(&pids[9],NULL,storeCurrentProcess,NULL);
	//storeCurrentProcess();
}

//functionality of Calculating Arrival, Burst and completion time button
void on_calc_ABC_time_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\033[44mCalculating Arrival, Burst and completion time\n");
	pthread_create(&pids[10],NULL,calcABCTime,NULL);
	//calcABCTime();
}

//functionality of Scheduling Algorithm button
void on_scheduling_algo_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\033[43m_________________Scheduling Algorithm_________________\n");
	pthread_create(&pids[11],NULL,scheduling_algo,NULL);
	//scheduling_algo();
}


void on_exit_button_clicked(GtkWidget *widget, gpointer data)
{
	printf("\n\033[45m_________________EXITING PROGRAM_________________\n");
	for(int i=0;i<12;i++)
		pthread_join(pids[i],NULL);
	exit(1);
}

int main(int argc, char *argv[]) 
{
	printf("\n\033[44m_________________T A S K   M A N A G E R_________________\n");
	// Initialize GTK
	gtk_init(&argc, &argv);
	GtkWidget *label;
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;
	
	// Create the main window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "System Monitor");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 300);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	//setting dimensions for button size
	int bw=90,bh=30;
	
	// Create buttons
 	GtkWidget *fork_button = gtk_button_new_with_label("Process creation");
	gtk_widget_set_size_request(fork_button, bw, bh);
	GtkWidget *cpu_button = gtk_button_new_with_label("CPU Usage");
	gtk_widget_set_size_request(cpu_button, bw, bh);
	GtkWidget *memory_button = gtk_button_new_with_label("Memory Usage");
	gtk_widget_set_size_request(memory_button, bw, bh);
	GtkWidget *disk_button = gtk_button_new_with_label("Disk Usage");
	gtk_widget_set_size_request(disk_button, bw, bh);
	GtkWidget *network_button = gtk_button_new_with_label("Network Usage");
	gtk_widget_set_size_request(network_button, bw, bh);
	GtkWidget *gpu_button = gtk_button_new_with_label("GPU Usage");
	gtk_widget_set_size_request(gpu_button, bw, bh);
	GtkWidget *scheduling_algo_button = gtk_button_new_with_label("Schedule Processes");
	gtk_widget_set_size_request(scheduling_algo_button, bw, bh);
	GtkWidget *process_button = gtk_button_new_with_label("Process Details");
	gtk_widget_set_size_request(process_button, bw, bh);
	GtkWidget *service_button = gtk_button_new_with_label("Service Management");
	gtk_widget_set_size_request(service_button, bw, bh);
	GtkWidget *system_button = gtk_button_new_with_label("System Info");
	gtk_widget_set_size_request(system_button, bw, bh);
	GtkWidget *suspend_process_button = gtk_button_new_with_label("Suspend Process");
	gtk_widget_set_size_request(suspend_process_button, bw, bh);
	GtkWidget *resume_process_button = gtk_button_new_with_label("Resume Process");
	gtk_widget_set_size_request(resume_process_button, bw, bh);
	GtkWidget *kill_process_button = gtk_button_new_with_label("Kill Process");
	gtk_widget_set_size_request(kill_process_button, bw, bh);
	GtkWidget *store_current_process_button = gtk_button_new_with_label("Store current process");
	gtk_widget_set_size_request(store_current_process_button, bw, bh);
	GtkWidget *calc_ABC_time_button = gtk_button_new_with_label("Calculation of Arrival, Burst and Completion Time");
	gtk_widget_set_size_request(calc_ABC_time_button, bw, bh);
	GtkWidget *exit_button = gtk_button_new_with_label("EXIT");  
	gtk_widget_set_size_request(exit_button, bw, bh);
    
	//colour
	provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider, "label { color: white; background-color: blue; }", -1, NULL);

	// Apply CSS to the screen
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	// Connect button signals to functions
 	g_signal_connect(fork_button, "clicked", G_CALLBACK(on_fork_button_clicked), NULL);
	g_signal_connect(cpu_button, "clicked", G_CALLBACK(on_cpu_button_clicked), NULL);
	g_signal_connect(memory_button, "clicked", G_CALLBACK(on_memory_button_clicked), NULL);
	g_signal_connect(disk_button, "clicked", G_CALLBACK(on_disk_button_clicked), NULL);
	g_signal_connect(network_button, "clicked", G_CALLBACK(on_network_button_clicked), NULL);
	g_signal_connect(gpu_button, "clicked", G_CALLBACK(on_gpu_button_clicked), NULL);
	g_signal_connect(scheduling_algo_button, "clicked", G_CALLBACK(on_scheduling_algo_button_clicked), NULL);
	g_signal_connect(process_button, "clicked", G_CALLBACK(on_process_button_clicked), NULL);
	g_signal_connect(service_button, "clicked", G_CALLBACK(on_service_button_clicked), NULL);
	g_signal_connect(system_button, "clicked", G_CALLBACK(on_system_button_clicked), NULL);
	g_signal_connect(suspend_process_button, "clicked", G_CALLBACK(on_suspend_process_button_clicked), NULL);
	g_signal_connect(resume_process_button, "clicked", G_CALLBACK(on_resume_process_button_clicked), NULL);
	g_signal_connect(kill_process_button, "clicked", G_CALLBACK(on_kill_process_button_clicked), NULL);
	g_signal_connect(store_current_process_button, "clicked", G_CALLBACK(on_store_current_process_button_clicked), NULL);
	g_signal_connect(calc_ABC_time_button, "clicked", G_CALLBACK(on_calc_ABC_time_button_clicked), NULL);
	g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_button_clicked), NULL);
    
	// Create a vertical box layout container
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
    
	// Add buttons to the layout
	gtk_box_pack_start(GTK_BOX(vbox), fork_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), cpu_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), memory_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), disk_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), network_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), gpu_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), scheduling_algo_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), process_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), service_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), system_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), suspend_process_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), resume_process_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), kill_process_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), store_current_process_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), calc_ABC_time_button, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), exit_button, TRUE, TRUE, 0);
    
	//Display 
	gtk_widget_show_all(window);

	// Start the GTK main loop
	gtk_main();
	
	for(int i=0;i<12;i++)
		pthread_join(pids[i],NULL);
	return 0;
}
