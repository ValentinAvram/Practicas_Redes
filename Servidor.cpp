#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "usuario.h" 
#include "juego.h"

#define MSG_SIZE 350
#define MAX_CLIENTS 30
#define PORT 2050
#define MAX_GAMES 10

using namespace std;

vector<Usuario> clientes;
vector<Juego> games;
int ngames = 0;

string strings[200];

//Funciones cadenas
bool cadenaComienzaCon(const char *cadena1, const char *cadena2); 
void split (string mensaje, char seperator);

//Funciones server
void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);
bool nombreCorrecto(const char *name);
bool passCorrecto(int descriptor, const char *pass);

// Control de juego
int unirJugadores(int sd);
int parejaJuego(int sd);
int unirJugadores(int sd);
bool userCorrecto(string nombreUser);
bool passCorrecto(string passUser);
bool login(int sdUser, char *nombre);
bool unlogUser(int sdUser);
bool deleteGame(int sdUser);
Juego getGame(int sdUser);
bool checkLoged(int sd);
bool checkRegistered(int sd);


int sd, new_sd;
int arrayClientes[MAX_CLIENTS];
int numClientes = 0;
char buffer[MSG_SIZE];
socklen_t from_len;
fd_set readfds, auxfds;

int main ( )
{
    system("clear");
	
	struct sockaddr_in sockname, from;
   	int salida;
	int recibidos;
   	char identificador[MSG_SIZE];
    int on, ret;

    // Abrir socket
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

	if(listen(sd,1) == -1)
    {
		perror("Error en la operación de listen");
		exit(1);
	}

    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);

    signal(SIGINT,manejador);
    
	//El servidor acepta una petición
	while(1)
    {
            
        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)  
        auxfds = readfds;
            
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
        if(salida > 0)
        {
            for(int i=0; i<FD_SETSIZE; i++)
            {   
                int cont = 0;
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
                            if(numClientes < MAX_CLIENTS)
                            {
                                arrayClientes[numClientes] = new_sd;
                                numClientes++;
                                FD_SET(new_sd,&readfds);
                                
                                strcpy(buffer, "Bienvenido al juego de la Ruleta de la Suerte\n");
                                
                                send(new_sd,buffer,sizeof(buffer),0);
                                
                                for(int j=0; j<(numClientes-1);j++)
                                {
                                    bzero(buffer,sizeof(buffer));
                                    sprintf(buffer, "Nuevo Cliente conectado\n");
                                    send(arrayClientes[j],buffer,sizeof(buffer),0);
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
                            for (int j = 0; j < numClientes; j++)
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer,"Desconexión servidor\n"); 
                                send(arrayClientes[j],buffer , sizeof(buffer),0);
                                close(arrayClientes[j]);
                                FD_CLR(arrayClientes[j],&readfds);
                            }
                            close(sd);
                            exit(-1);        
                        }
                    } 
                    else
                    {
                        bzero(buffer,sizeof(buffer));
                        recibidos = recv(i,buffer,sizeof(buffer),0);
                        
                        if(recibidos > 0)
                        {
                            if((cadenaComienzaCon(buffer, "REGISTER")) && (checkLoged(i) == false))
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
                                        strcpy(buffer, "–ERR. Usuario ya registrado!\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                        nuevo = false;
                                        //salirCliente(i,&readfds,&numClientes,arrayClientes);  
                                        //close(i);
                                    }
                                }
                                
                                if(nuevo == true)
                                {
                                    strcpy(buffer, "+Ok. Usuario registrado con exito!\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                    Usuario usuario;
                                    usuario.setNombre(name);
                                    usuario.setSd(i);
                                    usuario.setLoged(true);
                                    usuario.setPassword(password);
                                    usuario.setRegistered(true);
                                    clientes.push_back(usuario);
                                    
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

                            else if(strcmp(buffer,"SALIR\n") == 0)
                            {
                                salirCliente(i,&readfds,&numClientes,arrayClientes);   
                            }

                            else if ((cadenaComienzaCon(buffer, "USUARIO")) && ((checkLoged(i) == false) && checkRegistered(i) == false))
                            {
                                if (cadenaComienzaCon(buffer, "USUARIO\n"))
                                {
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

                                    while ((getline(&linea,&n,fichero)) != -1)
                                    {
                                        linea = strtok(linea, "|");

                                        if(strcmp(linea, aux) == 0)
                                        {
                                            Usuario usuario;
                                            usuario.setNombre(aux);
                                            usuario.setSd(i);
                                            usuario.setLoged(false);
                                                
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "+Ok. Usuario Correcto\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                            
                                                recibidos = recv(i,buffer,sizeof(buffer),0);
                                                if(recibidos > 0)
                                                {
                                                    if (cadenaComienzaCon(buffer, "PASSWORD"))
                                                    {
                                                        if (cadenaComienzaCon(buffer, "PASSWORD\n"))
                                                        {
                                                            strcpy(buffer, "–ERR. Error en la validacion\n");
                                                            send(i, buffer, sizeof(buffer), 0);
                                                        }
                                                        else
                                                        {
                                                            char *aux;
                                                            aux = strtok(buffer, " ");
                                                            aux = strtok(NULL, "\n");

                                                            FILE *fichero2;
                                                            fichero2 = fopen("users.txt", "r");
                                                            if(fichero2 == nullptr)
                                                            {
                                                                exit(-1);
                                                            }                                

                                                            char *linea2 = nullptr; 
                                                            size_t n2 = 0;

                                                            while ((getline(&linea2,&n2,fichero2)) != -1)
                                                            {
                                                                char sep='|';
                                                                split(linea, sep);  
                                                                string user = strings[0];
                                                                string lineastr(linea2);
                                                                bool eq=true;
                                                                lineastr.erase(0,user.size()+1);
                                                                lineastr.erase(lineastr.size()-2,lineastr.size());
                                                                linea=strdup(lineastr.c_str());

                                                                string auxstr(aux);
                                                                
                                                                for(int count=0; count<sizeof(aux);count++){
                                                                    if(linea[count]!=aux[count]){
                                                                        eq=false;
                                                                        
                                                                    }
                                                                }

                                                                if(eq == true)
                                                                {
                                                                    char *auxPass = strdup(lineastr.c_str());
                                                                    usuario.setPassword(auxPass);
                                                                    usuario.setLoged(true);
                                                                    usuario.setRegistered(false);
                                                                    clientes.push_back(usuario);
                                                                    strcpy(buffer, "+Ok. Usuario Validado\n");
                                                                    send(i, buffer, sizeof(buffer), 0);
                                                                }
                                                            } 
                                                            fclose ( fichero2 );
                                                        }
                                                    }
                                                    else if (cadenaComienzaCon(buffer, "PASSWORD\n") == false)
                                                    {
                                                        strcpy(buffer, "-Err. Introduzca su contraseña\n");
                                                        send(i, buffer, sizeof(buffer), 0);
                                                    }
                                                    if(usuario.getLoged() == false)
                                                    {
                                                    strcpy(buffer, "-Err. Inicio de sesión incorrecto.\n");
                                                    send(i, buffer, sizeof(buffer), 0);       
                                                    }
                                                }
                                            fclose ( fichero );
                                        }   
                                    }
                                }
                            }

                            else if((cadenaComienzaCon(buffer, "INICIAR-PARTIDA")) && ((checkLoged(i) == true) || checkRegistered(i) == true))
                            {
                                strcpy(buffer, "EL VIDEOJUEGO\n");
                                send(i, buffer, sizeof(buffer), 0);
                            }

                            else
                            {   bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "-Err. Opción NO valida\n");
                                send(i, buffer, sizeof(buffer), 0);
                            }
                        }

                            //Si el cliente introdujo ctrl+c
                        if(recibidos== 0)
                        {
                            printf("El socket %d, ha introducido ctrl+c\n", i);
                            salirCliente(i,&readfds,&numClientes,arrayClientes);
                        }
                    }
                }
            }
        }	    
    }
	close(sd);
	return 0;
}


