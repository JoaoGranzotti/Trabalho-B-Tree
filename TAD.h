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
void lerArquivoDeDados(){
    FILE *arqdados, *arqind;
    arqdados = fopen ("dados.dad", "r");
    arqind = fopen ("arvore.idx", "w");

    if (arqdados == NULL) {
       printf ("Houve um erro ao abrir o arquivo de dados.\n");
       return;
    }
    if(arqind == NULL){
        printf ("Houve um erro ao abrir o arquivo de indices.\n");
        return;
    }
    int id;
    int offSet = 0;
    while(!fseek(arqdados, offSet, SEEK_CUR)){
        fscanf(arqdados, "%d|%d", &offSet, &id);
        //função inserir
    }

    char mensagem[] = "Execucao da criacao do arquivo de indice 'arvore.idx' com base no arquivo de dados 'dados.dad'.";
    atualizaArquivoDeLog("log_X.txt", mensagem);
    fclose (arqdados);
    fclose (arqind);
}


// Não sei se vai usar, mas complementa a função acima
/*void criarArquivoDeIndices(){
    //aqui vai o codigo


    //Essa parte escreve o que foi feito no arquivo de log
    char mensagem[] = "Execucao da criacao do arquivo de indice 'arvore.idx' com base no arquivo de dados 'dados.dad'.";
    atualizaArquivoDeLog("log_X.txt", mensagem);
}*/


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
    char idConvertido[4];
    sprintf(idConvertido, "%d", novoRegistro.id);
    strcat(stringConcatenada, idConvertido);
    strcat(stringConcatenada, "|");
    strcat(stringConcatenada, novoRegistro.titulo);
    strcat(stringConcatenada, "|");
    strcat(stringConcatenada, novoRegistro.genero);
    strcat(stringConcatenada, "|");

    int tamanhoString = strlen(stringConcatenada);
    char tamanhoStringConvertido[64];
    sprintf(tamanhoStringConvertido, "%d", tamanhoString);
    strcat(tamanhoStringConvertido, stringConcatenada);
    //agora a nossa string a ser salva ta guardada em tamanhoStringConvertido

    int tamanhoTotal = strlen(tamanhoStringConvertido);
    //salvando no arquivo de dados
    fwrite(&tamanhoStringConvertido, tamanhoTotal, 1, arquivo);
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
