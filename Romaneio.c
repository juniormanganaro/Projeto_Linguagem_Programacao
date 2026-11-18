#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "romaneio.h"

/* ---------- Leitura de dados ---------- */

/* Le um texto com espacos e nao aceita campo vazio */
void lerTexto(char mensagem[], char texto[], int tamanho) {
    int tam, c;

    do {
        printf("%s", mensagem);
        if (fgets(texto, tamanho, stdin) == NULL) {      /* teclado fechado */
            printf("\nEntrada encerrada.\n");
            exit(1);
        }
        tam = strlen(texto);
        if (tam > 0 && texto[tam - 1] == '\n') {
            texto[tam - 1] = '\0';                          /* tira o ENTER */
        } else {
            while ((c = getchar()) != '\n' && c != EOF);    /* descarta o excesso */
        }
        if (strlen(texto) == 0) {
            printf("Erro: o campo nao pode ficar vazio.\n");
        }
    } while (strlen(texto) == 0);
}

/* Le um numero inteiro entre minimo e maximo */
int lerInteiro(char mensagem[], int minimo, int maximo) {
    char texto[20];
    int valor;

    do {
        lerTexto(mensagem, texto, 20);
        valor = atoi(texto);
        if (valor < minimo || valor > maximo) {
            printf("Erro: digite um numero entre %d e %d.\n", minimo, maximo);
        }
    } while (valor < minimo || valor > maximo);
    return valor;
}

/* Le um numero real maior ou igual a minimo (use ponto: 8.50) */
float lerReal(char mensagem[], float minimo) {
    char texto[20];
    float valor;

    do {
        lerTexto(mensagem, texto, 20);
        valor = atof(texto);
        if (valor < minimo) {
            printf("Erro: digite um valor maior ou igual a %.2f.\n", minimo);
        }
    } while (valor < minimo);
    return valor;
}

/* ---------- Arquivo ---------- */

/* Le os romaneios do arquivo para o vetor e devolve quantos foram lidos */
int carregarRomaneios(Romaneio lista[]) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    int qtd;

    if (arquivo == NULL) {
        return 0;                   /* arquivo ainda nao existe: nenhum romaneio */
    }
    qtd = fread(lista, sizeof(Romaneio), MAX_ROMANEIOS, arquivo);
    fclose(arquivo);
    return qtd;
}

/* Grava o vetor no arquivo. Devolve 1 se deu certo e 0 se deu erro */
int salvarRomaneios(Romaneio lista[], int qtd) {
    FILE *arquivo = fopen(ARQUIVO, "wb");

    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir/criar o arquivo %s.\n", ARQUIVO);
        return 0;
    }
    if (fwrite(lista, sizeof(Romaneio), qtd, arquivo) != (size_t)qtd) {
        printf("Erro: falha ao gravar no arquivo %s.\n", ARQUIVO);
        fclose(arquivo);
        return 0;
    }
    fclose(arquivo);
    return 1;
}

/* ---------- Exibicao ---------- */

void imprimirSituacao(int situacao) {
    if (situacao == PAGO) {
        printf("PAGO");
    } else {
        printf("PENDENTE");
    }
}

/* Mostra o romaneio completo */
void exibirRomaneio(Romaneio r) {
    int i;

    printf("\n========================================\n");
    printf("ROMANEIO No %d    Data: %s\n", r.numero, r.data);
    printf("Destino: %s\n", r.destino);
    printf("----------------------------------------\n");
    for (i = 0; i < r.qtd_itens; i++) {
        printf("%d) Produtor: %s | Produto: %s\n", i + 1, r.itens[i].produtor, r.itens[i].produto);
        printf("   %d volumes x %.2f kg x R$ %.2f = R$ %.2f\n", r.itens[i].volumes,
               r.itens[i].peso, r.itens[i].preco_unit, r.itens[i].preco_total);
    }
    printf("----------------------------------------\n");
    printf("Total de volumes  : %d\n", r.total_volumes);
    printf("Peso total        : %.2f kg\n", r.peso_total);
    printf("Valor dos produtos: R$ %.2f\n", r.valor_produtos);
    printf("Comissao (15%%)    : R$ %.2f\n", r.comissao);
    printf("Frete             : R$ %.2f\n", r.frete);
    printf("VALOR FINAL       : R$ %.2f\n", r.valor_final);
    printf("Situacao          : ");
    imprimirSituacao(r.situacao);
    printf("\n========================================\n");
}

/* ---------- Registrar romaneio (opcao 1) ---------- */

