#ifndef USUARIO_H
#define USUARIO_H

#include <string>

using namespace std;

//TODO: Pasar aqui la clase Jugador, y llamarla usuario
//TODO: Renombrar la clase a usuario
class Usuario{
    private:
    
    int sd;
    char nombre[250];
    char password[250];
    int estado;
    //Status 0 en cola, 1 jugando
    int sdRival;
    int idJuego;

    bool checkNombre;
    bool checkPass;

    bool 
    public:

    User(){
        sd=-1;
        estado = 0;
    }

    inline int getSd() {return sd;}
    inline char *getNombre() {return nombre;}
    inline char * getPassword() {return password;}
    inline int getStatus() {return estado;}

    inline void setSd(int Sd) { sd = Sd;}
    inline void setNombre(char *Nombre) {strncpy(nombre,Nombre,250);} // TODO: Es necesario el strncpy
    //...
    inline void setPassword(char *Password) { strncpy(password,Password, 250);}
    inline void setStatus(int Estado) {estado=Estado;}
};