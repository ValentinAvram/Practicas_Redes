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

//Funciones ficheros
void escrituraTXT(char buffer[350]);
void lecturaTXT();

//Funciones cadenas
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
//TODO: Funcion send y recieve

int main ( )
{
    system("clear");
    cout<<"Main.exe\n";
    // Aqui comprobar lentamente las funciones de clase serv
    /* Todo esto son ejemplos de funciones y todas funcionan correctamente :)
    Juego juego1;
    char *frase=juego1.getRandomLine();
    cout << "La frase es " << frase<<endl;
    char* efrase=juego1.encryptQuote(frase);
    cout << "La frase encriptada es "<<efrase<<endl;
    string letterstr = "A";
    char *letra= strdup(letterstr.c_str());
    efrase = juego1.revealLetterInPanel(frase, efrase, letra);
    cout <<"Revelar A: " << efrase << endl;
    string letratrue = "A";
    string letrafalse= "W";
    char *lettertrue = strdup(letratrue.c_str());
    char *letterfalse = strdup(letrafalse.c_str());
    if(juego1.getRight(frase, lettertrue) == true){
       cout<<"Hay A"<<endl;
    }
    if(juego1.getRight(frase, letterfalse) == false){
       cout<<"No hay W"<<endl;
    }
   if(juego1.isVowel(lettertrue)==true){
       cout<<lettertrue<<" es vocal"<<endl;
   }
   if(juego1.isVowel(letterfalse)==false){
       cout<<letterfalse<<" no es vocal"<<endl;
   }
   if(juego1.hasMoney(49)==false){
       cout<<"No hay dinero"<<endl;
   }
   if(juego1.hasMoney(51)==true){
       cout<<"Hay billetes"<<endl;
   }
    if(juego1.Resolver(frase)==true){
        cout<<"Funciona esto cuando aciertas"<<endl;
    }
    if(juego1.Resolver(frase)==false){
        cout<<"Funciona esto cuando fallas"<<endl;
    }
    */
    return 0;
}


int cadenaComienzaCon(const char *cadena1, const char *cadena2){
    int longitud = strlen(cadena2);
    if (strncmp(cadena1, cadena2, longitud) == 0) return 1;
    return 0;
}

void escrituraTXT(char buffer[350]) //TODO:
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
    if (!fichero)
    {
        printf("Error abriendo el archivo");
        exit(-1);
    }
    char buffer[MSG_SIZE];
    while (fgets(buffer, MSG_SIZE, fichero) != NULL)
    {
        Usuario newCliente;
        char *data;
        
        data = strtok(buffer, "//");
        newCliente.setNombre(data);
        
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

