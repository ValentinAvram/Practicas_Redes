#include "juego.h"
#include <time.h>
#include <fstream>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

bool cadenaComienza(const char *cadena1, const char *cadena2);

Juego::Juego()
{
    puntos1 = 0;
    puntos2 = 0;
    numP = 0;
}

bool Juego::newPlayer(int sd)
{
    if (numP == 0){
        setSd1(sd);
        numP++;
        return true;
    }
    else if(numP == 1){
        setSd2(sd);
        numP++;
        return true;
    }
    else{
        return false;
    }
}

char * Juego::encryptQuote(char *quote)
{ 
    char* equote;
    string efrase(quote);
    int n= strlen(quote);

    for(int g = 0; g<n-1; g++)
    {
        
        if(efrase[g]== ' ')
        {
            efrase[g] = ' ';
        }
        else
        {
            efrase[g] = '-';
        }
    }
    efrase[n+1]='\0';

    equote= strdup(efrase.c_str());

    return equote;
}

char *Juego::revealLetterInPanel(char *quote, char *equote, char *letter)
{ 

    int count=0;
    int n= strlen(quote);
    string quotestr(quote);
    string equotestr(equote);
    string letterstr(letter);
    for(int g = 0; g<n; g++)
    {
        if(equotestr[g]==letterstr[0])
        {
            letterstr[0]=42;
        }
    }
    for(int h=0; h<n+1; h++)
    {
    
        if(quotestr[h]==letterstr[0])
        {
            equotestr[h] = quotestr[h];
            count ++;
        }
    }
    equote= strdup(equotestr.c_str());

    return equote;
}

bool Juego::getRight(char *quote, char *letter)
{
    string quotestr(quote);
    string letterstr(letter);
    int n= strlen(quote);
    for(int g = 0; g<n; g++)
    {
        if(quote[g] == letter[0])
        {
            return true;
        }
    }

    return false;
}

bool Juego::isVowel(char *letter){ 
    
    string letra(letter);
    if(letra == "A"){
        return true;
    }

    if(letra == "E"){
        return true;
    }

    if(letra == "I"){
        return true;
    }

    if(letra == "O"){
        return true;
    }

    if(letra == "U"){
        return true;
    }

    return false;
}

bool Juego::hasMoney(int points){
    if(points<50)
    {
        return false;
    }
    return true;
}

bool Juego::Resolver(char *quote, int sd){ // NO FUNCIONA, NECESITA RCV Y SEND
    string quotestr(quote);
    char *buffer;
    int recibidos;

    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"+Ok. Introduzca el refran\nSi falla por ortografia, perderá\n");
    send(sd,buffer,250,0);
    
    recibidos = recv(sd,buffer,sizeof(buffer),0);
    string resolver(buffer);
    if (quote == resolver)//strcmp
    {
        bzero(buffer,sizeof(buffer));
        sprintf(buffer,"+Ok. Correcta!\n");
        send(sd,buffer,250,0);
        return true;
    }
    return false;
}

char *Juego::getRandomLine() // NO FUNCIONA, NECESITA RCV Y SEND
{
    int random = 0;
    srand(time(0));
    random = rand() % 8;

    FILE *fichero;
    fichero = fopen("refranes.txt", "r");
    if(fichero == nullptr)
    {
        exit(-1);
    }

    char *linea = nullptr;                               
    size_t n = 0;                                 
    vector<string> refranes;

    while ((getline(&linea, &n, fichero)) != -1)
    {
        refranes.push_back(linea); 
    }

    fclose(fichero);

    string quoteStr = refranes[random];
    quoteStr.pop_back(); 
    strncpy(quote, quoteStr.c_str(), 350);
    return quote;
}

