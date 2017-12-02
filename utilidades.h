#include <stdio.h>
#include <stdlib.h>

//função que verifica se não há caracteres especiais em um vetor de char.
//A função retorna 1 se não houverem caracteres especiais.
//A função retorna 0 se houverem caracteres especiais.
int entradaValida(char texto[]){
    int i = 0;
    char elemento = texto[i];

    while(elemento != '\n' && elemento != '\0'){
        if((elemento >= 'a' && elemento <= 'z') || (elemento >= 'A' && elemento <= 'Z') || (elemento >= '0' && elemento <= '9') || (elemento == ' ')){
            i++;
            elemento = texto[i];
        }
        else{
            printf("Entrada invalida.\n");
            return 0;
        }
    }
    return 1;
}

//retorna 1 se o texto for um numero
//retorna 0 se o texto não for um numero
int ehNumero(char texto[])
{
    int i = 0;
    char elemento = texto[i];
    while(elemento != '\0'){
        if(elemento >= '0' && elemento <= '9'){
            i++;
            elemento = texto[i];
        }
        else{
            printf("Entrada invalida.\n");
            return 0;
        }
    }
    return 1;
}

