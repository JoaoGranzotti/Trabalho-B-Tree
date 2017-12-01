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
    char ch; //ver se está sendo usada
    char id[3], titulo[30], genero[20];

    criarArquivoDeLog(ARQLOG);

    //Se ela quer que leia do usuario o nome do arquivo de dados,
    //tem que ver como vamos fazer a criacao do de indices,
    //sem que o usuario tenha interagido com o programa
    /*if (existeArqDados())
        criarArquivoDeIndice();*/

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
        system("cls"); //No windows, usar system("cls");
        switch(escolhaMenu){
        case 1:
            do {
                //está lendo sem considerar espacos, e acho q esta correto, ja que nome de arquivo eh bom fazer sem espaço
                printf("Digite o somente o nome do arquivo, sem a extensao, que deseja criar o arquivo de indice:\n");
                scanf("%s", entrada);

                if(entradaValida(entrada, 1))//1 indica que o modo de validacao esta para nomes de arquivos
                {
                    strcat(entrada, ".dad");
                    dadoValido = 1;
                }
            } while(dadoValido == 0);

            dadoValido = 0;

            criarArquivoDeIndice(entrada);
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
                }
            }while(!dadoValido);
            dadoValido = 0;

            do{
                printf("Digite o titulo da musica:\n");
                scanf("%[^\n]s", titulo);
                fflush(stdin);
                if (entradaValida(titulo, 0))//0 indica que o modo de validacao esta para entradas comuns
                {
                    strcpy(novoRegistro.titulo, titulo);
                    dadoValido = 1;
                }
            }while(!dadoValido);
            dadoValido = 0;

            do{
                printf("Digite o genero da musica:\n");
                scanf("%[^\n]s", genero);
                fflush(stdin);
                if (entradaValida(genero, 0))//0 indica que o modo de validacao esta para entradas comuns
                {
                    strcpy(novoRegistro.genero, genero);
                    dadoValido = 1;
                }
            }while(!dadoValido);
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
                    printf("Erro, id fora do limite permitido(0<id<255).\n");
                }
            }
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
            printf("Entrada invalida, digite novamente.\n");
            break;
        }
    }
    return 0;
}
