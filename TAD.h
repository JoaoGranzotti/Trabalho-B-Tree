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

//Verifica se arquivo de dados existe, para a criação do arquivo de dados
int existeArqDados()
{
    FILE *arquivo;
    arquivo = fopen (ARQDADOS, "r");
    if (arquivo == NULL) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

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
void criarArquivoDeIndice(){
    //Abrir os arquivos de dados e índice, o primeiro como leitura e o segundo como escrita
    FILE *arqdados, *arqind;
    arqdados = fopen (ARQDADOS, "rb");
    arqind = fopen (ARQIND, "wb");

    //Se não deu para abrir um deles
    if (arqdados == NULL) {
       printf ("Houve um erro ao abrir o arquivo de dados.\n");
       return;
    }
    if(arqind == NULL){
        printf ("Houve um erro ao abrir o arquivo de indices.\n");
        return;
    }

    //Variáveis a serem utilizadas
    int i, j; //Contadores
    int atualizado = 1; //Variável para dizer que o arquivo de índice está atualizado
    int RRNraiz; //Variável para guardar o RRN da página raiz
    int offSet = 0; //Variável para guardar o offset do próximo registro a ser inserido
    int pos; //Variável auxiliar para ajudar na leitura do registro
    long tamanhoArquivo; //Variável para obter o tamanho de um arquivo
    char size; //Variável para pegar o tamanho de um registro
    char buffer[1000]; //Vetor para pegar o conteúdo de um registro
    tRegistro reg; //Registro auxiliar

    //É escrito que o arquivo está atualizado
    fwrite(&atualizado, sizeof(int), 1, arqind);
    fclose(arqind);

    //Lendo o arquivo de dados, registro a registro
    while (fread(&size, sizeof(size), 1, arqdados)) {
        fread(buffer, size, 1, arqdados);
        pos = 1;
        sscanf(parser(buffer, &pos), "%d", &reg.id);
        strcpy(reg.titulo, parser(buffer, &pos));
        strcpy(reg.genero, parser(buffer, &pos));

        //Pega o tamanho do arquivo de índice
        arqind = fopen(ARQIND, "r+b");
        fseek(arqind, 0, SEEK_END);
        tamanhoArquivo = ftell(arqind);
        fclose(arqind);

        //Se só tiver o bit de estar atualizado ou não
        if(tamanhoArquivo == sizeof(int)){
            //Será criada a raiz da árvore B
            PAGINA pagNova;

            //Ela tem o id a ser inserido com offSet = 0
            pagNova.numeroChaves = 1;
            pagNova.chaves[0][0] = reg.id;
            pagNova.chaves[0][1] = offSet;
            pagNova.folha = 1;

            //As outras chaves e todos os filhos recebem -1
            for(i = 1; i < ORDEM-1; i++){
                for(j = 0; j < 2; j++){
                    pagNova.chaves[i][j] = -1;
                }
            }
            for(i = 0; i < ORDEM; i++){
                pagNova.filhos[i] = -1;
            }

            //Como é raiz, RRN = 0
            RRNraiz = 0;

            //Abre arquivo de dados e grava o que falta: RRN da raiz, offSet do último inserido e a página em si
            arqind = fopen(ARQIND, "ab");
            fwrite(&RRNraiz, sizeof(RRNraiz), 1, arqind);
            fwrite(&offSet, sizeof(offSet), 1, arqind);
            fwrite(&pagNova, sizeof(pagNova), 1, arqind);
            fclose(arqind);
        }
        //Se não, já possui uma ou mais páginas no arquivo de índice
        else{
            int flag = 1; //variável auxiliar para servir como uma verificação se é necessário inserir a chave
            int passouPorSplit = 0; //variável auxiliar que avisará se houve um split
            int RRNnovaPagSplit = -1; //variável para retornar o RRN da página splitada, e é igual a -1
                                      //pois o split inicial ocorrerá na folha
            int offSetAux = offSet; //atribuimos o offset para uma variável auxiliar para não perder o valor

            inserir(&reg.id, &offSetAux, RRNraiz, &RRNraiz, &flag, &passouPorSplit, &RRNnovaPagSplit);
        }

        //offSet pula o tamanho do registro para ir para o próximo
        offSet += sizeof(size)+size;
    }

    //Operação de escrever no log
    char *mensagem = malloc(100*sizeof(char));
    sprintf(mensagem, "Execucao da criacao do arquivo de indice 'arvore.idx' com base no arquivo de dados '%s'.\n", "dados.dad");
    atualizaArquivoDeLog(mensagem, 1);
    free(mensagem);

    //Fecha arquivos de dados, pois o de índice já foi fechado antes
    fclose (arqdados);
}

void inserirMusicaInd(int id, int offSet){

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
