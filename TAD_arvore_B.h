#include <stdio.h>
#include <stdlib.h>

#define ORDEM 5

typedef struct {
    int id;
    char titulo[30];
    char genero[20];
} tRegistro;

typedef struct Pagina{
    //numero de chaves na pagina
    int numeroChaves;
    //vetor de chaves
    int chaves[ORDEM-1][2];
    //vetor de ponteiros das paginas filhas
    int filhos[ORDEM];
    //define se é folha, folha = 1, não folha = 0
    int folha;
} PAGINA;

//Função que exporta da memória para o disco
void inserirEmDisco(PAGINA atual, int RRN, FILE *arqInd){
    int i;
    fseek(arqInd, 2, SEEK_SET);
    fseek(arqInd, RRN*tamanhoPagina, SEEK_CUR);
    fprintf(arqInd, "%d|", atual.numeroChaves);
    for(i = 0; i < ORDEM-1; i++){
        fprintf(arqInd, "%d/%d/", atual.chaves[i][0],  atual.chaves[i][1]);
    }
    fseek(arqInd, -1, SEEK_CUR);
    fprintf(arqInd, "|");
    for(i = 0; i < ORDEM-1; i++){
        fprintf(arqInd, "%d/", atual.filhos[i]);
    }
}

void buscar(int id_no){

}

// não sei se vai precisar, mas essa função vai fazer todos os passos da criação da arvore
void inserirArvoreB(int id, int offSet, int RRN, FILE *arqInd, FILE *arqDados){
    if(offSet == 0){
        //INSERE NÓ RAIZ
        //criando a pagina onde o no raiz vai ficar
        //precisa criar ??

        fprintf(arqInd, "1\n");
        fprintf(arqInd, "1|%d/%d/-1/-1/-1/-1/-1/-1|-1/-1/-1/-1/-1/1/\n", id, offSet);
        //numChaves|id/offset/...|RRNfilho/.../folha/\n
    }
    else{ //NCHAVES|chave1|chave2|chave3|filhos|
        //busca a posição que queremos com busca binaria
        //PAGINA buscado = buscar(id);

        //tem que ler do aruivo de indice pra criar a pagina ??
        int numChaves, i;
        fseek(arqDados, offSet, SEEK_CUR);
        PAGINA atual;
        fscanf(arqInd, "1|%d", &atual.numeroChaves);
        for(i = 0; i < 4; i++){
            fscanf(arqInd, "/%d/%d", &atual.chaves[i][0], &atual.chaves[i][1]);
        }

        fgetc(arqInd);
        for(i = 0; i < 5; i++){
            fscanf(arqInd, "%d/", &atual.filhos[i]);
        }

        fscanf(arqInd, "%d/", &atual.folha);

        //a partir daqui ja tem a pagina bunitin
        //agora vamos fazer uma busca binaria nas chaves pra saber onde tem que ir ou onde daria pra insserir
        //aqui é busca sequencial
        for(i = 0; i < atual.numeroChaves; i++){
            if(atual.chaves[i][0] > id)
                break;
        }
        int posCerta = i;

        if(atual.folha == 1){
            if(atual.numeroChaves < ORDEM-1){
                //insere ordenado
                //logica que funcione pra qualquer situação
                int vetor[ORDEM-1][2];
                for(i = posCerta; i < atual.numeroChaves; i++){
                    vetor[i][0] = atual.chaves[i][0];
                    vetor[i][1] = atual.chaves[i][1];
                }
                atual.chaves[posCerta][0] = id;
                atual.chaves[posCerta][1] = offSet;
                for(i = posCerta; i < atual.numeroChaves; i++){
                    atual.chaves[i+1][0] = vetor[i][0];
                    atual.chaves[i+1][1] = vetor[i][1];
                }
                atual.numeroChaves++;
                inserirEmDisco(atual, RRN, arqInd);
            }
            else{
                //split
            }
        }
        else{
            inserirArvoreB(id, offSet, atual.filhos[posCerta], arqInd, arqDados);
        }
    }
}

