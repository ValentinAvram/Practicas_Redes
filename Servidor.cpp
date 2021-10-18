#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <chrono>

#define MSG_SIZE 350
#define MAX_CLIENTS 30
#define PORT 2050
#define MAX_GAMES 10


using namespace std;
//TODO: Hacer funciones de "Existe X letra"

string strings[30];
vector<player> jugadores(20);
vector<game> partidas(10);
char stringToChar(string texto)
{
    char buffer = const_cast<char>(texto.c_str());
    return buffer;
}

//int char_size = sizeof(buffer) / sizeof(char);
string charToString(char buffer[350], int char_size) //TODO:: LUEGO
{
    int i;
    string texto = "";
    for (i = 0; i < char_size; i++) {
        texto = texto + buffer[i];
    }
    return texto;
}

// create custom split() function  
void split (string str, char seperator)  
{  
    int currIndex = 0, i = 0;  
    int startIndex = 0, endIndex = 0;  
    while (i <= str.length())  
    {  
        if (str[i] == seperator || i == str.length())  
        {  
            endIndex = i;  
            string subStr = "";  
            subStr.append(str, startIndex, endIndex - startIndex);  
            strings[currIndex] = subStr;  
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        i++;  
        }     
} 

string encryptQuote(string quote){
    string equote = "";
    int n= quote.size();
    char aux[n+1];

    //Pasamos de string a char
    strcpy(aux, quote.c_str());


    for(int i = 0; i<quote.size(); i++){
        if(aux[i]== ' '){
            aux[i] = ' ';
        }
        else{
            aux[i] = '-';
        }
    }

    //Pasamos de char a string
    int size = sizeof(aux) / sizeof(char);
    equote = charToString(aux, size);

    return equote;
}

string revealLetterInPanel(string quote, string equote, string letter){
    int count=0;
    for(int i = 0; i<quote.size(); i++){
        if(quote[i]==letter[0]){
            equote[i] = quote[i];
            count ++;
        }
    }
    cout<<"Hay "<<count<<" "<<letter<<endl;

    return equote;
}

bool getRight(string quote, string letter){
    for(int i = 0; i<quote.size(); i++){
        if(quote[i] == letter[0]){
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

bool Resolver(string quote){
    string resolve;
    cout<<"Introduzca el refran"<<endl;
    cout<<"Cuidado, si falla aunque sea por ortografia perdera"<<endl;
    getline(cin, resolve);
    //Espera dramatica
    sleep(15);
    cout<<"Y la respuesta es...";
    sleep(10);
    if (quote.compare(resolve) == -1){
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
    
}

class player{

	private:
	int descriptor;
    int status; // 0 sin logear, 1 en espera a game, 2 en game
	string name;
    string password;
    int points;

    public:

    player(int sd, int status, string nombre, string contra, int puntos) 
    {
    this->descriptor = sd;
    this->status = status;
    this->name = nombre;
    this->password = contra;
    this->points = puntos;
    }

    //Setters
    void setDescriptor(int desc)
    {
        descriptor = desc;
    }

    void setStatus(int status)
    {
        status=status;
    }

    void setName(string nombre)
    {
        name = nombre;
    }

    void setPassword(string contra)
    {
        password = contra;
    }

    void setPoints(int puntos)
    {
        points = puntos;
    }
    
    //Getters
    int getDescriptor()
    {
        return descriptor;
    }

    int getStatus()
    {
        return status;
    }

    string getName()
    {
        return name;
    }

    string getPass()
    {
        return password;
    } 

    int getPoints()
    {
        return points;
    }
};

class game{ // TOO: class game:player() ??

    private:
    int descriptor1, descriptor2;
    string name1, name2;
    int points1, points2;
    //TODO: Metodo publico que sea el juego entero?
    //TODO: CONSTRUCTORES
    public:

    game(int sd1, int sd2, string nombre1, string nombre2, int puntos1, int puntos2) 
    {
    this->descriptor1 = sd1;
    this->descriptor2 = sd2;
    this->name1 = name1;
    this->name2 = name2;
    this->points1 = points1;
    this->points2 = points2;
    }

    //Setters
    void setDescriptor1(int sd1)
    {
        descriptor1 = sd1;
    }
    
    void setDescriptor2(int sd2)
    {
        descriptor2 = sd2;
    }
    void setName1(string nombre1)
    {
        name1 = nombre1;
    }
    
    void setName2(string nombre2)
    {
        name2 = nombre2;
    }

    void setPoints1(int puntos1)
    {
        points1 = puntos1;
    }
    
    void setPoints2(int puntos2)
    {
        points2 = puntos2;
    }
    //Getters
    int getDescriptor1()
    {
        return descriptor1;
    }
    
    int getDescriptor2()
    {
        return descriptor2;
    }

    string getName1()
    {
        return name1;
    }
    
    string getName2()
    {
        return name2;
    }

    int getPoints1()
    {
        return points1;
    }

    int getPoints2()
    {
        return points2;
    }

    void startGame(player playerOne, player playerTwo){
        //Seleccionamos un refran aleatorio
        string quote = getRandomLine();
        //Encriptamos el refran aleatorio
        string enquote = encryptQuote(quote);
        //En primer lugar cambiamos el status de los jugadores
        playerOne.setStatus(2);
        playerTwo.setStatus(2);
        //Pasamos el descriptor de ambos jugadores
        setDescriptor1(playerOne.getDescriptor());
        setDescriptor2(playerTwo.getDescriptor());
        //Obtenemos el nombre de ambos jugadores
        setName1(playerOne.getName());
        setName2(playerTwo.getName());
        //Inicializamos los puntos de ambos jugadores a 0 antes de que comience la partida
        setPoints1(0);
        setPoints2(0);
        //Comenzamos la partida
        //La partida comienza con un bucle en el que no termina mientras este completo el panel
        //El sistema de turnos se basa en dos bucles en torno a la variable turn
        int turn = 1;
        //TODO: Funcion salir
        int complete = 77;
        //Creamos una variable para la letra que juegue cada jugador
        string letter;
        while(complete != -1){
            //Juega el primer jugador
            while(turn == 1){
                //Mostramos el panel
                cout<<enquote<<endl;

                cout<<"Que desea jugar"<<endl;
                //TODO: Spliteo para que lea solo la letra (ejemplo: CONSONANTE l)
                getline(cin, letter);
                //Vemos si quiere resolver el panel
                if(letter == "RESOLVER"){
                    if(Resolver(letter)==true){
                        turn = 10;
                        complete = -1;
                    }
                    else{
                        turn = 20;
                        complete = -1;
                    }
                }

                else{
                //Comprobamos si es vocal
                if(isVowel(letter) == true){
                    //Comprobamos si tiene puntos para comprar la vocal
                    if(hasMoney(getPoints1()) == true){
                        //Compra la vocal
                        setPoints1(getPoints1() - 50);
                        if(getRight(quote, letter) == true){
                            enquote = revealLetterInPanel(quote, enquote, letter);
                            //Comprobamos si ha ganado
                            if(complete = quote.compare(enquote) == -1){
                                turn = 10;
                                complete = -1;
                            }
                            else{
                                turn = 1;
                            }
                            
                        }
                        //Fallo
                        else{
                            turn = 2;
                        }
                    }
                    //No tiene puntos para comprar una vocal
                    else{
                        cout<<"No puedes comprar vocal"<<endl;
                        turn = 1;
                    }
                }
                //Ha elegido consonante
                else{
                        if(getRight(quote, letter) == true){
                            enquote = revealLetterInPanel(quote, enquote, letter);
                            //TODO: Solo gana 50 una vez y deberia ganar por las veces que acierta?
                            setPoints1(getPoints1() + 50 );
                            //Comprobamos si ha ganado
                            if(complete = quote.compare(enquote) == -1){
                                turn = 10;
                                complete = -1;
                            }
                            else{
                                turn = 1;
                            }  
                        }
                        //Fallo
                        else{
                            turn = 2;
                        }
                    }
                }
                    //Acaba el turno del jugador 1
            }
        
            while(turn == 2){
                cout<<enquote<<endl;

                cout<<"Que desea jugar??"<<endl;

                getline(cin, letter);
                if(letter == "RESOLVER"){
                    if(Resolver(letter) == true){
                        turn = 20;
                        complete = -1;
                    }
                    else{
                        turn = 10;
                        complete = -1;
                    }
                }
                else{
                if(isVowel(letter) == true){
                    if(hasMoney(getPoints2()) == true){
                        setPoints2(getPoints2() - 50);
                        if(getRight(quote, letter)==true){
                            enquote = revealLetterInPanel(quote, enquote, letter);
                            if(complete = quote.compare(enquote) == -1){
                                turn = 20;
                                complete = -1;
                            }
                            else{
                                turn = 2;
                            }
                        }
                        else{
                            turn = 1;
                        }
                    }
                    else{
                        cout<<"No tienes puntos para usar vocal"<<endl;
                        turn = 2;
                    }
                }
                else{
                    if(getRight(quote, letter)==true){
                        setPoints2(getPoints2()+50);
                        enquote=revealLetterInPanel(quote, enquote, letter);
                        if(complete = quote.compare(enquote) == -1){
                            turn = 20;
                            complete = -1;
                        }
                        else{
                            turn = 2;
                        }
                    }
                    else{
                        turn = 1;
                    }
                }
            }


            }
        }
        if(turn == 10){
            cout<<"Ha ganado "<<getName1()<<endl;
        }
        if(turn == 20){
            cout << "Ha ganado "<<getName2()<<endl;
        }

        finishGame(playerOne, playerTwo);

    }

    void finishGame(player playerOne, player playerTwo){
        playerOne.setStatus(0);
        playerTwo.setStatus(0);
        //TODO: Hay que hacer mas cosas?
    }
};

//PARA VALENTIN: Una funcion que retorne un char o que reciba un char es un movidote 
//PARA VALENTIN: Redundancia en los puntos de los jugadores
//PARA VALENTIN: Hacer salir es facil pero no se como hacer que se desconecten
//PARA VALENTIN: En teoria el juego esta terminado, a lo mejor hay bugs pero se corrigen ez

// TODO: Funciones de registro, una para user, otra para pas
int cadenaComienzaCon(const char *cadena1, const char *cadena2) {
  int longitud = strlen(cadena2);
  if (strncmp(cadena1, cadena2, longitud) == 0) return 1;
  return 0;
}

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

int main ( )
{

    system("clear");
    
	int sd, new_sd;
	struct sockaddr_in sockname, from; // Almacen de direcciones
	char buffer[MSG_SIZE];
    char identificador[MSG_SIZE];
	socklen_t from_len; //Length de la estructura de peticiones?
    fd_set readfds, auxfds; //Set de sockets que comprueben existencia de caracteres a leer //Crear mas colas?

   	int arrayClientes[MAX_CLIENTS]; //Crear mas arrays por partidas
    int numClientes = 0;
    int i,j,k,recibidos,on,ret,salida;
    
    bool register = false;
    bool login = false;  
    bool loged = false;
    // Apertura del socket
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    	exit (1);	
	}
    
    // Para reusar puertos
    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



	sockname.sin_family = AF_INET; // Siempre debe ser AF_INET
	sockname.sin_port = htons(PORT); // Puerto para conexión
	sockname.sin_addr.s_addr =  INADDR_ANY; // Direcciones por atender

    // Asociar socket a puerto
	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}
	
    // Length de la estructura de peticiones
	from_len = sizeof (from);

    // Habilitar el socket para que reciba conexiones
	if(listen(sd,30) == -1) // Max. 30 usuarios en cola
    {
		perror("Error en la operación de listen");
		exit(1);
	}
    
	    // Inicializar los conjuntos fd_set
    	FD_ZERO(&readfds);
    	FD_ZERO(&auxfds);
        // Añadir descriptores a sets
    	FD_SET(sd,&readfds);
    	FD_SET(0,&readfds); // TODO: Cambiar 0 por el descriptor del cliente?? cual sería?
    
   	
    	//Capturamos la señal SIGINT (Ctrl+c)
    	signal(SIGINT,manejador);
    
	
		// El servidor acepta una petición
	
		while(1){
            
            // Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;

            // Atender varios clientes a la vez

            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0)
            {
                for(i=0; i<FD_SETSIZE; i++)
                { 
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) 
                    {   
                        if( i == sd)
                        { 
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
                            {
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                // TODO: Crear nueva instancia PLAYER, playerN.descriptor = new_sd
                                // Funcion de registro que nos devuelva username y pass
                                // Asignar username y pass a nueva instancia.
                                // Agrupar instancias de dos en dos, en clase game

                                if(numClientes < MAX_CLIENTS)
                                {
                                    arrayClientes[numClientes] = new_sd;
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);

                                    // NombreClase nombre_objeto();
                                    // Enviar mensaje al nuevo cliente
                                    send(new_sd,buffer,sizeof(buffer),0);
                                    // TODO: Crear instancia jugador recien logeado / registrado
                                    for(j=0; j<(numClientes-1);j++)
                                    {
                                        bzero(buffer,sizeof(buffer)); // Deja buffer = "\0"
                                        sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd); // TODO: Cambiar el "new_sd" por el nick del usuario recien conectado
                                        send(arrayClientes[j],buffer,sizeof(buffer),0);
                                    }
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Demasiados clientes conectados\n");
                                    send(new_sd,buffer,sizeof(buffer),0);
                                    close(new_sd);
                                }
                                
                            }
                        }
                        else if (i == 0)
                        {
                            //Se ha introducido información de teclado del servidor
                            bzero(buffer, sizeof(buffer));
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0)
                            {
                                for (j = 0; j < numClientes; j++)
                                {
						            bzero(buffer, sizeof(buffer));
						            strcpy(buffer,"Desconexión servidor\n"); 
                                    send(arrayClientes[j],buffer , sizeof(buffer),0);
                                    close(arrayClientes[j]);
                                    FD_CLR(arrayClientes[j],&readfds);
                                }
                                close(sd);
                                exit(-1); 
                            }
                            //TODO: IMPLEMENTAR MENSAJES A MANDAR A LOS CLIENTES. INSTRUCCIONES DE USO SI ES, otra cosa no hay                        
                        } 
                        else
                        {
                            bzero(buffer,sizeof(buffer));

                            // Recepcion de mensaje siguiente elemento del set (primer for)
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            if(recibidos > 0)
                            {
                                // TODO: FUNCIONES DE REGISTRO, una para user, otra para pass???
                                // TODO: FUNCIONES DE LOGIN,
                                // TODO: Diferenciar usuario registrado de no registrado
                                // TODO: PARTIR BUFFER EN VARIABLES (strtok()), cambiar buffer por las variables
                                //TODO : TODO LO SIGUIENTE DENTRO DE UN BLUCE?
                                // TODO: Aqui todo lo del juego??
                                while(loged == false)
                                {
                                    string user = "0";
                                    string pass = "0";
                                    int char_size = sizeof(buffer) / sizeof(char);
                                    if(cadenaComienzaCon(buffer, "REGISTER -u"))
                                    {   
                                        string texto = charToString(buffer, char_size);                                 
                                        login = false;
                                        registrado = true;
                                        char separator = ' ';
                                        texto.erase(0,14);
                                        split(texto, separator);  
                                        string user = strings[0];
                                        string pass = strings[2];

                                        ifstream fich("users.txt");
                                        string linea;
                                        
                                        while (getline(fich, linea)) {
                                        
                                            split(linea, separator);
                                            
                                            string aux = strings[0];
                                            if(aux == user)
                                            {
                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer,"\n-Err. Nombre de usuario ya registrado. Intentelo de nuevo\n"); 
                                                send(arrayClientes[i],buffer , sizeof(buffer),0);
                                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                                            }
                                        }
                                        fich.close();

                                        ofstream fich2("users.txt", ios_base::app | ios_base::out);    
                                        if (!fich2)
                                        {
                                            bzero(buffer,sizeof(buffer));
                                            strcpy(buffer,"Error al abrir el archivo\n");
                                            send(arrayClientes[i],buffer , sizeof(buffer),0);
                                            salirCliente(i,&readfds,&numClientes,arrayClientes);
                                        }
                                        fich2 <<user<<" "<<pass<< endl;
                                        fich2.close();
                                        bzero(buffer,sizeof(buffer));
                                    }
                                    else if(cadenaComienzaCon(buffer, "USUARIO"))
                                    {
                                        string texto = charToString(buffer, char_size);
                                        login = true;
                                        registrado = false;
                                        bool exists =false;
                                        char separator = ' ';
                                        texto.erase(0,8);
                                        split(texto, separator);
                                        string user = strings[0];
                                        string aux = "0";
                                        ifstream fich("users.txt");
                                        string linea;

                                        while (getline(fich, linea) && exists == true) {
                                            
                                            split(linea, separator);
                                            string auxUser = strings[0];
                                            string auxPass = strings[1];
                                            
                                            if(auxUser == user)
                                            {
                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer,"\n+Ok. Usuario correcto\n");
                                                send(arrayClientes[i],buffer , sizeof(buffer),0);
                                                exists=true;
                                                aux = user;
                                            }  
                                        }
                                        fich.close();
                                        
                                        if(exists == false)
                                        {
                                            bzero(buffer,sizeof(buffer));
                                            strcpy(buffer,"\n-Err. Usuario incorrecto\n");
                                            send(arrayClientes[i],buffer , sizeof(buffer),0);
                                            salirCliente(i,&readfds,&numClientes,arrayClientes);
                                            
                                        }
                                        
                                        bzero(buffer,sizeof(buffer));

                                    }

                                    if(cadenaComienzaCon(buffer, "PASSWORD") && login == true)
                                    {
                                        bool exists =false;
                                        string texto = charToString(buffer, char_size);
                                        char separator = ' ';
                                        texto.erase(0,8);
                                        split(texto, separator);
                                        string pass = strings[0];
                                        string aux = "0";

                                        ifstream fich("users.txt");
                                        string linea;
                                        string auxPass;

                                        while (getline(fich, linea) && exists == false) {
                                            
                                            split(linea, separator);
                                            //TODO: Pasar linea de string a char
                                            char charLinea[MSG_SIZE];
                                            strcpy(charLinea,linea.c_str());

                                            char charUser[MSG_SIZE];
                                            strcpy(charUser,user.c_str());
                                            if(cadenaComienzaCon(charLinea, charUser) == 1)
                                            {
                                                string auxUser = strings[0];
                                                string auxPass = strings[1];
                                            }
                                            
                                            if(auxPass == pass)
                                            {
                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer,"\n+Ok. Usuario validado\n");
                                                send(arrayClientes[i],buffer , sizeof(buffer),0);
                                                exists=true;
                                                aux = pass;
                                            }
                                        }
                                        fich.close();

                                        if(exists == false)
                                        {
                                            bzero(buffer,sizeof(buffer));
                                            strcpy(buffer,"\n-Err. Usuario incorrecto\n");
                                            send(arrayClientes[i],buffer , sizeof(buffer),0);
                                            salirCliente(i,&readfds,&numClientes,arrayClientes);
                                        }

                                        bzero(buffer,sizeof(buffer));
                                    }

                                    string userName = user;
                                    string userPass = pass;

                                    vector<player> jugadores;
                                    
                                    if(user != "0" && pass != "0")
                                    {
                                        for(int i = 0; i < 30; i++)
                                        { //TODO: PROBABLY MAL// COMO HACER ESTO
                                            //jugadores[0] = new player(new_sd, 1, userName, userPass, 0)
                                        }
                                        loged = true;
                                    }
                                    else
                                    {
                                        bzero(buffer,sizeof(buffer));
                                        strcpy(buffer,"\n-Err. ERROR EN EL LOGIN 500\n");
                                        send(arrayClientes[i],buffer , sizeof(buffer),0);
                                        salirCliente(i,&readfds,&numClientes,arrayClientes);
                                        salirCliente(i,&readfds,&numClientes,arrayClientes);
                                    }
                                }


                                if(strcmp(buffer,"INICAR-PARTIDA\n") == 0)
                                {
                                    //TODO: ..
                                    printf("+Ok. A organizar las colas");
                                    bzero(buffer,sizeof(buffer));
                                }

                                if(strcmp(buffer,"SALIR\n") == 0)
                                {
                                    printf("+Ok. Desconexion procesada");
                                    bzero(buffer,sizeof(buffer));
                                    salirCliente(i,&readfds,&numClientes,arrayClientes); 
                                }

                                else
                                {   // TODO: RECEPCION DE MENSAJES POR PARTE DE LOS CLIENTES
                                    // TODO: Mensaje de error
                                    sprintf(identificador,"<%d>: %s",i,buffer);
                                    bzero(buffer,sizeof(buffer));

                                    strcpy(buffer,identificador);

                                    printf("%s\n", buffer);

                                    for(j=0; j<numClientes; j++)
                                    if(arrayClientes[j] != i)
                                    send(arrayClientes[j],buffer,sizeof(buffer),0);  
                                }
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos == 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                printf("+Ok. Desconexion procesada");
                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,sizeof(buffer),0);


}

void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}
