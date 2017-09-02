#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <mntent.h>
#include <libudev.h>
#include <unistd.h>


struct udev_device* obtener_hijo(struct udev* udev,struct udev_device* padre,const char* subsistema){
	struct udev_device* hijo = NULL;
	struct udev_enumerate *enumerar = udev_enumerate_new(udev);
	
	udev_enumerate_add_match_parent(enumerar,padre);
	udev_enumerate_add_match_subsystem(enumerar,subsistema);
	udev_enumerate_scan_devices(enumerar);

	struct udev_list_entry *dispositivos = udev_enumerate_get_list_entry(enumerar);
	struct udev_list_entry *entrada;
	
	udev_list_entry_foreach(entrada,dispositivos) {
		const char *ruta=udev_list_entry_get_name(entrada);
		hijo = udev_device_new_from_syspath(udev,ruta);
		break;
	}

	udev_enumerate_unref(enumerar);
	return hijo;
}

static void enumerar_disp_alm_masivo(struct udev* udev,int archivo){
	printf("hola xD");
	char * buf = (char *)malloc(sizeof(char)*100);
	memset (buf,0,sizeof(char)*100);
	
	struct udev_enumerate* enumerar = udev_enumerate_new(udev);

	//Buscamos los dispositivos USB del tipo SCSI (MASS STORAGE)
	udev_enumerate_add_match_subsystem(enumerar, "scsi");
	udev_enumerate_add_match_property(enumerar, "DEVTYPE", "scsi_device");
	udev_enumerate_scan_devices(enumerar);

	//Obtenemos los dispositivos con dichas caracteristicas
	struct udev_list_entry *dispositivos = udev_enumerate_get_list_entry(enumerar);
	struct udev_list_entry *entrada;

	//char * devnode = "";
	//Recorremos la lista obtenida
	udev_list_entry_foreach(entrada, dispositivos) {
		const char* ruta = udev_list_entry_get_name(entrada);
		struct udev_device* scsi = udev_device_new_from_syspath(udev, ruta);

		//Obtenemos la informacion pertinente del dispositivo
		struct udev_device* block = obtener_hijo(udev, scsi, "block");
		struct udev_device* scsi_disk = obtener_hijo(udev, scsi, "scsi_disk");

		struct udev_device* usb = udev_device_get_parent_with_subsystem_devtype(scsi, "usb", "usb_device");

		if (block) {
			printf("block = %s, usb = %s:%s, scsi = %s\n",
				udev_device_get_devnode(block),
				udev_device_get_sysattr_value(usb, "idVendor"),
				udev_device_get_sysattr_value(usb, "idProduct"),
				udev_device_get_sysattr_value(scsi, "vendor"));
				buf[0]="block = %s, usb = %s:%s, scsi = %s\n",
				udev_device_get_devnode(block),
				udev_device_get_sysattr_value(usb, "idVendor"),
				udev_device_get_sysattr_value(usb, "idProduct"),
				udev_device_get_sysattr_value(scsi, "vendor");
				fwrite(buf, strlen(buf),1,archivo);
				
				memset (buf,0,sizeof(char)*100);
		}

		if (block) {
			udev_device_unref(block);
		}

		if (scsi_disk) {
			udev_device_unref(scsi_disk);
		}

		udev_device_unref(scsi);
	}

	udev_enumerate_unref(enumerar);
	//return devnode;
}



int main(int argc, char* argv[])
{
	FILE *fp= NULL;
	FILE *fp1= NULL;	
	

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
	sid = setsid();
	if(sid < 0)
	{
		exit(1);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	fp = fopen("log_usb.log", "w");
	while (1)
	{	
		printf("while");
		struct udev *udev;
		udev = udev_new();
		enumerar_disp_alm_masivo(udev,fp);
		//aqui debe ir la funcion a usarse
		//sleep(2);		
		//fp1 = popen("top -bn2 | grep '%Cpu' | tail -1", "r");
		//fread(buf,100,1, fp1);
		//fwrite(buf, strlen(buf),1,fp);
		//memset (buf,0,sizeof(char)*100);
		
	}
	fclose(fp);
	return (0);
}
