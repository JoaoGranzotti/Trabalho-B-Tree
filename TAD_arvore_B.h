#include <stdio.h>
#include <stdlib.h>
#include "TAD_log.h"

#define ORDEM 5

#define NAOENCONTRADO -2
#define ERRO -1

#define ARQIND "arvore.idx"

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

//Função auxiliar que atualiza o offset do cabeçalho do arquivo de índices
void atualizaOffSetCabecalho(int offSet){
    //Aqui, se refere ao arquivo de índices
    FILE *arqind = fopen(ARQIND, "r+b");

    //Pulamos o bit de atualizado e o RRN raiz
    fseek(arqind, 2*sizeof(int), SEEK_SET);
    fwrite(&offSet, sizeof(offSet), 1, arqind);

    fclose(arqind);
}

//Função para inserir uma página no fim do arquivo de índice
void inserirEmDisco(PAGINA atual){
    FILE *arqInd;
    arqInd = fopen(ARQIND, "ab");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo %s para escrever uma nova pagina. Erro 0x0001.\n", ARQIND);
        return;
    }

    fwrite(&atual, sizeof(PAGINA), 1, arqInd);

    fclose(arqInd);
}

//Função que obtém o RRN de uma nova página a ser inserida no fim do arquivo
int getNovoRRN(){
    FILE *arq;
    arq = fopen(ARQIND, "rb");
    if(arq == NULL){
        printf("Nao foi possivel abrir o arquivo %s para obter novo RRN. Erro 0x0002.\n", ARQIND);
        return ERRO;
    }

    fseek(arq, 0, SEEK_END);
    long tam = ftell(arq);
    fclose(arq);

    //O RRN novo é o tamanho do arquivo, tirando as flags iniciais, dividido
    //pelo tamanho de cada página.
    return (tam-3*sizeof(int))/sizeof(PAGINA);
}

//Função que atualiza uma página já existente no arquivo de índice
void atualizarIndice(PAGINA *pag, int RRNpag)
{
    FILE *arqInd;
    arqInd = fopen(ARQIND, "r+b");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo %s para atualizar indice. Erro 0x0003.\n", ARQIND);
        return;
    }

    fseek(arqInd, 3*sizeof(int)+RRNpag*sizeof(PAGINA), SEEK_SET);
    fwrite(pag, sizeof(PAGINA), 1, arqInd);

    fclose(arqInd);
}

//Função para atualizar o RRN da raiz. Utilizada quando há split na página raiz.
void atualizarRaiz(int RRNraiz){
    FILE *arqInd;
    arqInd = fopen(ARQIND, "r+b");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo %s para atualizar raiz. Erro 0x0004.\n", ARQIND);
        return;
    }

    //É pulado um int pois ele se refere ao bit de atualizado.
    fseek(arqInd, sizeof(int), SEEK_SET);
    fwrite(&RRNraiz, sizeof(RRNraiz), 1, arqInd);

    fclose(arqInd);
}

//Função para encontrar o offSet do id buscado. Começa da raiz.
int busca(int id, int RRNatual){
    //Contador
    int i;
    //Abrir arquivo de índice para busca
    FILE *arq = fopen(ARQIND, "rb");
    //Se não abriu o arquivo,
    if(arq == NULL){
        return ERRO;
    }

    //Struct para pegar a página do arquivo de índices
    PAGINA buscaPag;

    //Percorrimento no arquivo de índice para pegar a página em disco, e logo fecha o arquivo
    fseek(arq, 3*sizeof(int) + RRNatual*sizeof(buscaPag), SEEK_SET);
    fread(&buscaPag, sizeof(buscaPag), 1, arq);
    fclose(arq);

    //Busca pela chave na página atual
    for(i = 0; i < buscaPag.numeroChaves && id >= buscaPag.chaves[i][0]; i++){
        //Se o id estiver na página,
        if(buscaPag.chaves[i][0] == id){
            //Retorna o offSet do id
            printf("Funcao busca - id %d encontrado com offSet %d\n", id, buscaPag.chaves[i][1]);
            return buscaPag.chaves[i][1];
        }
        //senão, continua...
    }
    //Aqui, i é a posição que id "deveria estar". Ir para o filho i, se existir.

    if(buscaPag.filhos[i] != -1){
        return busca(id, buscaPag.filhos[i]);
    }
    //Se não existir filho, então a chave não existe na árvore.
    else{
        return NAOENCONTRADO;
    }
}

