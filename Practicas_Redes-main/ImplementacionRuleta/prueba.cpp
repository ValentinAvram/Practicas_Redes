#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <bits/stdc++.h> 
#include <iostream>

using namespace std;

bool getRight(string quote, string letter){
    for(int i = 0; i<quote.size(); i++){
        if(quote[i] == letter[0]){
            return true;
        }
    }

    return false;
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
    for(int j=0; j<size; j++){
        equote = equote + aux[j];
    }

    return equote;
}

int main(){

    int comp = 1;
    int player=1;

    printf("Introduzca su cadena a Encriptar\n");
    string cadena;
    getline(cin, cadena);
    string ecadena;
    ecadena = encryptQuote(cadena);
    cout<<ecadena<<endl;
    while(comp != -1){
            while(player == 1){
            player = 2;
            cout<<"Esta jugando el jugador 1"<<endl;
            printf("\n");
            printf("Introduzca una letra para mostrar\n");
            string letra;
            cin >> letra;
            if(getRight(cadena,letra)==true){
                player = 1;
            }
            ecadena = revealLetterInPanel(cadena, ecadena, letra);
            cout<<ecadena<<endl;
            printf("\n");

            cout<<"Comprobando si esta completa"<<endl;

            if(comp = cadena.compare(ecadena) == -1){
                player = 10;
            }
            }

            while(player == 2){
            player = 1;
            cout<<"Esta jugando el jugador 2"<<endl;
            printf("\n");
            printf("Introduzca una letra para mostrar\n");
            string letra;
            cin >> letra;
            if(getRight(cadena,letra)==true){
                player = 2;
            }
            ecadena = revealLetterInPanel(cadena, ecadena, letra);
            cout<<ecadena<<endl;
            printf("\n");

            cout<<"Comprobando si esta completa"<<endl;

            if(comp = cadena.compare(ecadena) ==-1){
                player = 20;
            }
            }
    }

    cout<<"Felicidades, lo has completado"<<endl;
    if (player == 10){
        cout<<"Ha ganado el jugador 1"<<endl;
    }
    if(player == 20){
        cout<<"Ha ganado el jugador 2"<<endl;
    }

    return 0;
}