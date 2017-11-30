#include <stdio.h>
#include <stdlib.h>

#define ORDEM 5

#define NAOENCONTRADO -2
#define ERRO -1
#define CRIOURAIZ 0
#define JAEXISTE 1
#define INSERIUNORMAL 2
#define INSERIUCOMSPLIT 3

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
    //define se � folha, folha = 1, n�o folha = 0
    int folha;
} PAGINA;

void inserirEmDisco(PAGINA atual){
    FILE *arqInd;
    arqInd = fopen(ARQIND, "ab");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo arvore.idx para escrever uma nova pagina. Erro 0x0001.\n");
        return;
    }

    fwrite(&atual, sizeof(PAGINA), 1, arqInd);

    fclose(arqInd);
}

int getNovoRRN(){
    FILE *arq;
    arq = fopen(ARQIND, "rb");
    if(arq == NULL){
        printf("Nao foi possivel abrir o arquivo arvore.idx para obter novo RRN. Erro 0x0002.\n");
        return ERRO;
    }

    fseek(arq, 0, SEEK_END);
    long tam = ftell(arq);
    fclose(arq);

    return (tam-sizeof(int))/sizeof(PAGINA);
}

void atualizarIndice(PAGINA *pag, int RRNpag)
{
    FILE *arqInd;
    arqInd = fopen(ARQIND, "r+b");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo arvore.idx para atualizar indice. Erro 0x0003.\n");
        return;
    }

    fseek(arqInd, 2*sizeof(int)+RRNpag*sizeof(PAGINA), SEEK_SET);
    fwrite(pag, sizeof(PAGINA), 1, arqInd);

    fclose(arqInd);
}

void atualizarRaiz(int RRNraiz){
    FILE *arqInd;
    arqInd = fopen(ARQIND, "r+b");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo arvore.idx para atualizar raiz. Erro 0x0004.\n");
        return;
    }

    fseek(arqInd, sizeof(int), SEEK_SET);
    fwrite(&RRNraiz, sizeof(RRNraiz), 1, arqInd);

    fclose(arqInd);
}

//Fun��o para encontrar o offSet do id buscado. Come�a da raiz.
int busca(int id, int RRNatual){
    //Contador
    int i;
    //Abrir arquivo de �ndice para busca
    FILE *arq = fopen(ARQIND, "rb");
    //Se n�o abriu o arquivo,
    if(arq == NULL){
        return ERRO;
    }

    //Struct para pegar a p�gina do arquivo de �ndices
    PAGINA buscaPag;

    //Percorrimento no arquivo de �ndice para pegar a p�gina em disco, e logo fecha o arquivo
    fseek(arq, 2*sizeof(int) + RRNatual*sizeof(buscaPag), SEEK_SET);
    fread(&buscaPag, sizeof(buscaPag), 1, arq);
    fclose(arq);

    //Busca pela chave na p�gina atual
    for(i = 0; i < buscaPag.numeroChaves && id >= buscaPag.chaves[i][0]; i++){
        //Se o id estiver na p�gina,
        if(buscaPag.chaves[i][0] == id){
            //Retorna o offSet do id
            printf("Funcao busca - id %d encontrado com offSet %d\n", id, buscaPag.chaves[i][1]);
            return buscaPag.chaves[i][1];
        }
        //sen�o, continua...
    }
    //Aqui, i � a posi��o que id "deveria estar". Ir para o filho i, se existir.

    if(buscaPag.filhos[i] != -1){
        return busca(id, buscaPag.filhos[i]);
    }
    //Se n�o existir filho, ent�o a chave n�o existe na �rvore.
    else{
        return NAOENCONTRADO;
    }
}

