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
#include <ifstream.h>
#include <iostream>


#define MSG_SIZE 350
#define MAX_CLIENTS 30
#define PORT 2050
#define MAX_GAMES 10


using namespace std;
//TODO: Hacer funciones de "Existe X letra"

//TODO: Funcion encriptar refran
string encryptQuote(string quote){
    string equote;
    for(int i = 0; i<quote.size(); i++){
        if(quote[i]== ' '){
            equote[i] = ' ';
        }
        else{
            equote[i] = '-';
        }
    }

    //MOSTRAR EL MENSAJE

    for(int k=0; k<quote.size(); k++){
        printf("%c",equote[k]);
    }
    printf("\n");
}

//TODO: Unico recieve

/* Constructor declarado en la definición de la clase como función prototipo
Persona(const std::string& nombre,int edad, float peso, float estatura);
// Implementación fuera de la definición de la clase

Persona::Persona(const string& nombre,int edad, float peso, float estatura){
  this -> nombre = nombre; 
  this -> edad = edad;
  this -> peso = peso;
  this -> estatura = estatura;
}
*/

class player{

	private:
	int descriptor;
    int status; // 0 sin logear, 1 en espera a game, 2 en game
	char name[MSG_SIZE];
    char password[MSG_SIZE];
    int points;

    public:

    //Setters
    void setDescriptor(int desc)
    {
        descriptor = desc;
    }

    void setStatus(int status)
    {
        status=status;
    }

    void setName(char nombre[MSG_SIZE])
    {
        name = nombre;
    }

    void setPassword(char contra[MSG_SIZE])
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

    char getName()
    {
        return name;
    }

    char getPass()
    {
        return password;
    } 

    int getPoints()
    {
        return points;
    }
    //TODO: Añadir Getters y Setters
};

class game{ // TODO: class game:player() ??

    private:
    int descriptor1, descriptor2;
    //TODO: Metodo publico que sea el juego entero?
    //TODO: Añadir Getters y Setters

    public:

    //Setters
    void setDescriptor1(int sd1)
    {
        descriptor1 = sd1;
    }
    
    void setDescriptor2(int sd2)
    {
        descriptor2 = sd2;
    }

    //Getters
    int getDescriptor1()
    {
        return Descriptor1;
    }
    
    int getDescriptor2()
    {
        return Descriptor2;
    }
};
//ESTRUCTURA LOGIN
struct login{
    char username[30];
    char password[30];
};


// TODO: Funciones de registro, una para user, otra para pass
void registration()
{
    char username[30];
    char password[30];

    FILE *log;
    log=fopen("users.txt","w");
    if(log == NULL)
    {
        fputs("Error abriendo el fichero \n", stderr);
        exit(1);
    }

    struct login 1;

    //TODO: Hacerlo todo en la misma linea
    printf("Introduzca su nombre de usuario\n");
    fgets(username, 30, stdin);
    printf("Introduzca su password");
    fgets(password, 30, stdin);

    fwrite(&1,sizeof(1), 1, log);
    fclose(log);
    system("CLS");

    //TODO: Que lleve a lista de espera
}
// TODO: Comprobar funciones de inicio de sesión

void login(){
    char USUARIOcheck[30]="USUARIO";
    char PASSWORDcheck[30]="PASSWORD";
    char username[30];
    char password[30];
    FILE *log;

    log = fopen("users.txt","r");
    if(log==NULL){
        system("Error abriendo el archivo\n", stderr);
        exit(1);
    }

    struct login 1;

    printf("Introduce tu usuario y password como en el ejemplo\n");
    printf("\nUSUARIO nombredeusuario\n\n");
    scanf("%s\n", USUARIOcheck);
    fgets(username, 30, stdin);
    printf("Introduce tu password como en el ejemplo\n");
    printf("\nPASSWORD tupassword\n\n");
    scanf("%s\n", PASSWORDcheck);
    fgets(password, 30, stdin);
    while(fread(&1,sizeof(1), 1, log))
    {
        if(strcmp(username,1.username)==0 && strcmp(password,1.password)==0)
        {
            printf("Inicio de sesion exitoso\n");
        }
        else
        {
            printf("Error en las credenciales\n");
        }
    }
    fclose(log);
    return;
}


// TODO: Funciones que comprueben si un user esta registrado o no
//TODO : usar strtok

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);
void FichRead();

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
    
    bool register = true;
    bool login = true;  

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

                                // TODO: Hacer funcion BufferEmpiezaPorX()
                                // TODO: FUNCIONES DE REGISTRO, una para user, otra para pass???
                                // TODO: FUNCIONES DE LOGIN,
                                // TODO: Diferenciar usuario registrado de no registrado
                                // TODO: PARTIR BUFFER EN VARIABLES (strtok()), cambiar buffer por las variables
 
                                    while(loged == false)
                                    {
                                        if(strcmp(var1, "USUARIO ") == 0)
                                        {   
                                            //TODO: Funcion comprobar que usuario existe y aux ++
                                            //if(existe){ok}
                                            //else{exit()}
                                            printf("Pillar user\n");
                                            register = false;
                                        }
                                        
                                        //TODO: Si existe el usuario, comprobar que corresponder con las pass (buscar user +1 linea?)
                                        if(strcmp(buffer, "PASSWORD") == 0)
                                        {
                                            //if(concuerda){ok}
                                            //else{exit()}
                                            printf("Pillar pass\n");
                                            login = true;
                                            if(loged = true)
                                            {                                            
                                                strcpy(buffer, "Inicio de sesión correcto, bienvenido, %d\n",/*Username*/);
                                            }
                                        }

                                        if(strcmp(buffer, "REGISTRO") == 0 && register == true)
                                        {
                                            //TODO: Registro, comprobar que no exista ya, etc etc etc
                                            printf("Pillar REGISTRO\n");
                                            strcpy(buffer, "Registro correcto, bienvenido, %d\n",/*Username*/);
                                            loged = true;
                                        }
                                        else if(register == false)
                                        {
                                            strcpy(buffer, "No es posible registrarse. Reinicie el cliente\n");
                                        }
                                    }
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
                                //TODO : TODO LO SIGUIENTE DENTRO DE UN BLUCE?
                                // TODO: Aqui todo lo del juego??

                                if(strcmp(buffer,"INICAR-PARTIDA\n") == 0)
                                {
                                    //TODO: ..
                                    printf("+Ok. A organizar las colas");
                                    
                                }

                                if(strcmp(buffer,"SALIR\n") == 0)
                                {
                                    printf("+Ok. Desconexion procesada");
                                    salirCliente(i,&readfds,&numClientes,arrayClientes); 
                                }

                                else
                                {   // TODO: RECEPCION DE MENSAJES POR PARTE DE LOS CLIENTES
                                    //TODO: Mensaje de error
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
void FichRead()
{
    string nombre = "refranes.txt"
    string quote;    
    
    ifstream fichero(nombre.c_str());
    if( fichero.fail() )
    {
        cout << "No existe el fichero!" << endl;
        exit(1);
    }
 
    while (! fichero.eof()) 
    {
        getline(fichero,quote);
        if (! fichero.eof()) 
        cout << quote << endl;
    }
    fichero.close();
 
    ifstream fichero(nombre.c_str());
}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}
