Programación de Sistemas - Proyecto Final
1 - Descripcion
Programa cliente-servidor, que se encarga del monitoreo de dispositivos de almacenamiento que nos permite listar las mismas, además de ofrecer opciones como:
Listar Dispositivos
Renombrar dispositivo
Leer archivo
Escribir archivo
Salir

2 - Instalacion

Para en funcionamiento de este programa es necesario contar con las siguientes librerias:
	
	Libreria libudev
	Libreria libmicrohttpd
	Libreria jsmn

	$ sudo apt install libudev-dev
	$ sudo apt install libmicrohttpd*

3 - Uso general

Tras compilar todo con make
Se debe ejecutar bin/daemon
Luego conectar el o los dispositivos USB
Luego se ejecuta el bin/servidor
Y finalmente el src/cliente.py


4 - Autores
	Julio Alvia
	Danilo Torres

