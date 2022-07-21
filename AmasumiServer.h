#ifndef __AMASER_H
#define __AMASER_H

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


class amasumiHttpServer {

private:

	amasumiHttpServer() = default;
	~amasumiHttpServer() = default;

	void serverListen(u_short& port,int& httpd);
	void errorPrint(char* errInfo);
	static void* stringParse(void* arg);
	static int getLine(int client,char* buf,size_t size);
	static void unimplemented(int client);
	static void header(int client);
	static void getMethodRespo(int client);
	static void postMethodRespo(int client);
	static void readAll(int client);
	

public:

	static amasumiHttpServer* getServer();
	static void destoryServer();
	static amasumiHttpServer* server;

	void serverExec();


};


#endif
