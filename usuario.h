#ifndef USUARIO_H
#define USUARIO_H

#include <string>

using namespace std;


class Usuario{
    private:

    int sd;
    string name;
    string password;
    int status; //Status 0 register, 1 only name, 2 logged, 3 in queue, 4 in game
    int idGame; // Solo si status == 2 (inGame)


    public:
    Usuario();

    inline int getSd() {return sd;}
    inline string getName() {return name;}
    inline string getPassword() {return password;}
    inline int getStatus() {return status;}
    inline int getIdGame() {return idGame;}

    inline void setSd(int Sd) { sd = Sd;}
    inline void setNombre(string Name) {name = Name;} //Es necesario el strncpy??
    inline void setPassword(string Password) { password = Password;} //Es necesario el strncpy??
    inline void setStatus(int Status) {status = Status;}
    inline void setIdGame(int IDGame) {idGame = IDGame;}

    bool checkLogin(string name, string pass);
    bool checkName(string name);
};

#endif