void registrarRomaneio(void) {
    Romaneio lista[MAX_ROMANEIOS];
    Romaneio novo;
    Item item;
    int qtd, i, continuar;

    printf("\n=== REGISTRAR ROMANEIO ===\n");
    qtd = carregarRomaneios(lista);
    if (qtd >= MAX_ROMANEIOS) {
        printf("Erro: limite de %d romaneios atingido.\n", MAX_ROMANEIOS);
        return;
    }

    /* Dados gerais */
    novo.numero = lerInteiro("Numero do romaneio: ", 1, 99999);
    for (i = 0; i < qtd; i++) {
        if (lista[i].numero == novo.numero) {
            printf("Erro: ja existe um romaneio com esse numero.\n");
            return;
        }
    }
    lerTexto("Data (DD/MM/AAAA): ", novo.data, 11);
    if (strlen(novo.data) != 10 || novo.data[2] != '/' || novo.data[5] != '/') {
        printf("Erro: data invalida. Use o formato DD/MM/AAAA.\n");
        return;
    }
    lerTexto("Destino: ", novo.destino, 50);

    /* Itens: repete ate o usuario nao querer mais (ou chegar em MAX_ITENS) */
    novo.qtd_itens = 0;
    novo.total_volumes = 0;
    novo.peso_total = 0;
    novo.valor_produtos = 0;
    do {
        printf("\n--- Item %d ---\n", novo.qtd_itens + 1);
        lerTexto("Produtor: ", item.produtor, 50);
        lerTexto("Produto (variedade da uva): ", item.produto, 50);
        item.volumes = lerInteiro("Quantidade de volumes: ", 1, 100000);
        item.peso = lerReal("Peso de cada volume (kg): ", 0.01f);
        item.preco_unit = lerReal("Preco por kg (R$): ", 0.01f);
        item.preco_total = item.volumes * item.peso * item.preco_unit;

        novo.itens[novo.qtd_itens] = item;
        novo.qtd_itens++;
        novo.total_volumes += item.volumes;
        novo.peso_total += item.volumes * item.peso;
        novo.valor_produtos += item.preco_total;

        if (novo.qtd_itens < MAX_ITENS) {
            continuar = lerInteiro("Adicionar outro item? (1 - Sim, 2 - Nao): ", 1, 2);
        } else {
            printf("Limite de itens atingido.\n");
            continuar = 2;
        }
    } while (continuar == 1);

    /* Totais */
    novo.frete = lerReal("Valor do frete (R$): ", 0);
    novo.comissao = novo.valor_produtos * COMISSAO;
    novo.valor_final = novo.valor_produtos + novo.comissao + novo.frete;
    novo.situacao = PENDENTE;

    /* Grava no arquivo */
    lista[qtd] = novo;
    qtd++;
    if (salvarRomaneios(lista, qtd)) {
        printf("\nRomaneio registrado com sucesso!\n");
        exibirRomaneio(novo);
    }
}

/* ---------- Historico (opcao 2) ---------- */

/* Diz se o romaneio atende a busca. criterio: 1 numero, 2 produtor,
   3 produto, 4 data, 5 listar todos */
int corresponde(Romaneio r, int criterio, char texto[], int numero) {
    int i;

    if (criterio == 5) {
        return 1;
    }
    if (criterio == 1) {
        return r.numero == numero;
    }
    if (criterio == 4) {
        return strcmp(r.data, texto) == 0;
    }
    for (i = 0; i < r.qtd_itens; i++) {
        if (criterio == 2 && strstr(r.itens[i].produtor, texto) != NULL) {
            return 1;
        }
        if (criterio == 3 && strstr(r.itens[i].produto, texto) != NULL) {
            return 1;
        }
    }
    return 0;
}

void historicoRomaneios(void) {
    Romaneio lista[MAX_ROMANEIOS];
    char texto[50] = "";
    int qtd, criterio, numero = 0, i, achados = 0, posicao, opcao;

    printf("\n=== HISTORICO DE ROMANEIOS ===\n");
    qtd = carregarRomaneios(lista);
    if (qtd == 0) {
        printf("Nenhum romaneio registrado ainda.\n");
        return;
    }

    /* Escolha da busca */
    printf("1 - Buscar por numero\n2 - Buscar por produtor\n3 - Buscar por produto\n");
    printf("4 - Buscar por data\n5 - Listar todos\n0 - Voltar\n");
    criterio = lerInteiro("Opcao: ", 0, 5);
    if (criterio == 0) {
        return;
    }
    if (criterio == 1) {
        numero = lerInteiro("Numero do romaneio: ", 1, 99999);
    } else if (criterio == 2) {
        lerTexto("Nome do produtor: ", texto, 50);
    } else if (criterio == 3) {
        lerTexto("Nome do produto: ", texto, 50);
    } else if (criterio == 4) {
        lerTexto("Data (DD/MM/AAAA): ", texto, 11);
    }

    /* Mostra os romaneios encontrados */
    printf("\nResultados:\n");
    for (i = 0; i < qtd; i++) {
        if (corresponde(lista[i], criterio, texto, numero)) {
            printf("No %d | %s | %s | R$ %.2f | ", lista[i].numero, lista[i].data,
                   lista[i].destino, lista[i].valor_final);
            imprimirSituacao(lista[i].situacao);
            printf("\n");
            achados++;
        }
    }
    if (achados == 0) {
        printf("Nenhum romaneio encontrado para essa busca.\n");
        return;
    }

    /* Visualizacao completa e alteracao da situacao */
    numero = lerInteiro("\nNumero do romaneio para ver detalhes (0 para voltar): ", 0, 99999);
    if (numero == 0) {
        return;
    }
    posicao = -1;
    for (i = 0; i < qtd; i++) {
        if (lista[i].numero == numero) {
            posicao = i;
        }
    }
    if (posicao == -1) {
        printf("Erro: romaneio nao encontrado.\n");
        return;
    }
    exibirRomaneio(lista[posicao]);
    if (lista[posicao].situacao == PENDENTE) {
        opcao = lerInteiro("Marcar como PAGO? (1 - Sim, 2 - Nao): ", 1, 2);
        if (opcao == 1) {
            lista[posicao].situacao = PAGO;
            if (salvarRomaneios(lista, qtd)) {
                printf("Situacao alterada para PAGO.\n");
            }
        }
    }
}
