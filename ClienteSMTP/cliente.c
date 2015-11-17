/*******************************************************
Protocolos de Transporte
Grado en Ingeniería Telemática
Dpto. Ingeníería de Telecomunicación
Univerisdad de Jaén

Fichero: cliente.c
Versión: 1.0
Fecha: 23/09/2012
Descripción:
	Cliente de eco sencillo TCP.

Autor: Juan Carlos Cuevas Martínez

*******************************************************/
#include <stdio.h>
#include <winsock.h>
#include <time.h>
#include <conio.h>

#include "protocol.h"


//prueba modificacion


int main(int *argc, char *argv[])
{
	SOCKET sockfd;
	struct sockaddr_in server_in;
	char buffer_in[1024], buffer_out[1024],input[1024];
	int recibidos=0,enviados=0;
	int estado=S_HELO;
	char option, guion[]=COMP;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

    char ipdest[16];
	char default_ip[16]="127.0.0.1";
	
	//Inicialización Windows sockets
	wVersionRequested=MAKEWORD(1,1);
	err=WSAStartup(wVersionRequested,&wsaData);
	if(err!=0)
		return(0);

	if(LOBYTE(wsaData.wVersion)!=1||HIBYTE(wsaData.wVersion)!=1)
	{
		WSACleanup();
		return(0);
	}
	//Fin: Inicialización Windows sockets

	do{
		sockfd=socket(AF_INET,SOCK_STREAM,0);

		if(sockfd==INVALID_SOCKET)
		{
			printf("CLIENTE> ERROR AL CREAR SOCKET\r\n");
			exit(-1);
		}
		else
		{
			printf("CLIENTE> SOCKET CREADO CORRECTAMENTE\r\n");

		
			printf("CLIENTE> Introduzca la IP destino (pulsar enter para IP por defecto): ");
			gets(ipdest);

			if(strcmp(ipdest,"")==0)
				strcpy(ipdest,default_ip);


			server_in.sin_family=AF_INET;
			server_in.sin_port=htons(TCP_SERVICE_PORT);
			server_in.sin_addr.s_addr=inet_addr(ipdest);
			
			estado=S_INI;
		
			// establece la conexion de transporte
			if(connect(sockfd,(struct sockaddr*)&server_in,sizeof(server_in))==0)
			{
				printf("CLIENTE> CONEXION ESTABLECIDA CON %s:%d\r\n",ipdest,TCP_SERVICE_PORT);
			








		
				//Inicio de la máquina de estados
				do{
					switch(estado)
					{
					case S_INI:
						// Se recibe el mensaje de bienvenida
					case S_HELO:
						sprintf_s (buffer_out, sizeof(buffer_out), "%s%s",EHLO,CRLF);
						break;	
					case S_MAIL:
						// establece la conexion de aplicacion 
						printf("CLIENTE> Introduzca el Remitente (enter para salir): ");
						gets(input);
						if(strlen(input)==0)												
						{
							sprintf_s (buffer_out, sizeof(buffer_out), "%s%s",SD,CRLF);
							estado=S_QUIT;
						}
						else sprintf_s (buffer_out, sizeof(buffer_out), "%s%s%s:%s%s",MAIL,SP,FROM,input,CRLF);
						
						break;
					case S_RCPT:
						break;
					case S_DATA:
						break;
					case S_CAB:
						break;
					case S_SALIR:
						break;
					}








					if (estado != S_INI){
						// Ejercicio: Comprobar el estado de envio
						enviados = send(sockfd, buffer_out, (int)strlen(buffer_out), 0);						//envio datagramas

						if (enviados == SOCKET_ERROR){
							printf("Error en el envio");
							estado = S_QUIT;
						}
						if (enviados > 0){
							printf("Se han enviado %d bytes \r\n ", enviados);

						}
						if (enviados == 0){
							printf("Conexion ha sido liberada de forma acordada");
							estado = S_QUIT;
						}
					}









					//Recibo
					do{
						
						recibidos=recv(sockfd,buffer_in,512,0);

						if(recibidos<=0)																	//si datos recividos por el sever menores o iguales a 0 error en recepcion
						{
							DWORD error=GetLastError();
							if(recibidos<0)
							{
								printf("CLIENTE> Error %d en la recepción de datos\r\n",error);
								estado=S_QUIT;
							}
							else
							{
								printf("CLIENTE> Conexión con el servidor cerrada\r\n");
								estado=S_QUIT;
							}
						}else{
												
																//Modificacion para en caso de estar en modo datos muestre por pantalla el resultado |||||||||||||||||||||||||||||||||||||||||||||||||QUITADO EL if
								buffer_in[recibidos] = 0x00;
								printf(buffer_in);
						}
						
					}while (buffer_in[3] == guion[0] );
						


						//------------------------------------------------------------------Fin modificacion
						if (estado != S_QUIT && ((strncmp(buffer_in, "2", 1) == 0) || (strncmp(buffer_in, "3", 1) == 0))){                                                                     // ||||||||||||||||||||||||||||||||||||||||||||MODIFICACION PARA RECIBIDOS 2XX
								estado++;  
								

						}else{
							estado = S_QUIT;
						}
				}while(estado!=S_QUIT);
				
	
		
			}
			else
			{
				printf("CLIENTE> ERROR AL CONECTAR CON %s:%d\r\n",ipdest,TCP_SERVICE_PORT);
			}		
			// fin de la conexion de transporte
			closesocket(sockfd);
			
		}	
		printf("-----------------------\r\n\r\nCLIENTE> Volver a conectar (S/N)\r\n");
		option=_getche();

	}while(option!='n' && option!='N');

	
	
	return(0);

}
