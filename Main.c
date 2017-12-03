#include <stdio.h>
#include <stdlib.h>
#include "TAD.h"
#include "utilidades.h"

int main(){
    char entrada[100];
    int idAux; //usado na leitura do id
    int dadoValido = 0; //usado na leitura dos dados do registro
    int rodando = 1;
    int escolhaMenu;
    int idMusica;
    char id[3], titulo[30], genero[20];

    criarArquivoDeLog(ARQLOG);

    if (existeArqDados())
        criarArquivoDeIndice(FUNC_0);

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
        fflush(stdin);
        system("clear"); //No ubuntu, usar system("clear");
        switch(escolhaMenu){
        case 1:
            criarArquivoDeIndice(FUNC_1);
            break;
        case 2:
            printf("Inserir nova musica:\n");
            tRegistro novoRegistro;

            do{
                printf("Digite um numero inteiro com ID da musica:\n");
                scanf("%s", id);
                fflush(stdin);
                if(ehNumero(id))
                {
                    idAux = atoi(id);
                    if (idAux > 0 && idAux < 255)
                    {
                        novoRegistro.id = idAux;
                        dadoValido = 1;
                    }
                    else{
                        printf("Entrada invalida. Digite um id entre 0 e 255 exclusive.\n");
                    }
                }
            } while(!dadoValido);
            dadoValido = 0;

            do{
                printf("Digite o titulo da musica:\n");
                scanf("%s", titulo);
                if (entradaValida(titulo))
                {
                    strcpy(novoRegistro.titulo, titulo);
                    dadoValido = 1;
                }
            } while(!dadoValido);
            dadoValido = 0;

            do{
                printf("Digite o genero da musica:\n");
                scanf("%s", genero);
                if (entradaValida(genero))
                {
                    strcpy(novoRegistro.genero, genero);
                    dadoValido = 1;
                }
            } while(!dadoValido);
            dadoValido = 0;

            inserirMusica(novoRegistro);
            break;
        case 3:
            printf("Digite um numero inteiro com ID da musica a ser pesquisada:\n");
            scanf("%s", id);
            fflush(stdin);
            if(ehNumero(id))
            {
                idAux = atoi(id);
                if (idAux > 0 && idAux < 255)
                {
                    pesquisarMusica(idAux);
                }
                else
                {
                    printf("Erro, id fora do limite permitido (0 < id < 255).\n");
                }
            }
            break;
        case 4:
            printf("\nNão implementado.\n");
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
