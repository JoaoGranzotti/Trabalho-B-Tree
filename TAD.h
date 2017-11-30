#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD_arvore_B.h"

#define ARQLOG "log_X.txt"
#define ARQDADOS "dados.dad"

//##############################
//#  FUNÇÕES PARA ADIMINISTRAR #
//#  O ARQUIVO DE LOG DE ACOR- #
//#  DO COM O PDF.             #
//##############################

//Abre o arquivo de log, ou o cria se ele não existir
void criarArquivoDeLog(){
    FILE *arquivo;
    arquivo = fopen(ARQLOG, "w");
    if (arquivo == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return;
    }
    fprintf(arquivo, "Historico das operacoes realizadas no programa:\n");
    printf ("Arquivo de 'log' criado com sucesso.\n");
    fclose (arquivo);
}

// coloca as ações no arquivo de log
void atualizaArquivoDeLog(char linhaAdicionada[], int att){
    FILE *arquivo;
    arquivo = fopen (ARQLOG, "a");
    if (arquivo == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return;
    }
    fprintf(arquivo, linhaAdicionada);
    if(att == 1)
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
void criarArquivoDeIndice(char nomeArqDados[]){
    FILE *arqdados, *arqind;
    arqdados = fopen (nomeArqDados, "rb");
    arqind = fopen (ARQIND, "wb");

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

        arqind = fopen(ARQIND, "r+b");
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
            arqind = fopen(ARQIND, "ab");
            fwrite(&RRNraiz, sizeof(RRNraiz), 1, arqind);
            fwrite(&pagNova, sizeof(pagNova), 1, arqind);
            fclose(arqind);
        }
        else{
            int flag = 1; //variável auxiliar para servir como uma verificação se é necessário inserir a chave
            int passouPorSplit = 0;
            int RRNnovaPagSplit = -1;//pois o split inicial ocorrerá na folha
            int offSetAux = offSet;
            inserir(&reg.id, &offSetAux, RRNraiz, &RRNraiz, &flag, &passouPorSplit, &RRNnovaPagSplit);
        }

        offSet += sizeof(size)+size;
    }


    char *mensagem = malloc(100*sizeof(char));
    sprintf(mensagem, "Execucao da criacao do arquivo de indice 'arvore.idx' com base no arquivo de dados '%s'.\n", nomeArq);
    atualizaArquivoDeLog(mensagem, 1);
    free(mensagem);
    fclose (arqdados);
    fclose (arqind);
}

//Insere as musicas no arquivo dados.dad
void inserirMusica(tRegistro novoRegistro){
    char size;
    int i, pos;
    char buffer[1000];

    FILE *arquivo;
    arquivo = fopen (ARQDADOS, "ab");
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
    char *mensagem = malloc(100*sizeof(char));
    sprintf(mensagem, "Execucao de operacao de INSERCAO de %d, %s, %s.\n", novoRegistro.id, novoRegistro.titulo, novoRegistro.genero);
    atualizaArquivoDeLog(mensagem, 1);
    free(mensagem);
}

//Função para pesquisar música dentro do arquivo de dados a partir do arquivo de índice.
void pesquisarMusica(int idBusca){
    //Variáveis
    FILE *arq;
    char size;
    char buffer[1000];
    char *mensagem;
    int RRNraiz;
    int offSet;
    int pos;
    tRegistro reg; 

    //Abre o arquivo de índice para obter o RRN raiz
    arq = fopen(ARQIND, "rb");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo de indice. Erro 0x1000.\n");
        return;
    }

    mensagem = malloc(100*sizeof(char));
    sprintf(mensagem, "Execucao de operacao de PESQUISA de %d\n", idBusca);
    atualizaArquivoDeLog(mensagem, 0);

    //Pega o RRN raiz.
    fseek(arq, sizeof(int), SEEK_SET);
    fread(&RRNraiz, sizeof(RRNraiz), 1, arq);
    fclose(arq);

    offSet = busca(idBusca, RRNraiz);
    if(offSet == ERRO){
        printf("Nao foi possivel abrir o arquivo arvore.idx para buscar o id %d. Erro 0x1001.\n", idBusca);
        return;
    }
    else if(offSet == NAOENCONTRADO){
        printf("O id %d nao foi encontrado.\n", idBusca);
        sprintf(mensagem, "Chave %d nao encontrada.\n", idBusca);
        atualizaArquivoDeLog(mensagem, 1);
        free(mensagem);
        return;
    }

    //Abre o arquivo de dados para obter o registro pesquisado.
    arq = fopen(ARQDADOS, "r");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo de dados. Erro 0x1002.\n");
        return;
    }

    //Pega o registro pesquisado.
    fseek(arq, offSet, SEEK_SET);

    fread(&size, sizeof(size), 1, arq);
    fread(buffer, size, 1, arq);
    pos = 1;
    sscanf(parser(buffer, &pos), "%d", &reg.id);
    strcpy(reg.titulo, parser(buffer, &pos));
    strcpy(reg.genero, parser(buffer, &pos));

    printf("Registro encontrado:\nid = %d\ntitulo = %s\ngenero = %s\n", reg.id, reg.titulo, reg.genero);

    //Essa parte escreve o que foi feito no arquivo de log
    sprintf(mensagem, "Chave %d encontrada, offset %d,\n"
                      "Titulo: %s, Genero: %s\n",
                      reg.id, offSet, reg.titulo, reg.genero);
    atualizaArquivoDeLog(mensagem, 1);
    free(mensagem);

    return;
}

void removerMusica(int id_in){
    tRegistro novoRegistro;
    printf("ainda nao esta pronto");


    //Essa parte escreve o que foi feito no arquivo de log
    char *mensagem = malloc(2*sizeof(int));
    sprintf(mensagem, " %d", novoRegistro.id);
    atualizaArquivoDeLog(mensagem, 1);
    free(mensagem);
}

void mostrarArvore(){
    printf("ainda nao esta pronto");


    //Essa parte escreve o que foi feito no arquivo de log
    //sprintf(*mensagem, "%d, %s, %s.", novoRegistro.id, novoRegistro.titulo, novoRegistro.genero);
    //atualizaArquivoDeLog(mensagem, 1);
    return;
}
