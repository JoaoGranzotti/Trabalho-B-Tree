#include <stdio.h>
#include <stdlib.h>
#include "TAD_arvore_B.h"

//TAD da estrutura de dados de fila. Cada função é autoexplicativa, exceto a última.

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
    pagFila *aux;
    aux = malloc(sizeof(pagFila));

    aux->pag = inserir;
    aux->level = altura;
    aux->prox = NULL;

    if(F->size == 0){
        F->front = aux;
        F->back = aux;
        F->front->prox = F->back;
    }
    else{
        F->back->prox = aux;
        F->back = aux;
    }

    F->size++;
}

PAGINA pull(Fila *F, int *nivel){
    PAGINA retorno;
    pagFila *aux;

    aux = F->front;
    retorno = aux->pag;
    *nivel = aux->level;

    F->front = F->front->prox;

    aux->prox = NULL;
    free(aux);

    F->size--;

    return retorno;
}

int empty(Fila *F){
    if(F->size == 0)
        return 1;
    else return 0;
}

//Função para busca em largura na árvore B.
void bfs(int RRNbfs){
    int i;
    int nivel = 0;
    PAGINA busca;
    PAGINA pagBFS;
    PAGINA k;
    char *mensagem;
    Fila F;

    mensagem = malloc(30*sizeof(char));

    //Cria-se a fila para podemos utilizá-la
    criarFila(&F);

    //Pegamos a página para começar a bfs
    pagBFS = getPagina(RRNbfs);
    //Se a função getPagina retornar erro, ou seja, uma página com número de chaves -1
    if(pagBFS.numeroChaves == -1){
        printf("Nao foi possivel encontrar a pagina raiz com RRN %d. Erro 0x3000.\n", RRNbfs);
    }

    //Colocamos a página inicial na fila
    push(&F, pagBFS, nivel);

    //Enquanto a fila não está vazia, ou seja, não percorremos todos os nós
    while(!empty(&F)){
        //Pegamos o nó da frente da fila e o nível respectivo
        k = pull(&F, &nivel);

        //Joga no arquivo de log o nivel e o número de chaves
        sprintf(mensagem, "%d %d ", nivel, k.numeroChaves);
        atualizaArquivoDeLog(mensagem, LOG_MSG_OFF);

        //Esse for irá jogar todos os filhos na fila e colocará todas as chaves da página no arquivo de log
        for(i = 0; i <= k.numeroChaves; i++){
            //Se não extrapolamos o número máximo de chaves
            if(i != k.numeroChaves){
                //Joga no arquivo de log a chave e seu offset
                sprintf(mensagem, "<%d/%d> ", k.chaves[i][0], k.chaves[i][1]);
                atualizaArquivoDeLog(mensagem, LOG_MSG_OFF);
            }

            //Se existir filho, insere na fila
            if(k.filhos[i] != -1){
                //Pega o RRN referente ao filho
                int RRN = k.filhos[i];
                //E pega a página referente ao RRN
                busca = getPagina(RRN);

                //Se a busca deu erro, ou seja, retornou a página que era setada como "erro"
                if(busca.numeroChaves == -1){
                    printf("Nao foi possivel encontrar a pagina com RRN %d. Erro 0x3001.\n", RRN);
                    return;
                }

                //Insere a página filha na fila, com um nível somado
                push(&F, busca, nivel+1);
            }
        }
        //Somente para ter um \n no arquivo de log:
        atualizaArquivoDeLog("\n", LOG_MSG_OFF);
    }

    //Terminamos esvaziando a fila caso exista algum elemento ainda e liberando a memória utilizada
    esvaziaFila(&F);
    free(mensagem);
}
