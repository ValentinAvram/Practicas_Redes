#include <stdio.h>
#include <iostream>
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
int numUsers = 0, nGames = 0;
fd_set readfds, auxfds;
int sd, new_sd;

bool cadenaComienzaCon(const char *cadena1, const char *cadena2)
{
    int longitud = strlen(cadena2);
    if (strncmp(cadena1, cadena2, longitud) == 0) return true;
    return false;
} 

bool getStatus(int sd, int num)
{
    for(int i = 0; i < users.size(); i++)
    {
        if((users[i].getSd() == sd) && (users[i].getStatus()) != num)
        {
            return false;
        }
    }
    return true;
}

int checkStatus(int sd)
{
    for(int i = 0; i < users.size(); i++)
    {
        if(users[i].getSd() == sd)
        {
            return users[i].getStatus();
        }
    }
    return -1;
}

void salirCliente(int socket, fd_set * readfds, int * numUsers, int arrayClientes[])
{
  
    char buffer[350];
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
   
    for(int o = 0; o<users.size(); o++)
    {
        if(users[o].getSd() == socket)
        {
            users.erase(users.begin() + o);
        }
    }
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
        string quote = newGame.getRandomLine();
        string equote = newGame.encryptQuote(quote);

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
            if(users[j].getSd() == sd)
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
                for(int j=0; j<users.size(); j++)
                {
                    if(users[j].getSd() == sd)
                    {
                        users[j].setStatus(4);
                    }
                }
                return true;
            }
        }

        Juego newGame;
        string quote = newGame.getRandomLine();
        string equote = newGame.encryptQuote(quote);

        newGame.setIDGame(nGames);
        newGame.setSd1(sd);
        newGame.SetPoints1(0);
        newGame.setPoints2(0);
        newGame.setTurn(1);
        newGame.setQuote(quote);
        newGame.setEquote(equote);
        newGame.setLetter("-");
        newGame.setNumP(1);

        createGame(newGame);

        for(int k=0; k<users.size(); k++)
        {
            if(users[k].getSd() == sd)
            {
                users[k].setStatus(4);
            }
        }

        return true;

    }

    return false;

}

int getUserGame(int sd)
{
    for(int i=0; i<users.size(); i++)
    {
        if(users[i].getSd()==sd)
        {
            return users[i].getIdGame();
        }
    }
    return -1;
}

