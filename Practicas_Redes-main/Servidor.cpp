#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include "usuario.h"
#include "juego.h"
#include <vector>

#define MSG_SIZE 350
#define MAX_CLIENTS 30
#define PORT 2050

using namespace std;

vector <Usuario> users;
vector <Juego> games;
int arrayUsers[MAX_CLIENTS];
int numUsers, nGames;
fd_set readfds, auxfds;
int sd, new_sd;

bool cadenaComienzaCon(const char *cadena1, const char *cadena2)
{
    int longitud = strlen(cadena2);
    if (strncmp(cadena1, cadena2, longitud) == 0) return true;
    return false;
} 

int getStatus(int sd)
{
    for(int i = 0; i < users.size(); i++)
    {
        if((users[i].getSd() == sd))
        {
            return users[i].getStatus();
        }
    }
    return -2;
}

void salirCliente(int socket, fd_set * readfds, int * numUsers, int arrayClientes[])
{
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numUsers) - 1; j++)
        if (arrayUsers[j] == socket)
            break;
    for (; j < (*numUsers) - 1; j++)
        (arrayUsers[j] = arrayUsers[j+1]);
    
    (*numUsers)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numUsers); j++)
        if(arrayUsers[j] != socket)
            send(arrayUsers[j],buffer,sizeof(buffer),0);


}

void manejador (int signum)
{
    char buffer[350];
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    for (int j = 0; j < numUsers; j++)
    {
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "–Err. Cierre del servidor!\n");
        send(arrayUsers[j], buffer, sizeof(buffer), 0);
        close(arrayUsers[j]);
        FD_CLR(arrayUsers[j], &readfds);
    }
    close(sd);
    exit(-1);
}

void createGame(Juego juego)
{
    if(nGames < 10)
    {
        games.push_back(juego);
        nGames++;
    }
}

void deleteGame(int idG)
{
    for(vector<Juego>::iterator i = games.begin(); i != games.end(); i++)
    {
        if(i->getIdJuego() == idG)
        {
            games.erase(i);
            nGames--;
        }
    }
}

void deleteUser(int sd)
{
    for(vector<Usuario>::iterator i = users.begin(); i != users.end(); i++)
    {
        if(i->getSd() == sd)
        {
            users.erase(i);
            numUsers--;
        }
    }
}


bool searchGame(int sd)
{

    if(nGames == 0)
    {
        Juego newGame;
        string quote = getRandomLine();
        string equote = encryptQuote(quote);

        newGame.setIDGame(1);
        newGame.setSd1(sd);
        newGame.SetPoints1(0);
        newGame.setPoints2(0);
        newGame.setTurn(1);
        newGame.setQuote(quote);
        newGame.setEquote(equote);
        newGame.setLetter("-");
        newGame.setNumP(1);

        for(int j=0; j<users.size(); j++)
        {
            if(users[j].getSd == sd)
            {
                users[j].setStatus(4);
            }
        }

        createGame(newGame);

        return true;
    }
    
    else if(nGames>0 && nGames<10)
    {
        for(int i=0; i<games.size(); i++)
        {
            if(games[i].getNumP()==1)
            {
                games[i].setSd2(sd);
                games[i].setNumP(2);

                return true;
            }
        }

        Juego newGame;
        quote = getRandomLine();
        equote = encryptQuote(quote);

        newGame.setIDGame(1);
        newGame.setSd1(sd);
        newGame.SetPoints1(0);
        newGame.setPoints2(0);
        newGame.setTurn(1);
        newGame.setQuote(quote);
        newGame.setEquote(equote);
        newGame.setLetter("-");
        newGame.setNumP(1);

        createGame(newGame);

        for(int k=0; j<users.size(); k++)
        {
            if(users[k].getSd == sd)
            {
                users[k].setStatus(4);
            }
        }

        return true;

    }

    return false;

}

Juego getUserGame(int sd)
{
    for(int i=0; i<users.size(); i++)
    {
        if(users[i].getSd()==sd)
        {
            return user[i].getIdGame;
        }
    }

    return -1;
}



