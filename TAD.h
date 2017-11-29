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

int camp_var_reg_var(tRegistro e, char *buffer) {
     sprintf(buffer, "|%d|%s|%s|", e.id, e.titulo, e.genero);
     return strlen(buffer);
}

char *parser(char *buffer, int *pos) {
    int posi = *pos;

    while(buffer[*pos] != '|')
        (*pos)++;

    buffer[*pos] = '\0';
    (*pos)++;
    return &buffer[posi];
}

// vai pegar as informações do arquivo de dados para criar a partir dele o arquivo de indices
void criarArquivoDeIndice(){
    FILE *arqdados, *arqind;
    arqdados = fopen ("dados.dad", "rb");
    arqind = fopen ("arvore.idx", "wb");

    if (arqdados == NULL) {
       printf ("Houve um erro ao abrir o arquivo de dados.\n");
       return;
    }
    if(arqind == NULL){
        printf ("Houve um erro ao abrir o arquivo de indices.\n");
        return;
    }

    int i;
    int atualizado = 1;
    int RRNraiz;
    int offSet = 0;
    int pos;
    int retorno; //valor auxiliar de retorno da função de inserção. Servirá para
                 //escrever no arquivo de log.
    long posArq;
    long tamanhoArquivo;
    char size;
    char buffer[1000];
    tRegistro reg;

    fwrite(&atualizado, sizeof(int), 1, arqind);
    fclose(arqind);

    while (fread(&size, sizeof(size), 1, arqdados)) {
        fread(buffer, size, 1, arqdados);
        pos = 1;
        sscanf(parser(buffer, &pos), "%d", &reg.id);
        strcpy(reg.titulo, parser(buffer, &pos));
        strcpy(reg.genero, parser(buffer, &pos));

        arqind = fopen("arvore.idx", "r+b");
        fseek(arqind, 0, SEEK_END);
        tamanhoArquivo = ftell(arqind);
        fclose(arqind);

        if(tamanhoArquivo == sizeof(int)){
            PAGINA pagNova;
            pagNova.numeroChaves = 1;
            pagNova.chaves[0][0] = reg.id;
            pagNova.chaves[0][1] = offSet;
            pagNova.folha = 1;
            for(i = 0; i < ORDEM; i++){
                pagNova.filhos[i] = -1;
            }

            RRNraiz = 0;
            arqind = fopen("arvore.idx", "ab");
            fwrite(&RRNraiz, sizeof(RRNraiz), 1, arqind);
            fwrite(&pagNova, sizeof(pagNova), 1, arqind);
            fclose(arqind);
        }
        else{
            int flag = 1; //variável auxiliar para servir como uma verificação se é necessário inserir a chave
            int passouPorSplit = 0;
            int RRNnovaPagSplit = -1;//pois o split inicial ocorrerá na folha
            inserir(&reg.id, &offSet, RRNraiz, &RRNraiz, &flag, &passouPorSplit, &RRNnovaPagSplit);
        }

        offSet += sizeof(size)+size;
    }


    char mensagem[] = "Execucao da criacao do arquivo de indice 'arvore.idx' com base no arquivo de dados 'dados.dad'.\n";
    atualizaArquivoDeLog("log_X.txt", mensagem);
    fclose (arqdados);
    fclose (arqind);
}

//Insere as musicas no arquivo de dados.dad
void inserirMusica(tRegistro novoRegistro){
    char size;
    int i, pos;
    char buffer[1000];

    FILE *arquivo;
    arquivo = fopen ("dados.dad", "ab");
    if (arquivo == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return;
    }

    size = camp_var_reg_var(novoRegistro, buffer);
    fwrite(&size, sizeof(size), 1, arquivo);
    fwrite(buffer, size, 1, arquivo);

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
    sprintf(mensagem, "Execucao de operacao de PESQUISA de %d.\n", novoRegistro.id);
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
