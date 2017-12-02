#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD_arvore_B.h"

#define ARQDADOS "dados.dad"

//###############################
//#   FUNÇÕES PARA MANIPULAR O  #
//#   ARQUIVO DE DADOS DE ACOR- #
//#   DO COM O PDF.             #
//###############################

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

//Função que divide o registro com as barras | e atribui para um vetor
int camp_var_reg_var(tRegistro e, char *buffer) {
     sprintf(buffer, "|%d|%s|%s|", e.id, e.titulo, e.genero);
     return strlen(buffer);
}


//Função que identifica as posições que possuem | e removem o caracter
char *parser(char *buffer, int *pos) {
    int posi = *pos;

    while(buffer[*pos] != '|')
        (*pos)++;

    buffer[*pos] = '\0';
    (*pos)++;
    return &buffer[posi];
}

// vai pegar as informações do arquivo de dados para criar a partir dele o arquivo de indices
void criarArquivoDeIndice(int modoLog){
    //Abrir os arquivos de dados e índice, o primeiro como leitura e o segundo como escrita
    FILE *arqdados, *arqind;
    arqdados = fopen (ARQDADOS, "rb");
    arqind = fopen (ARQIND, "wb");

    //Se não deu para abrir um deles
    if (arqdados == NULL) {
       printf ("Houve um erro ao abrir o arquivo de dados %s. Erro 0x1000.\n", ARQDADOS);
       return;
    }
    if(arqind == NULL){
        printf ("Houve um erro ao abrir o arquivo de indices %s. Erro 0x1001.\n", ARQIND);
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

            inserir(&reg.id, &offSetAux, RRNraiz, &RRNraiz, &flag, &passouPorSplit, &RRNnovaPagSplit, FUNC_1);
        }

        //offSet pula o tamanho do registro para ir para o próximo
        offSet += sizeof(size)+size;

        //Como offSet, agora, é o offSet do próximo elemento a ser inserido, registramos ele
        //no cabeçalho do arquivo de índices
        atualizaOffSetCabecalho(offSet);
    }

    //Operação de escrever no log
    //Somente escreve no log se for feita a operação 1.
    if(modoLog == FUNC_1){
        char *mensagem = malloc(100*sizeof(char));
        sprintf(mensagem, "Execucao da criacao do arquivo de indice %s com base no arquivo de dados %s.\n", ARQIND, ARQDADOS);
        atualizaArquivoDeLog(mensagem, LOG_MSG_ON);
        free(mensagem);
    }

    //Fecha arquivos de dados, pois o de índice já foi fechado antes
    fclose (arqdados);
}

