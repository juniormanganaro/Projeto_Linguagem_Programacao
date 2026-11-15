#ifndef PRODUTOR_H
#define PRODUTOR_H

#include "utils.h"

#define MAX_PRODUTORES 100
#define ARQ_PRODUTORES "produtores.dat"

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char cpf_cnpj[TAM_DOC];
    char telefone[TAM_TELEFONE];
    char endereco[TAM_ENDERECO];   /* propriedade ou localidade */
} Produtor;

int  carregarProdutores(Produtor lista[]);
int  salvarProdutores(Produtor lista[], int qtd);
int  buscarProdutor(Produtor lista[], int qtd, int codigo);
void exibirListaProdutores(Produtor lista[], int qtd);
void cadastrarProdutor(void);
void listarProdutores(void);

#endif
