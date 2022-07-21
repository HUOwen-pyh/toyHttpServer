#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "AmasumiServer.h"



int main(void){

	amasumiHttpServer* Server = amasumiHttpServer::getServer(); //get instance of server
	Server->serverExec();										//start the server
	amasumiHttpServer::destoryServer();                         //free the pointer
	return 0;

}
