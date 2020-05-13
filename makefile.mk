a.out:server.o client.o
	gcc server.o client.o
server.o:server.c server.h
	gcc -c server.h server.c 
client.o:client.c server.h
	gcc -c  server.h client.c
