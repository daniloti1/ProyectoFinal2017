#include <stdio.h>
#include <string.h>
#include <mntent.h>
#include <libudev.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#define PORT 8888

/*
struct udev_device* obtener_hijo(struct udev* udev,struct udev_device* padre,const char* subsistema)
{
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

static char* enumerar_disp_alm_masivo(struct udev* udev)
{
	struct udev_enumerate* enumerar = udev_enumerate_new(udev);

	//Buscamos los dispositivos USB del tipo SCSI (MASS STORAGE)
	udev_enumerate_add_match_subsystem(enumerar, "scsi");
	udev_enumerate_add_match_property(enumerar, "DEVTYPE", "scsi_device");
	udev_enumerate_scan_devices(enumerar);

	//Obtenemos los dispositivos con dichas caracteristicas
	struct udev_list_entry *dispositivos = udev_enumerate_get_list_entry(enumerar);
	struct udev_list_entry *entrada;

	char * devnode = "";
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
				devnode = udev_device_get_devnode(block),
				udev_device_get_sysattr_value(usb, "idVendor"),
				udev_device_get_sysattr_value(usb, "idProduct"),
				udev_device_get_sysattr_value(scsi, "vendor"));
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
	return devnode;
}
*/

void request_completed (void *cls, struct MHD_Connection *connection, 
     		        void **con_cls,
                        enum MHD_RequestTerminationCode toe)
{
  	printf("funciono");  
}

int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls) 
{
  	const char *page  = "<html><body>Hello, browser!</body></html>";

	struct MHD_Response *response;
  	int ret;

  	response = MHD_create_response_from_buffer (strlen (page), (void*) page, MHD_RESPMEM_PERSISTENT);

	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
 	MHD_destroy_response (response);

  	return ret;
}

int main(int argc, char** argv) {
	//struct udev *udev = udev_new();

	/*
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;

   	struct udev_monitor *mon;
	int fd;
	*/

	//enumerar_disp_alm_masivo(udev);

	


	struct MHD_Daemon* daemon;

	daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,&answer_to_connection,NULL,MHD_OPTION_NOTIFY_COMPLETED, request_completed,NULL, MHD_OPTION_END);

	getchar ();

  	MHD_stop_daemon (daemon);

	

}