int main ( )
{
    system("clear");

	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
    char identificador[MSG_SIZE];
	socklen_t from_len;
    int on, ret, salida, recibidos;

	//Abrir socket

  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    	exit (1);	
	}
    
    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(PORT);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}

	from_len = sizeof (from);


	if(listen(sd,1) == -1){
		perror("Error en la operación de listen");
		exit(1);
	}

    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);

    signal(SIGINT,manejador);
    
    //Servidor acepta petición
	while(1)
    {      
    //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)       
        auxfds = readfds;
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
        if(salida > 0)
        {    
            for(int i=0; i<FD_SETSIZE; i++)
            {
                //Buscamos el socket por el que se ha establecido la comunicación
                if(FD_ISSET(i, &auxfds)) 
                {
                    if( i == sd)
                    {  
                        if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
                        {
                            perror("Error aceptando peticiones");
                        }
                        else
                        {
                            if(numUsers < MAX_CLIENTS)
                            {
                                arrayUsers[numUsers] = new_sd;
                                numUsers++;
                                FD_SET(new_sd,&readfds);
                            
                                strcpy(buffer, "Bienvenido al juego de la Ruleta de la Suerte\n");                            
                                send(new_sd,buffer,sizeof(buffer),0);
                            
                                for(int j=0; j<(numUsers-1);j++)
                                {
                                    bzero(buffer,sizeof(buffer));
                                    sprintf(buffer, "Nuevo Cliente conectado. SD = %d\n",new_sd);
                                    send(arrayUsers[j],buffer,sizeof(buffer),0);
                                }
                            }

                            else
                            {
                                bzero(buffer,sizeof(buffer));
                                strcpy(buffer,"Demasiados clientes conectados\n");
                                send(new_sd,buffer,sizeof(buffer),0);
                                close(new_sd);
                            }        
                        }     
                    }
                    else if (i == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        fgets(buffer, sizeof(buffer),stdin);    
                        //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                        if(strcmp(buffer,"SALIR\n") == 0)
                        {
                                
                            for (int j = 0; j < numUsers; j++)
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer,"Desconexión servidor\n"); 
                                send(arrayUsers[j],buffer , sizeof(buffer),0);
                                close(arrayUsers[j]);
                                FD_CLR(arrayUsers[j],&readfds);
                            }
                            close(sd);
                            exit(-1);        
                        }      
                    }

                    else
                    {
                    //Mensajes de los usuarios
                        bzero(buffer,sizeof(buffer));  
                        recibidos = recv(i,buffer,sizeof(buffer),0);
                        if(recibidos > 0)
                        {
                            if(strcmp(buffer,"SALIR\n") == 0)
                            {
                                salirCliente(i,&readfds,&numUsers,arrayUsers);       
                            }
                            else if((cadenaComienzaCon(buffer, "REGISTER")) && (getStatus(i) == -1))
                            {
                                string entrada(buffer);
                                entrada.erase(0, 11);

                                bool nuevo = true;
                                char *aux;
                                char *name;
                                char *password;
                                char *flag;
                                aux = strtok(buffer, " ");
                                aux = strtok(NULL, " ");
                                aux = strtok(NULL, " ");
                                name = aux;
                                aux = strtok(NULL, " ");
                                flag = aux;
                                aux = strtok(NULL, "\n");
                                password = aux;

                                string strname(name);
                                string strpass(password);

                                strname += "|";
                                strname += strpass;
                                char *charEntrada = strdup(strname.c_str());
                                string strlane(charEntrada);
                                strlane += "\n";
                                char *salida = strdup(strlane.c_str());
                                
                                FILE *fichero;
                                fichero = fopen("users.txt", "r+");
                                
                                if(fichero == nullptr)
                                {
                                    exit(-1);
                                }                                

                                char *linea = nullptr; 
                                size_t n = 0;

                                while ((getline(&linea,&n,fichero)) != -1)
                                {
                                    linea = strtok(linea, "|");
                                    
                                    if(strcmp(linea, name) == 0)
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "–ERR. Usuario ya registrado!\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                        nuevo = false;
                                        //TODO: Posible fallo al sacar user
                                        salirCliente(i,&readfds,&numUsers,arrayUsers);  
                                    }
                                }
                                
                                if(nuevo == true)
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "+Ok. Usuario registrado con exito!\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                    Usuario usuario;
                                    usuario.setName(name);
                                    usuario.setSd(i);
                                    usuario.setPassword(password);
                                    usuario.setIdGame(-1);
                                    usuario.setStatus(2);
                                    users.push_back(usuario);
                                    
                                    FILE *fichero2;
                                    fichero2 = fopen("users.txt", "a");

                                    if(fichero2 == nullptr)
                                    {
                                        exit(-1);
                                    }                                
                                    fputs(salida, fichero2);
 	                                fclose ( fichero2 );
                                }
                                fclose(fichero);
                            }

                            else if((cadenaComienzaCon(buffer, "USUARIO")) && (getStatus(i) == -1))
                            {
                                if (cadenaComienzaCon(buffer, "USUARIO\n"))
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "–ERR. Error en la validación\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                                else
                                {
                                    char *aux;
                                    aux = strtok(buffer, " ");
                                    aux = strtok(NULL, "\n");

                                    FILE *fichero;
                                    fichero = fopen("users.txt", "r");
                                    if(fichero == nullptr)
                                    {
                                        exit(-1);
                                    }                                

                                    char *linea = nullptr; 
                                    size_t n = 0;

                                    Usuario newUser;
                                    string name_ = aux;
                                    
                                    if(newUser.checkName(name_) == true)
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "+OK. Nombre de usuario correcto!\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                        salirCliente(i,&readfds,&numUsers,arrayUsers); 
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "+OK. Nombre de usuario correcto!\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                        newUser.setIdGame(-1);
                                        newUser.setName(name_);
                                        newUser.setSd(i);
                                        newUser.setStatus(1);
                                        users.push_back(newUser);
                                    }
                                    fclose(fichero);
                                }
                            }

                            else if((cadenaComienzaCon(buffer, "PASSWORD")) && (getStatus(i) == 1))
                            {
                                //TODO: Acceder a la instancia XY del vector
                                char *aux;
                                aux = strtok(buffer, " ");
                                aux = strtok(NULL, "\n");
                                string pass_ = aux;

                                for(int u = 0; u < users.size(); u++)
                                {
                                    if(users[u].getSd() == i)
                                    {
                                        string name_ = users[u].getName();
                                        if(users[u].checkLogin(name_, pass_) == true)
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "+OK. Inicio de sesión correcto!\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                            users[u].setPassword(pass_);
                                        }
                                        else
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-ERR. Password incorrecta!\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                            deleteUser(i);
                                            salirCliente(i,&readfds,&numUsers,arrayUsers);
                                        }
                                    }
                                }
                            }

                            //TODO: SISTEMA DE JUEGO, FUNCIONES SEPARADAS SEGUN STATUS
                            else if((cadenaComienzaCon(buffer, "INICIAR PARTIDA")) && (getStatus(i) == 2))
                            {
                                //Comprobar si hay alguien en cola
                            }
                            else if((cadenaComienzaCon(buffer, "CONSONANTE")) && (getStatus(i) == 4))
                            {
                                //Checkea idjuego de cada jugador
                                //Desargarnos las variables
                                //Juegar con esas variables
                                //Guardar cambios
                            }
                            else if((cadenaComienzaCon(buffer, "VOCAL")) && (getStatus(i) == 4))
                            {
                                //Checkea idjuego de cada jugador
                                //Desargarnos las variables
                                //Juegar con esas variables
                                //Guardar cambios
                            }
                            else if((cadenaComienzaCon(buffer, "RESOLVER")) && (getStatus(i) == 4))
                            {
                                //Checkea idjuego de cada jugador
                                //Desargarnos las variables
                                //Juegar con esas variables
                                //Guardar cambios
                            }
                            else if((cadenaComienzaCon(buffer, "PUNTUACION")) && (getStatus(i) == 4))
                            {
                                //Checkea idjuego de cada jugador
                                //Desargarnos las variables
                                //Juegar con esas variables
                                //Guardar cambios
                            }
                            else 
                            {   
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "-Err. Opcion NO valida!\n");
                                send(i, buffer, sizeof(buffer), 0);
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos == 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                salirCliente(i,&readfds,&numUsers,arrayUsers);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
}