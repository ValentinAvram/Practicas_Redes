#ifndef JUEGO_H
#define JUEGO_H

#include <string>
#include <cstring>

using namespace std;

class Juego{
    private:

    int idGame;
    int sd1;
    int sd2;
    int points1;
    int points2;
    //Jugadores que hay
    int numP;
    int turn;

    string quote;
    string equote;
    string letter;

    public: //TODO: Añadir Getters y Setters que faltan

    Juego();

    inline int getIdJuego() {return idGame;}
    inline int getSd1() {return sd1;}
    inline int getSd2() {return sd2;}   
    inline int getPuntos1() {return points1;}
    inline int getPuntos2() {return points2;}
    inline int getNumP() {return numP;}
    inline string getQuote() {return quote;}
    inline string getEQuote() {return equote;}
    inline string getLetter() {return letter;}
    inline int getTurn() {return turn;}


    inline void setIDGame(int IdGame) {idGame = IdGame;}
    inline void setSd1(int Sd1) {sd1=Sd1;}
    inline void setSd2(int Sd2) {sd2=Sd2;}
    inline void SetPoints1(int Puntos1) {points1=Puntos1;}
    inline void setPoints2(int Puntos2) {points2=Puntos2;}
    inline void setNumP(int NumP) {numP=NumP;}
    inline void setQuote(string Quote) {quote = Quote;}
    inline void setEquote(string Equote) {equote = Equote;}
    inline void setLetter(string Letter) {letter = Letter;}
    inline void setTurn(int Turn) {turn = Turn;}

    bool newPlayer(int sd);
    string encryptQuote(string  quote);
    string revealLetterInPanel(string quote, string equote, string letter);
    int getRight(string quote, string letter);
    bool isVowel(string letter);
    bool hasMoney(int points);
    bool Resolver(string quote, string userTry);
    string getRandomLine();
    void game(string quote, int Puntos1, int Puntos2, int sd1, int sd2);
    bool isComplete(string quote, string equote);
    Juego returnGame(int idGame);
};

#endif