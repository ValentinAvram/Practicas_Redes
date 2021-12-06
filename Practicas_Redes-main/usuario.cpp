#include "usuario.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

//Añadir constructor
Usuario::Usuario()
{
    sd = -1;
    status = -1;
}


/*
Devuelve true si el nombre existe,
false si no existe
*/
bool Usuario::checkName(string name)
{
    FILE * archivo;
    archivo = fopen("users.txt", "r+");

    if(archivo == nullptr)
    {
        exit(-1);
    }

    char *linea = nullptr; 
    size_t n = 0;
    bool checkin = true;
    while ((getline(&linea,&n,archivo)) != -1)
    {
        linea = strtok(linea, "|");
        string name_ = linea;

        if(name_ == name)
        {
            checkin = false; // Nombre repetido
        }
    }

    fclose(archivo);

    if(checkin == true)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
Devuelve true si el login es correcto
False si no
*/
bool Usuario::checkLogin(string nombre, string pass)
{
    FILE * archivo;
    archivo = fopen("users.txt", "r+");

    if(archivo == nullptr)
    {
        exit(-1);
    }

    char *linea = nullptr; 
    size_t n = 0;
    bool checkin = true;
    while ((getline(&linea,&n,archivo)) != -1)
    {
        char *ptr;
        ptr = strtok(linea, "|");
        string name_ = ptr;
        char *aux = strtok(NULL, "\n");
        string pass_ = aux;

        if(name_ == nombre && pass_ == pass)
        {
            checkin = false; // Nombre repetido
        }
    }

    fclose(archivo);

    if(checkin == true)
    {
        return false;
    }
    else
    {
        return true;
    }
}

