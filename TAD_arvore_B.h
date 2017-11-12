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
int inserirEmDisco(PAGINA atual, int RRN){
    FILE *arqInd;
    arqInd = fopen("arvore.idx", "rb+");

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
    fclose(arqInd);
    return RRN;
}

int buscar(int id_no){

}

void split(int id, int offSet, int RRN, PAGINA *atual, int *idPromovido, int *offSetPromovido, int *RRNP, PAGINA *novaPagina, FILE *arqInd){
    //criando um esquema de suposta pagina temporaria com um espaço a mais
    int dados[5][2];
    int filhos[6];
    int i, j, pos;
    //calculando a posição do dado novo na pagina temporaria
    i = 0;
    while(id > atual->chaves[i][0]){
        i++;
    }
    pos = i;
    //colocando os dados na suposta pagina temporaria
    for(j=0;j<i;j++){
        dados[j][0] = atual->chaves[j][0];
        dados[j][1] = atual->chaves[j][1];
    }
    //da um shift a partir do ponto i
    for(j=ORDEM-2;j>i;j--){
        dados[j][0] = atual->chaves[j-1][0];
        dados[j][1] = atual->chaves[j-1][1];
    }
    dados[pos][0] = id;
    dados[pos][1] = offSet;


    //testando se esta mesmo
    for(i=0;i<5;i++){
        printf("id: %d\n", dados[i][0]);
        printf("offset: %d\n", dados[i][1]);
    }

    //colocando os filhos tambem
    for(j=0;j<pos+1;j++){
        filhos[j] = atual->filhos[j];
    }
    //da um shift a partir do ponto i
    for(j=ORDEM-1;j>pos+1;j--){
        filhos[j] = atual->filhos[j-1];
    }
    //filhos[pos+1] = RRN;//RRN /////////////@@@@@@@@@@@@@@@
    filhos[pos+1] = *RRNP;
    //Daqui pra frente a pagina temporaria esta no formato desejado

    //testando se esta mesmo
    printf("\n\nSegunda verificacao:\n");
    for(i=0;i<5;i++){
        printf("id: %d\n", dados[i][0]);
        printf("offset: %d\n", dados[i][1]);
    }

    //vamos agora colocar os valores nas duas novas paginas de acordo com a regra
    *idPromovido = dados[2][0];  //2 é o valor do meio
    *offSetPromovido = dados[2][1];   //2 é o valor do meio
    fseek(arqInd, 0, SEEK_END);
    *RRNP = ftell(arqInd);

    novaPagina->chaves[0][0] = dados[3][0];
    novaPagina->chaves[0][1] = dados[3][1];
    novaPagina->chaves[1][0] = dados[4][0];
    novaPagina->chaves[1][1] = dados[4][1];
    novaPagina->chaves[2][0] = 255;
    novaPagina->chaves[2][1] = 255;
    novaPagina->chaves[3][0] = 255;
    novaPagina->chaves[3][1] = 255;

    atual->chaves[0][0] = dados[0][0];
    atual->chaves[0][1] = dados[0][1];
    atual->chaves[1][0] = dados[1][0];
    atual->chaves[1][1] = dados[1][1];
    atual->chaves[2][0] = 255;
    atual->chaves[2][1] = 255;
    atual->chaves[3][0] = 255;
    atual->chaves[3][1] = 255;

    atual->filhos[0] = filhos[0];
    atual->filhos[1] = filhos[1];
    atual->filhos[2] = filhos[2];
    atual->filhos[3] = 255;
    atual->filhos[4] = 255;

    novaPagina->filhos[0] = filhos[3];
    novaPagina->filhos[1] = filhos[4];
    novaPagina->filhos[2] = filhos[5];
    novaPagina->filhos[3] = 255;
    novaPagina->filhos[4] = 255;

    atual->numeroChaves = 2;
    novaPagina->numeroChaves = 2;
}

