#include <stdio.h>

#define ORDEM 5

int main(){
    FILE *arquivo;
        arquivo = fopen ("testinho.dad", "wb");
        if (arquivo == NULL) {
            printf ("Houve um erro ao abrir o arquivo.\n");
        return;
        }
    char dadoLido [16];
    char stringzinha[] = "pa�oca";
    char troca = 't';
    fwrite(&stringzinha, sizeof(stringzinha), 1, arquivo);
    fseek(arquivo, 3, SEEK_SET);
    fwrite(&troca, sizeof(troca), 1, arquivo);
}
