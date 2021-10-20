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

    int numP; 
    //TODO: A lo mejor necesitas "string letter" para ciertas de tus funciones del juego
    string quote;
    string equote;
    string letter; //Añadido

    public: //TODO: Añadir Getters y Setters que faltan

    inline int getIdJuego() {return idJuego;}

    inline int getSd1() {return sd1;}
    inline int getSd2() {return sd2;}
    
    inline int getPuntos1() {return puntos1;}
    inline int getPuntos2() {return puntos2;}

    inline int getNumP() {return numP;}

    inline string getQuote() {return quote;}
    inline string getEQuote() {return equote;}
    inline string getLetter() {return letter;}

    inline void setIdJuego(int IDJuego) {idJuego = IDJuego;}

    inline void setSd1(int Sd1) {sd1=Sd1;}
    inline void setSd2(int Sd2) {sd2=Sd2;}

    inline void setPuntos1(int Puntos1) {puntos1=Puntos1;}
    inline void setPuntos2(int Puntos2) {puntos2=Puntos2;}

    inline void setNumP(int NumP) {numP=NumP;}
    //...
    inline void setQuote(string Quote) {quote = Quote;}
    inline void setLetter(string Letter) {letter = Letter;}

    bool newPlayer(int sd);
    string encryptQuote(string quote);
    string revealLetterInPanel(string quote, string equote, string letter);
    bool getRight(string quote, string letter);
    bool isVowel(string letra);
    bool hasMoney(int points);
    bool Resolver(string quote);
    string getRandomLine();
};