//TODO: Funcion de unir jugadores
//TODO: Funcion que busca game asociado a un jugador / -1;

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
                                bzero(buffer, sizeof(buffer));
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
                            else if((cadenaComienzaCon(buffer, "REGISTER")) && (getStatus(i, -1)))
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

                                strname +="|";
                                strname += strpass;
                                strname += "\n";
                                
                                const char *salida = strname.c_str();

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

                            else if((cadenaComienzaCon(buffer, "USUARIO")) && (getStatus(i, -1) ))
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
                                    Usuario newUser;
                                    string name_ = aux;

                                    if(newUser.checkName(name_) == true)
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
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "-Err. Nombre de usuario incorrecto!\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                        salirCliente(i,&readfds,&numUsers,arrayUsers);
                                    }
                                }
                            }

                            else if((cadenaComienzaCon(buffer, "PASSWORD")) && (getStatus(i, 1)))
                            {
                                //TODO CHECK STATUS
                                if (cadenaComienzaCon(buffer, "PASSWORD\n"))
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
                                    string pass_ = aux;
                                    string name_ ="*";
                                    for(int l = 0; l < users.size(); l++)
                                    {
                                        if(users[l].getSd() == i)
                                        {
                                           name_ = users[l].getName();
                                        }
                                    }

                                    Usuario fakeUser;
                                    if(name_ != "*")
                                    {
                                        if(fakeUser.checkLogin(name_, pass_) == true)
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "+OK. Inicio de sesion correcto\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                            for(int l = 0; l < users.size(); l++)
                                            {
                                                if(users[l].getSd() == i)
                                                {
                                                    users[l].setPassword(pass_);
                                                    users[l].setStatus(2);
                                                }   
                                            }
                                        }
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "–ERR. Error en la validación\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }
                                }
                            }

                            //TODO: SISTEMA DE JUEGO, FUNCIONES SEPARADAS SEGUN STATUS
                            else if((cadenaComienzaCon(buffer, "INICIAR PARTIDA")) && (getStatus(i,2)))
                            {
                                //Añadir comprobaciones externas status
                                if(checkStatus(i) == 2)
                                {
                                    //TODO:
                                    if(searchGame(i) == true)
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "+OK. Buscando partida . . .\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "+OK. En cuanto tenga un rival, comenzara la partida!\n");
                                        send(i, buffer, sizeof(buffer), 0);

                                        int idg = getUserGame(i);
                                        for(int z = 0; z < games.size(); z++)
                                        {
                                            if(games[z].getNumP() == 2)
                                            {
                                                int j1 = games[z].getSd1();
                                                int j2 = games[z].getSd2();

                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+OK. Rival encontrado\n");
                                                send(j1, buffer, sizeof(buffer), 0);
                                                sleep(1);
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+OK. Partida Iniciada!\n");
                                                send(j2, buffer, sizeof(buffer), 0);
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+OK. Eres el jugador 1\n");
                                                send(j1, buffer, sizeof(buffer), 0);
                                                bzero(buffer, sizeof(buffer));
                                                sprintf(buffer, "+OK. El panel es : %s\n", games[z].getEquote());
                                                send(j1, buffer, sizeof(buffer), 0);

                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+OK. Rival encontrado\n");
                                                send(j1, buffer, sizeof(buffer), 0);
                                                sleep(1);
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+OK. Partida Iniciada!\n");
                                                send(j2, buffer, sizeof(buffer), 0);
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+OK. Eres el jugador 2\n");
                                                send(j2, buffer, sizeof(buffer), 0);
                                                bzero(buffer, sizeof(buffer));
                                                sprintf(buffer, "+OK. El panel es : %s\n", games[z].getEquote());
                                                send(j2, buffer, sizeof(buffer), 0);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "-ERR. Lo sentimos, nuestros servidores estan llenos. Intentelo mas tarde!\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }

                                }
                                else
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "-Err. Opcion NO valida!\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                            }


                            //TODO: comprobar isVowel que creo que puede dar algun fallo y errores al compilar
                            
                            else if((cadenaComienzaCon(buffer, "CONSONANTE")) && (getStatus(i,4)))
                            {
                                if (cadenaComienzaCon(buffer, "CONSONANTE\n"))
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "–ERR. Error en la validación\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }

                                //Añadir comprobaciones externas status
                                if(checkStatus(i) == 4)
                                {
                                int idg = getUserGame(i);
                                
                                char *aux;
                                aux = strtok(buffer, " ");
                                aux = strtok(NULL, "\n");
                                string con = aux;
                                cout<<"tamano "<<con.lenght<<endl;

                                if(con.lenght() > 1)
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "–ERR. Error opcion NO valida\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }

                                //comprobar que es consonante
                                
                                for(int o = 0 ; o<games.size() ; o++)
                                {
                                    if(games[o].getIdJuego() == idg)
                                    {
                                        if((games[o].getTurn() == 1) && (games[o].getSd1() == i))
                                        {
                                            if(!isVowel(con))
                                            {
                                                if(games[o].getRight(games[o].getQuote(), con))
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Has acertado la letra\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    games[o].revealLetterInPanel(games[o].getQuote(), games[o].getEquote(), con);
                                                    games[o].setPoints1(games[o].getPuntos1() + 10);

                                                    if(games[o].isComplete(games[o].getQuote(), games[o].getEquote()))
                                                    {
                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "+Ok. Has ganado!!\n");
                                                        send(i, buffer, sizeof(buffer), 0);

                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "-Err. Has perdido...\n");
                                                        send(games[o].getSd2(), buffer, sizeof(buffer), 0);
                                                    }
                                                }

                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Has fallado la letra\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Err. Te toca\n");
                                                    send(games[o].getSd2(), buffer, sizeof(buffer), 0);

                                                    games[o].setTurn(2);
                                                }
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "-Err. Eso no es una consonante\n");
                                                send(i, buffer, sizeof(buffer), 0);

                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+Ok. Te toca\n");
                                                send(games[o].getSd2(), buffer, sizeof(buffer), 0);
                                                games[o].setTurn(2);
                                            }
                                        }

                                        else if((games[o].getTurn() == 2) && (games[o].getSd2() == i))
                                        {
                                            if(!isVowel(con))
                                            {
                                                if(games[o].getRight(games[o].getQuote(), 0))
                                                {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+Ok. Has acertado la letra\n");
                                                send(i, buffer, sizeof(buffer), 0);

                                                games[o].revealLetterInPanel(games[o].getQuote(), games[o].getEquote(), con);
                                                games[o].setPoints1(games[o].getPuntos1() + 10);

                                                    if(games[o].isComplete(games[o].getQuote(), games[o].getEquote()))
                                                    {
                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "+Ok. Has ganado!!\n");
                                                        send(i, buffer, sizeof(buffer), 0);

                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "-Err. Has perdido...\n");
                                                        send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                    }
                                                }

                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Has fallado la letra\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Te toca\n");
                                                    send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                    games[o].setTurn(1);
                                                }
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "-Err. Eso no es una consonante\n");
                                                send(i, buffer, sizeof(buffer), 0);

                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+Ok. Te toca\n");
                                                send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                games[o].setTurn(1);
                                            }
                                            
                                            
                                        }

                                        else
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-Err. Fallo en el juego\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                        }
                                    }
                                }
                                    //TODO:
                                }
                                else
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "-Err. Opcion NO valida!\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                            }
                            else if((cadenaComienzaCon(buffer, "VOCAL")) && (getStatus(i,4)))
                            {
                                //Añadir comprobaciones externas status 
                                if(checkStatus(i) == 4)
                                {
                                    int idg = getUserGame(i);
                                    
                                    char *aux;
                                    aux = strtok(buffer, " ");
                                    aux = strtok(NULL, "\n");
                                    string voc = aux;
                                    cout<<"tamano "<<voc.lenght<<endl;

                                    if(con.lenght() > 1)
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "–ERR. Error opcion NO valida\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }


                                    for(int o = 0 ; o<games.size() ; o++)
                                    {
                                        if(games[o].getIdJuego() == idg)
                                        {
                                            if((games[o].getTurn() == 1) && (games[o].getSd1() == i))
                                            { 
                                                if(isVowel(voc))
                                                {
                                                    if(games[o].getPoints1()<50)
                                                    {
                                                        games[o].setPoints1(games[o].getPuntos1()-50);
                                                        if(games[o].getRight(games[o].getQuote(), voc))
                                                        {
                                                            bzero(buffer, sizeof(buffer));
                                                            strcpy(buffer, "+Ok. Has acertado la letra\n");
                                                            send(i, buffer, sizeof(buffer), 0);

                                                            games[o].revealLetterInPanel(games[o].getQuote(), games[o].getEquote(), voc);
                                                            games[o].setPoints1(games[o].getPuntos1() + 10);

                                                            if(games[o].isComplete(games[o].getQuote(), games[o].getEquote()))
                                                            {
                                                                bzero(buffer, sizeof(buffer));
                                                                strcpy(buffer, "+Ok. Has ganado!!\n");
                                                                send(i, buffer, sizeof(buffer), 0);

                                                                bzero(buffer, sizeof(buffer));
                                                                strcpy(buffer, "-Err. Has perdido...\n");
                                                                send(games[o].getSd2(), buffer, sizeof(buffer), 0);
                                                            }
                                                        }

                                                        else
                                                        {
                                                            bzero(buffer, sizeof(buffer));
                                                            strcpy(buffer, "-Err. Has fallado la letra\n");
                                                            send(i, buffer, sizeof(buffer), 0);

                                                            games[o].setTurn(2);
                                                            bzero(buffer, sizeof(buffer));
                                                            strcpy(buffer, "+Ok. Te toca\n");
                                                            send(games[o].getSd2(), buffer, sizeof(buffer), 0);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "-Err. No tienes puntos\n");
                                                        send(i, buffer, sizeof(buffer), 0); 
                                                    }
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Eso no es una vocal\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Tu turno\n");
                                                    send(games[o].getSd2(), buffer, sizeof(buffer), 0);
                                                }
                                            }

                                            else if((games[o].getTurn() == 2) && (games[o].getSd2() == i))
                                            {
                                                if(isVowel(voc))
                                                {
                                                    if(games[o].getPoints2()<50)
                                                    {
                                                        games[o].setPoints2(games[o].getPuntos2()-50);
                                                        if(games[o].getRight(games[o].getQuote(), voc))
                                                        {
                                                            bzero(buffer, sizeof(buffer));
                                                            strcpy(buffer, "+Ok. Has acertado la letra\n");
                                                            send(i, buffer, sizeof(buffer), 0);

                                                            games[o].revealLetterInPanel(games[o].getQuote(), games[o].getEquote(), voc);
                                                            games[o].setPoints2(games[o].getPuntos1() + 10);

                                                            if(games[o].isComplete(games[o].getQuote(), games[o].getEquote()))
                                                            {
                                                                bzero(buffer, sizeof(buffer));
                                                                strcpy(buffer, "+Ok. Has ganado!!\n");
                                                                send(i, buffer, sizeof(buffer), 0);

                                                                bzero(buffer, sizeof(buffer));
                                                                strcpy(buffer, "-Err. Has perdido...\n");
                                                                send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                            }
                                                        }

                                                        else
                                                        {
                                                            bzero(buffer, sizeof(buffer));
                                                            strcpy(buffer, "-Err. Has fallado la letra\n");
                                                            send(i, buffer, sizeof(buffer), 0);

                                                            games[o].setTurn(2);
                                                            bzero(buffer, sizeof(buffer));
                                                            strcpy(buffer, "+Ok. Te toca\n");
                                                            send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "-Err. No tienes puntos\n");
                                                        send(i, buffer, sizeof(buffer), 0); 
                                                    }
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Eso no es una vocal\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Tu turno\n");
                                                    send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                }

                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "-Err. Fallo en el juego\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "-Err. Opcion NO valida!\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }        
                            }
                            else if((cadenaComienzaCon(buffer, "RESOLVER")) && (getStatus(i,4)))
                            {
                                //Añadir comprobaciones externas status
                                if(checkStatus(i) == 4)
                                {
                                    //TODO:
                                    int idg = getUserGame(i);
                                    
                                    char *aux;
                                    aux = strtok(buffer, " ");
                                    aux = strtok(NULL, "\n");
                                    string res = aux;

                                    for(int o; o < games.size(); o++)
                                    {
                                        if(games[o].getIdJuego() == idg)
                                        {
                                            if((games[o].getTurn() == 1) && (games[o].getSd1() == i))
                                            {
                                                if(games[o].Resolver(games[o].getQuote(), res))
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Has ganado!!\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Has perdido...\n");
                                                    send(games[o].getSd2(), buffer, sizeof(buffer), 0);
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Has perdido...\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Has ganado!!\n");
                                                    send(games[o].getSd2(), buffer, sizeof(buffer), 0);
                                                }
                                            }
                                            else if((games[o].getTurn() == 2) && (games[o].getSd2() == i))
                                            {
                                                if(games[o].Resolver(games[o].getQuote(), res))
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Has ganado!!\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Has perdido...\n");
                                                    send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Has perdido...\n");
                                                    send(i, buffer, sizeof(buffer), 0);

                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Has ganado!!\n");
                                                    send(games[o].getSd1(), buffer, sizeof(buffer), 0);
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "-Err. Opcion NO valida!\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                            }
                            else if((cadenaComienzaCon(buffer, "PUNTUACION")) && (getStatus(i,4)))
                            {
                                //Añadir comprobaciones externas status
                                int idg;
                                int p1 =-10;
                                int p2 =-10;
                            
                                if(checkStatus(i) == 4)
                                {
                                    int idg = getUserGame(i);

                                    for(int t = 0; t < games.size(); t++)
                                    {
                                        if(games[t].getIdJuego() == idg)
                                        {
                                            if(games[t].getSd1() == i)
                                            {
                                                p1 = games[t].getPuntos1();
                                            }
                                            else
                                            {
                                                p2 = games[t].getPuntos2();
                                            }
                                        }
                                    }
                                    if(p1 !=-10)
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        sprintf(buffer, "+OK. Su puntuacion es : %i\n", p1);
                                        send(i, buffer, sizeof(buffer), 0);    
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        sprintf(buffer, "+OK. Su puntuacion es : %i\n", p2);
                                        send(i, buffer, sizeof(buffer), 0); 
                                    }
                                }
                                else
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "-Err. Opcion NO valida!\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                            }
                            else 
                            {   
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "-Err. Opcion NO valida!\n");
                                send(i, buffer, sizeof(buffer), 0);
                            }
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