void Juego::game(char* quote,int Puntos1, int Puntos2, int sd1, int sd2)
{
    cout<<"Entra aqui\n"<<endl;
    cout<<"sd1 es "<<sd1<<endl;
    cout<<"sd2 es "<<sd2<<endl;
    char* buffer;
    bzero(buffer,sizeof(buffer));
    sprintf(buffer, "+Ok. Partida encontrada !!\n");
    send(sd1, buffer, sizeof(buffer), 0);
    send(sd2, buffer, sizeof(buffer), 0);
    

    sprintf(buffer,"+Ok. Eres el jugador 1\n");
    send(sd1, buffer, sizeof(buffer), 0);

    sprintf(buffer,"+Ok. Eres el jugador 2\n");
    send(sd2, buffer, sizeof(buffer), 0);


    char *equote = encryptQuote(quote);
    sprintf(buffer, "+Ok. La frase encriptada es: %s \n",equote);
    send(sd1, buffer, sizeof(buffer), 0);
    send(sd2, buffer, sizeof(buffer), 0);
    int turn=0;
    char* letter;
    while(isComplete(quote, equote)==false)
    {
        cout<<"Entra en el sistema de turno fase 1 (isComplete) "<<isComplete(quote,equote)<<endl;
        while(turn == 0)
        {
            sprintf(buffer, "Turno del jugador 1: \n");
            send(sd1, buffer, sizeof(buffer), 0);
            send(sd2, buffer, sizeof(buffer), 0);
            cout<<"Entra en el sistema de turnos fase 2 (turn 0): "<<turn<<endl;
            turn = 1;
            

            bzero(buffer,sizeof(buffer));
            sprintf(buffer,"+Ok. Introduzca la letra que desea despejar, o bien elija RESOLVER\n");
            cout<<"peta aqui 1"<<endl;
            send(sd1,buffer,sizeof(buffer),0);
            cout<<"peta aqui 1"<<endl;
            //TODO: Leer cosas bien por buffer y fixear si hubiera que fixear
            //int recibidos1 = recv(sd1,letter,sizeof(letter),0);
            cout<<letter<<endl;
            if(cadenaComienza(letter, "RESOLVER"))
            {
                if(Resolver(quote,sd1)==true)
                {
                    bzero(buffer,sizeof(buffer));
                    sprintf(buffer,"+Ok. Ha ganado el jugador 1!\n");
                    send(sd1,buffer,sizeof(buffer),0);
                }
                else
                {
                    bzero(buffer,sizeof(buffer));
                    sprintf(buffer,"+Ok. Ha ganado el jugador 2!\n");
                    send(sd1,buffer,sizeof(buffer),0);
                }
            }
            else
            {
                if(isVowel(letter)==true)
                {
                    if(hasMoney(Puntos1)==true)
                    {
                        setPuntos1(getPuntos1()-50);
                        if(getRight(quote, letter)==true)
                        {
                            turn = 0;
                            equote=(revealLetterInPanel(quote,equote,letter));
                        }
                        else{
                            bzero(buffer,sizeof(buffer));
                            sprintf(buffer,"-Err. Ha fallado!\n");
                            send(sd1,buffer,sizeof(buffer),0);
                        }
                    }
                    else
                    {
                        bzero(buffer,sizeof(buffer));
                        sprintf(buffer,"-Err. No tiene suficientes puntos para comprar vocal. Repite turno!\n");
                        send(sd1,buffer,sizeof(buffer),0);
                        letter[0]=42;
                        turn=0;
                    }
                }
                else
                {
                    if(getRight(quote,letter)==true)
                    {
                        turn=0;
                        setPuntos1(getPuntos1()+50);
                        equote=revealLetterInPanel(quote,equote,letter);
                    }
                }
            }
        }
        while(turn==1)
        {
            cout<<"Entra en el sistema de turnos fase 2 (turn 1): "<<turn<<endl;
            turn = 0;
            
            bzero(buffer,sizeof(buffer));
            sprintf(buffer,"+Ok. Turno del jugador 2\n");
            send(sd2,buffer,sizeof(buffer),0);
            send(sd1,buffer,sizeof(buffer),0);
            sprintf(buffer,"+Ok. Introduzca la letra que desea despejar, o bien elija RESOLVER\n");
            send(sd2,buffer,sizeof(buffer),0);

            int recibidos1 = recv(sd2,letter,sizeof(letter),0);
               if(cadenaComienza(letter, "RESOLVER"))
               {
                   if(Resolver(quote,sd2)==true)
                   {
                        bzero(buffer,sizeof(buffer));
                        sprintf(buffer,"+Ok. Ha ganado el jugador 2!\n");
                        send(sd2,buffer,sizeof(buffer),0);
                   }
                   else
                   {
                        bzero(buffer,sizeof(buffer));
                        sprintf(buffer,"+Ok. Ha ganado el jugador 2!\n");
                        send(sd2,buffer,sizeof(buffer),0);
                   }

               }
               else
               {
                if(isVowel(letter)==true)
                {
                    if(hasMoney(Puntos2)==true)
                    {
                        setPuntos2(getPuntos2()-50);
                        if(getRight(quote, letter)==true)
                        {
                            turn=1;
                            equote=(revealLetterInPanel(quote,equote,letter));
                        }
                        else
                        {
                            bzero(buffer,sizeof(buffer));
                            sprintf(buffer,"-Err. Ha fallado!\n");
                            send(sd2,buffer,sizeof(buffer),0);
                        }

                    }
                    else
                    {
                        bzero(buffer,sizeof(buffer));
                        sprintf(buffer,"-Err. No tiene suficientes puntos para comprar vocal. Repite turno!\n");
                        send(sd2,buffer,sizeof(buffer),0);

                        letter[0]=42;
                        turn = 1;
                    }

                }
                else
                {
                    if(getRight(quote,letter)==true)
                    {
                        turn=1;
                        setPuntos2(getPuntos2()+50);
                        equote=revealLetterInPanel(quote,equote,letter);
                    }
                }
            }
        }
    }
}
bool Juego::isComplete(char*quote, char* equote){
    string quotestr(quote);
    string equotestr(equote);

    if(quotestr==equotestr){
        return true;
    }
    return false;
}

bool cadenaComienza(const char *cadena1, const char *cadena2){
    int longitud = strlen(cadena2);
    if (strncmp(cadena1, cadena2, longitud) == 0) return true;
    return false;
}