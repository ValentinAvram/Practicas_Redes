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
        if(quote[h]==toupper(letter[0]))
        {
            equote[h] = toupper(quote[h]);
            count ++;
        }
    }

    return equote;
}

int Juego::getRight(string quote, string letter)
{
    int cont = 0;
    int n = quote.size();
    for(int g = 0; g<n; g++)
    { 
        char aux = quote[g];
        if(((aux == toupper(letter[0]))) || (aux == (tolower(letter[0]))))
        {
            cont ++;
        }
    }
    return cont;
}

bool Juego::isVowel(string letter)
{ 
    
    if(letter == "A" || letter == "a"){
        return true;
    }

    else if(letter == "E" || letter == "e"){
        return true;
    }

    else if(letter == "I" || letter == "i"){
        return true;
    }

    else if(letter == "O" || letter == "o"){
        return true;
    }

    else if(letter == "U" || letter == "u"){
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