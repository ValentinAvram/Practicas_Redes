/**
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
int addPoints{

}

int subtrackPoints{

}

void seeQuote{

}
**/

int main(){

printf("ADMIN\n");
printf("Introduce la frase\n");

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
bool isComplete = false; //HACER FUNCION QUE VEA SI ESTA COMPLETO

    while(isComplete == false){
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
                    }
                }
                printf("Habia %i %s\n", counter, letter);
                printf("%s\n\n\n", quote_);
            }
        }

        while(changeplayer==0){

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
                    }
                }
                printf("Habia %i %s\n", counter, letter);
                printf("%s\n\n\n", quote_);
            }
        }
    
    }
}