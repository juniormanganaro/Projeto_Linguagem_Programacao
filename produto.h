#ifndef PRODUTO_H
#define PRODUTO_H

#include "utils.h"

#define MAX_PRODUTOS 100
#define ARQ_PRODUTOS "produtos.dat"

typedef struct {
    int codigo;
    char nome[TAM_NOME];          /* variedade da uva */
    char unidade[TAM_UNIDADE];    /* unidade de comercializacao (caixa, kg...) */
    float preco_unit;
} Produto;

int  carregarProdutos(Produto lista[]);
int  salvarProdutos(Produto lista[], int qtd);
int  buscarProduto(Produto lista[], int qtd, int codigo);
void exibirListaProdutos(Produto lista[], int qtd);
void cadastrarProduto(void);
void listarProdutos(void);

#endif
