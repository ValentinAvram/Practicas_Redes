#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>


#define MSG_SIZE 350
#define MAX_CLIENTS 30
#define PORT 2050
#define MAX_GAMES 10

using namespace std;

vector<User> clientes;
vector<Ruleta> games;
int ngames = 0;
char stringToChar(string texto)
{
    char *buffer = const_cast<char*>(texto.c_str());
    return *buffer;
}

//int char_size = sizeof(buffer) / sizeof(char);
string charToString(char buffer[350], int char_size)
{
    int g;
    string texto = "";
    for (g = 0; g < char_size; g++) {
        texto = texto + buffer[g];
    }
    return texto;
}
 
void split (string str, char seperator)  
{  
    int currIndex = 0, g = 0;  
    int startIndex = 0, endIndex = 0;  
    while (g <= str.length())  
    {  
        if (str[g] == seperator || g == str.length())  
        {  
            endIndex = g;  
            string subStr = "";  
            subStr.append(str, startIndex, endIndex - startIndex);  
            strings[currIndex] = subStr;  
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        g++;  
        }     
} 

string encryptQuote(string quote){
    string equote = "";
    int n= quote.size();
    char aux[n+1];

    //Pasamos de string a char
    strcpy(aux, quote.c_str());


    for(int g = 0; g<quote.size(); g++){
        if(aux[g]== ' '){
            aux[g] = ' ';
        }
        else{
            aux[g] = '-';
        }
    }

    //Pasamos de char a string
    int size = sizeof(aux) / sizeof(char);
    equote = charToString(aux, size);

    return equote;
}

string revealLetterInPanel(string quote, string equote, string letter){
    int count=0;
    for(int g = 0; g<quote.size(); g++){
        if(equote[g]==letter[0]){
            letter[0]=42;
        }
        if(quote[g]==letter[0]){
            equote[g] = quote[g];
            count ++;
        }
    }
    cout<<"Hay "<<count<<" "<<letter<<endl;

    return equote;
}

bool getRight(string quote, string letter){
    for(int g = 0; g<quote.size(); g++){
        if(quote[g] == letter[0]){
            return true;
        }
    }

    return false;
}

bool isVowel(string letra){
    if(letra == "a"){
        return true;
    }

    if(letra == "e"){
        return true;
    }

    if(letra == "i"){
        return true;
    }

    if(letra == "o"){
        return true;
    }

    if(letra == "u"){
        return true;
    }

    return false;
}

bool hasMoney(int points){
    if(points<50){
        return false;
    }
    return true;
}

bool Resolver(char* quote){
    char resolver[MSG_SIZE];
    cout<<"Introduzca el refran"<<endl;
    cout<<"Cuidado, si falla aunque sea por ortografia perdera"<<endl;
    getline(cin, resolve);
    //Espera dramatica
    sleep(1);
    cout<<"Y la respuesta es...";
    sleep(10);
    int compare = strcmp(&quote, &resolver);
    if (compare == 0){
        cout<<"CORRECTA!!!"<<endl;
        return true;
    }
    else{
        cout<<"INCORRECTA"<<endl;
        return false;
    }
}

string getRandomLine(){
    string line;
        int random=0;
    int numOfLines=8;
    ifstream File("refranes.txt");

    srand(time(0));
    random = rand() % 8;

    while(getline(File,line))
    {
        ++numOfLines;

        if(numOfLines == random){
            return line;
        }
    }
    return line;
}


int cadenaComienzaCon(const char *cadena1, const char *cadena2) {
  int longitud = strlen(cadena2);
  if (strncmp(cadena1, cadena2, longitud) == 0) return 1;
  return 0;
}

class Client{
    private:
    
    int sd;
    char nombre[250];
    char password[250];
    int estado;
    //Status 0 en cola, 1 jugando
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
    inline void setNombre(char *Nombre) {strncpy(nombre,Nombre,250);}
    //...
    inline void setPassword(char *Password) { strncpy(password,Password, 250);}
    inline void setStatus(int Estado) {estado=Estado;}
};

//TODO: Game
class Ruleta{
    private:

    int sd1;
    int sd2;

    int puntos1;
    int puntos2;

    int numP;

    char quote[MSG_SIZE];

    public:

    Ruleta(){
        puntos1=0;
        puntos2=0;
        numP=0;
    }

    inline int getSd1() {return sd1;}
    inline int getSd2() {return sd2;}
    
    inline int getPuntos1() {return puntos1;}
    inline int getPuntos2() {return puntos2;}

    inline int getNumP() {return numP;}

    inline char *getQuote() {return quote;}

    inline void setSd1(int Sd1) {sd1=Sd1;}
    inline void setSd2(int Sd2) {sd2=Sd2;}

    inline void setPuntos1(int Puntos1) {puntos1=Puntos1;}
    inline void setPuntos2(int Puntos2) {puntos2=Puntos2;}

    inline void setNumP(int NumP) {numP=NumP;}
    //...
    inline void setQuote(char *Quote) {strncpy(quote, Quote, MSG_SIZE);}

    bool newPlayer(int sd){
        if (numP == 0){
            setSd1(sd);
            numP++;
            return true;
        }
        else if(numP == 1){
            setSd2(sd)
            numP++;
            return true;
        }
        else{
            return false;
        }
    }
};
//TODO: Server Functions

void escrituraTXT(char buffer[350])
{
    FILE* fichero;
    fichero = fopen("users.txt", "w");
    fputs(buffer, fichero); //TODO: Añadir \n al buffer en el main
    fclose(fichero);
}

void lecturaTXT()
{
    FILE *fichero;
    fichero = fopen("users.txt", "r");
    if (!f)
    {
        printf("Error abriendo el archivo")
        exit(-1);
    }
    char buffer[MSG_SIZE];
    while (fgets(buffer, MSG_SIZE, fichero) != NULL)
    {
        Cliente newCliente;
        char *data;
        
        data = strtok(cadena, "//");
        newCliente.setNombre(aux);
        
        data = strtok(NULL, "\n");
        newCliente.setNombre(data);
        
    }
    fclose(fichero);
}

// Control de juego

void nuevoGame(Ruleta ruleta)
{
    if (ngames < MAX_GAMES)
    {
        games.push_back(ruleta);
        ngames_++;
    }
}

int parejaJuego(int sd)
{
    for (int i = 0; i < (int)games.size(); i++)
    {
        if (games[i].getNumP() == 2)
        {
            if (games[i].getSd1() == sd){return games[i].getSd2();}
            else if (games[i].getSd2() == sd){return games[i].getSd1();}
        }
    }
    return 0;
}


int unirJugadores(int sd)
{
    if (ngames == 0)
    {
        Ruleta ruleta;
        ruleta.newPlayer(sd);
        nuevoGame(ruleta);
        return 1;
    }
    else if (ngames > 1 &&  ngames < 10)
    {
        Ruleta ruleta = games[ngames - 1];
        if (ruleta.newPlayer(sd))
        {
            games.pop_back();
            games.push_back(game);
            return 1;
        }//TODO: Cambiar esto. Reestructurar array modo profe
        else if (ngames + 1 < 10)
        {
            Ruleta ruleta;
            ruleta.newPlayer(sd);
            nuevoGame(ruleta);
            return 1;
        }
    }
    return 0;
}


void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);


int main ( )
{
    system("clear");
    cout<<"Main.exe\n";

    return 0;
}
