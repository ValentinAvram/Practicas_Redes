#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

int main (int argc, char * const argv[])
{
    system("clear");
	if(argc != 2)
	{
		printf("Error en el numero de argumentos\n");
		printf("Por favor, introduzca la IP del servidor como argumento\n");

		exit(-1);
	}

	int sd; // socket
	char *dir_ip = argv[1];
	struct sockaddr_in sockname;
	char buffer[250];
	socklen_t len_sockname;
    fd_set readfds, auxfds;
    int salida;
    int fin = 0;
	
    
  	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    	exit (1);	
	}

	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2050);
	sockname.sin_addr.s_addr =  inet_addr(dir_ip); //TODO: AÑADIR LA IP POR PANTALLA

	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}
    //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);

    
	do
	{
        auxfds = readfds;
        salida = select(sd+1,&auxfds,NULL,NULL,NULL);
        //Recepción de mensajes del servidor
        if(FD_ISSET(sd, &auxfds)){
            
            bzero(buffer,sizeof(buffer));
            int recibidos = recv(sd,buffer,sizeof(buffer),0);
            if(recibidos > 0)
            {
                printf("\n%s\n",buffer);
                
                if(strcmp(buffer,"Demasiados clientes conectados\n") == 0)
                fin =1;
                
                if(strcmp(buffer,"Desconexión servidor\n") == 0)
                fin =1;
                
                if(strcmp(buffer,"–Err. Cierre del servidor!\n") == 0)
                fin =1;
            }
        }
        else
        {
            //Envio de mensajes al servidor
            if(FD_ISSET(0,&auxfds))
			{
                bzero(buffer,sizeof(buffer));
                fgets(buffer,sizeof(buffer),stdin);
                
                if(strcmp(buffer,"SALIR\n") == 0)
				{
                    fin = 1;
                }
                send(sd,buffer,sizeof(buffer),0);   
            }     
        }			
    }while(fin == 0);	
    
	close(sd);
    return 0;	

}
