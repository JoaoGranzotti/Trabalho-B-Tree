#include <stdio.h>
#include <stdlib.h>

#define ORDEM 5

#define CRIOURAIZ 0
#define JAEXISTE 1
#define INSERIUNORMAL 2
#define INSERIUCOMSPLIT 3

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
    arqInd = fopen("testinho.dad", "ab");

    fwrite(&atual, sizeof(PAGINA), 1, arqInd);

    fclose(arqInd);
}

//*offset � o valor de retorno do offset
//*RRNinserir � o RRN em que se deve inserir o id atual
/*int buscar(int id, int RRN, int *offset, int *RRNinserir){
    //Abertura do arquivo de �ndice em modo leitura bin�rio.
    //� feito assim para evitar bugs.
    FILE *arq = fopen("arvore.idx", "rb");

    //Contador para verifica��o posterior do id.
    int i;
    //Declara��o da p�gina em mem�ria que receber� da leitura em disco.
    PAGINA recebido;

    //Vamos at� a posi��o do RRN da recurs�o, pulando o primeiro int destinado
    //a informar se o arquivo de �ndices est� atualizado ou n�o.
    fseek(arq, sizeof(PAGINA)*RRN+sizeof(int), SEEK_SET);
    //Lemos a p�gina em disco.
    fread(&recebido, sizeof(PAGINA), 1, arq);
    //Fechamos o arquivo para evitar bugs posteriores.
    fclose(arq);

    //Itera��o para verificar se o id est� na p�gina obtida. Se n�o estiver,
    //a vari�vel i ser� a posi��o em que o id "deveria" estar.
    for(i = 0; i < recebido.numeroChaves && id >= recebido.chaves[i][0]; i++){
        //Se o id estiver na p�gina,
        if(recebido.chaves[i][0] == id){
            //Deixa na vari�vel offset o offset do id no arquivo de dados
            *offset = recebido.chaves[i][1];
            //Retorna o RRN no arquivo de dados do id
            return RRN;
        }
    }

    //Est� na p�gina folha e n�o encontrou o id
    if(recebido.folha == 1){
        *RRNinserir = RRN;
        return -1;
    }

    //N�o est� na folha e ainda n�o encontrou o id
    else
        //Recurs�o para ir ao filho da p�gina onde id deveria estar.
        return buscar(id, recebido.filhos[i], offset, RRNinserir, caminhoPercorrido, auxIndices);
}*/

void ordenaPagina(PAGINA *pag, int idInsert, int offSetInsert){
    int i, j, aux;
    pag->chaves[pag->numeroChaves][0] = idInsert;
    pag->chaves[pag->numeroChaves][1] = offSetInsert;
    pag->numeroChaves++;

    for(i = 0; i < pag->numeroChaves; i++){
        for(j = 1; j < pag->numeroChaves; j++){
            if(pag->chaves[j][0] < pag->chaves[j-1][0]){
                aux = pag->chaves[j][0];
                pag->chaves[j][0] = pag->chaves[j-1][0];
                pag->chaves[j-1][0] = aux;

                aux = pag->chaves[j][1];
                pag->chaves[j][1] = pag->chaves[j-1][1];
                pag->chaves[j-1][1] = aux;
            }
        }
    }
}