int inserir(int RRN, int id, int offSet, int *RRNP, int *idP, int *offSetP){
    int i, j, valorRetorno;
    FILE *arqInd;
    arqInd = fopen("arvore.idx", "rb");
    if(RRN == 255){
        *idP = id;
        *offSetP = offSet;
        *RRNP = -1;
        return 1;
    }
    else{
        //arrumar a leitura dessa carralha
        //lendo pagina no RRN corrente
        PAGINA atual;
        int temporario, testa;
        fseek(arqInd, RRN, SEEK_SET);
        fread(&temporario, 1, 1, arqInd);
        atual.numeroChaves = temporario;
        fseek(arqInd, 1, SEEK_CUR);
        for(i=0;i<4;i++){
            fread(&temporario, 1, 1, arqInd);
            atual.chaves[i][0] = temporario;
            fseek(arqInd, 1, SEEK_CUR);
            fread(&temporario, 1, 1, arqInd);
            atual.chaves[i][1] = temporario;
            fseek(arqInd, 1, SEEK_CUR);
        }
        for(i=0;i<5;i++){
            fread(&temporario, 1, 1, arqInd);
            atual.filhos[i] = temporario;
            fseek(arqInd, 1, SEEK_CUR);
        }
        fread(&atual.folha, 1, 1, arqInd);
        //a partir daqui já tem a pagina bunitin

        //testando se pegou certo
        /*
        printf("numero chaves: %d\n", atual.numeroChaves);
        for(i=0;i<4;i++){
            printf("id: %d, offset: %d\n", atual.chaves[i][0], atual.chaves[i][1]);
        }
        for(i=0;i<5;i++){
            printf("filhos: %d\n", atual.filhos[i]);
        }
        printf("folha: %d\n", atual.folha);
        */

        //Acha a posição
        i = 0;
        while(id > atual.chaves[i][0] && atual.chaves[i][0] != -1 && i < 4){
            i++;
        }
        if(atual.chaves[i][0] == id){
            printf("JÁ TEM ESSE ID !!!");
        }
        else{
            valorRetorno = inserir(atual.filhos[i], id, offSet, RRNP, idP, offSetP); /// os parametros 3 e 4
            if(valorRetorno == 0){
                return 0;
            }
            else{
                if(atual.numeroChaves < 4){
                    i = 0;
                    while(*idP > atual.chaves[i][0] && atual.chaves[i][0] != -1 && i < 4){
                        i++;
                    }
                    atual.numeroChaves++;
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
                    printf("Vai inserir no RRN %d seguinte dado:\n", RRN);
                    printf("numero chaves: %d\n", atual.numeroChaves);
                    for(i=0;i<4;i++){
                        printf("id: %d, offset: %d\n", atual.chaves[i][0], atual.chaves[i][1]);
                    }
                    for(i=0;i<5;i++){
                        printf("filhos: %d\n", atual.filhos[i]);
                    }
                    printf("folha: %d\n", atual.folha);
                    system("PAUSE");
                    inserirEmDisco(atual, RRN);
                    return 0;
                }
                else{
                    //parte do split, pika grossa e grande !!
                    PAGINA paginaNova;
                    printf("Vai entrar no split\n");
                    split(*idP, *offSetP, RRN, &atual, idP, offSetP, RRNP, &paginaNova, arqInd);
                    printf("Voltou do split\n");

                    printf("Vai inserir no RRN %d seguinte dado:\n", RRN);
                    printf("numero chaves: %d\n", atual.numeroChaves);
                    for(i=0;i<4;i++){
                        printf("id: %d, offset: %d\n", atual.chaves[i][0], atual.chaves[i][1]);
                    }
                    for(i=0;i<5;i++){
                        printf("filhos: %d\n", atual.filhos[i]);
                    }
                    printf("folha: %d\n", atual.folha);
                    system("PAUSE");
                    inserirEmDisco(atual, RRN);

                    printf("Vai inserir no RRN %d seguinte dado:\n", RRN);
                    printf("numero chaves: %d\n", paginaNova.numeroChaves);
                    for(i=0;i<4;i++){
                        printf("id: %d, offset: %d\n", paginaNova.chaves[i][0], paginaNova.chaves[i][1]);
                    }
                    for(i=0;i<5;i++){
                        printf("filhos: %d\n", paginaNova.filhos[i]);
                    }
                    printf("folha: %d\n", paginaNova.folha);
                    system("PAUSE");
                    inserirEmDisco(paginaNova, *RRNP);
                    return 1;
                }
            }
        }
    }
}

