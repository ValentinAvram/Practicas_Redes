#ifndef USUARIO_H
#define USUARIO_H

#include <string.h>

using namespace std;


class Usuario{
    private:
    
    int sd;
    char nombre[250];
    char password[250];
    int estado; //Status 0 conectado, 1 cola, 2 jugando
    
    int sdRival;
    int idJuego;


    public:
    
    bool checkNombre;
    bool checkPass;
    bool loged;
    bool registered;

    Usuario();

    inline int getSd() {return sd;}
    inline char *getNombre() {return nombre;}
    inline char *getPassword() {return password;}
    inline int getStatus() {return estado;}
    inline bool getLoged() {return loged;}
    inline bool getRegistered() {return registered;}

    inline void setSd(int Sd) { sd = Sd;}
    inline void setNombre(char *Nombre) {strncpy(nombre, Nombre, 250);} //Es necesario el strncpy??
    //...
    inline void setPassword(char *Password) { strncpy(password, Password, 250);} //Es necesario el strncpy??
    inline void setStatus(int Estado) {estado=Estado;}
    inline void setLoged(bool Logeado){loged = Logeado;}
    inline void setRegistered(bool Registrado){registered = Registrado;}

    bool checkLogin();

};

#endif