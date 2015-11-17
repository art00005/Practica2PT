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
	char buffer_in[1024], buffer_out[1024],input[1024],buffer_time[80];
	int recibidos=0,enviados=0,variableDatos=0,punto=1,envioVariosRCPT=0,zone=0;
	int estado=S_HELO , estado2=0;
	char option, guion[]=COMP;
	struct tm* timeinfo;
	time_t rawtime;
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
						envioVariosRCPT=0;
						printf("CLIENTE> Introduzca el destinatario (enter para salir): ");
						gets(input);
						if(strlen(input)==0)												
						{
							sprintf_s (buffer_out, sizeof(buffer_out), "%s%s",SD,CRLF);
							estado=S_QUIT;
						}
						else{
							sprintf_s (buffer_out, sizeof(buffer_out), "%s%s%s:%s%s",RCPT,SP,TO,input,CRLF);
							printf("¿Desea añadir otro destinatario?: S/N %s",CRLF);
							gets(input);
							if(strcmp(input,"S") == 0 || strcmp(input,"s") == 0 || strcmp(input,"Si") == 0 || strcmp(input,"si") == 0) envioVariosRCPT = 1;
						}
						break;
					case S_DATA:																
						sprintf_s (buffer_out, sizeof(buffer_out), "%s%s%s",DATA,SP,CRLF);	
						break;
					case S_CAB:
						estado2=1;
						do{
							variableDatos=0;
							switch(estado2){
								case 1:
									estado2++;
									zone = getTimeZone();
									time( &rawtime );
									timeinfo = localtime( &rawtime );
									strftime(buffer_time, 80,"Date: %a, %d %b %Y %X ", timeinfo);			
									if (zone > 0) sprintf_s (buffer_out, sizeof(buffer_out), "%s%s+%i%s%s",buffer_time,SP,zone,SP,CRLF);
									else (buffer_out, sizeof(buffer_out), "%s%s%i%s%s",buffer_time,SP,zone,SP,CRLF);
									variableDatos=enviarData(buffer_out,sockfd);
									printf("%i",variableDatos);
									if (variableDatos <= 0) {estado = S_QUIT; estado2 = 7;}
									break;
								case 2:
									estado2++;
									printf("Remitente: ");
									gets(input);
									sprintf_s (buffer_out, sizeof(buffer_out), "From: <%s>%s%s",input,SP,CRLF);
									variableDatos=enviarData(buffer_out,sockfd);
									if (variableDatos <= 0) {estado = S_QUIT; estado2 = 7;}
									break;
								case 3:
									estado2++;
									printf("Asunto: ");
									gets(input);
									sprintf_s (buffer_out, sizeof(buffer_out), "Subject: %s%s%s",input,SP,CRLF);
									variableDatos=enviarData(buffer_out,sockfd);
									if (variableDatos <= 0) {estado = S_QUIT; estado2 = 7;}
									break;
								case 4:
									estado2++;
									printf("Destinatario: ");
									gets(input);
									sprintf_s (buffer_out, sizeof(buffer_out), "To: <%s>%s%s%s",input,SP,CRLF);
									variableDatos=enviarData(buffer_out,sockfd);
									if (variableDatos <= 0) {estado = S_QUIT; estado2 = 7;}
									else{
										printf("¿Desea añadir otro destinatario?: S/N %s",CRLF);
										gets(input);
										if(strcmp(input,"S") == 0 || strcmp(input,"s") == 0 || strcmp(input,"Si") == 0 || strcmp(input,"si") == 0) estado2--;
										else { sprintf_s (buffer_out, sizeof(buffer_out),"%s%s",CRLF,CRLF);
										variableDatos=enviarData(buffer_out,sockfd);
										if (variableDatos <= 0) {estado = S_QUIT; estado2 = 7;}
										}
									}
									break;
								case 5:
									break;
								case 6:
									break;
							}
						}while (estado2 != 7);

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
								if (estado != S_RCPT && envioVariosRCPT == 1) estado--;

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







int enviarData(char buffer_out[1024], SOCKET sockfd)
{
	int enviados=0;
	enviados = send(sockfd, buffer_out, (int)strlen(buffer_out), 0);					
	if (enviados == SOCKET_ERROR || enviados == 0){
		printf("Ha habido un error");
	}
	return enviados;
}


int getTimeZone()
{
   TIME_ZONE_INFORMATION tziOld;
	DWORD dwRet;

	dwRet = GetTimeZoneInformation(&tziOld);

	 if(dwRet == TIME_ZONE_ID_STANDARD || dwRet == TIME_ZONE_ID_UNKNOWN)    
      tziOld.StandardBias/60;
   else if( dwRet == TIME_ZONE_ID_DAYLIGHT )
      return tziOld.DaylightBias/60;
   else
   {
      printf("GTZI failed (%d)\n", GetLastError());
      return 0;
   }

  

}