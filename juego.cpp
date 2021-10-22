#include "juego.h"
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

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
{ // TODO: Compruebalo
    char* equote;
    string efrase(quote);
    int n= strlen(quote);

    for(int g = 0; g<n; g++)
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

char *Juego::revealLetterInPanel(char *quote, char *equote, char *letter) //TODO: A lo mejor falla?¿
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

bool Juego::isVowel(char *letter){ //TODO: A lo mejor falla???
    
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

bool Juego::Resolver(char *quote){ // NO FUNCIONA, NECESITA RCV Y SEND
    string quotestr(quote);
    string resolver;
    cout<<"Introduzca el refran"<<endl;
    cout<<"Cuidado, si falla aunque sea por ortografia perdera"<<endl;
    getline(cin, resolver);
    cout<<"Y la respuesta es...";
    if (quote == resolver)//strcmp
    {
        //cout<<"CORRECTA!!!"<<endl;
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

void Juego::game(char* quote,int Puntos1, int Puntos2, int Sd1, int Sd2){
    //Inicializar puntos y sds
    char *equote = encryptQuote(quote);
    int turn=0;
    char* letter;
    while(isComplete(quote, equote)==false){
        while(turn == 0){
            turn = 1;
            //TODO: Pedir por pantalla que introduzca una letra al sd1
            //TODO: Leer por pantalla la letra del sd1
            string letterstr(letter);
            if(letterstr=="RESOLVER"){
                if(Resolver(quote)==true){
                    //TODO: Mostrar por pantalla que ha ganado el jugador 1
                }
                else{
                    //TODO: Mostrar por pantalla que ha ganado el jugador 2
                }
            }
            else{
                if(isVowel(letter)==true){
                    if(hasMoney(Puntos1)==true){
                        setPuntos1(getPuntos1()-50);
                        if(getRight(quote, letter)==true){
                            turn = 0;
                            equote=(revealLetterInPanel(quote,equote,letter));
                        }
                        else{
                            //TODO:Mostrar por pantalla que ha fallado
                        }
                    }
                    else{
                        //TODO: Mostrar por pantalla que no tiene dinero para comprar vocal, repite turno
                        letter[0]=42;
                        turn=0;
                    }
                }
                else{
                    if(getRight(quote,letter)==true){
                        turn=0;
                        setPuntos1(getPuntos1()+50);
                        equote=revealLetterInPanel(quote,equote,letter);
                    }
                }
            }
        }
        while(turn==1){
            turn = 0;
            //TODO: Pedir por pantalla que introduzca una letra al sd2
            //TODO: Leer por pantalla la letra del sd2
            string letterstr(letter);
               if(letterstr=="RESOLVER"){
                   if(Resolver(quote)==true){
                       //TODO: Mostrar por pantalla que ha ganado el jugador 2
                   }
                   else{
                       //TODO: Mostrar por pantalla que ha ganado el jugador 1
                   }

               }
               else{
                if(isVowel(letter)==true){
                    if(hasMoney(Puntos2)==true){
                        setPuntos2(getPuntos2()-50);
                        if(getRight(quote, letter)==true){
                            turn=1;
                            equote=(revealLetterInPanel(quote,equote,letter));
                        }
                        else{
                            //TODO:Mostrar por pantalla que ha fallado
                        }

                    }
                    else{
                        //TODO: Mostrar por pantalla que no tiene dinero para comprar vocal, repite turno
                        letter[0]=42;
                        turn = 1;
                    }

                }
                else{
                    if(getRight(quote,letter)==true){
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