bool cadenaComienzaCon(const char *cadena1, const char *cadena2){
    int longitud = strlen(cadena2);
    if (strncmp(cadena1, cadena2, longitud) == 0) return true;
    return false;
}

void nuevoGame(Juego juego)
{
    if (ngames < MAX_GAMES)
    {
        games.push_back(juego);
        ngames++;
    }
}

int parejaJuego(int sd)
{
    for (int i = 0; i < (int)games.size(); i++)
    {
        if (games[i].getNumP() == 2)
        {
            if (games[i].getSd1() == sd){return games[i].getSd2();}
            else if (games[i].getSd2() == sd){return games[i].getSd1();}
        }
    }
    return 0;
}

int unirJugadores(int sd)
{
    if (ngames == 0)
    {
        Juego juego;
        juego.newPlayer(sd);
        nuevoGame(juego);
        return 1;
    }
    else if (ngames > 1 &&  ngames < 10)
    {
        Juego juego = games[ngames - 1];
        if (juego.newPlayer(sd))
        {
            games.pop_back();
            games.push_back(juego);
            return 1;
        }//TODO: Cambiar esto. Reestructurar array modo profe
        else if (ngames < 10)
        {
            Juego juego;
            juego.newPlayer(sd);
            nuevoGame(juego);
            return 1;
        }
    }
    return 0;
}

