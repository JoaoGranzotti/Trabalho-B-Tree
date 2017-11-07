#include <stdio.h>
#include "TAD_arvore_B.h"

#define ORDEM 5

typedef struct {
    int numeroChaves;
    int chaves[ORDEM-1][2];
    int folha;
    int filhos[ORDEM];
} PAGINA;

int main(){
    int i, j;
    int id, offSet;
    FILE *arq = fopen ("arvore.idx", "r+");
    while(1){
        printf("id e offSet\n");
        scanf("%d %d", &id, &offSet);
        inserirArvoreB(id, offSet, 0)
    }



    return 0;
}
