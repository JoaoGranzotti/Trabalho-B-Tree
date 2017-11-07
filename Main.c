#include <stdio.h>
#include <stdlib.h>
#include "TAD.h"
#include "utilidades.h"

int main(){
    char nomeArquivoLog[] = "log_X.txt";
    char nomeArquivoDados[] = "dados.dad";
    char nomeArquivoIndice[] = "arvore.idx";

    int rodando = 1;
    int entradaValida = 0;
    int escolhaMenu;
    int idMusica;

    printf("Trabalho Alg. II - Arvore B\n");
    while(rodando == 1){
        printf("Digite o numero da opcao desejada:\n");
        printf("1. Criar indice.\n");
        printf("2. Inserir Musica.\n");
        printf("3. Pesquisar Musica por ID.\n");
        printf("4. Remover Musica por ID.\n");
        printf("5. Mostrar Arvore-B.\n");
        printf("6. Fechar o programa.\n");
        scanf("%d", &escolhaMenu);
        switch(escolhaMenu){
        case 1:
            lerArquivoDeDados();
            criarArquivoDeIndices();
            break;
        case 2:
            system("CLS");
            printf("Tela inserir nova musica:\n");
            tRegistro novoRegistro;
            while(entradaValida == 0){
                printf("Digite um numero inteiro com ID da musica:\n");
                char entrada[100];
                scanf("%[^\n]s", entrada);
                setbuf(stdin, NULL);
                entradaValida = entradaValida + numeroOuChar(entrada);
                novoRegistro.id = (int)entrada;
            }
            while(entradaValida == 1){
                printf("Digite o titulo da musica:\n");
                scanf("%[^\n]s", novoRegistro.titulo);
                setbuf(stdin, NULL);
                entradaValida = entradaValida + verificaEntrada(novoRegistro.titulo);
            }
            while(entradaValida == 2){
                printf("Digite o genero da musica:\n");
                scanf("%[^\n]s", novoRegistro.genero);
                setbuf(stdin, NULL);
                entradaValida = entradaValida + verificaEntrada(novoRegistro.genero);
            }
            inserirMusica(novoRegistro);
            entradaValida = 0;
            break;
        case 3:
            printf("Digite um numero inteiro com ID da musica a ser pesquisada:");
            scanf("%d", &idMusica);
            pesquisarMusica(idMusica);
            break;
        case 4:
            printf("Digite um numero inteiro com ID da musica a ser removida:");
            scanf("%d", &idMusica);
            removerMusica(idMusica);
            break;
        case 5:
            mostrarArvore();
            break;
        case 6:
            rodando = 0;
            break;
        default:
            printf("Entrada inválida, digite novamente.\n");
            break;
        }
    }
    return 0;
}
