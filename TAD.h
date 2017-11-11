#include <stdio.h>
#include <stdlib.h>
#include "TAD_arvore_B.h"
#include <string.h>

//###############################
//#   FUN��ES PARA ADIMINISTRAR #
//#   O ARQUIVO DE LOG DE ACOR- #
//#   DO COM O PDF.             #
//###############################

//Abre o arquivo de log, ou o cria se ele n�o existir
void criarArquivoDeLog(char nomeArquivoLog[]){
    FILE *arquivo;
    arquivo = fopen (nomeArquivoLog, "w");
    if (arquivo == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return;
    }
    fprintf(arquivo, "Historico das operacoes realizadas no programa:");
    printf ("Arquivo de 'log' criado com sucesso.\n");
    fclose (arquivo);
}

// coloca as a��es no arquivo de log
void atualizaArquivoDeLog(char nomeArquivoLog[], char linhaAdicionada[]){
    FILE *arquivo;
    arquivo = fopen (nomeArquivoLog, "a");
    if (arquivo == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return;
    }
    fprintf(arquivo, linhaAdicionada);
    printf ("Arquivo de 'log' atualizado com sucesso.\n");
    fclose (arquivo);
}

//###############################
//#   FUN��ES PARA MANIPULAR O  #
//#   ARQUIVO DE DADOS DE ACOR- #
//#   DO COM O PDF.             #
//###############################

// vai pegar as informa��es do arquivo de dados para criar a partir dele o arquivo de indices
void criarArquivoDeIndice(){
    FILE *arqdados, *arqind;
    arqdados = fopen ("dados.dad", "rb");
    arqind = fopen ("arvore.idx", "rb+");

    if (arqdados == NULL) {
       printf ("Houve um erro ao abrir o arquivo de dados.\n");
       return;
    }
    if(arqind == NULL){
        printf ("Houve um erro ao abrir o arquivo de indices.\n");
        return;
    }
    char dadoLido[3];
    int splitouRiaz = 0;
    int i;
    int id;
    int tamanho = 0;
    int offSet = 0;
    int raiz;
    int *RRNP;
    int *idP;
    int *offSetP;
    //verifica se o arqInd esta vazio
    fseek(arqind, 0, SEEK_END);
    int temCoisa = ftell(arqind);
    //if(temCoisa == 0){
        //pegando o primeiro dado do arquivo de dados
        fseek(arqdados, 0, SEEK_SET);
        fread(dadoLido, 3, 1, arqdados);
        fseek(arqdados, -2, SEEK_CUR);
        id = dadoLido[2];
        offSet = 1;
        tamanho = dadoLido[0];

        PAGINA atual;
        atual.numeroChaves = 1;
        atual.chaves[0][0] = id;
        atual.chaves[0][1] = offSet;
        for(i=1;i<4;i++){
            atual.chaves[i][0] = -1;
            atual.chaves[i][1] = -1;
        }
        for(i=0;i<5;i++){
            atual.filhos[i] = -1;
        }
        atual.folha = 1;
        int a = 3;
        int b = 1;
        char pipe[1] = "|";
        fseek(arqind, 0, SEEK_SET);
        fwrite(&a, 1, 1, arqind);
        fwrite(&pipe, 1, 1, arqind);
        fwrite(&b, 1, 1, arqind);
        inserirEmDisco(atual, 3, arqind);
    //}
    fseek(arqdados, 1, SEEK_SET);
    while(fread(dadoLido, 3, 1, arqdados) != 0){
        fseek(arqdados, -2, SEEK_CUR);
        id = dadoLido[2];
        offSet = tamanho+1;
        tamanho = dadoLido[0] + tamanho;
        fseek(arqind, 0, SEEK_SET);
        fread(dadoLido, 3, 1, arqind);
        raiz = dadoLido[0];
        //pegando a raiz
            fseek(arqind, 1, SEEK_SET);

        //temos o ID OFFSET RAIZ
        printf("antes da funcao inseir\n");
        splitouRiaz = inserir(raiz, id, offSet, RRNP, idP, offSetP, arqind);
        if(splitouRiaz == 1){
            atual.numeroChaves = 1;
            atual.chaves[0][0] = idP;
            atual.chaves[0][1] = offSetP;
            for(i=1;i<4;i++){
                atual.chaves[i][0] = -1;
                atual.chaves[i][1] = -1;
            }
            for(i=2;i<5;i++){
                atual.filhos[i] = -1;
            }
            atual.filhos[i] = raiz;
            atual.filhos[i] = RRNP;
            atual.folha = 0;
            fseek(arqind, 0, SEEK_END);
            a = ftell(arqind);
            b = 1;
            char pipe[1] = "|";
            fseek(arqind, 0, SEEK_SET);
            fwrite(&a, 1, 1, arqind);
            fwrite(&pipe, 1, 1, arqind);
            fwrite(&b, 1, 1, arqind);
            inserirEmDisco(atual, 3, arqind);
        }
        printf("depois da funcao inseir\n");
    }


    char mensagem[] = "Execucao da criacao do arquivo de indice 'arvore.idx' com base no arquivo de dados 'dados.dad'.";
    atualizaArquivoDeLog("log_X.txt", mensagem);
    fclose (arqdados);
    fclose (arqind);
}

