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
#include <string>
#include <iostream>
#include <vector>
#include <fstream>


#define MSG_SIZE 350
#define MAX_CLIENTS 30
#define PORT 2050
#define MAX_GAMES 10

using namespace std;

vector<User> clientes;
vector<Ruleta> games;
int ngames = 0;

//Funciones ficheros
void escrituraTXT(char buffer[350]);
void lecturaTXT();

//Funciones cadenas
char stringToChar(string texto);
string charToString(char buffer[350], int char_size);
void split (string str, char seperator);//TODO: A lo mejor borrar split?
int cadenaComienzaCon(const char *cadena1, const char *cadena2); 

//Funciones server
void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

// Control de juego
int unirJugadores(int sd);
int parejaJuego(int sd);
int unirJugadores(int sd);
bool userCorrecto(string nombreUser);
bool passCorrecto(string passUser);
bool login(int sdUser, string nombre);
bool unlogUser(int sdUser);
bool deleteGame(int sdUser);
Juego getGame(int sdUser);

int main ( )
{
    system("clear");
    cout<<"Main.exe\n";
    // Aqui comprobar lentamente las funciones de clase serv
    return 0;
}

char stringToChar(string texto)
{
    char *buffer = const_cast<char*>(texto.c_str());
    return *buffer;
}

//int char_size = sizeof(buffer) / sizeof(char);
string charToString(char buffer[350], int char_size)
{
    int g;
    string texto = "";
    for (g = 0; g < char_size; g++) {
        texto = texto + buffer[g];
    }
    return texto;
}
//TODO: A lo mejor borrar split?
void split (string str, char seperator)  
{  
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

int cadenaComienzaCon(const char *cadena1, const char *cadena2){
    int longitud = strlen(cadena2);
    if (strncmp(cadena1, cadena2, longitud) == 0) return 1;
    return 0;
}

void escrituraTXT(char buffer[350])
{
    FILE* fichero;
    fichero = fopen("users.txt", "w");
    fputs(buffer, fichero); //TODO: Añadir \n al buffer en el main
    fclose(fichero);
}

void lecturaTXT()
{
    FILE *fichero;
    fichero = fopen("users.txt", "r");
    if (!f)
    {
        printf("Error abriendo el archivo")
        exit(-1);
    }
    char buffer[MSG_SIZE];
    while (fgets(buffer, MSG_SIZE, fichero) != NULL)
    {
        Cliente newCliente;
        char *data;
        
        data = strtok(cadena, "//");
        newCliente.setNombre(aux);
        
        data = strtok(NULL, "\n");
        newCliente.setNombre(data);
        
    }
    fclose(fichero);
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
            games.push_back(game);
            return 1;
        }//TODO: Cambiar esto. Reestructurar array modo profe
        else if (ngames < 10)
        {
            Juego juego;
            juego.newPlayer(sd);
            nuevoGame(jeugo);
            return 1;
        }
    }
    return 0;
}

bool userCorrecto(string nombreUser)
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

bool passCorrecto(string passUser)
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

bool login(int sdUser, string nombre)
{
    for (int i = 0; i < (int)clientes.size(); i++)
    {
        if ((strcmp(clientes[i].getNombre(), nombre) == 0) && (users_[i].getSd() == -1))
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
{//TODO: Cambiada Iterator?
    for (int i = 0; i < (int)<games.size(); i++)
    {
        {
            if (games[i].getSd1() == sdUser)
            {
                games.erase(games[i]);
                ngames--;
                return true;
            }
            else if (games[i].getSd2() == sdUser)
            {
                games.erase(games[i]);
                ngames--;
                return true;
            }
        }
    }
    return false;
}

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