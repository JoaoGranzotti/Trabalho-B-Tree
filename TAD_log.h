#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQLOG "log_X.txt"

#define LOG_MSG_ON 1
#define LOG_MSG_OFF 0

#define FUNC_0 -1
#define FUNC_1 0
#define FUNC_2 1

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
       printf ("Houve um erro ao abrir o arquivo de log %s. Erro 0x2000.\n", ARQLOG);
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
       printf ("Houve um erro ao abrir o arquivo de log %s. Erro 0x2001.\n", ARQLOG);
       return;
    }
    fprintf(arquivo, linhaAdicionada);
    if(att == LOG_MSG_ON)
        printf ("Arquivo de 'log' atualizado com sucesso.\n");

    fclose (arquivo);
}
