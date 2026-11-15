#ifndef ROMANEIO_H
#define ROMANEIO_H

#include "utils.h"
#include "produtor.h"
#include "produto.h"

#define MAX_ROMANEIOS 100
#define MAX_ITENS 15                 /* linhas do romaneio, como no modelo em papel */
#define ARQ_ROMANEIOS "romaneios.dat"
#define TAXA_COMISSAO 0.15f          /* 15% sobre o valor dos produtos */

/* Tipo de operacao */
#define COMPRA 1
#define VENDA  2

/* Situacao de pagamento */
#define PAGO     1
#define PENDENTE 2

/* Uma linha da tabela do romaneio */
typedef struct {
    int cod_produtor;
    char nome_produtor[TAM_NOME];
    int cod_produto;
    char nome_produto[TAM_NOME];
    int volumes;
    float peso;
    float preco_unit;
    float preco_total;          /* peso * preco_unit */
} ItemRomaneio;

/* O romaneio completo: cabecalho + vetor de itens + totais */
typedef struct {
    int numero;
    char data[TAM_DATA];
    int tipo;                   /* COMPRA ou VENDA */
    char destino[TAM_NOME];
    ItemRomaneio itens[MAX_ITENS];
    int qtd_itens;
    int total_volumes;
    float peso_total;
    float valor_produtos;
    float comissao;
    float frete;
    float valor_final;
    int situacao;               /* PAGO ou PENDENTE */
} Romaneio;

int  carregarRomaneios(Romaneio lista[]);
int  salvarRomaneios(Romaneio lista[], int qtd);
int  buscarRomaneio(Romaneio lista[], int qtd, int numero);
Romaneio calcularTotais(Romaneio r);
ItemRomaneio lerItem(Produtor produtores[], int qtdProdutores,
                     Produto produtos[], int qtdProdutos);
void textoTipo(int tipo, char destino[]);
void textoSituacao(int situacao, char destino[]);
void exibirRomaneio(Romaneio r);
int  romaneioCorresponde(Romaneio r, int criterio, int valorNumerico, const char textoBusca[]);
void registrarRomaneio(void);
void historicoRomaneios(void);

#endif
