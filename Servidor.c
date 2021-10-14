#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
//#include <arpa/inet.h>
//#include <netdb.h>

#define MAX_CLIENTES 30
#define MAX_CHARACTERS 350
#define PORT 2050

struct acc_player
{

    int desctriptor;
    char username[MAX_CHARACTERS];
    char password[MAX_CHARACTERS];
    char channel[MAX_CHARACTERS]; //Esto paque?

};

void manejador(int signum);

void discClient(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);


int main(int argc, char const *argv []){

    system("clear");

    if(argc != 2)
    {
        perror("Error en el numero de argumentos!\n");
        exit(EXIT_FAILURE);
    }

    //TODO: char refran[250] = argv[1];

    // Descriptor del socket y buffer de datos 

    int sd, new_sd; //Descriptores
    struct sockaddr_in sockname, from; //Almacen de dirrecciones.
    char buffer[MAX_CHARACTERS];

    socklen_t from_len; // Length del Length de la estructura de peticiones???
    //sockets que comprueban existencia de caracteres a leer.
    fd_set readfds, auxfds; //TODO: Crear nuevo set de partidas, o 1 por partida?

    int salida, arrayClientes[MAX_CLIENTES]; // TODO: Crear nuevo array de partidas?
    int numClientes = 0; 
    int recibidos, i, j, k, on, ret;

    char identificador[MAX_CHARACTERS];

    // Apertura del socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == 1)
    {
        perror("No se puede abrir el socket cliente\n");
        exit(EXIT_FAILURE);
    }

    // Para reusar puertos
    on = 1;
    ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockname.sin_family = AF_INET; // Siempre debe ser AF_INET
    sockname.sin_port = htons(PORT); // Puerto para conexión
    sockname.sin_addr.s_addr = INADDR_ANY; // Direcciones por atender

    // Asociar socket a puerto
    if(bind (sd, (struct sockaddr *) &sockname, sizeof(sockname)) == -1)
    {
        perror("Error al asociar el socket al puerto \n");
        exit(EXIT_FAILURE);
    }

    // Length de la estructura de peticiones?
    from_len = sizeof(from);

    // Habilitar el socket para que reciba conexiones
    if(listen(sd, 30) == -1) //Max. 30 en usuarios a la espera? Cambiar por 20?
    {
        perror("Error en la operacion de listen \n");
        exit(EXIT_FAILURE);
    }

    // Inicializar los conjuntos fd_set

    FD_ZERO (&readfds);
    FD_ZERO(&auxfds);
    // Añadir descriptores a sets
    FD_SET(sd, &readfds);
    FD_SET(sd, &readfds); // TODO:Añade el descriptor al set "readfs" ?? // Cambiar sd por "0"?

    // Capturar la señal SIGINT (CTRL + C) // No toca ??
    signal(SIGINT, manejador);

    // TODO : EL SERVIDOR ACEPTA PETICION
    while(1)
    {
        // A la espera de mensajes // conexiones de los jugadores

        auxfds = readfds;

        // Atender varios clientes a la vez

        salida = select(FD_SETSIZE, &auxfds, NULL, NULL, NULL);

        if(salida > 0)
        {
            for(i = 0; i < FD_SETSIZE; i++)
            {
                //Buscamos el socket que ha establecido conexión
                if(FD_ISSET(i, &auxfds))
                {
                    if(i == sd)
                    {
                        if((new_sd = accept(sd, (struct sockaddr *) &from, &from_len)) == -1)
                        {
                            perror("Error aceptando peticiones");
                        }
                        else
                        {
                            if(numClientes < MAX_CLIENTES)
                            {
                                arrayClientes[numClientes] = new_sd;
                                numClientes ++;
                                FD_SET(new_sd, &readfds);

                                strcpy(buffer, "AQUI EMPEIZA EL JUEGO\n");

                                // Enviar mensaje al nuevo cliente
                                send(new_sd, buffer, sizeof(buffer), 0); //No se muy bien como va lo de las flags , "0".

                                for(j = 0; j<(numClientes-1); j++)
                                {
                                    bzero(buffer, sizeof(buffer)); // Deja buffer = "\0"
                                    sprintf(buffer, "NUEVO CLIENTE CONECTADO: %d\n", new_sd);
                                    send(arrayClientes[j], buffer, sizeof(buffer), 0);
                                }
                            }
                            else
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer,"SERVIDOR LLENO. INTENTELO MAS TARDE\n");
                                send(new_sd, buffer, sizeof(buffer), 0);
                                // Cerrar socket
                                close(new_sd);
                            }
                        }
                    }
                    else if(i == 0)
                    {// Se ha introducido informacion de teclado.// Que chucha significa eso
                        bzero(buffer, sizeof(buffer));
                        fgets(buffer, sizeof(buffer), stdin);

                        /* Controlar si se ha introducido SALIR (desde el propio server), cerrar todos los sockets
                        y salir del servidor */
                        if(strcmp(buffer, "SALIR\n") == 0)
                        {
                            for(j = 0; j < numClientes; j++)
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "DESCONEXION SERVIDOR\n");

                                send(arrayClientes[j], buffer, sizeof(buffer), 0);
                                close(arrayClientes[j]);
                                FD_CLR(arrayClientes[j], &readfds); // Se elimina el sd del array
                            }
                            close(sd);
                            exit(-1);
                        }
                        //TODO: IMPLEMENTAR MENSAJES A MANDAR A LOS CLIENTES
                    }
                    else
                    {
                        bzero(buffer,sizeof(buffer));

                        // Recepcion de mensaje siguiente elemento del set (primer for)
                        recibidos = recv(i, buffer,sizeof(buffer), 0);

                        if(recibidos > 0)
                        {
                            if(strcmp(buffer, "SALIR\n") == 0)
                            {
                                discClient(i, &readfds, &numClientes, arrayClientes);
                            }
                            else
                            {
                               //sprintf(identificador, "PRUEBA", i, buffer); // Overflow de los 350 caracteres, , cambiar PRUEBA por" <%d>: %s"
                                bzero(buffer, sizeof(buffer));

                                strcpy(buffer, identificador);

                                printf("%s\n", buffer);

                                for(j = 0; j<numClientes; j++)
                                {
                                    if(arrayClientes[j] != i)
                                    {
                                        send(arrayClientes[j], buffer, sizeof(buffer), 0);
                                    }
                                }
                                if(recibidos == 0)
                                {
                                    printf("El socket %d, ha introducido ctrl + c \n", i);
                                    //Eliminar ese socket
                                    discClient(i, &readfds, &numClientes, arrayClientes);
                                }
                            }
                        }
                    }
                }
            }
            close(sd);
            exit(0);
        }
    }
    exit(0); //?
}   

void discClient(int socket, fd_set * readfds, int * numClientes, int arrayClientes[])
{
    char buffer[250];
    int j;

    close(socket);
    FD_CLR(socket, readfds);

    // Re-estructurar el array de clientes

    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (j = 0; j < (*numClientes) - 1; j++)// WTF????
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--; 

    bzero(buffer, sizeof(buffer));
    sprintf(buffer, "Desconexion del cliente: %d \n", socket);

    for(j=0; j<(*numClientes); j++)
    {
        if(arrayClientes[j] != socket)
        {
            send(arrayClientes[j], buffer, sizeof(buffer), 0);
        }
    }
}

void manejador(int signum) // TODO: Cambiar esto por lo que sea.
{
    printf("\n Se ha recibido la señal sigint \n");
    signal(SIGINT, manejador);
}