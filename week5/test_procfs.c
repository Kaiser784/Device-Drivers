#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{

	int fd;
	char option;
	
	printf("Welcome to the Procfs DEMO..\n");
	fd = open("/proc/chr_proc",O_RDWR);
	if(fd < 0)
	{
		printf("Unable to Open the Device File...\n");
		return 0;
	}
	
	while(1)
	{
		printf("************** Please Enter Your Options***************\n");
		printf("			1. Write				\n");
		printf("			2. Read				\n");
		printf("			3. Exit					\n");
		scanf("%c", &option);
		printf(" Your Options are = %c \n", option);

		switch(option)
		{
			case '1':
				printf("Enter the String to Write in to the Driver...\n");
				scanf(" %[^\t\n]s", write_buf);
				printf("Data Writtern...\n");
				write(fd, write_buf, strlen(write_buf)+1);
				printf("Write Operation Completed ... DONE...\n");
				break;
 			case '2':
				printf("Data is Reading....\n");
				read(fd, read_buf, 1024);
				printf("Done....\n\n");
				printf("Data = %s\n\n", read_buf);
				break;
			case '3':
				close(fd);
				exit(1);
				break;
			default:
				printf("Enter the Valid Option = %c\n",option);
				break;
			}
	}
	close(fd);
}