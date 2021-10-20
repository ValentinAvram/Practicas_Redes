#ifndef JUEGO_H
#define JUEGO_H

#include <cstring>

using namespace std;

class Juego{
    private:

    int idJuego;

    int sd1;
    int sd2;

    int puntos1;
    int puntos2;

    int numP; 

    char quote[350];
    char equote[350];
    char letter[350]; //Añadido

    public: //TODO: Añadir Getters y Setters que faltan

    Juego();

    inline int getIdJuego() {return idJuego;}

    inline int getSd1() {return sd1;}
    inline int getSd2() {return sd2;}
    
    inline int getPuntos1() {return puntos1;}
    inline int getPuntos2() {return puntos2;}

    inline int getNumP() {return numP;}

    inline char *getQuote() {return quote;}
    inline char *getEQuote() {return equote;}
    inline char *getLetter() {return letter;}

    inline void setIdJuego(int IDJuego) {idJuego = IDJuego;}

    inline void setSd1(int Sd1) {sd1=Sd1;}
    inline void setSd2(int Sd2) {sd2=Sd2;}

    inline void setPuntos1(int Puntos1) {puntos1=Puntos1;}
    inline void setPuntos2(int Puntos2) {puntos2=Puntos2;}

    inline void setNumP(int NumP) {numP=NumP;}
    //...
    inline void setQuote(char *Quote) {strncpy(quote, Quote, 350);}
    inline void setLetter(char *Letter) {strncpy(letter, Letter, 250);}

    bool newPlayer(int sd);
    char *encryptQuote(char * quote);
    char *revealLetterInPanel(char * quote, char * equote, char *letter);
    bool getRight(char *quote, char *letter);
    bool isVowel(char *letra);
    bool hasMoney(int points);
    bool Resolver(char *quote);
    char *getRandomLine();
};

#endif