#include "juego.h"

using namespace std;

//TODO: Añadir aqui las funciones de la clase juego
//Añadir constructor

Juego::Juego()
{
    puntos1=0;
    puntos2=0;
    numP=0;
}

//TODO: Pasar funciones juego.
// Posiblemente necesites una variable "string letter en la private". Con sus getters y setters
void Juego::newPlayer(int sd)
{
    if (numP == 0){
        setSd1(sd);
        numP++;
    }
    else if(numP == 1){
        setSd2(sd)
        numP++;
    }
    else{
        exit -1;
    }
}

string Juego::encryptQuote(string quote)
{
    string equote = "";
    int n= quote.size();
    char aux[n+1];

    //Pasamos de string a char
    strcpy(aux, quote.c_str());

    for(int g = 0; g<quote.size(); g++)
    {
        if(aux[g]== ' ')
        {
            aux[g] = ' ';
        }
        else
        {
            aux[g] = '-';
        }
    }

    //Pasamos de char a string
    int size = sizeof(aux) / sizeof(char);
    equote = charToString(aux, size);

    return equote;
}

string Juego::revealLetterInPanel(string quote, string equote, string letter)
{
    int count=0;
    for(int g = 0; g<quote.size(); g++)
    {
        if(equote[g]==letter[0])
        {
            letter[0]=42;
        }
        if(quote[g]==letter[0])
        {
            equote[g] = quote[g];
            count ++;
        }
    }
    cout<<"Hay "<<count<<" "<<letter<<endl;

    return equote;
}

bool Juego::getRight(string quote, string letter)
{
    for(int g = 0; g<quote.size(); g++)
    {
        if(quote[g] == letter[0])
        {
            return true;
        }
    }

    return false;
}

bool Juego::isVowel(string letra){
    if(letra == "a"){
        return true;
    }

    if(letra == "e"){
        return true;
    }

    if(letra == "i"){
        return true;
    }

    if(letra == "o"){
        return true;
    }

    if(letra == "u"){
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

bool Juego::Resolver(string quote){
    string resolver;
    cout<<"Introduzca el refran"<<endl;
    cout<<"Cuidado, si falla aunque sea por ortografia perdera"<<endl;
    getline(cin, resolver);
    //Espera dramatica
    sleep(10);
    cout<<"Y la respuesta es...";
    sleep(10);
    if (quote == resolver)
    {
        cout<<"CORRECTA!!!"<<endl;
        return true;
    }
    else
    {
        cout<<"INCORRECTA"<<endl;
        return false;
    }
}

string Juego::getRandomLine()
{
    string line;
    int random=0;
    int numOfLines=8;
    ifstream File("refranes.txt");

    srand(time(0));
    random = rand() % 8;

    while(getline(File,line))
    {
        ++numOfLines;

        if(numOfLines == random)
        {
            return line;
        }
    }
    return line;
}
