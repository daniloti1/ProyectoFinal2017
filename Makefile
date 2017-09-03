servidor: src/servidor.c
	gcc -Wall src/servidor.c -o bin/servidor -ludev -lmicrohttpd
	cc src/servidor.c -o bin/servidor_segun_tutorial -I$PATH_TO_LIBMHD_INCLUDES -L$PATH_TO_LIBMHD_LIBS -lmicrohttpd
clean:
	rm bin/servidor

