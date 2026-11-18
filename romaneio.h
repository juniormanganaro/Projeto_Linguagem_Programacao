#ifndef ROMANEIO_H
#define ROMANEIO_H

#define MAX_ITENS 10           
#define MAX_ROMANEIOS 100       
#define ARQUIVO "romaneios.dat"
#define COMISSAO 0.15f         
#define PENDENTE 0
#define PAGO 1


typedef struct {
    char produtor[50];
    char produto[50];           
    int volumes;
    float peso;                
    float preco_unit;         
    float preco_total;          
} Item;


typedef struct {
    int numero;
    char data[11];             
    char destino[50];
    Item itens[MAX_ITENS];
    int qtd_itens;
    int total_volumes;
    float peso_total;
    float valor_produtos;
    float comissao;
    float frete;
    float valor_final;
    int situacao;               

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