bool userCorrecto(char *nombreUser)
{
    for(int i = 0; i < (int)clientes.size(); i++)
    {
        if(strcmp(clientes[i].getNombre(), nombreUser) == 0)
        {
            clientes[i].checkNombre = true;
            return true;
        }
    }
    return false;
}

bool passCorrecto(char *passUser)
{
    for(int i = 0; i < (int)clientes.size(); i++)
    {
        if(strcmp(clientes[i].getPassword(), passUser) == 0)
        {
            clientes[i].checkPass = true;
            return true;
        }
    }
    return false;
}

bool login(int sdUser, char *nombre)
{
    for (int i = 0; i < (int)clientes.size(); i++)
    {
        if ((strcmp(clientes[i].getNombre(), nombre) == 0) && (clientes[i].getSd() == -1))
        {
            clientes[i].setSd(sdUser);
            return true;
        }
    }
    return false;
}

bool unlogUser(int sdUser)
{
    for (int i = 0; i < (int)clientes.size(); i++)
    {
        if (clientes[i].getSd() == sdUser)
        {
            clientes[i].setSd(-1);
            return true;
        }
    }
    return false;
}

bool deleteGame(int sdUser)
{//TODO: IMPLEMENTAR ITERATOR
    for (vector<Juego>::iterator i = games.begin(); i != games.end(); i++)
    {
        {
            if (i->getSd1() == sdUser)
            {
                games.erase(i);
                ngames--;
                return true;
            }
            else if (i->getSd2() == sdUser)
            {
                games.erase(i);
                ngames--;
                return true;
            }
        }
    }
    return false;
}
//TODO: A lo mejor borrar
Juego getGame(int sdUser)
{
    for (int i = 0; i < (int)games.size(); i++)
    {
        if (games[i].getNumP() == 2)
        {
            if (games[i].getSd1() == sdUser)
            {
                return games[i];
            }
            else if (games[i].getSd2() == sdUser)
            {
                return games[i];
            }
        }
    }
    Juego voidGame;
    return voidGame;
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[])
{
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,sizeof(buffer),0);


}

void manejador (int signum){
    char buffer[350];
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    for (int j = 0; j < numClientes; j++)
    {
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "–Err. Cierre del servidor!\n");
        send(arrayClientes[j], buffer, sizeof(buffer), 0);
        close(arrayClientes[j]);
        FD_CLR(arrayClientes[j], &readfds);
    }
    close(sd);
    exit(-1);
}

void split (string str, char seperator)  
{  
    char *salida;
    int currIndex = 0, g = 0;  
    int startIndex = 0, endIndex = 0;  
    while (g <= str.length())  
    {  
        if (str[g] == seperator || g == str.length())  
        {  
            endIndex = g;  
            string subStr = "";  
            subStr.append(str, startIndex, endIndex - startIndex);  
            strings[currIndex] = subStr;  
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        g++;  
    }  
}

bool nombreCorrecto(const char *name)
{
    for (int i = 0; i < (int)clientes.size(); i++)
    {
        if (strcmp(clientes[i].getNombre(), name) == 0)
        {
            return true;
        }
    }
    return false;
}

bool passCorrecto(int descriptor, const char *pass)
{
    for (int i = 0; i < (int)clientes.size(); i++)
    {
        if ((strcmp(clientes[i].getPassword(), pass) == 0) && (clientes[i].getSd() == descriptor))
        {
            return true;
        }
    }
    return false;
}

bool checkLoged(int sd)
{
    for(int i = 0; i < clientes.size(); i++)
    {
        if((clientes[i].getSd() == sd && clientes[i].getLoged() == true))
        {
            return true;
        }
    }
    return false;
}

bool checkRegistered(int sd)
{
    for(int i = 0; i < clientes.size(); i++)
    {
        if((clientes[i].getSd() == sd && clientes[i].getRegistered() == true))
        {
            return true;
        }
    }
    return false;
}