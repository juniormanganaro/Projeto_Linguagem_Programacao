#include <stdio.h>
#include <string.h>
#include "produtor.h"

/* Le todos os produtores do arquivo para o vetor. Retorna a quantidade lida.
   Se o arquivo ainda nao existe (primeira execucao), retorna 0. */
int carregarProdutores(Produtor lista[]) {
    FILE *arquivo = fopen(ARQ_PRODUTORES, "rb");
    int qtd;

    if (arquivo == NULL) {
        return 0;
    }

    qtd = fread(lista, sizeof(Produtor), MAX_PRODUTORES, arquivo);
    fclose(arquivo);
    return qtd;
}

/* Grava todo o vetor no arquivo. Retorna 1 em caso de sucesso e 0 em caso de erro. */
int salvarProdutores(Produtor lista[], int qtd) {
    FILE *arquivo = fopen(ARQ_PRODUTORES, "wb");

    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir/criar o arquivo %s.\n", ARQ_PRODUTORES);
        return 0;
    }

    if (fwrite(lista, sizeof(Produtor), qtd, arquivo) != (size_t)qtd) {
        printf("Erro: falha ao gravar os dados em %s.\n", ARQ_PRODUTORES);
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

/* Retorna a posicao do produtor no vetor ou -1 se nao existir. */
int buscarProdutor(Produtor lista[], int qtd, int codigo) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

/* Mostra a tabela de produtores ja carregados em um vetor. */
void exibirListaProdutores(Produtor lista[], int qtd) {
    int i;

    if (qtd == 0) {
        printf("Nenhum produtor cadastrado.\n");
        return;
    }

    imprimirLinha('-', 90);
    printf("%-7s %-25s %-15s %-12s %s\n", "Codigo", "Nome", "CPF/CNPJ", "Telefone", "Localidade");
    imprimirLinha('-', 90);
    for (i = 0; i < qtd; i++) {
        printf("%-7d %-25.25s %-15s %-12s %.25s\n",
               lista[i].codigo, lista[i].nome, lista[i].cpf_cnpj,
               lista[i].telefone, lista[i].endereco);
    }
    imprimirLinha('-', 90);
}

/* Opcao 1 do menu: cadastra um novo produtor. */
void cadastrarProdutor(void) {
    Produtor lista[MAX_PRODUTORES];
    Produtor novo;
    int qtd, tam;

    printf("\n=== CADASTRO DE PRODUTOR ===\n");
    qtd = carregarProdutores(lista);

    if (qtd >= MAX_PRODUTORES) {
        printf("Erro: limite de %d produtores atingido.\n", MAX_PRODUTORES);
        return;
    }

    novo.codigo = lerInteiro("Codigo do produtor: ", 1, 99999);
    while (buscarProdutor(lista, qtd, novo.codigo) != -1) {
        printf("Erro: ja existe um produtor com esse codigo.\n");
        novo.codigo = lerInteiro("Codigo do produtor: ", 1, 99999);
    }

    lerTexto("Nome do produtor: ", novo.nome, TAM_NOME);

    do {
        lerNumeroTexto("CPF (11 digitos) ou CNPJ (14 digitos): ", novo.cpf_cnpj, 11, 14);
        tam = strlen(novo.cpf_cnpj);
        if (tam == 12 || tam == 13) {
            printf("Erro: CPF tem 11 digitos e CNPJ tem 14.\n");
        }
    } while (tam == 12 || tam == 13);

    lerNumeroTexto("Telefone com DDD (10 ou 11 digitos): ", novo.telefone, 10, 11);
    lerTexto("Propriedade ou localidade: ", novo.endereco, TAM_ENDERECO);

    lista[qtd] = novo;
    qtd++;

    if (salvarProdutores(lista, qtd)) {
        printf("\nProdutor cadastrado com sucesso!\n");
    }
}

/* Opcao 5 do menu: lista os produtores cadastrados. */
void listarProdutores(void) {
    Produtor lista[MAX_PRODUTORES];
    int qtd = carregarProdutores(lista);

    printf("\n=== PRODUTORES CADASTRADOS ===\n");
    exibirListaProdutores(lista, qtd);
}