//Função para split
void split(PAGINA *pag, int *idInserir, int *offSetInserir, int *RRNraiz, int RRNpagAtual, int *RRNnovaPagSplit, int modoLog)
{
    PAGINA pagSplit;
    int chaves[ORDEM][2];
    int filhos[ORDEM+1];
    int i, j;
    int flag;

    printf("\nNumero chaves: %d\n", pag->numeroChaves);
    for(i = 0; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            printf("pag->chaves[%d][%d] = %d\n", i, j, pag->chaves[i][j]);
        }
    }
    for(i = 0; i < ORDEM; i++){
        printf("pag->filhos[%d] = %d\n", i, pag->filhos[i]);
    }
    printf("Folha: %d\n\n", pag->folha);

    //Posição da chave que será promovida. Válido para qualquer ordem.
    int posPromo = ORDEM/2;

    //atribuição das informações das chaves da página atual ao vetor auxiliar de chaves
    for (i = 0; i < ORDEM-1; i++)
    {
        chaves[i][0] = pag->chaves[i][0];
        printf("\npag->chaves[%d][0] = %d\n", i, pag->chaves[i][0]);
        printf("chaves[%d][0] = %d\n\n", i, chaves[i][0]);
        chaves[i][1] = pag->chaves[i][1];
        filhos[i] = pag->filhos[i];
    }
    //Fazendo o mesmo para o último filho, que o for acima não trata
    filhos[i] = pag->filhos[i];

    //Verificação do local de inserção da chave no vetor auxiliar de chaves
    for (i = 0; i < ORDEM && *idInserir > chaves[i][0]; i++)
    {
        //Não precisa de código aqui dentro...
    }

    int posColocar;

    //Caso *idInserir seja maior que todas as chaves, chegaremos numa posição inválida.
    //O if abaixo trata isso. Se é a maior chave, posColocar é a última posição válida
    if(i == ORDEM)
        posColocar = ORDEM-1;
    else
        posColocar = i;
    printf("posColocar no split = %d\n\n", posColocar);

    //Inserção ordenada
    //Shift dos elementos maiores que o a ser inserido
    for (i = ORDEM-1; i > posColocar; i--)
    {
        chaves[i][0] = chaves[i-1][0];
        chaves[i][1] = chaves[i-1][1];
    }

    //Inserção do novo elemento na posição correta
    chaves[posColocar][0] = *idInserir;
    chaves[posColocar][1] = *offSetInserir;

    //Inserção do filho na posição correta
    //Realiza o deslocamento dos filhos
    for(i = ORDEM; i > (posColocar+1); i--)
    {
        filhos[i] = filhos[i-1];
    }

    //Neste ponto, i = posColocar
    filhos[i] = (*RRNnovaPagSplit);


    //i vai até posPromo, pois todas as chaves antes dessa posição são da página antiga
    for(i = 0; i < posPromo; i++){
        for(j = 0; j < 2; j++){
            pag->chaves[i][j] = chaves[i][j];
        }
    }
    //Coincidentemente, o número de chaves na página antiga é igual a posPromo também,
    //pelo cálculo ser o mesmo
    pag->numeroChaves = posPromo;

    //Filhos da página original serão os filhos do vetor auxiliar até a posição posPromo
    for(i = 0; i <= posPromo; i++){
        pag->filhos[i] = filhos[i];
    }

    //para as demais chaves do nó, atribuímos nulo(-1) tanto para a chave, quanto para seu offset
    for(i = posPromo; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            pag->chaves[i][j] = -1;
        }
    }

    //e para os filhos também
    for(i = posPromo+1; i < ORDEM; i++){
        pag->filhos[i] = -1;
    }

    //colocamos na pagina auxiliar pagSplit a outra metade da pagina
    //k é o parâmetro utilizado para pegar a outra metade dos filhos da página
    //Para ORDEM = 5, k = 3.
    int k = ORDEM/2 + 1;

    //Atribuindo as chaves para a página nova advinda do split
    for(i = 0; i+k < ORDEM; i++){
        for(j = 0; j < 2; j++){
            pagSplit.chaves[i][j] = chaves[i+k][j];
            //só fazemos isso pra deixar o elemento central do vetor de chaves intacto, para ser usado na promoção
        }
    }

    //O número de chaves da página nova advinda do split é:
    pagSplit.numeroChaves = (ORDEM-1)/2;

    //Os filhos de pagSplit terão o mesmo deslocamento k e irão com i até posPromo
    for(i = 0; i+k <= ORDEM; i++){
        pagSplit.filhos[i] = filhos[i+k];
    }

    //para as demais chaves do nó, atribuímos nulo(-1)
    for(i = ORDEM - k; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            pagSplit.chaves[i][j] = -1;
        }
    }

    //e para os filhos também
    for(i = ORDEM - k + 1; i < ORDEM; i++){
        pagSplit.filhos[i] = -1;
    }

    //Se a página splitada for folha, a nova página será folha. Senão, não será.
    if(pag->folha == 1){
        pagSplit.folha = 1;
    }
    else{
        pagSplit.folha = 0;
    }

    //Pega o novo RRN para que ele "suba" na recursão
    //É feita verificação de se o arquivo foi aberto corretamente
    flag = getNovoRRN();
    if(flag != ERRO){
        *RRNnovaPagSplit = flag;
    }
    else{
        printf("Nao foi possivel obter novo RRN a partir do arquivo %s. Erro 0x0005.\n", ARQIND);
        return;
    }

    //Insere a nova página no fim do arquivo de índices
    inserirEmDisco(pagSplit);
    //Atualiza a página que sofreu split
    atualizarIndice(pag, RRNpagAtual);

    //print de teste, checar as chaves
    for(i = 0; i < ORDEM; i++){
        printf("chaves[%d][0] = %d\n", i, chaves[i][0]);
    }
    printf("ORDEM/2 = %d\n", ORDEM/2);
    printf("chaves[ORDEM/2][0] = %d\n", chaves[ORDEM/2][0]);

    //para o caso ORDEM 5, chave 2, intermediária, sobe para o antecessor ao nó atual
    (*idInserir) = chaves[ORDEM/2][0];
    (*offSetInserir) = chaves[ORDEM/2][1];

    //Como houve chave promovida, devemos listar isso no arquivo de log
    //Se estivermos na funcionalidade 2, colocamos no arquivo de log
    if(modoLog == FUNC_2){
        char *mensagem = malloc(50*sizeof(char));
        sprintf(mensagem, "Chave %d promovida.\n", *idInserir);
        atualizaArquivoDeLog(mensagem, LOG_MSG_OFF);
        free(mensagem);
    }

    printf("idInserir = %d, offSetInserir = %d\n", *idInserir, *offSetInserir);

    printf("\nPag antiga:");
    printf("\nNumero chaves: %d\n", pag->numeroChaves);
    for(i = 0; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            printf("pag->chaves[%d][%d] = %d\n", i, j, pag->chaves[i][j]);
        }
    }
    for(i = 0; i < ORDEM; i++){
        printf("pag->filhos[%d] = %d\n", i, pag->filhos[i]);
    }
    printf("Folha: %d\n\n", pag->folha);

    printf("PagSplit:");
    printf("\nNumero chaves: %d\n", pagSplit.numeroChaves);
    for(i = 0; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            printf("pag->chaves[%d][%d] = %d\n", i, j, pagSplit.chaves[i][j]);
        }
    }
    for(i = 0; i < ORDEM; i++){
        printf("pag->filhos[%d] = %d\n", i, pagSplit.filhos[i]);
    }
    printf("Folha: %d\n\n", pagSplit.folha);

    //se é raiz
    if (RRNpagAtual == (*RRNraiz))
    {
        PAGINA novaRaiz;
        //Nova raiz recebe chave promovida
        //Seus filhos são a raiz original(pag), à esquerda
        //e a nova página criada para split(pagSplit), à direita
        novaRaiz.chaves[0][0] = *idInserir;
        novaRaiz.chaves[0][1] = *offSetInserir;
        novaRaiz.filhos[0] = RRNpagAtual;
        novaRaiz.filhos[1] = *RRNnovaPagSplit;
        novaRaiz.numeroChaves = 1;

        //O resto das chaves, offsets e filhos serão -1.
        for(i = 1; i < ORDEM-1; i++){
            for(j = 0; j < 2; j++){
                novaRaiz.chaves[i][j] = -1;
            }
        }

        for(i = 2; i < ORDEM; i++){
            novaRaiz.filhos[i] = -1;
        }

        //A raiz nunca será folha após um split.
        novaRaiz.folha = 0;

        //Atualiza nova raiz
        //Verificação se foi possível obter novo RRN
        flag = getNovoRRN();
        if(flag != ERRO){
            *RRNraiz = flag;
        }
        else{
            printf("Nao foi possivel obter novo RRN a partir do arquivo %s. Erro 0x0006.\n", ARQIND);
            return;
        }

        //Insere a nova raiz no fim do arquivo de índices
        inserirEmDisco(novaRaiz);
        //Precisa atualizar o arquivo de índice para marcar o RRN da nova raiz.
        atualizarRaiz(*RRNraiz);
    }

    return;
}