//Insere as musicas no arquivo dados.dad
void inserirMusica(tRegistro novoRegistro){
    char size;
    int i, j, pos;
    char buffer[1000];
    char *mensagem = malloc(100*sizeof(char));
    int RRNraiz;
    int offsetNovaChave;
    int flagBusca;
    long tamArq;

    //Essa parte escreve o que foi feito no arquivo de log: inserção de um elemento
    sprintf(mensagem, "Execucao de operacao de INSERCAO de %d, %s, %s.\n", novoRegistro.id, novoRegistro.titulo, novoRegistro.genero);
    atualizaArquivoDeLog(mensagem, LOG_MSG_ON);

    //Abrimos o arquivo de dados e o de indices para escrever o novo dado
    FILE *arqdados, *arqind;
    //Abrimos como append para escrever no fim
    arqdados = fopen (ARQDADOS, "ab");
    if (arqdados == NULL) {
       printf ("Houve um erro ao abrir o arquivo de dados %s. Erro 0x1002.\n", ARQDADOS);
       return;
    }

    //Abrimos como r+ para começarmos no início do arquivo e pegarmos o RRN da raiz para a posterior busca
    //e o offset da nova chave a ser inserida
    arqind = fopen(ARQIND, "r+b");
    if(arqind == NULL){
        printf("Houve um erro ao abrir o arquivo de indice %s. Erro 0x1003.\n", ARQIND);
        return;
    }

    //Vamos até o fim do arquivo para ver o tamanho do arquivo
    fseek(arqind, 0, SEEK_END);
    tamArq = ftell(arqind);

    //E já voltamos para o começo do arquivo, pulando o bit de atualizado
    fseek(arqind, sizeof(int), SEEK_SET);

    //Se estiver vazio, ou seja, só tiver o bit de atualizado,
    if(tamArq == sizeof(int)){
        //Temos nossa primeira raiz, e já inserimos o RRN raiz e o offset da nova chave no cabeçalho
        RRNraiz = 0;
        offsetNovaChave = 0;

        //Inserimos uma página vazia
        PAGINA novaPag;
        //Para não dar erro, enchemos ela com -1
        for(i = 0; i < ORDEM-1; i++){
            for(j = 0; j < 2; j++){
                novaPag.chaves[i][j] = -1;
            }
        }
        for(i = 0; i < ORDEM; i++){
            novaPag.filhos[i] = -1;
        }
        novaPag.numeroChaves = 0;
        novaPag.folha = 1;

        //Inserimos as informações no arquivo de índices
        fwrite(&RRNraiz, sizeof(RRNraiz), 1, arqind);
        fwrite(&offsetNovaChave, sizeof(offsetNovaChave), 1, arqind);
        fwrite(&novaPag, sizeof(novaPag), 1, arqind);
    }
    //Se não estiver vazio:
    else{
        //Pegamos o RRN da raiz e o offset da proxima chave a ser inserida
        fread(&RRNraiz, sizeof(RRNraiz), 1, arqind);
        fread(&offsetNovaChave, sizeof(offsetNovaChave), 1, arqind);
    }
    //Fechamos o arquivo pois não precisamos mais dele nessa função
    fclose(arqind);

    //Buscamos o id passado a partir da raiz
    flagBusca = busca(novoRegistro.id, RRNraiz);

    //Se deu erro, informamos que deu erro
    if(flagBusca == ERRO){
        printf("Ocorreu erro ao buscar o id no arquivo de indice %s. Erro 0x1004.\n", ARQIND);
    }
    //Se não encontramos, inserimos normalmente
    else if(flagBusca == NAOENCONTRADO){
        //Inserindo no aquivo de dados
        size = camp_var_reg_var(novoRegistro, buffer);
        fwrite(&size, sizeof(size), 1, arqdados);
        fwrite(buffer, size, 1, arqdados);

        //Inserindo no arquivo de índice
        int flag = 1; //variável auxiliar para servir como uma verificação se é necessário inserir a chave
        int passouPorSplit = 0; //variável auxiliar que avisará se houve um split
        int RRNnovaPagSplit = -1; //variável para retornar o RRN da página splitada, e é igual a -1
                                  //pois o split inicial ocorrerá na folha
        int offSetAux = offsetNovaChave; //atribuimos o offset para uma variável auxiliar para não perder o valor
        inserir(&novoRegistro.id, &offSetAux, RRNraiz, &RRNraiz, &flag, &passouPorSplit, &RRNnovaPagSplit, FUNC_2);
        
        //Atualizamos o offset do cabeçalho para futura inserção
        offsetNovaChave += sizeof(size)+size;
        atualizaOffSetCabecalho(offsetNovaChave);

        //Parte destinada para o log
        sprintf(mensagem, "Chave %d inserida com sucesso.\n", novoRegistro.id);
        atualizaArquivoDeLog(mensagem, LOG_MSG_ON);

        printf ("Arquivo %s e %s atualizados com sucesso.\n", ARQDADOS, ARQIND);
    }
    //Se encontrou
    else{
        printf("O id ja existe! Nao houve insercao da chave no arquivo de indices %s. Erro 0x1005.\n", ARQIND);
        //Parte destinada para o log
        sprintf(mensagem, "Chave %d duplicada.\n", novoRegistro.id);
        atualizaArquivoDeLog(mensagem, LOG_MSG_ON);
    }

    fclose (arqdados);
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
        printf("Erro ao abrir o arquivo de indice %s. Erro 0x1006.\n", ARQIND);
        return;
    }

    //Já escrevemos no arquivo de log que estamos na operação de busca.
    mensagem = malloc(100*sizeof(char));
    sprintf(mensagem, "Execucao de operacao de PESQUISA de %d.\n", idBusca);
    atualizaArquivoDeLog(mensagem, LOG_MSG_OFF);

    //Pega o RRN raiz.
    fseek(arq, sizeof(int), SEEK_SET);
    fread(&RRNraiz, sizeof(RRNraiz), 1, arq);
    fclose(arq);

    //É feita a busca pelo offSet do id.
    offSet = busca(idBusca, RRNraiz);

    //Se deu erro,
    if(offSet == ERRO){
        printf("Nao foi possivel abrir o arquivo %s para buscar o id %d. Erro 0x1007.\n", ARQIND, idBusca);
        return;
    }
    //Se não encontrou,
    else if(offSet == NAOENCONTRADO){
        printf("O id %d nao foi encontrado.\n", idBusca);
        sprintf(mensagem, "Chave %d nao encontrada.\n", idBusca);
        atualizaArquivoDeLog(mensagem, LOG_MSG_ON);
        free(mensagem);
        return;
    }

    //Abre o arquivo de dados para obter o registro pesquisado.
    arq = fopen(ARQDADOS, "r");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo de dados %s. Erro 0x1008.\n", ARQDADOS);
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

    //Escreve o que foi feito no arquivo de log
    sprintf(mensagem, "Chave %d encontrada, offset %d,\n"
                      "Titulo: %s, Genero: %s.\n",
                      reg.id, offSet, reg.titulo, reg.genero);
    atualizaArquivoDeLog(mensagem, LOG_MSG_ON);
    free(mensagem);

    return;
}
