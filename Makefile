servidor: servidor.c
	gcc -Wall servidor.c -o servidor -ludev -lmicrohttpd
	cc servidor.c -o servidor_segun_tutorial -I$PATH_TO_LIBMHD_INCLUDES -L$PATH_TO_LIBMHD_LIBS -lmicrohttpd
clean:
	rm servidor

