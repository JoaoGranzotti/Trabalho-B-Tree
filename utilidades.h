#include <stdio.h>
#include <stdlib.h>

//fun��o que verifica se n�o h� caracteres especiais em um vetor de char.
//A fun��o retorna 1 se n�o houverem caracteres especiais.
//A fun��o retorna 0 se houverem caracteres especiais.
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
//retorna 0 se o texto n�o for um numero
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

