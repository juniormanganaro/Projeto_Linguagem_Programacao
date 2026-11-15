#include <stdio.h>
#include "produto.h"

/* Le todos os produtos do arquivo para o vetor. Retorna a quantidade lida.
   Se o arquivo ainda nao existe (primeira execucao), retorna 0. */
int carregarProdutos(Produto lista[]) {
    FILE *arquivo = fopen(ARQ_PRODUTOS, "rb");
    int qtd;

    if (arquivo == NULL) {
        return 0;
    }

    qtd = fread(lista, sizeof(Produto), MAX_PRODUTOS, arquivo);
    fclose(arquivo);
    return qtd;
}

/* Grava todo o vetor no arquivo. Retorna 1 em caso de sucesso e 0 em caso de erro. */
int salvarProdutos(Produto lista[], int qtd) {
    FILE *arquivo = fopen(ARQ_PRODUTOS, "wb");

    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir/criar o arquivo %s.\n", ARQ_PRODUTOS);
        return 0;
    }

    if (fwrite(lista, sizeof(Produto), qtd, arquivo) != (size_t)qtd) {
        printf("Erro: falha ao gravar os dados em %s.\n", ARQ_PRODUTOS);
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

/* Retorna a posicao do produto no vetor ou -1 se nao existir. */
int buscarProduto(Produto lista[], int qtd, int codigo) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

/* Mostra a tabela de produtos ja carregados em um vetor. */
void exibirListaProdutos(Produto lista[], int qtd) {
    int i;

    if (qtd == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    imprimirLinha('-', 65);
    printf("%-7s %-25s %-15s %s\n", "Codigo", "Variedade", "Unidade", "Preco unit.");
    imprimirLinha('-', 65);
    for (i = 0; i < qtd; i++) {
        printf("%-7d %-25.25s %-15.15s R$ %8.2f\n",
               lista[i].codigo, lista[i].nome, lista[i].unidade, lista[i].preco_unit);
    }
    imprimirLinha('-', 65);
}

/* Opcao 2 do menu: cadastra uma nova variedade de uva. */
void cadastrarProduto(void) {
    Produto lista[MAX_PRODUTOS];
    Produto novo;
    int qtd;

    printf("\n=== CADASTRO DE PRODUTO ===\n");
    qtd = carregarProdutos(lista);

    if (qtd >= MAX_PRODUTOS) {
        printf("Erro: limite de %d produtos atingido.\n", MAX_PRODUTOS);
        return;
    }

    novo.codigo = lerInteiro("Codigo do produto: ", 1, 99999);
    while (buscarProduto(lista, qtd, novo.codigo) != -1) {
        printf("Erro: ja existe um produto com esse codigo.\n");
        novo.codigo = lerInteiro("Codigo do produto: ", 1, 99999);
    }

    lerTexto("Variedade da uva: ", novo.nome, TAM_NOME);
    lerTexto("Unidade de comercializacao (ex: caixa, kg): ", novo.unidade, TAM_UNIDADE);
    novo.preco_unit = lerFloat("Preco unitario (R$): ", 0.01f);

    lista[qtd] = novo;
    qtd++;

    if (salvarProdutos(lista, qtd)) {
        printf("\nProduto cadastrado com sucesso!\n");
    }
}

/* Opcao 6 do menu: lista os produtos cadastrados. */
void listarProdutos(void) {
    Produto lista[MAX_PRODUTOS];
    int qtd = carregarProdutos(lista);

    printf("\n=== PRODUTOS CADASTRADOS ===\n");
    exibirListaProdutos(lista, qtd);
}
