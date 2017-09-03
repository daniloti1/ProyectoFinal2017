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

struct udev_device* obtener_hijo(struct udev* udev,struct udev_device* padre,const char* subsistema);
void enumerar_disp_alm_masivo(struct udev* udev,int archivo);
const char* direccion(const char *dir);