//Fun��o para split
void split(PAGINA *pag, int *idInserir, int *offSetInserir, int *RRNraiz, int RRNpagAtual, int *RRNnovaPagSplit) //falta pegarmos o RRN da raiz
{
    PAGINA pagSplit;
    int chaves[ORDEM][2];
    int filhos[ORDEM+1];
    int i, j;
    int flag;

    //atribui��o das informa��es das chaves da p�gina atual ao vetor auxiliar de chaves
    for (i = 0; i < pag->numeroChaves; i++)
    {
        chaves[i][0] = pag->chaves[i][0];
        chaves[i][1] = pag->chaves[i][1];
        filhos[i] = pag->filhos[i];
    }
    //Fazendo o mesmo para o �ltimo filho, que o for acima n�o trata
    filhos[i] = pag->filhos[i];

    //Verifica��o do local de inser��o da chave no vetor auxiliar de chaves
    for (i = 0; i < (ORDEM-1) && *idInserir > chaves[i][0]; i++)
    {
        //N�o precisa de c�digo aqui dentro...
    }


    int posColocar = i;

    //Inser��o ordenada
    //Shift dos elementos maiores que o a ser inserido
    for (i = (ORDEM-1) ; i > posColocar; i--)
    {
        chaves[i][0] = chaves[i-1][0];
        chaves[i][1] = chaves[i-1][1];
    }

    //Inser��o do novo elemento na posi��o correta
    chaves[posColocar][0] = *idInserir;
    chaves[posColocar][1] = *offSetInserir;

    //Inser��o do filho na posi��o correta
    //Realiza o deslocamento dos filhos
    for(i = ORDEM; i > (posColocar+1); i--)
    {
        filhos[i] = filhos[i-1];
    }

    //Neste ponto, ctrl = posColocar
    filhos[i] = (*RRNnovaPagSplit);


    //i vai at� (ORDEM-1)/2 = 2 por ser o tamanho minimo de chaves numa pagina
    for(i = 0; i < (ORDEM-1)/2; i++){
        for(j = 0; j < 2; j++){
            pag->chaves[i][j] = chaves[i][j];
        }
    }
    pag->numeroChaves = 2;

    //Filhos da p�gina original(da esquerda)
    for(i = 0; i < (ORDEM-1)/2 + 1; i++){
        pag->filhos[i] = filhos[i];
    }
    
    /*
    pag->filhos[0] = filhos[0];
    pag->filhos[1] = filhos[1];
    pag->filhos[2] = filhos[2];
    */

    //para as demais chaves do n�, atribu�mos nulo(-1) tanto para a chave, quanto para seu offset
    for(i = (ORDEM-1)/2; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            pag->chaves[i][j] = -1;
        }
    }

    for(i = (ORDEM-1)/2 + 1; i < ORDEM-1; i++){
        pag->filhos[i] = -1;
    }

    /*
    pag->filhos[3] = -1;
    pag->filhos[4] = -1;
    */

    //colocamos na pagina auxiliar pagSplit a outra metade da pagina
    //k � o par�metro utilizado para pegar a outra metade dos filhos da p�gina
    //Para ORDEM = 5, k = 3.
    int k = (ORDEM-1)/2 + 1;
    for(i = 0; i < (ORDEM-1)/2; i++){
        for(j = 0; j < 2; j++){
            pagSplit.chaves[i][j] = chaves[i+k][j];//s� fazemos isso pra deixar o elemento central do vetor de chaves intacto, para ser usado na promo��o
        }
    }
    pagSplit.numeroChaves = 2;

    for(i = 0; i < (ORDEM-1)/2 + 1; i++){
        pagSplit.filhos[i] = filhos[i+k];
    }

    /*
    pagSplit.filhos[0] = filhos[3];
    pagSplit.filhos[1] = filhos[4];
    pagSplit.filhos[2] = filhos[5];
    */

    //para as demais chaves do n�, atribu�mos nulo(-1)
    for(i = (ORDEM-1)/2; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            pagSplit.chaves[i][j] = -1;
        }
    }

    pagSplit.filhos[3] = -1;
    pagSplit.filhos[4] = -1;

    //Se a p�gina splitada for folha, a nova p�gina ser� folha. Sen�o, n�o ser�.
    if(pag->folha == 1){
        pagSplit.folha = 1;
    }
    else{
        pagSplit.folha = 0;
    }


    //Pega o novo RRN para que ele "suba" na recurs�o
    //� feita verifica��o de se o arquivo foi aberto corretamente
    flag = getNovoRRN();
    if(flag != ERRO){
        *RRNnovaPagSplit = flag;
    }
    else{
        printf("Nao foi possivel obter novo RRN a partir do arquivo arvore.idx. Erro 0x0006.\n");
        return;
    }
    
    //Insere a nova p�gina no fim do arquivo de �ndices
    inserirEmDisco(pagSplit);
    //Atualiza a p�gina que sofreu split
    atualizarIndice(pag, RRNpagAtual);

    //print de teste, checar as chaves
    for(i = 0; i < ORDEM; i++){
        printf("chaves[%d][0] = %d\n", i, chaves[i][0]);
    }
    printf("chaves[ORDEM/2][0] = %d\n", chaves[ORDEM/2][0]);

    //para o caso ORDEM 5, chave 2, intermedi�ria, sobe para o antecessor ao n� atual
    (*idInserir) = chaves[ORDEM/2][0];
    (*offSetInserir) = chaves[ORDEM/2][1];
    printf("idInserir = %d, offSetInserir = %d\n", *idInserir, *offSetInserir);

    //se � raiz
    if (RRNpagAtual == (*RRNraiz))
    {
        PAGINA novaRaiz;
        //Nova raiz recebe chave promovida
        //Seus filhos s�o a raiz original(pag), � esquerda
        //e a nova p�gina criada para split(pagSplit), � direita
        novaRaiz.chaves[0][0] = *idInserir;
        novaRaiz.chaves[0][1] = *offSetInserir;
        novaRaiz.filhos[0] = RRNpagAtual;
        novaRaiz.filhos[1] = *RRNnovaPagSplit;
        novaRaiz.numeroChaves = 1;

        //O resto das chaves, offsets e filhos ser�o -1.
        for(i = 1; i < ORDEM-1; i++){
            for(j = 0; j < 2; j++){
                novaRaiz.chaves[i][j] = -1;
            }
        }

        for(i = 2; i < ORDEM; i++){
            novaRaiz.filhos[i] = -1;
        }

        //A raiz nunca ser� folha ap�s um split.
        novaRaiz.folha = 0;

        //Atualiza nova raiz
        //Verifica��o se foi poss�vel obter novo RRN
        flag = getNovoRRN();
        if(flag != ERRO){
            *RRNraiz = flag;
        }
        else{
            printf("Nao foi possivel obter novo RRN a partir do arquivo arvore.idx. Erro 0x0007.\n");
            return;
        }

        //Insere a nova raiz no fim do arquivo de �ndices
        inserirEmDisco(novaRaiz);
        //Precisa atualizar o arquivo de �ndice para marcar o RRN da nova raiz.
        atualizarRaiz(*RRNraiz);

        //A linha abaixo n�o sei se � necess�ria, porque estaremos na �ltima volta da recurs�o/primeira recurs�o
        //E, assim, n�o dar� problema na fun��o de inserir... Mas deixei aqui pra lembramos de discutir isso
        //(*precisaInserir) = 0; //pois chegou na raiz. Logo, n�o h� mais p�ginas acima
    }

    return;
}


