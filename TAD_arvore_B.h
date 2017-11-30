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
    //define se é folha, folha = 1, não folha = 0
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
    fseek(arq, 2*sizeof(int) + RRNatual*sizeof(buscaPag), SEEK_SET);
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
void split(PAGINA *pag, int *idInserir, int *offSetInserir, int *RRNraiz, int RRNpagAtual, int *RRNnovaPagSplit) //falta pegarmos o RRN da raiz
{
    PAGINA pagSplit;
    int chaves[ORDEM][2];
    int filhos[ORDEM+1];
    int i, j;
    int flag;

    //atribuição das informações das chaves da página atual ao vetor auxiliar de chaves
    for (i = 0; i < pag->numeroChaves; i++)
    {
        chaves[i][0] = pag->chaves[i][0];
        chaves[i][1] = pag->chaves[i][1];
        filhos[i] = pag->filhos[i];
    }
    //Fazendo o mesmo para o último filho, que o for acima não trata
    filhos[i] = pag->filhos[i];

    //Verificação do local de inserção da chave no vetor auxiliar de chaves
    for (i = 0; i < (ORDEM-1) && *idInserir > chaves[i][0]; i++)
    {
        //Não precisa de código aqui dentro...
    }


    int posColocar = i;

    //Inserção ordenada
    //Shift dos elementos maiores que o a ser inserido
    for (i = (ORDEM-1) ; i > posColocar; i--)
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

    //Neste ponto, ctrl = posColocar
    filhos[i] = (*RRNnovaPagSplit);


    //i vai até (ORDEM-1)/2 = 2 por ser o tamanho minimo de chaves numa pagina
    for(i = 0; i < (ORDEM-1)/2; i++){
        for(j = 0; j < 2; j++){
            pag->chaves[i][j] = chaves[i][j];
        }
    }
    pag->numeroChaves = 2;

    //Filhos da página original(da esquerda)
    for(i = 0; i < (ORDEM-1)/2 + 1; i++){
        pag->filhos[i] = filhos[i];
    }
    
    /*
    pag->filhos[0] = filhos[0];
    pag->filhos[1] = filhos[1];
    pag->filhos[2] = filhos[2];
    */

    //para as demais chaves do nó, atribuímos nulo(-1) tanto para a chave, quanto para seu offset
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
    //k é o parâmetro utilizado para pegar a outra metade dos filhos da página
    //Para ORDEM = 5, k = 3.
    int k = (ORDEM-1)/2 + 1;
    for(i = 0; i < (ORDEM-1)/2; i++){
        for(j = 0; j < 2; j++){
            pagSplit.chaves[i][j] = chaves[i+k][j];//só fazemos isso pra deixar o elemento central do vetor de chaves intacto, para ser usado na promoção
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

    //para as demais chaves do nó, atribuímos nulo(-1)
    for(i = (ORDEM-1)/2; i < ORDEM-1; i++){
        for(j = 0; j < 2; j++){
            pagSplit.chaves[i][j] = -1;
        }
    }

    pagSplit.filhos[3] = -1;
    pagSplit.filhos[4] = -1;

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
        printf("Nao foi possivel obter novo RRN a partir do arquivo arvore.idx. Erro 0x0006.\n");
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
    printf("chaves[ORDEM/2][0] = %d\n", chaves[ORDEM/2][0]);

    //para o caso ORDEM 5, chave 2, intermediária, sobe para o antecessor ao nó atual
    (*idInserir) = chaves[ORDEM/2][0];
    (*offSetInserir) = chaves[ORDEM/2][1];
    printf("idInserir = %d, offSetInserir = %d\n", *idInserir, *offSetInserir);

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
            printf("Nao foi possivel obter novo RRN a partir do arquivo arvore.idx. Erro 0x0007.\n");
            return;
        }

        //Insere a nova raiz no fim do arquivo de índices
        inserirEmDisco(novaRaiz);
        //Precisa atualizar o arquivo de índice para marcar o RRN da nova raiz.
        atualizarRaiz(*RRNraiz);

        //A linha abaixo não sei se é necessária, porque estaremos na última volta da recursão/primeira recursão
        //E, assim, não dará problema na função de inserir... Mas deixei aqui pra lembramos de discutir isso
        //(*precisaInserir) = 0; //pois chegou na raiz. Logo, não há mais páginas acima
    }

    return;
}


//Função de inserção da chave "id" com offset "offSet" na Árvore B. RRNatual começa sendo o da raiz.
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

    //Se não é folha, prossegue com a busca
    if(pagAtual.folha == 0){
        //recursão para o filho correspondente à chave a ser inserida
        inserir(id, offSet, pagAtual.filhos[i], RRNraiz, precisaInserir, passouPorSplit, RRNnovaPagSplit);
    }

    if(*precisaInserir)
    {
        if (pagAtual.numeroChaves >= (ORDEM-1))
        {
            //Nó está cheio, é necessário realizar um split
            split(&pagAtual, id, offSet, RRNraiz, RRNatual, RRNnovaPagSplit);//Lembrar de fazer cabeçalho com a raiz no arquivo
            (*passouPorSplit) = 1;
        }
        else
        {
            for (i = 0; i < pagAtual.numeroChaves && *id > pagAtual.chaves[i][0]; i++)
            {
                //Não precisa de código aqui dentro...
            }

            int posColocar = i;
            printf("posColocar = %d\n", posColocar);
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