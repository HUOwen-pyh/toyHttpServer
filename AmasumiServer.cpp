#include "AmasumiServer.h"

amasumiHttpServer* amasumiHttpServer::server = new amasumiHttpServer();

amasumiHttpServer* amasumiHttpServer::getServer(){

	return server;

}


void amasumiHttpServer::destoryServer(){

	delete server;
	server = nullptr;

}

void amasumiHttpServer::serverExec(){

	int serverSock;
 	u_short port;
	int clientSock;
 	struct sockaddr_in clientName;
	socklen_t clientNameLen = sizeof(clientName);
	pthread_t newthread;
	serverListen(port,serverSock);
	printf("httpd running on port %d\n", port);
	do{
 		printf("1");
		clientSock = accept(serverSock, (struct sockaddr *)&clientName,&clientNameLen);
 
	       	if (clientSock == -1)errorPrint("accept");
	       	
 		if (pthread_create(&newthread, NULL, stringParse, (void *)(size_t)clientSock) != 0){
 		
	 		perror("pthread_create");
	 	}
	 		
	 		
		pthread_join(newthread,NULL);
	}while(1);

	close(serverSock);
	return;

}

void amasumiHttpServer::serverListen(u_short& port, int& httpd){
	port = 0;
 	struct sockaddr_in name;
	httpd = socket(PF_INET, SOCK_STREAM, 0);
	
	if (httpd == -1)errorPrint("socket");

 	memset(&name, 0, sizeof(name));
 	name.sin_family = AF_INET;
 	name.sin_port = htons(port);
 	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)errorPrint("bind");

 	if (port == 0){
  		socklen_t  namelen = sizeof(name);
  		if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)errorPrint("getsockname");
 		port = ntohs(name.sin_port);
 	}
 
 	if (listen(httpd, 25) < 0)errorPrint("listen");
	
 	return;

}

void amasumiHttpServer::errorPrint(char* errInfo){

	perror(errInfo);
	exit(1);

}

int amasumiHttpServer::getLine(int client,char* buf,size_t size){

	char c = 0;
	int i = 0;
	int n;
	while((i<size-1) && (c!='\n')){
	
		n=recv(client,&c,1,0);
		if(n<0){
			printf("receive error\n");
			exit(1);
		}
		
		if(c=='\r')c='\n';
		buf[i] = c;
		i++;
	
	}
	buf[i] = 0;
	return i;

}

void amasumiHttpServer::readAll(int client){

	char buf[1024];
	int n = recv(client,buf,sizeof(buf),0);
	return;

}

void amasumiHttpServer::unimplemented(int client){

	char buf[1024];

	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
 	send(client, buf, strlen(buf), 0);
 	sprintf(buf, "Content-Type: text/html\r\n");
 	send(client, buf, strlen(buf), 0); 
	sprintf(buf, "\r\n");
 	send(client, buf, strlen(buf), 0);
 	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
 	send(client, buf, strlen(buf), 0);
 	sprintf(buf, "</TITLE></HEAD>\r\n");
 	send(client, buf, strlen(buf), 0);
 	sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
 	send(client, buf, strlen(buf), 0);
 	sprintf(buf, "</BODY></HTML>\r\n");
 	send(client, buf, strlen(buf), 0);

	return;
}

void amasumiHttpServer::header(int client){

        char buf[1024];

        sprintf(buf, "HTTP/1.0 200 OK\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "Content-Type: text/html\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "\r\n");
        send(client, buf, strlen(buf), 0);
	return;
}


void amasumiHttpServer::getMethodRespo(int client){

	header(client);
	char buf[1024];

        sprintf(buf, "<HTML><HEAD><TITLE>Get Respo\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "</TITLE></HEAD>\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "<BODY><form method=\"POST\"><input type=\"submit\" value=\"2Post\"></form>\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "</BODY></HTML>\r\n");
        send(client, buf, strlen(buf), 0);
        
	return;

}

void amasumiHttpServer::postMethodRespo(int client){
        
	header(client);
	char buf[1024];

        sprintf(buf, "<HTML><HEAD><TITLE>Post Respo\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "</TITLE></HEAD>\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "<BODY><form method=\"GET\"><input type=\"submit\" value=\"2Get\"></form>\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "</BODY></HTML>\r\n");
        send(client, buf, strlen(buf), 0);
        
	return;

}


void* amasumiHttpServer::stringParse(void* arg){
	int client = (int)(size_t)arg;
	char buf[512];
	char method[16];
	char url[256];
	char path[512];
	size_t i = 0, j = 0;
	int numchars = getLine(client, buf, sizeof(buf));
	readAll(client);

 	while (buf[j]!=' ' && (i < sizeof(method) - 1)){
  
		method[i] = buf[j];
  		i++; j++;
 	}
 	method[i] = '\0';

 	if (strcasecmp(method, "GET") && strcasecmp(method, "POST")){
 		unimplemented(client);
 	} else if (strcasecmp(method, "GET") == 0){
                getMethodRespo(client);
        } else {
                postMethodRespo(client);
        }
 	close(client);

	return NULL;
}