//Função de inserção da chave "id" com offset "offSet" na Árvore B. RRNatual começa sendo o da raiz.
void inserir(int *id, int *offSet, int RRNatual, int *RRNraiz, int *precisaInserir, int *passouPorSplit, int *RRNnovaPagSplit, int modoLog){
    int i, j;
    int chaveAux[2];
    PAGINA pagAtual;

    FILE *arqInd;
    arqInd = fopen(ARQIND, "r+b");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo %s para inserir chave. Erro 0x0007.\n", ARQIND);
        return;
    }

    fseek(arqInd, 3*sizeof(int)+RRNatual*sizeof(PAGINA), SEEK_SET);
    fread(&pagAtual, sizeof(PAGINA), 1, arqInd);

    fclose(arqInd);

    //Se não é folha, prossegue com a busca
    if(pagAtual.folha == 0){
        //Busca pela posição que deveria estar
        for(i = 0; i < pagAtual.numeroChaves && *id >= pagAtual.chaves[i][0]; i++){
            //Não precisa de código. O for terminará com i na posição que queremos.
        }

        //recursão para o filho correspondente à chave a ser inserida
        inserir(id, offSet, pagAtual.filhos[i], RRNraiz, precisaInserir, passouPorSplit, RRNnovaPagSplit, modoLog);
    }

    if(*precisaInserir)
    {
        //Se a pagina está cheia, fazer split
        if (pagAtual.numeroChaves >= (ORDEM-1))
        {
            //Parte destinada para o log
            //Se for a funcionalidade 2
            if(modoLog == FUNC_2){
                char *mensagem = malloc(30*sizeof(char));
                sprintf(mensagem, "Divisao de no - pagina %d\n", RRNatual);
                atualizaArquivoDeLog(mensagem, LOG_MSG_OFF);
                free(mensagem);
            }

            //Nó está cheio, é necessário realizar um split
            split(&pagAtual, id, offSet, RRNraiz, RRNatual, RRNnovaPagSplit, modoLog);
            (*passouPorSplit) = 1;
        }
        else
        {
            for (i = 0; i < pagAtual.numeroChaves && *id > pagAtual.chaves[i][0]; i++)
            {
                //Não precisa de código aqui dentro...
            }

            int posColocar = i;
            printf("eh aqui msm posColocar = %d\n", posColocar);
            printf("id = %d, offSet = %d\n", *id, *offSet);

            //Inserção ordenada
            //Shift dos elementos maiores que o a ser inserido
            for (i = pagAtual.numeroChaves; i > posColocar; i--)
            {
                pagAtual.chaves[i][0] = pagAtual.chaves[i-1][0];
                pagAtual.chaves[i][1] = pagAtual.chaves[i-1][1];
            }

            //Inserção do novo elemento na posição correta
            pagAtual.chaves[posColocar][0] = *id;
            pagAtual.chaves[posColocar][1] = *offSet;
            pagAtual.numeroChaves++;

            //Inserção do filho na posição correta, caso venha de um split
            if((*passouPorSplit) == 1)
            {
                //Realiza o deslocamento dos filhos
                for(i = pagAtual.numeroChaves; i > posColocar+1; i--)
                {
                    pagAtual.filhos[i] = pagAtual.filhos[i-1];
                }

                //Neste ponto, i = posColocar
                pagAtual.filhos[i] = (*RRNnovaPagSplit);

                (*passouPorSplit) = 0;
            }
            (*precisaInserir) = 0;
        }
    }

    atualizarIndice(&pagAtual, RRNatual);

    return;
}
