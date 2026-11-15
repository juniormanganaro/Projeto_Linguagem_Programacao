#ifndef UTILS_H
#define UTILS_H

/* Tamanhos padrao dos campos de texto (ja incluindo o '\0') */
#define TAM_NOME      60
#define TAM_ENDERECO  100
#define TAM_DOC       15   /* CPF (11) ou CNPJ (14) */
#define TAM_TELEFONE  12   /* DDD + numero (10 ou 11 digitos) */
#define TAM_DATA      11   /* DD/MM/AAAA */
#define TAM_UNIDADE   20

/* Leitura segura de dados (repete ate o usuario digitar algo valido) */
void lerLinha(char destino[], int tamanho);
void lerTexto(const char mensagem[], char destino[], int tamanho);
int  lerInteiro(const char mensagem[], int minimo, int maximo);
float lerFloat(const char mensagem[], float minimo);
void lerNumeroTexto(const char mensagem[], char destino[], int tamMin, int tamMax);
void lerData(const char mensagem[], char destino[]);

/* Validacao e busca em texto */
int dataValida(const char data[]);
int contemTexto(const char texto[], const char busca[]);

/* Apresentacao */
void imprimirLinha(char simbolo, int tamanho);
void pausar(void);

#endif
