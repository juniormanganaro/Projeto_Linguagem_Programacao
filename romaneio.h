#ifndef ROMANEIO_H
#define ROMANEIO_H

#define MAX_ITENS 10            /* itens por romaneio */
#define MAX_ROMANEIOS 100       /* romaneios guardados no arquivo */
#define ARQUIVO "romaneios.dat"
#define COMISSAO 0.15f          /* 15% sobre o valor dos produtos */
#define PENDENTE 0
#define PAGO 1

/* Uma linha da tabela do romaneio */
typedef struct {
    char produtor[50];
    char produto[50];           /* variedade da uva */
    int volumes;
    float peso;                 /* peso de cada volume (kg) */
    float preco_unit;           /* preco por kg */
    float preco_total;          /* volumes * peso * preco_unit */
} Item;

/* O romaneio completo: cabecalho, vetor de itens e totais */
typedef struct {
    int numero;
    char data[11];              /* DD/MM/AAAA */
    char destino[50];
    Item itens[MAX_ITENS];
    int qtd_itens;
    int total_volumes;
    float peso_total;
    float valor_produtos;
    float comissao;
    float frete;
    float valor_final;
    int situacao;               /* PENDENTE ou PAGO */
} Romaneio;

void lerTexto(char mensagem[], char texto[], int tamanho);
int lerInteiro(char mensagem[], int minimo, int maximo);
float lerReal(char mensagem[], float minimo);
int carregarRomaneios(Romaneio lista[]);
int salvarRomaneios(Romaneio lista[], int qtd);
void imprimirSituacao(int situacao);
void exibirRomaneio(Romaneio r);
int corresponde(Romaneio r, int criterio, char texto[], int numero);
void registrarRomaneio(void);
void historicoRomaneios(void);

#endif