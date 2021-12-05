#include "usuario.h"
#include <stdio.h>
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

bool Usuario::checkLogin(string name, string pass)
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

        while(ptr != NULL)
        {
            string name_ = ptr;
            string pass_ = ptr;

            printf("\nPRUEBA!!\n");
            printf("Name %s, Pass %s");

            if(name_ == name && pass_ == password)
            {
                checkin = false; // Nombre repetido
            }
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

