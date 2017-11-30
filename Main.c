#include <stdio.h>
#include <stdlib.h>
#include "TAD.h"
#include "utilidades.h"

int main(){
    char entrada[100];
    int rodando = 1;
    int entradaValida;
    int escolhaMenu;
    int idMusica;
    char ch;

    criarArquivoDeLog(ARQLOG);
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
        entradaValida = 0;
        fflush(stdin);
        system("clear");
        switch(escolhaMenu){
        case 1:
            while(entradaValida == 0){
                printf("Digite o nome do arquivo, com extensao, que deseja criar o arquivo de indice:\n");
                scanf("%s", entrada);
                entradaValida += verificaEntrada(entrada);
            }
            strcat(entrada, ".dat");
            criarArquivoDeIndice(entrada);
            break;
        case 2:
            printf("Inserir nova musica:\n");
            tRegistro novoRegistro;
            while(entradaValida == 0){
                printf("Digite um numero inteiro com ID da musica:\n");
                scanf("%d", &novoRegistro.id);
                getchar();
                entradaValida += 1;
            }
            while(entradaValida == 1){
                printf("Digite o titulo da musica:\n");
                scanf("%s", novoRegistro.titulo);
                entradaValida += verificaEntrada(novoRegistro.titulo);
            }
            while(entradaValida == 2){
                printf("Digite o genero da musica:\n");
                scanf("%s", novoRegistro.genero);
                entradaValida += verificaEntrada(novoRegistro.genero);
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
