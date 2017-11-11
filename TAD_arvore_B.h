#include <stdio.h>
#include <stdlib.h>

#define ORDEM 5
#define TAMINDICE 30

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
//recebe a pagina a ser inserida, a posicao em que devemos inseri-la e o arquivo
int inserirEmDisco(PAGINA atual, int RRN, FILE *arqInd){
    int i, j;

    char pipe[1] = "|";
    char barra[1] = "/";

    fseek(arqInd, RRN, SEEK_SET);
    fwrite(&atual.numeroChaves, 1, 1, arqInd);
    fwrite(&pipe, 1, 1, arqInd);
    for(i=0; i<(ORDEM-1); i++){
        j = i;
        fwrite(&atual.chaves[j][0], 1, 1, arqInd);
        fwrite(&barra, 1, 1, arqInd);
        fwrite(&atual.chaves[j][1], 1, 1, arqInd);
        if(j != ORDEM-2){
            fwrite(&barra, 1, 1, arqInd);
        }
        else{
            fwrite(&pipe, 1, 1, arqInd);
        }
    }
    for(i=0;i<=ORDEM-1;i++){
        j = i;
        fwrite(&atual.filhos[j], 1, 1, arqInd);
        if(j!=ORDEM-1){
            fwrite(&barra, 1, 1, arqInd);
        }
        else{
            fwrite(&pipe, 1, 1, arqInd);
        }
    }
    fwrite(&atual.folha, 1, 1, arqInd);
    fwrite(&pipe, 1, 1, arqInd);

    /*char nChaves[1];
    char chave[1];
    char filho[1];
    char folha[1];
    char linhaAdicionada[TAMINDICE] = "";
    sprintf(nChaves, "%d", atual.numeroChaves);
    //criando a string a ser escrita
    strcat(linhaAdicionada, nChaves);
    strcat(linhaAdicionada, "|");
    printf("Essa eh a string: %s\n", linhaAdicionada);
    for(i=0; i<(ORDEM-1); i++){
        j = i;
        sprintf(chave, "%d", atual.chaves[j][0]);
        strcat(linhaAdicionada, chave);
        strcat(linhaAdicionada, "/");
        sprintf(chave, "%d", atual.chaves[j][1]);
        strcat(linhaAdicionada, chave);
        if(j != ORDEM-2){
            strcat(linhaAdicionada, "/");
        }
        else{
            strcat(linhaAdicionada, "|");
        }
        printf("Essa eh a string: %s\n", linhaAdicionada);
    }
    for(i=0;i<=ORDEM-1;i++){
        j = i;
        sprintf(filho, "%d", atual.filhos[j]);
        strcat(linhaAdicionada, filho);
        if(j!=ORDEM-1){
            strcat(linhaAdicionada, "/");
        }
        else{
            strcat(linhaAdicionada, "|");
        }
        printf("Essa eh a string: %s\n", linhaAdicionada);
    }
    printf("Essa eh a string: %s\n", linhaAdicionada);
    sprintf(folha, "%d", atual.folha);
    strcat(linhaAdicionada, folha);
    strcat(linhaAdicionada, "|");
    //daqui pra baixo a estring ta pronta

    //vamos guarda saporra agora !!!
    fseek(arqInd, RRN, SEEK_SET);
    fwrite(&linhaAdicionada, sizeof(linhaAdicionada), 1, arqInd);
    */
    return RRN;
}

int buscar(int id_no){

}

