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

const char* direccion(const char *dir){
	FILE *fp;
	struct mntent *mn;
	
	fp = setmntent("/etc/mtab", "r");
	if (fp == NULL) {
		printf("Fallo apertura del archivo\n");
		exit(1);
	}
	while ((mn = getmntent(fp)) != NULL){
		if(strstr(mn->mnt_fsname,dir)>0){
			endmntent(fp);
			return mn->mnt_dir;
		}
	}
	endmntent(fp);
	return "no se ha montado el usb";
}

void enumerar_disp_alm_masivo(struct udev* udev,int archivo){
	char * buf = (char *)malloc(sizeof(char)*500);
	memset (buf,0,sizeof(char)*500);
	
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
	int n=0;
	udev_list_entry_foreach(entrada, dispositivos) {
		const char* ruta = udev_list_entry_get_name(entrada);
		struct udev_device* scsi = udev_device_new_from_syspath(udev, ruta);

		//Obtenemos la informacion pertinente del dispositivo
		struct udev_device* block = obtener_hijo(udev, scsi, "block");
		struct udev_device* scsi_disk = obtener_hijo(udev, scsi, "scsi_disk");

		struct udev_device* usb = udev_device_get_parent_with_subsystem_devtype(scsi, "usb", "usb_device");

		if (block && scsi_disk && usb) {
			const char *node=udev_device_get_devnode(block);
			int n=sprintf(buf, "nodo = %s,El punto de montaje = %s, id(idVendor:idProduct)= %s:%s, scsi = %s\n",
				node,
				direccion(node),
				udev_device_get_sysattr_value(usb, "idVendor"),
				udev_device_get_sysattr_value(usb, "idProduct"),
				udev_device_get_sysattr_value(scsi, "vendor"));
			write (archivo, buf, n);
		}

		if (block) {
			udev_device_unref(block);
		}

		if (scsi_disk) {
			udev_device_unref(scsi_disk);
		}
		if(n==0){
			write (archivo, "no hay dispositivos conectado\n", 30);
		}
		udev_device_unref(scsi);
	}

	udev_enumerate_unref(enumerar);
}
