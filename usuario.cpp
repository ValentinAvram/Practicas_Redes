#include "usuario.h"

using namespace std;

//TODO: Añadir aqui las funciones de la clase usuario
//Añadir constructor
Usuario::Usuario(int descriptor)
{
    sd = descriptor;
    estado = 0;

    checkNombre = false;
    checkPass = false;
}

//TODO: Funcion de comprobar login
bool Usuario::checkLogin()
{
    if(checkNombre == true && checkPass == true)
    {
        return true;
    }
    return false;
}