//Insere as musicas no arquivo de dados.dad
void inserirMusica(tRegistro novoRegistro){
    FILE *arquivo;
    arquivo = fopen ("dados.dad", "ab");
    if (arquivo == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return;
    }
    //removendo os \n
    novoRegistro.titulo[strcspn(novoRegistro.titulo, "\n")] = 0;
    novoRegistro.genero[strcspn(novoRegistro.genero, "\n")] = 0;

    char stringConcatenada[64] = "|";
    strcat(stringConcatenada, novoRegistro.titulo);
    strcat(stringConcatenada, "|");
    strcat(stringConcatenada, novoRegistro.genero);
    strcat(stringConcatenada, "|");

    //a partir daqui a string esta no padrao desejado !!!
    int tamanho = strlen(stringConcatenada);
    int tamanhoTotal = tamanho + 2;
    char pipe = '|';

    //salvando no arquivo de dados
    fwrite(&tamanhoTotal, 1, 1, arquivo);
    fwrite(&pipe, sizeof(pipe), 1, arquivo);
    fwrite(&novoRegistro.id, 1, 1, arquivo);
    fwrite(&stringConcatenada, tamanho, 1, arquivo);
    printf ("Arquivo de 'dados' atualizado com sucesso.\n");
    fclose (arquivo);

    //Essa parte escreve o que foi feito no arquivo de log
    char mensagem[100];
    sprintf(mensagem, "Execucao de operacao de INSERCAO de %d, %s, %s.\n", novoRegistro.id, novoRegistro.titulo, novoRegistro.genero);
    atualizaArquivoDeLog("log_X.txt", mensagem);
}

void pesquisarMusica(int id_in){
    tRegistro novoRegistro;
    printf("ainda nao esta pronto");


    //Essa parte escreve o que foi feito no arquivo de log
    char *mensagem = malloc(50*sizeof(char));
    sprintf(mensagem, "Execucao de operacao de PESQUISA de %d.", novoRegistro.id);
    atualizaArquivoDeLog("log_X.txt", mensagem);
    free(mensagem);
}

void removerMusica(int id_in){
    tRegistro novoRegistro;
    printf("ainda nao esta pronto");


    //Essa parte escreve o que foi feito no arquivo de log
    char *mensagem = malloc(2*sizeof(int));
    sprintf(mensagem, " %d", novoRegistro.id);
    atualizaArquivoDeLog("log_X.txt", mensagem);
    free(mensagem);
}

void mostrarArvore(){
    printf("ainda nao esta pronto");


    //Essa parte escreve o que foi feito no arquivo de log
    //sprintf(*mensagem, "%d, %s, %s.", novoRegistro.id, novoRegistro.titulo, novoRegistro.genero);
    //atualizaArquivoDeLog("log_X.txt", mensagem);
    return;
}
