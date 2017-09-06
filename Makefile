servidor: src/servidor.c libutil.so daemon
	gcc -Wall -I./include -L./lib src/servidor.c ./lib/libutil.so -o bin/servidor -lmicrohttpd	
libutil.so: libutil.o
	gcc -shared -Fpic -I./include ./obj/libutil.o -o ./lib/libutil.so
libutil.o: src/libutil.c
	gcc -Wall -I./include ./src/libutil.c -fPIC -c -o ./obj/libutil.o	
daemon: src/daemon.c libutil.so
	gcc -Wall -I./include -L./lib src/daemon.c ./lib/libutil.so -o bin/daemon -ludev
clean:
	rm bin/*
	rm obj/*
	rm lib/*
buscarDaemon:	
	ps -A | grep " daemon"
