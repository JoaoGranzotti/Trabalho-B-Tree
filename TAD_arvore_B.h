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
    //define se � folha, folha = 1, n�o folha = 0
    int folha;
} PAGINA;

//Fun��o que exporta da mem�ria para o disco
void inserirEmDisco(PAGINA atual, int RRN, FILE *arqInd){
    int i;
    fseek(arqInd, 2, SEEK_SET);
//    fseek(arqInd, RRN*tamanhoPagina, SEEK_CUR);
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

// n�o sei se vai precisar, mas essa fun��o vai fazer todos os passos da cria��o da arvore
void inserirArvoreB(int id, int offSet, int RRN, int raiz, FILE *arqInd, FILE *arqDados){
    if(raiz == -1){
        //INSERE N� RAIZ
        //criando a pagina onde o no raiz vai ficar
        //precisa criar ??

        //fprintf(arqInd, "1\n");
        //fprintf(arqInd, "1|%d/%d/-1/-1/-1/-1/-1/-1|-1/-1/-1/-1/-1/1/\n", id, offSet);
        //numChaves|id/offset/...|RRNfilho/.../folha/\n
    }
    else{
        //NCHAVES|chave1|chave2|chave3|filhos|folha

        //tem que ler do aruivo de indice pra criar a pagina
        int numChaves, i, j;
        PAGINA atual;
        char linhaLida[30];//tamanho fixo das linhas
        fread(linhaLida, sizeof(linhaLida), 1, arqInd);
        atual.numeroChaves = linhaLida[0];
        j = 0;
        for(i=2; i<=14; i+4){
            atual.chaves[j][0] = linhaLida[i];
            atual.chaves[j][1] = linhaLida[i+2];
            j++;
        }
        j=0
        for(i=18; i<=26; i+2){
            atual.filhos[j] = linhaLida[i];
            j++;
        }
        atual.folha = linhaLida[29]
        //a partir daqui ja tem a pagina bunitin

        //Agora vamos ver se � folha ou n�o, se for manda bala pra inserir, se n�o vamo procurar uma que seja
        //aqui � busca sequencial
        for(i = 0; i < atual.numeroChaves; i++){
            if(atual.chaves[i][0] > id)
                break;
        }
        int posCerta = i;

        if(atual.folha == 1){
            if(atual.numeroChaves < ORDEM-1){
                //insere ordenado
                //logica que funcione pra qualquer situa��o
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
                inserirEmDisco(atual, RRN, arqInd);// essa fun��o tem que ser arrumada !!!!
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

