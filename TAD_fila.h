#include <stdio.h>
#include <stdlib.h>
#include "TAD_arvore_B.h"

typedef struct pagfila{
    PAGINA pag;
    int level;
    struct pagfila *prox;
} pagFila;

typedef struct {
    pagFila *front;
    pagFila *back;
    int size;
} Fila;

void criarFila(Fila *F){
    F->front = NULL;
    F->back = NULL;
    F->size = 0;
}

void esvaziaFila(Fila *F){
    if(F->size == 0)
        return;
    else{
        pagFila *aux;
        aux = F->front;

        pagFila *prox;
        prox = aux->prox;

        while(F->size > 0){
            free(aux);
            F->size--;
            aux = prox;
            prox = aux->prox;
        }
    }
}

void push(Fila *F, PAGINA inserir, int altura){
    pagFila *aux, *auxfim;
    aux = malloc(sizeof(pagFila));

    aux->pag = inserir;
    aux->level = altura;
    aux->prox = NULL;

    printf("passou push\n");

    if(F->size == 0){
        F->front = aux;
        F->back = aux;
        F->front->prox = F->back;
    }
    else{
        auxfim = F->back;
        auxfim->prox = aux;
        aux->prox = NULL;
        F->back = aux;
    }

    F->size++;
}

PAGINA pull(Fila *F, int *nivel){
    PAGINA retorno;
    pagFila *busca;
    pagFila *aux;

    aux = F->back;
    retorno = aux->pag;
    *nivel = aux->level;

    printf("passou pull\n");

    busca = F->front;
    while(busca->prox != aux){
        busca = busca->prox;
    }

    busca->prox = NULL;
    free(aux);

    F->size--;

    return retorno;
}

int empty(Fila *F){
    if(F->size == 0)
        return 1;
    else return 0;
}

void bfs(int RRNbfs){
    int i;
    int nivel = 0;
    PAGINA busca;
    PAGINA pagBFS;
    Fila F;
    criarFila(&F);

    pagBFS = getPagina(RRNbfs);
    if(pagBFS.numeroChaves == -1){
        printf("Nao foi possivel encontrar a pagina raiz com RRN %d. Erro 0x3000.\n", RRNbfs);
    }

    push(&F, pagBFS, nivel);
    while(!empty(&F)){
        PAGINA k = pull(&F, &nivel);
        printf("%d %d ", nivel, k.numeroChaves);
        for(i = 0; i <= k.numeroChaves; i++){
            if(i != k.numeroChaves){
                printf("<%d/%d> ", k.chaves[i][0], k.chaves[i][1]);
            }
            if(k.filhos[i] != -1){
                int RRN = k.filhos[i];
                busca = getPagina(RRN);

                //Se a busca deu erro, ou seja, retornou a página que era setada como "erro"
                if(busca.numeroChaves == -1){
                    printf("Nao foi possivel encontrar a pagina com RRN %d. Erro 0x3001.\n", RRN);
                    return;
                }

                push(&F, busca, nivel+1);
            }
        }
        printf("\n");
    }

    esvaziaFila(&F);
}