void inserir(int id, int offSet, int RRNatual){
    int i, j;
    int chaveAux[2];
    PAGINA pagAtual;

    FILE *arqInd;
    arqInd = fopen("arvore.idx", "r+b");

    fseek(arqInd, sizeof(int)+RRNatual*sizeof(PAGINA), SEEK_SET);
    fwrite(&pagAtual, sizeof(PAGINA), 1, arqInd);

    if(pagAtual.folha == 0){
        fclose(arqInd);

        //Busca pela posi��o de inser��o da chave na p�gina atual
        //Por n�o ser folha, a inser��o n�o ocorrer� nesta p�gina
        for(i = 0; i < recebido.numeroChaves && id >= recebido.chaves[i][0]; i++){
            //Se o id estiver na p�gina,
            if(recebido.chaves[i][0] == id){
                //Deixa na vari�vel offset o offset do id no arquivo de dados
                *offset = recebido.chaves[i][1];
                //Retorna pois a chave j� existe
                return;
            }
            //sen�o, continua...
        }
        //recus�o para o filho correspondente � chave a ser inserida
        return inserir(id, offSet, pagAtual.filhos[i]);
    }
    else
    {
        if (pagAtual.numeroChaves >= (ORDEM-1))
        {
            //N� est� cheio, � necess�rio realizar um split
            split(&pagAtual, id, offSet, RRNraiz, RRNatual, arqInd);//Lembrar de fazer cabe�alho com a raiz no arquivo
        }
        else
        {
            for (i = 0; i < pagAtual.numeroChaves && id>pagAtual.chaves[i][0]; i++)
            {
                //...
            }

            int posColocar = i;

            for (i = (pagAtual.numeroChaves -1) ; i > posColocar; i--)
            {
                pagAtual.chaves[i][0] = pagAtual.chaves[i-1][0];
                pagAtual.chaves[i][1] = pagAtual.chaves[i-1][1];
            }

            pagAtual.chaves[posColocar][0] = id;
            pagAtual.chaves[posColocar][1] = offSet;

            /*

            //inser��o da nova chave no fim da p�gina(n�)
            pagAtual.chaves[pagAtual.numeroChaves][0] = id;
            pagAtual.chaves[pagAtual.numeroChaves][1] = offSet;

            pagAtual.numeroChaves++;

            //ordena��o da p�gina(n�)
            //BubbleSort
            for (i = 0; i < pagAtual.numeroChaves; i++)
            {
                for (j = 0; j < pagAtual.numeroChaves-1; j++)
                {
                    if (pagAtual.chaves[j][0] > pagAtual.chaves[j+1][0])
                    {
                         chaveAux[0] = pagAtual.chaves[j][0];
                         chaveAux[1] = pagAtual.chaves[j][1];
                         pagAtual.chaves[j][0] = pagAtual.chaves[j+1][0];
                         pagAtual.chaves[j][1] = pagAtual.chaves[j+1][1];
                         pagAtual.chaves[j+1][0] = chaveAux[0];
                         pagAtual.chaves[j+1][1] = chaveAux[1];
                    }
                }
            }*/
        }
        return;
    }
}

void ordenaChaves(int *chaves, int *filhos) //modificar fun��o para que manipule tamb�m o vetor de filhos
{
    int chaveAux[2];
    for(i = 0; i < ORDEM+1; i++){
        for(j = 1; j < ORDEM+1; j++){
            if(chaves[j][0] < chaves[j-1][0]){
                chaveAux = chaves[j];
                chaves[j] = chaves[j-1];
                chaves[j-1] = chaveAux;
            }
        }
    }
}

int getNovoRRN(){
    FILE *arq = fopen("arvore.idx", "rb");
    fseek(arq, 0, SEEK_END);
    long tam = ftell(arq);
    fclose(arq);

    return (tam-sizeof(int))/sizeof(PAGINA);
}

//Aqui tem que ir atualizando as p�ginas no arquivo de �ndice a cada itera��o da recurs�o?
void split(PAGINA *pag, int idInserir, int offSetInserir, int *RRNraiz, int RRNpagAtual, FILE *arqInd) //falta pegarmos o RRN da raiz
{
    PAGINA pagSplit;
    int chaves[ORDEM][2];
    int filhos[ORDEM];
    int i, j;

    //se � raiz
    if (RRNpagAtual == (*RRNraiz))
    {
        PAGINA novaRaiz;
        //Fazer essa parte...
    }
    else
    {
        //atribui��o das informa��es das chaves da p�gina atual ao vetor auxiliar de chaves
        for (i = 0; i < pag->numeroChaves; i++)
        {
            chaves[i][0] = pag->chaves[i][0];
            chaves[i][1] = pag->chaves[i][1];
            filhos[i] = pag->filhos[i];
        }
        //Fazendo o mesmo para o �ltimo filho, que o for acima n�o trata
        filhos[i] = pag->filhos[i];

        //Inser��o da chave atual na p�gina
        chaves[i][0] = idInserir;
        chaves[i][1] = offSetInserir;

        //Revisei at� aqui...
        ordenaChaves(chaves, filhos); //modificar fun��o para que manipule tamb�m o vetor de filhos

        //i vai at� (ORDEM-1)/2 = 2 por ser o tamanho minimo de chaves numa pagina
        for(i = 0; i < (ORDEM-1)/2; i++){
            for(j = 0; j < 2; j++){
                pag->chaves[i][j] = chaves[i][j];
            }
        }
        pag->numeroChaves = 2;

        //para as demais chaves do n�, atribu�mos nulo(-1) tanto para a chave, quanto para seu offset
        for(i = (ORDEM-1)/2; i < ORDEM-1; i++){
            for(j = 0; j < 2; j++){
                pag->chaves[i][j] = -1;
            }
        }

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

        //para as demais chaves do n�, atribu�mos nulo(-1)
        for(i = (ORDEM-1)/2; i < ORDEM-1; i++){
            for(j = 0; j < 2; j++){
                pagSplit.chaves[i][j] = -1;
            }
        }

        //chave 2, intermedi�ria, sobe para o antecessor ao n� atual
    }

    return;
}
