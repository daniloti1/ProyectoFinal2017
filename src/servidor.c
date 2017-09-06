#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <mntent.h>
#include <libudev.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#define PORT 8888


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

	//if (method == "GET") {


  	const char *page  = "{ ‘solicitud’: ‘listar_dispositivos’, ‘dispositivos’: [ {‘nombre’: ...,‘id’: ‘vendor:device’,‘montaje’ : ‘/home/...’,‘nodo’: ‘/dev/...’}, { ...} ],‘status’: ..., ‘str_error’ : ...}";
	printf("Answer to conection");

	//MHD_get_connection_values (connection, MHD_HEADER_KIND, keyValueIterator,NULL);

	struct MHD_Response *response;
  	int ret;

  	response = MHD_create_response_from_buffer (strlen (page), (void*) page, MHD_RESPMEM_PERSISTENT);

	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
 	MHD_destroy_response (response);

  	return ret;
}

int main(int argc, char** argv) {

	struct MHD_Daemon* daemon;

	daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,&answer_to_connection,NULL,MHD_OPTION_NOTIFY_COMPLETED, request_completed,NULL, MHD_OPTION_END);

	getchar ();

  	MHD_stop_daemon (daemon);

	

}
