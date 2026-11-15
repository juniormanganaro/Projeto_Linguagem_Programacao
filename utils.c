#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

/* Le uma linha do teclado, remove o '\n' e descarta o que passar do tamanho. */
void lerLinha(char destino[], int tamanho) {
    int tam, c;

    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\nEntrada encerrada. Finalizando o programa.\n");
        exit(1);
    }

    tam = strlen(destino);
    if (tam > 0 && destino[tam - 1] == '\n') {
        destino[tam - 1] = '\0';
    } else {
        /* linha maior que o buffer: limpa o restante do teclado */
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

/* Le um texto obrigatorio (nao vazio) que caiba em 'tamanho' caracteres. */
void lerTexto(const char mensagem[], char destino[], int tamanho) {
    char entrada[256];
    int valido = 0;

    while (!valido) {
        printf("%s", mensagem);
        lerLinha(entrada, 256);

        if (strlen(entrada) == 0) {
            printf("Erro: o campo nao pode ficar vazio.\n");
        } else if ((int)strlen(entrada) >= tamanho) {
            printf("Erro: maximo de %d caracteres.\n", tamanho - 1);
        } else {
            strcpy(destino, entrada);
            valido = 1;
        }
    }
}

/* Le um inteiro entre minimo e maximo. */
int lerInteiro(const char mensagem[], int minimo, int maximo) {
    char entrada[100], extra;
    int valor = 0, valido = 0;

    while (!valido) {
        printf("%s", mensagem);
        lerLinha(entrada, 100);

        if (sscanf(entrada, "%d %c", &valor, &extra) != 1) {
            printf("Erro: digite um numero inteiro valido.\n");
        } else if (valor < minimo || valor > maximo) {
            printf("Erro: digite um valor entre %d e %d.\n", minimo, maximo);
        } else {
            valido = 1;
        }
    }
    return valor;
}

/* Le um numero real >= minimo. Aceita virgula ou ponto como separador. */
float lerFloat(const char mensagem[], float minimo) {
    char entrada[100], extra;
    float valor = 0;
    int i, valido = 0;

    while (!valido) {
        printf("%s", mensagem);
        lerLinha(entrada, 100);

        for (i = 0; entrada[i] != '\0'; i++) {
            if (entrada[i] == ',') {
                entrada[i] = '.';
            }
        }

        if (sscanf(entrada, "%f %c", &valor, &extra) != 1) {
            printf("Erro: digite um numero valido (ex: 12.50).\n");
        } else if (valor < minimo) {
            printf("Erro: o valor deve ser maior ou igual a %.2f.\n", minimo);
        } else {
            valido = 1;
        }
    }
    return valor;
}

/* Le um texto formado apenas por digitos, com tamanho entre tamMin e tamMax. */
void lerNumeroTexto(const char mensagem[], char destino[], int tamMin, int tamMax) {
    char entrada[256];
    int i, tam, valido = 0;

    while (!valido) {
        printf("%s", mensagem);
        lerLinha(entrada, 256);
        tam = strlen(entrada);

        valido = 1;
        for (i = 0; i < tam; i++) {
            if (!isdigit((unsigned char)entrada[i])) {
                valido = 0;
            }
        }

        if (!valido) {
            printf("Erro: digite apenas numeros (sem pontos, tracos ou espacos).\n");
        } else if (tam < tamMin || tam > tamMax) {
            printf("Erro: o campo deve ter entre %d e %d digitos.\n", tamMin, tamMax);
            valido = 0;
        }
    }
    strcpy(destino, entrada);
}

/* Confere se a data esta no formato DD/MM/AAAA e se existe no calendario. */
int dataValida(const char data[]) {
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, dia, mes, ano;

    if (strlen(data) != 10) {
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') {
                return 0;
            }
        } else if (!isdigit((unsigned char)data[i])) {
            return 0;
        }
    }

    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');
    ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100 +
          (data[8] - '0') * 10 + (data[9] - '0');

    if (ano < 1900 || ano > 2100 || mes < 1 || mes > 12) {
        return 0;
    }

    /* ano bissexto */
    if (mes == 2 && ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0)) {
        dias[1] = 29;
    }

    return dia >= 1 && dia <= dias[mes - 1];
}

/* Le uma data valida (DD/MM/AAAA). */
void lerData(const char mensagem[], char destino[]) {
    char entrada[100];
    int valido = 0;

    while (!valido) {
        printf("%s", mensagem);
        lerLinha(entrada, 100);

        if (dataValida(entrada)) {
            strcpy(destino, entrada);
            valido = 1;
        } else {
            printf("Erro: data invalida. Use o formato DD/MM/AAAA.\n");
        }
    }
}

/* Retorna 1 se 'busca' aparece dentro de 'texto' (sem diferenciar maiusculas). */
int contemTexto(const char texto[], const char busca[]) {
    int i, j;
    int tamTexto = strlen(texto);
    int tamBusca = strlen(busca);

    for (i = 0; i + tamBusca <= tamTexto; i++) {
        j = 0;
        while (j < tamBusca &&
               tolower((unsigned char)texto[i + j]) == tolower((unsigned char)busca[j])) {
            j++;
        }
        if (j == tamBusca) {
            return 1;
        }
    }
    return 0;
}

/* Imprime uma linha de separacao. */
void imprimirLinha(char simbolo, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        printf("%c", simbolo);
    }
    printf("\n");
}

/* Espera o usuario apertar ENTER. */
void pausar(void) {
    char entrada[100];
    printf("\nPressione ENTER para continuar...");
    lerLinha(entrada, 100);
}
