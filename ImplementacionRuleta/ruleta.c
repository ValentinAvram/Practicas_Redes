/**
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

printf("ADMIN\n");
printf("Introduce la frase\n");

//CREAMOS NUESTRO REFRAN
char quote[250];
fgets(quote, sizeof(quote), stdin);
printf("FRASE: ");
puts(quote);
printf("Size of quote: %zu \n",strlen(quote));

size_t lenght = strlen(quote);

char quote_[250];
int mask[lenght];
for(int j=0; j<lenght; j++){
    mask[j]=0;
}

//ENCRIPTAMOS NUESTRO REFRAN
for(int i = 0; (i<lenght-1); i++){
    
    if(quote[i] == ' '){
        quote_[i]=' ';
    }
    else{
        quote_[i]='-';
    }
}

int pointsA=0;
int pointsB=0;

printf("Buenas jugadores nombre1, nombre2\n");

int changeplayer = 0;
int isComplete = 0; //HACER FUNCION QUE VEA SI ESTA COMPLETO

    while(isComplete == 0){
        while(changeplayer==0){

            printf("La frase es: \n");

            for(int k= 0; k<lenght-1; k++){
            printf("%c",quote_[k]);
            }

            printf("\n\nPuntos de nombre1: %i\n", pointsA);

            printf("Que desea jugar?\n");

            printf("Introduzca ~VOCAL (intro) vocal~ para revelar una vocal\n");

            printf("Introduzca ~CONSONANTE (intro) consonante~ para revelar una consonante\n");

            printf("Ejemplo: \nCONSONANTE\nL\n");

            char letter[50];
            char mode[50];
            char vocal[50] = "VOCAL";
            char consonante[50] = "CONSONANTE";

            scanf("%s\n", mode);
            fgets(letter, sizeof(letter), stdin);

            //COMPROBACIÓN PARA QUE SE JUEGUE EN MAYÚSCULAS
            if(letter[0]>90){
                letter[0] = letter[0] - 32;
                printf("Has elegido la letra: %c\n", *letter);
            }

            //COMPROBAMOS SI EL USUARIO JUEGA CON VOCAL O CONSONANTE
            int vocalcheck = strcmp(vocal, mode);
            int consonantecheck = strcmp(consonante, mode);
            
            //EL JUGADOR UNO HA SELECCIONADO VOCAL
            if(vocalcheck == 0){
                printf("Has elegido vocal\n");
                
                //EL JUGADOR UNO NO TIENE PUNTOS PARA COMPRAR VOCAL CAMBIAMOS LA LETRA POR UNA CARACTER QUE NO PUEDA ESTAR EN EL PANEL
                if(pointsA<15){
                    printf("No tienes saldo para una vocal\n");
                    letter[0] = 42;
                }

                //EL JUGADOR UNO HA ACERTADO LA VOCAL
                changeplayer = 1;
                int counter=0;
                //PARA QUE PUEDA SEGUIR JUGANDO TRAS NO PODER COMPRAR VOCAL
                if(letter[0] == 42){
                    changeplayer = 0;
                }

                for(int k=0; k < lenght; ++k) {
                    if (quote[k] == letter[0]) {
                    mask[k] = 1;
                    quote_[k] = quote[k];
                    counter++;
                    changeplayer=0;

                    for(int i = 0; i< lenght; i++){
                            if(mask[i] == 0){
                                isComplete = 0; 
                            }
                        }

                        printf("Quieres salir ADMIN?\n");
                        int saliradmin;
                        scanf("%d", &saliradmin);
                        if(saliradmin == 1){
                            isComplete = 1;
                        }
                    }

                }
                printf("Habia %i %s\n", counter, letter);
                printf("%s\n\n\n", quote_);
            }

            if(consonantecheck == 0){
                printf("Has elegido consonante\n");
                changeplayer=1;
                int counter=0;

                for(int k=0; k < lenght; ++k) {
                    if (quote[k] == letter[0]) {
                    mask[k] = 1;
                    quote_[k] = quote[k];
                    counter++;
                    changeplayer=0;
                    pointsA = pointsA + 25;

                    for(int i = 0; i< lenght; i++){
                            if(mask[i] == 0){
                                isComplete = 0; 
                            }
                        }
                        //FUNCION DE SALIDA PARA BETATESTERS 
                        printf("Quieres salir ADMIN?\n");
                        int saliradmin;
                        scanf("%d", &saliradmin);
                        if(saliradmin == 1){
                            isComplete = 1;
                        }
                    }
                }

                //VER LA PALABRA
                printf("\n\n\n");
                printf("Habia %i %s\n", counter, letter);
                printf("%s\n\n\n", quote_);
            }
        }

        while(changeplayer==1){

            printf("La frase es: \n");

            for(int k= 0; k<lenght-1; k++){
            printf("%c",quote_[k]);
            }

            printf("\n\nPuntos de nombre2: %i\n", pointsB);

            printf("Que desea jugar?\n");

            printf("Introduzca ~VOCAL (intro) vocal~ para revelar una vocal\n");

            printf("Introduzca ~CONSONANTE (intro) consonante~ para revelar una consonante\n");

            printf("Ejemplo: \nCONSONANTE\nL\n");

            char letter[50];
            char mode[50];
            char vocal[50] = "VOCAL";
            char consonante[50] = "CONSONANTE";

            scanf("%s\n", mode);
            fgets(letter, sizeof(letter), stdin);

            if(letter[0]>90){
                letter[0] = letter[0] - 32;
                printf("Has elegido la letra: %c\n", *letter);
            }

            int vocalcheck = strcmp(vocal, mode);
            int consonantecheck = strcmp(consonante, mode);
            

            if(vocalcheck == 0){
                printf("Has elegido vocal\n");
                
                if(pointsB<15){
                    printf("No tienes saldo para una vocal\n");
                    letter[0] = 0;
                }
                
                changeplayer = 1;
                int counter=0;

                //MARCAMOS LA LETRA QUE HEMOS ACERTADO
                for(int k=0; k < lenght; ++k) {
                    if (quote[k] == letter[0]) {
                    mask[k] = 1;
                    quote_[k] = quote[k];
                    counter++;
                    changeplayer=0;

                    for(int i = 0; i< lenght; i++){
                            if(mask[i] == 0){
                                isComplete = 0; 
                            }
                        }

                        printf("Quieres salir ADMIN?\n");
                        int saliradmin;
                        scanf("%d", &saliradmin);
                        if(saliradmin == 1){
                            isComplete = 1;
                        }

                    }

                }
                printf("Habia %i %s\n", counter, letter);
                printf("%s\n\n\n", quote_);
            }

            if(consonantecheck == 0){
                printf("Has elegido consonante\n");
                changeplayer=1;
                int counter=0;

                for(int k=0; k < lenght; ++k) {
                    if (quote[k] == letter[0]) {
                        mask[k] = 1;
                        quote_[k] = quote[k];
                        counter++;
                        changeplayer=0;
                        pointsB = pointsB + 25;

                        for(int i = 0; i< lenght; i++){
                            if(mask[i] == 0){
                                isComplete = 0; 
                            }
                        }

                        printf("Quieres salir ADMIN?\n");
                        int saliradmin;
                        scanf("%d", &saliradmin);
                        if(saliradmin == 1){
                            isComplete = 1;
                        }
    
                    }

                    }
                printf("Habia %i %s\n", counter, letter);
                printf("%s\n\n\n", quote_);
                }
                
            }
        }
        
        isComplete = 1;

        

    if(changeplayer == 0){
        printf("Ha ganado jugador1!!!\n");
    }

    if(changeplayer == 1){
        printf("Ha ganado jugador2 !!!\n");
    }

}