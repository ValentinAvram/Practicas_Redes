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
    points1 = 0;
    points2 = 0;
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
}// If return false, game is full

string Juego::encryptQuote(string quote)
{
    int n= quote.size();

    for(int g = 0; g<n-1; g++)
    {
        
        if(quote[g]== ' ')
        {
            quote[g] = ' ';
        }
        else
        {
            quote[g] = '-';
        }
    }
    quote[n+1]='\0';

    return quote;
}

string Juego::revealLetterInPanel(string quote, string equote, string letter)
{
    int count = 0;
    int n = quote.size();

    for(int g = 0; g<n; g++)
    {
        if(equote[g]==letter[0])
        {
            letter[0]=42;
        }
    }

    for(int h=0; h<n+1; h++)
    {
        if(quote[h]==letter[0])
        {
            equote[h] = quote[h];
            count ++;
        }
    }

    return equote;
}

bool Juego::getRight(string quote, string letter)
{
    int n = quote.size();

    for(int g = 0; g<n; g++)
    {
        if(quote[g] == letter[0])
        {
            return true;
        }
    }

    return false;
}

bool Juego::isVowel(string letter)
{ 
    
    if(letter == "A"){
        return true;
    }

    if(letter == "E"){
        return true;
    }

    if(letter == "I"){
        return true;
    }

    if(letter == "O"){
        return true;
    }

    if(letter == "U"){
        return true;
    }

    return false;
}

bool Juego::hasMoney(int points)
{
    if(points<50)
    {
        return false;
    }
    return true;
}

bool Juego::Resolver(string quote, string userTry)
{ 
    //userTry es la frase que introduce el usuario
    if (quote == userTry)//strcmp
    {
        return true;
    }
    return false;
}//Return true if users try is correct

string Juego::getRandomLine() 
{
    srand(time(0));
    int random = rand() % 8;

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
    quote = quoteStr;
    return quote;
}

/*
void Juego::game(string quote, int Puntos1, int Puntos2, int sd1, int sd2)
{
    cout<<"Entra aqui\n"<<endl;
    cout<<"sd1 es "<<sd1<<endl;
    cout<<"sd2 es "<<sd2<<endl;
    char* buffer;
    bzero(buffer,350);
    sprintf(buffer, "+Ok. Partida encontrada !!\n");
    send(sd1, buffer, 250, 0);
    send(sd2, buffer, 250, 0);
    // Si le mando que jugador es cada 1 peta xd
    char *equote = encryptQuote(quote);
    sprintf(buffer, "+Ok. La frase encriptada es: %s \n",equote);
    send(sd1, buffer, 250, 0);
    send(sd2, buffer, 250, 0);
    cout<<"peta aqui 1"<<endl;
    int turn=0;
    char* letter;
    cout<<"peta aqui 2"<<endl;
    while(isComplete(quote, equote)==false)
    {
        cout<<"Entra en el sistema de turno fase 1 (isComplete) "<<isComplete(quote,equote)<<endl;
        while(turn == 0)
        {
            sprintf(buffer, "Turno del jugador 1: \n");
            send(sd1, buffer, 350, 0);
            send(sd2, buffer, 350, 0);
            cout<<"Entra en el sistema de turnos fase 2 (turn 0): "<<turn<<endl;
            turn = 1;
            

            bzero(buffer,350);
            sprintf(buffer,"+Ok. Introduzca la letra que desea despejar, o bien elija RESOLVER\n");
            send(sd1,buffer,350,0);
            cout<<"peta aqui 3"<<endl;
            //TODO: Leer cosas bien por buffer y fixear si hubiera que fixear
            //int recibidos1 = recv(sd1,letter,sizeof(letter),0);
            cout<<letter<<endl;
            if(cadenaComienza(letter, "RESOLVER"))
            {
                if(Resolver(quote,sd1)==true)
                {
                    bzero(buffer,350);
                    sprintf(buffer,"+Ok. Ha ganado el jugador 1!\n");
                    send(sd1,buffer,350,0);
                }
                else
                {
                    bzero(buffer,350);
                    sprintf(buffer,"+Ok. Ha ganado el jugador 2!\n");
                    send(sd1,buffer,350,0);
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
                            bzero(buffer,350);
                            sprintf(buffer,"-Err. Ha fallado!\n");
                            send(sd1,buffer,350,0);
                        }
                    }
                    else
                    {
                        bzero(buffer,350);
                        sprintf(buffer,"-Err. No tiene suficientes puntos para comprar vocal. Repite turno!\n");
                        send(sd1,buffer,350,0);
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
            
            bzero(buffer,350);
            sprintf(buffer,"+Ok. Turno del jugador 2\n");
            send(sd2,buffer,350,0);
            send(sd1,buffer,350,0);
            sprintf(buffer,"+Ok. Introduzca la letra que desea despejar, o bien elija RESOLVER\n");
            send(sd2,buffer,350,0);

            int recibidos1 = recv(sd2,letter,sizeof(letter),0);
               if(cadenaComienza(letter, "RESOLVER"))
               {
                   if(Resolver(quote,sd2)==true)
                   {
                        bzero(buffer,350);
                        sprintf(buffer,"+Ok. Ha ganado el jugador 2!\n");
                        send(sd2,buffer,350,0);
                   }
                   else
                   {
                        bzero(buffer,350);
                        sprintf(buffer,"+Ok. Ha ganado el jugador 2!\n");
                        send(sd2,buffer,350,0);
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
                            bzero(buffer,350);
                            sprintf(buffer,"-Err. Ha fallado!\n");
                            send(sd2,buffer,350,0);
                        }

                    }
                    else
                    {
                        bzero(buffer,350);
                        sprintf(buffer,"-Err. No tiene suficientes puntos para comprar vocal. Repite turno!\n");
                        send(sd2,buffer,350,0);

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
}*/

bool Juego::isComplete(string quote, string equote){

    if(quote == equote){
        return true;
    }
    return false;
}

bool cadenaComienza(const char *cadena1, const char *cadena2){
    int longitud = strlen(cadena2);
    if (strncmp(cadena1, cadena2, longitud) == 0) return true;
    return false;
}