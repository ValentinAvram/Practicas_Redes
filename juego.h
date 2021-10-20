#ifndef JUEGO_H
#define JUEGO_H
#define MSG_SIZE 350

#include <string>

using namespace std;

class Juego{
    private:

    int idJuego;

    int sd1;
    int sd2;

    int puntos1;
    int puntos2;

    int numP; // Lo necesitamos?

    char quote[MSG_SIZE];
    char equote[MSG_SIZE]; //Añadido

    public: //TODO: Añadir Getters y Setters que faltan

    inline int getSd1() {return sd1;}
    inline int getSd2() {return sd2;}
    
    inline int getPuntos1() {return puntos1;}
    inline int getPuntos2() {return puntos2;}

    inline int getNumP() {return numP;}

    inline char *getQuote() {return quote;}

    inline void setSd1(int Sd1) {sd1=Sd1;}
    inline void setSd2(int Sd2) {sd2=Sd2;}

    inline void setPuntos1(int Puntos1) {puntos1=Puntos1;}
    inline void setPuntos2(int Puntos2) {puntos2=Puntos2;}

    inline void setNumP(int NumP) {numP=NumP;}
    //...
    inline void setQuote(char *Quote) {strncpy(quote, Quote, MSG_SIZE);}
    
    bool newPlayer(int sd);
    string encryptQuote(string quote);
    string revealLetterInPanel(string quote, string equote, string letter);
    bool getRight(string quote, string letter);
    bool isVowel(string letra);
    bool hasMoney(int points);
    bool Resolver(char* quote);
    string getRandomLine();
};