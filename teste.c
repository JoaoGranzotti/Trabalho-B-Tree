#include <stdio.h>
#include <stdlib.h>

#define ORDEM 5

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

void lePaginas(){
    FILE *arq = fopen("arvore.idx", "rb");
    if (arq == NULL){
        printf("Nao abriu o arquivo. Erro 0x0182931.");
        exit(0);
    }

    PAGINA leitura;

    int i = 0;
    int j, k;
    int att, RRNraiz;

    fread(&att, sizeof(att), 1, arq);
    fread(&RRNraiz, sizeof(RRNraiz), 1, arq);
    printf("Atualizado: %d\n", att);
    printf("RRN raiz = %d\n", RRNraiz);

    while(fread(&leitura, sizeof(leitura), 1,  arq)){
        printf("Pagina numero %d:\n", i);
        printf("Numero de Chaves: %d\n", leitura.numeroChaves);
        for(j = 0; j < ORDEM-1; j++){
            for(k = 0; k < 2; k++){
                printf("Chave[%d][%d] = %d\n", j, k, leitura.chaves[j][k]);
            }
        }
        for(j = 0; j < ORDEM; j++){
            printf("Filhos[%d] = %d\n", j, leitura.filhos[j]);
        }
        printf("%sh folha: %d\n\n", (leitura.folha) ? "E" : "Nao e", leitura.folha);

        i++;
    }
}

int main(){
    lePaginas();
    /*int i, j;
    int RRN = 0;
    FILE *arqdados;
    arqdados = fopen("arvore.idx", "rb");

    if (arqdados == NULL) {
        printf ("Houve um erro ao abrir o arquivo.\n");
        return 0;
    }*/
    
    /*int att = 1;
    fwrite(&att, sizeof(int), 1, arqdados);
    fwrite(&RRN, sizeof(int), 1, arqdados);
    fclose(arqdados);

    PAGINA pag;
    int x;
    printf("Quantas paginas vc quer por? ");
    scanf("%d", &x);
    while(x--){
        printf("Insira o numero de chaves da pagina (menor ou igual a 4):");
        scanf("%d", &pag.numeroChaves);
        for(i = 0; i < pag.numeroChaves; i++){
            for(j = 0; j < 2; j++){
                if(j == 0){
                    printf("Insira o id da chave %d: ", i);
                    scanf("%d", &pag.chaves[i][0]);
                }
                else{
                    printf("Insira o offset da chave %d: ", i);
                    scanf("%d", &pag.chaves[i][1]);
                }
            }
        }
        for(i = 0; i <= pag.numeroChaves; i++){
            printf("RRN do filho %d: ", i);
            scanf("%d", &pag.filhos[i]);
        }
        printf("Eh folha? 1 para sim e 0 para nao: ");
        scanf("%d", &pag.folha);

        inserirEmDisco(pag);
    }

    arqdados = fopen("testinho.dad", "rb");

    fseek(arqdados, 0, SEEK_END);
    long tamanhoArquivo = ftell(arqdados);
    printf("%lf", tamanhoArquivo);*/

    /*fread(&att, sizeof(int), 1, arqdados);
    if(att == 1)
        printf("Esta atualizado!\n");

    printf("Digite o id a ser buscado: ");
    int id;
    int offset;
    scanf("%d", &id);
    int busca = buscar(id, 0, &offset);
    if(busca == -1){
        printf("Nao achou :(");
    }
    else{
        printf("id %d encontrado no RRN: %d, com offset: %d", id, busca, offset);
    }

    PAGINA recebido;
    arqdados = fopen("testinho.dad", "rb");
    printf("Selecione o RRN de uma das paginas q vc colocou. ");
    scanf("%d", &RRN);

    fseek(arqdados, RRN*sizeof(PAGINA)+sizeof(int), SEEK_SET);
    fread(&recebido, sizeof(PAGINA), 1, arqdados);
    printf("A pagina possui:\nNumero de chaves: %d\n", recebido.numeroChaves);
    for(i = 0; i < recebido.numeroChaves; i++){
        printf("Chave %d: ", i);
        for(j = 0; j < 2; j++){
            if (j == 0)
                printf("id = %d ", recebido.chaves[i][0]);
            else
                printf("offset = %d\n", recebido.chaves[i][1]);
        }
    }
    printf("RRN dos filhos em sequencia: ");
    for(i = 0; i <= recebido.numeroChaves; i++){
        printf("%d ", recebido.filhos[i]);
    }
    printf("\nEh folha: %d", recebido.folha);*/

    return 0;
}