#include <stdio.h>
#include <stdlib.h>
#include "TAD_arvore_B.h"
#include <string.h>

//###############################
//#   FUNÇÕES PARA ADIMINISTRAR #
//#   O ARQUIVO DE LOG DE ACOR- #
//#   DO COM O PDF.             #
//###############################

//Abre o arquivo de log, ou o cria se ele não existir
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

// coloca as ações no arquivo de log
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
//#   FUNÇÕES PARA MANIPULAR O  #
//#   ARQUIVO DE DADOS DE ACOR- #
//#   DO COM O PDF.             #
//###############################

// vai pegar as informações do arquivo de dados para criar a partir dele o arquivo de indices
void criarArquivoDeIndice(){
    FILE *arqdados, *arqind;
    arqdados = fopen ("dados.dad", "rb");
    arqind = fopen ("arvore.idx", "ab");

    if (arqdados == NULL) {
       printf ("Houve um erro ao abrir o arquivo de dados.\n");
       return;
    }
    if(arqind == NULL){
        printf ("Houve um erro ao abrir o arquivo de indices.\n");
        return;
    }
    char dadoLido[3];
    int id;
    int tamanho = 0;
    int offSet = 0;
    int raiz;
    int *RRNP;
    int *idP;
    int *offSetP;
    fseek(arqdados, 0, SEEK_SET);
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
        inserir(raiz, id, offSet, RRNP, idP, offSetP, arqind);
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