void split(int id, int offSet, int RRN, PAGINA *atual, int *idPromovido, int *offSetPromovido, int *RRNP, PAGINA *novaPagina, FILE *arqInd){
    //criando um esquema de suposta pagina temporaria com um espaço a mais
    int dados[5][2];
    int filhos[6];
    int i, j;
    //calculando a posição do dado novo na pagina temporaria
    i = 0;
    while(id > atual->chaves[i][0]){
        i = i++;
    }

    //colocando os dados na suposta pagina temporaria
    for(j=0;j<i;j++){
        dados[j][0] = atual->chaves[j][0];
        dados[j][1] = atual->chaves[j][1];
    }
    //da um shift a partir do ponto i
    for(j=ORDEM-1;j>i;j--){
        dados[j][0] = atual->chaves[j-1][0];
        dados[j][1] = atual->chaves[j-1][1];
    }
    dados[i][0] = id;
    dados[i][1] = offSet;

    //colocando os filhos tambem
    for(j=0;j<i+1;j++){
        filhos[j] = atual->filhos[j];
    }
    //da um shift a partir do ponto i
    for(j=ORDEM;j>i+1;j--){
        filhos[j] = atual->filhos[j-1];
    }
    filhos[i+1] = RRN;//RRN
    //Daqui pra frente a pagina temporaria esta no formato desejado

    //vamos agora colocar os valores nas duas novas paginas de acordo com a regra
    *idPromovido = dados[2][0];  //2 é o valor do meio
    *offSetPromovido = dados[2][1];   //2 é o valor do meio
    fseek(arqInd, 0, SEEK_END);
    *RRNP = ftell(arqInd);

    novaPagina->chaves[0][0] = dados[3][0];
    novaPagina->chaves[0][1] = dados[3][1];
    novaPagina->chaves[1][0] = dados[4][0];
    novaPagina->chaves[1][0] = dados[4][1];

    atual->chaves[0][0] = dados[0][0];
    atual->chaves[0][1] = dados[0][0];
    atual->chaves[1][0] = dados[1][0];
    atual->chaves[1][1] = dados[1][0];

    atual->filhos[0] = filhos[0];
    atual->filhos[1] = filhos[1];
    atual->filhos[2] = filhos[2];

    novaPagina->filhos[0] = filhos[3];
    novaPagina->filhos[1] = filhos[4];
    novaPagina->filhos[2] = filhos[5];
}

int inserir(int RRN, int id, int offSet, int *RRNP, int *idP, int *offSetP, FILE *arqInd){
    int i, j, valorRetorno;
    if(RRN == -1){
        printf("chegou aqui");
        *idP = id;
        *offSetP = offSet;
        *RRNP = -1;
        return 1;
    }
    else{
        printf("Ta entrando");
        //arrumar a leitura dessa carralha
        //lendo pagina no RRN corrente
        PAGINA atual;
        char linhaLida[TAMINDICE];//tamanho fixo das linhas
        fseek(arqInd, RRN, SEEK_SET);
        fread(&linhaLida, sizeof(linhaLida), 1, arqInd);
        atual.numeroChaves = linhaLida[0];
        j = 0;
        for(i=2; i<=14; i+4){
            atual.chaves[j][0] = linhaLida[i];
            atual.chaves[j][1] = linhaLida[i+2];
            j++;
        }
        j = 0;
        for(i=18; i<=26; i+2){
            atual.filhos[j] = linhaLida[i];
            j++;
        }
        atual.folha = linhaLida[29];
        //a partir daqui já tem a pagina bunitin

        //Acha a posição
        i = 0;
        while(id > atual.chaves[i][0] && atual.chaves[i][0] != -1 && i < 4){
            i++;
        }
        if(atual.chaves[i][0] == id){
            printf("JÁ TEM ESSE ID !!!");
        }
        else{
            printf("Chegou nessa parte!!");
            valorRetorno = inserir(atual.filhos[i], id, offSet, RRNP, idP, offSetP, arqInd); /// os parametros 3 e 4
            if(valorRetorno == 0){
                return 0;
            }
            else{
                if(atual.numeroChaves < 4){
                    i = 0;
                    while(*idP > atual.chaves[i][0] && atual.chaves[i][0] != -1 && i < 4){
                        i++;
                    }
                    //da um shift a partir do ponto i
                    for(j=ORDEM-1;j>i;j--){
                        atual.chaves[j][0] = atual.chaves[j-1][0];
                        atual.chaves[j][1] = atual.chaves[j-1][1];
                    }
                    atual.chaves[i][0] = *idP;
                    atual.chaves[i][1] = *offSetP;
                    for(j=0;j<ORDEM;j++){
                        atual.filhos[j] = -1;
                    }
                    inserirEmDisco(atual, RRN, arqInd);
                    return 0;
                }
                else{
                    //parte do split, pika grossa e grande !!
                    PAGINA paginaNova;
                    split(*idP, *offSetP, RRN, &atual, idP, offSetP, RRNP, &paginaNova, arqInd);
                    inserirEmDisco(atual, RRN, arqInd);
                    inserirEmDisco(paginaNova, *RRNP, arqInd);
                    return 1;
                }
            }
        }
    }
}

