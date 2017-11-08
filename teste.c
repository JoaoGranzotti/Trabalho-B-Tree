#include <stdio.h>
#include "TAD_arvore_B.h"

#define ORDEM 5

int main(){
    FILE *arquivo;
        arquivo = fopen ("testinho.dad", "wb");
        if (arquivo == NULL) {
            printf ("Houve um erro ao abrir o arquivo.\n");
        return;
        }
    char dadoLido [16];
    char stringzinha[] = "paçoca";
    fwrite(&stringzinha, sizeof(stringzinha), 1, arquivo);
    //fseek(arquivo, 0, SEEK_CUR);
    //fread(dadoLido, 15, 1, arquivo);
    //printf("\n%s\n",dadoLido);
}
