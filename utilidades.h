#include <stdio.h>
#include <stdlib.h>

//fun��o que verifica se n�o h� caracteres especiais em um vetor de char.
//A fun��o retorna 1 se n�o houverem caracteres especiais.
//A fun��o retorna 0 se houverem caracteres especiais.
int verificaEntrada(char texto[]){
    int i = 0;
    char elemento = texto[i];
    while(elemento != '\0'){
        if((elemento >= 'a' && elemento <= 'z')||(elemento >= 'A' && elemento <= 'Z')){
            i++;
            elemento = texto[i];
        }else{
            if(elemento >= '0' && elemento <= '9'){
                i++;
                elemento = texto[i];
            }
            else{
                if(elemento == ' '){
                    i++;
                    elemento = texto[i];
                }else{
                    printf("Entrada inv�lida. '%c'\n", elemento);
                    return 0;
                }
            }
        }
    }
    return 1;
}

//retorna 1 se o texto for um numero
//retorna 0 se o texto n�o for um numero
int numeroOuChar(char texto[]){
    int i = 0;
    char elemento = texto[i];
    while(elemento != '\0'){
        if((elemento >= 'a' && elemento <= 'z')||(elemento >= 'A' && elemento <= 'Z')){
            printf("Entrada inv�lida.\n");
            return 0;
        }else{
            if(elemento >= '0' && elemento <= '9'){
                i++;
                elemento = texto[i];
            }
            else{
                printf("Entrada inv�lida.\n");
                return 0;
            }
        }
    }
    return 1;
}

