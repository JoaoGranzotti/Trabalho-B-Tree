#include <stdio.h>
#include <stdlib.h>
#include "TAD_arvore_B.h"

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






    /*char elemento;
    int tamanho = fgetc(arquivo);
    while(elemento = fgetc(arquivo) != EOF){
        while(elemento != '|'){
            fgetc(arquivo);
        }
        while(elemento = fgetc(arquivo) != '|'){

        }
        while(elemento = fgetc(arquivo) != '|'){

        }
    }*/

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
    arquivo = fopen ("dados.dad", "a");
    if (arquivo == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return;
    }
    int tamanhoTitulo = 0;
    while( novoRegistro.titulo[tamanhoTitulo] != '\0'){
        tamanhoTitulo++;
    }
    int tamanhoGenero = 0;
    while( novoRegistro.genero[tamanhoGenero] != '\0'){
        tamanhoGenero++;
    }
    int tamanho = tamanhoTitulo + tamanhoGenero + 8;
    fprintf(arquivo, "%d|%d|%s|%s|", tamanho, novoRegistro.id, novoRegistro.titulo, novoRegistro.genero);
    printf ("Arquivo de 'dados' atualizado com sucesso.\n");
    fclose (arquivo);

    //Essa parte escreve o que foi feito no arquivo de log
    char *mensagem = malloc(100*sizeof(char));
    sprintf(mensagem, "Execucao de operacao de INSERCAO de %d, %s, %s.", novoRegistro.id, novoRegistro.titulo, novoRegistro.genero);
    atualizaArquivoDeLog("log_X.txt", mensagem);
    free(mensagem);
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
