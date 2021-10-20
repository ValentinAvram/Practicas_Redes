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
