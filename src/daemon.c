#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <mntent.h>
#include <libudev.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include "util.h"


int main(int argc, char* argv[])
{
	int fp;
	pid_t process_id = 0;
	pid_t sid = 0;
	process_id = fork();

	if (process_id < 0)
	{
		printf("Fallo en el fork\n");

		exit(1);
	}

	if (process_id > 0)
	{
		printf("Numero del proceso hijo %d \n", process_id);
		exit(0);
	}
	umask(0);
	fp = open ("log_usb", O_WRONLY | O_CREAT, 0600);
	if (fp == -1) {
		printf("Error al abrir archivo");
		exit(0);
	}
	sid = setsid();
	if(sid < 0)
	{
		exit(1);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	
	
	while (1)
	{	
		printf("while");
		struct udev *udev;
		udev = udev_new();
		enumerar_disp_alm_masivo(udev,fp);
		sleep(2);
		
	}
	return (0);
}