//Fun��o de inser��o da chave "id" com offset "offSet" na �rvore B. RRNatual come�a sendo o da raiz.
void inserir(int *id, int *offSet, int RRNatual, int *RRNraiz, int *precisaInserir, int *passouPorSplit, int *RRNnovaPagSplit){
    int i, j;
    int chaveAux[2];
    PAGINA pagAtual;

    FILE *arqInd;
    arqInd = fopen(ARQIND, "r+b");
    if(arqInd == NULL){
        printf("Nao foi possivel abrir o arquivo arvore.idx para inserir chave. Erro 0x0005.\n");
        return;
    }

    fseek(arqInd, 2*sizeof(int)+RRNatual*sizeof(PAGINA), SEEK_SET);
    fread(&pagAtual, sizeof(PAGINA), 1, arqInd);

    fclose(arqInd);

    //Se n�o � folha, prossegue com a busca
    if(pagAtual.folha == 0){
        //recurs�o para o filho correspondente � chave a ser inserida
        inserir(id, offSet, pagAtual.filhos[i], RRNraiz, precisaInserir, passouPorSplit, RRNnovaPagSplit);
    }

    if(*precisaInserir)
    {
        if (pagAtual.numeroChaves >= (ORDEM-1))
        {
            //N� est� cheio, � necess�rio realizar um split
            split(&pagAtual, id, offSet, RRNraiz, RRNatual, RRNnovaPagSplit);//Lembrar de fazer cabe�alho com a raiz no arquivo
            (*passouPorSplit) = 1;
        }
        else
        {
            for (i = 0; i < pagAtual.numeroChaves && *id > pagAtual.chaves[i][0]; i++)
            {
                //N�o precisa de c�digo aqui dentro...
            }

            int posColocar = i;
            printf("posColocar = %d\n", posColocar);
            printf("id = %d, offSet = %d\n", *id, *offSet);

            //Inser��o ordenada
            //Shift dos elementos maiores que o a ser inserido
            for (i = pagAtual.numeroChaves; i > posColocar; i--)
            {
                pagAtual.chaves[i][0] = pagAtual.chaves[i-1][0];
                pagAtual.chaves[i][1] = pagAtual.chaves[i-1][1];
            }

            //Inser��o do novo elemento na posi��o correta
            pagAtual.chaves[posColocar][0] = *id;
            pagAtual.chaves[posColocar][1] = *offSet;
            pagAtual.numeroChaves++;

            //Inser��o do filho na posi��o correta, caso venha de um split
            if((*passouPorSplit) == 1)
            {
                int i;
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