#include <stdio.h>
#include <stdlib.h>

//função que verifica se não há caracteres especiais em um vetor de char.
//A função retorna 1 se não houverem caracteres especiais.
//A função retorna 0 se houverem caracteres especiais.
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
                    printf("Entrada inválida. '%c'\n", elemento);
                    return 0;
                }
            }
        }
    }
    return 1;
}

//retorna 1 se o texto for um numero
//retorna 0 se o texto não for um numero
int numeroOuChar(char texto[]){
    int i = 0;
    char elemento = texto[i];
    while(elemento != '\0'){
        if((elemento >= 'a' && elemento <= 'z')||(elemento >= 'A' && elemento <= 'Z')){
            printf("Entrada inválida.\n");
            return 0;
        }else{
            if(elemento >= '0' && elemento <= '9'){
                i++;
                elemento = texto[i];
            }
            else{
                printf("Entrada inválida.\n");
                return 0;
            }
        }
    }
    return 1;
}

