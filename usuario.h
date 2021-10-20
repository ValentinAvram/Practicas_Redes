#ifndef USUARIO_H
#define USUARIO_H

#include <string>

using namespace std;


class Usuario{
    private:
    
    int sd;
    string nombre;
    string password;
    int estado; //Status 0 en cola, 1 jugando
    
    int sdRival;
    int idJuego;

    bool checkNombre;
    bool checkPass;

    public:
    
    inline int getSd() {return sd;}
    inline string getNombre() {return nombre;}
    inline string getPassword() {return password;}
    inline int getStatus() {return estado;}

    inline void setSd(int Sd) { sd = Sd;}
    inline void setNombre(string Nombre) {nombre = Nombre;} //Es necesario el strncpy??
    //...
    inline void setPassword(string Password) { password = Password;} //Es necesario el strncpy??
    inline void setStatus(int Estado) {estado=Estado;}
};