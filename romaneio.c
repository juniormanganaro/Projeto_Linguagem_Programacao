#include <stdio.h>
#include <string.h>
#include "romaneio.h"

/* Le todos os romaneios do arquivo para o vetor. Retorna a quantidade lida.
   Se o arquivo ainda nao existe (primeira execucao), retorna 0. */
int carregarRomaneios(Romaneio lista[]) {
    FILE *arquivo = fopen(ARQ_ROMANEIOS, "rb");
    int qtd;

    if (arquivo == NULL) {
        return 0;
    }

    qtd = fread(lista, sizeof(Romaneio), MAX_ROMANEIOS, arquivo);
    fclose(arquivo);
    return qtd;
}

/* Grava todo o vetor no arquivo. Retorna 1 em caso de sucesso e 0 em caso de erro. */
int salvarRomaneios(Romaneio lista[], int qtd) {
    FILE *arquivo = fopen(ARQ_ROMANEIOS, "wb");

    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir/criar o arquivo %s.\n", ARQ_ROMANEIOS);
        return 0;
    }

    if (fwrite(lista, sizeof(Romaneio), qtd, arquivo) != (size_t)qtd) {
        printf("Erro: falha ao gravar os dados em %s.\n", ARQ_ROMANEIOS);
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

/* Retorna a posicao do romaneio no vetor ou -1 se nao existir. */
int buscarRomaneio(Romaneio lista[], int qtd, int numero) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (lista[i].numero == numero) {
            return i;
        }
    }
    return -1;
}

/* Calcula os totais do romaneio, a comissao e o valor final.
   Valor final = valor dos produtos + comissao + frete. */
Romaneio calcularTotais(Romaneio r) {
    int i;

    r.total_volumes = 0;
    r.peso_total = 0;
    r.valor_produtos = 0;

    for (i = 0; i < r.qtd_itens; i++) {
        r.total_volumes += r.itens[i].volumes;
        r.peso_total += r.itens[i].peso;
        r.valor_produtos += r.itens[i].preco_total;
    }

    r.comissao = r.valor_produtos * TAXA_COMISSAO;
    r.valor_final = r.valor_produtos + r.comissao + r.frete;
    return r;
}

/* Converte o codigo do tipo de operacao em texto. */
void textoTipo(int tipo, char destino[]) {
    if (tipo == COMPRA) {
        strcpy(destino, "COMPRA");
    } else {
        strcpy(destino, "VENDA");
    }
}

/* Converte o codigo da situacao de pagamento em texto. */
void textoSituacao(int situacao, char destino[]) {
    if (situacao == PAGO) {
        strcpy(destino, "PAGO");
    } else {
        strcpy(destino, "PENDENTE");
    }
}

/* Pede ao usuario os dados de um item, escolhendo produtor e produto ja cadastrados. */
ItemRomaneio lerItem(Produtor produtores[], int qtdProdutores,
                     Produto produtos[], int qtdProdutos) {
    ItemRomaneio item;
    int codigo, posProdutor, posProduto;

    do {
        codigo = lerInteiro("Codigo do produtor (0 para listar): ", 0, 99999);
        if (codigo == 0) {
            exibirListaProdutores(produtores, qtdProdutores);
            posProdutor = -1;
        } else {
            posProdutor = buscarProdutor(produtores, qtdProdutores, codigo);
            if (posProdutor == -1) {
                printf("Erro: produtor nao encontrado.\n");
            }
        }
    } while (posProdutor == -1);
    item.cod_produtor = produtores[posProdutor].codigo;
    strcpy(item.nome_produtor, produtores[posProdutor].nome);

    do {
        codigo = lerInteiro("Codigo do produto (0 para listar): ", 0, 99999);
        if (codigo == 0) {
            exibirListaProdutos(produtos, qtdProdutos);
            posProduto = -1;
        } else {
            posProduto = buscarProduto(produtos, qtdProdutos, codigo);
            if (posProduto == -1) {
                printf("Erro: produto nao encontrado.\n");
            }
        }
    } while (posProduto == -1);
    item.cod_produto = produtos[posProduto].codigo;
    strcpy(item.nome_produto, produtos[posProduto].nome);

    item.volumes = lerInteiro("Quantidade de volumes: ", 1, 100000);
    item.peso = lerFloat("Peso total do item (kg): ", 0.01f);

    printf("Preco cadastrado para %s: R$ %.2f\n", item.nome_produto, produtos[posProduto].preco_unit);
    item.preco_unit = lerFloat("Preco unitario deste item (0 = usar o cadastrado): ", 0);
    if (item.preco_unit == 0) {
        item.preco_unit = produtos[posProduto].preco_unit;
    }

    item.preco_total = item.peso * item.preco_unit;
    return item;
}

/* Mostra o romaneio completo, no estilo do documento em papel. */
void exibirRomaneio(Romaneio r) {
    int i;
    char tipo[10], situacao[10];

    textoTipo(r.tipo, tipo);
    textoSituacao(r.situacao, situacao);

    printf("\n");
    imprimirLinha('=', 78);
    printf(" ROMANEIO No %d\n", r.numero);
    printf(" Data: %s    Tipo: %s    Destino: %s\n", r.data, tipo, r.destino);
    imprimirLinha('=', 78);
    printf(" %-4s %-17s %-15s %7s %10s %9s %11s\n",
           "Item", "Produtor", "Produto", "Volumes", "Peso (kg)", "P. Unit.", "P. Total");
    imprimirLinha('-', 78);
    for (i = 0; i < r.qtd_itens; i++) {
        printf(" %-4d %-17.17s %-15.15s %7d %10.2f %9.2f %11.2f\n",
               i + 1, r.itens[i].nome_produtor, r.itens[i].nome_produto,
               r.itens[i].volumes, r.itens[i].peso,
               r.itens[i].preco_unit, r.itens[i].preco_total);
    }
    imprimirLinha('-', 78);
    printf(" Total de volumes : %d\n", r.total_volumes);
    printf(" Peso total       : %.2f kg\n", r.peso_total);
    printf(" Valor dos produtos: R$ %.2f\n", r.valor_produtos);
    printf(" Comissao (%.0f%%)   : R$ %.2f\n", TAXA_COMISSAO * 100, r.comissao);
    printf(" Frete            : R$ %.2f\n", r.frete);
    printf(" VALOR FINAL      : R$ %.2f\n", r.valor_final);
    printf(" Situacao         : %s\n", situacao);
    imprimirLinha('=', 78);
}

/* Opcao 3 do menu: registra um novo romaneio com varios itens. */
void registrarRomaneio(void) {
    Romaneio lista[MAX_ROMANEIOS];
    Produtor produtores[MAX_PRODUTORES];
    Produto produtos[MAX_PRODUTOS];
    Romaneio novo;
    int qtd, qtdProdutores, qtdProdutos, maisItens, confirmar;

    printf("\n=== REGISTRO DE ROMANEIO ===\n");

    qtd = carregarRomaneios(lista);
    qtdProdutores = carregarProdutores(produtores);
    qtdProdutos = carregarProdutos(produtos);

    if (qtd >= MAX_ROMANEIOS) {
        printf("Erro: limite de %d romaneios atingido.\n", MAX_ROMANEIOS);
        return;
    }
    if (qtdProdutores == 0 || qtdProdutos == 0) {
        printf("Erro: cadastre ao menos um produtor e um produto antes de registrar um romaneio.\n");
        return;
    }

    /* Dados gerais */
    novo.numero = lerInteiro("Numero do romaneio: ", 1, 99999);
    while (buscarRomaneio(lista, qtd, novo.numero) != -1) {
        printf("Erro: ja existe um romaneio com esse numero.\n");
        novo.numero = lerInteiro("Numero do romaneio: ", 1, 99999);
    }
    lerData("Data (DD/MM/AAAA): ", novo.data);
    novo.tipo = lerInteiro("Tipo de operacao (1 - Compra, 2 - Venda): ", 1, 2);
    lerTexto("Destino: ", novo.destino, TAM_NOME);

    /* Itens (laço de repetição: pelo menos 1 item, no maximo MAX_ITENS) */
    novo.qtd_itens = 0;
    do {
        printf("\n--- Item %d de no maximo %d ---\n", novo.qtd_itens + 1, MAX_ITENS);
        novo.itens[novo.qtd_itens] = lerItem(produtores, qtdProdutores, produtos, qtdProdutos);
        novo.qtd_itens++;

        if (novo.qtd_itens < MAX_ITENS) {
            maisItens = lerInteiro("Adicionar outro item? (1 - Sim, 2 - Nao): ", 1, 2);
        } else {
            printf("Limite de itens do romaneio atingido.\n");
            maisItens = 2;
        }
    } while (maisItens == 1);

    /* Frete, totais e situacao */
    novo.frete = lerFloat("Valor do frete (R$): ", 0);
    novo = calcularTotais(novo);
    novo.situacao = lerInteiro("Situacao do pagamento (1 - Pago, 2 - Pendente): ", 1, 2);

    exibirRomaneio(novo);
    confirmar = lerInteiro("Confirmar gravacao? (1 - Sim, 2 - Nao): ", 1, 2);

    if (confirmar == 1) {
        lista[qtd] = novo;
        qtd++;
        if (salvarRomaneios(lista, qtd)) {
            printf("\nRomaneio registrado com sucesso!\n");
        }
    } else {
        printf("\nRegistro cancelado. Nada foi gravado.\n");
    }
}

/* Verifica se o romaneio atende ao criterio de busca do historico.
   criterio: 1 = numero, 2 = produtor, 3 = produto, 4 = data, 5 = tipo. */
int romaneioCorresponde(Romaneio r, int criterio, int valorNumerico, const char textoBusca[]) {
    int i;

    switch (criterio) {
        case 1:
            return r.numero == valorNumerico;
        case 2:
            for (i = 0; i < r.qtd_itens; i++) {
                if (contemTexto(r.itens[i].nome_produtor, textoBusca)) {
                    return 1;
                }
            }
            return 0;
        case 3:
            for (i = 0; i < r.qtd_itens; i++) {
                if (contemTexto(r.itens[i].nome_produto, textoBusca)) {
                    return 1;
                }
            }
            return 0;
        case 4:
            return strcmp(r.data, textoBusca) == 0;
        case 5:
            return r.tipo == valorNumerico;
    }
    return 0;
}

/* Opcao 4 do menu: consulta romaneios e permite marcar um romaneio como pago. */
void historicoRomaneios(void) {
    Romaneio lista[MAX_ROMANEIOS];
    int resultados[MAX_ROMANEIOS];
    int qtd, qtdRes = 0, criterio, valorNumerico = 0;
    int i, numero, posicao, encontrado, opcao;
    char textoBusca[TAM_NOME] = "";
    char tipo[10], situacao[10];

    printf("\n=== HISTORICO DE ROMANEIOS ===\n");
    qtd = carregarRomaneios(lista);

    if (qtd == 0) {
        printf("Nenhum romaneio registrado ainda.\n");
        return;
    }

    printf("Consultar por:\n");
    printf("  1 - Numero do romaneio\n");
    printf("  2 - Produtor\n");
    printf("  3 - Produto\n");
    printf("  4 - Data\n");
    printf("  5 - Tipo de operacao (compra ou venda)\n");
    printf("  0 - Voltar\n");
    criterio = lerInteiro("Opcao: ", 0, 5);

    switch (criterio) {
        case 0:
            return;
        case 1:
            valorNumerico = lerInteiro("Numero do romaneio: ", 1, 99999);
            break;
        case 2:
            lerTexto("Nome (ou parte do nome) do produtor: ", textoBusca, TAM_NOME);
            break;
        case 3:
            lerTexto("Nome (ou parte do nome) do produto: ", textoBusca, TAM_NOME);
            break;
        case 4:
            lerData("Data (DD/MM/AAAA): ", textoBusca);
            break;
        case 5:
            valorNumerico = lerInteiro("Tipo (1 - Compra, 2 - Venda): ", 1, 2);
            break;
    }

    /* Monta a lista de resultados (posicoes no vetor) */
    for (i = 0; i < qtd; i++) {
        if (romaneioCorresponde(lista[i], criterio, valorNumerico, textoBusca)) {
            resultados[qtdRes] = i;
            qtdRes++;
        }
    }

    if (qtdRes == 0) {
        printf("\nNenhum romaneio encontrado para a busca informada.\n");
        return;
    }

    printf("\n%d romaneio(s) encontrado(s):\n", qtdRes);
    imprimirLinha('-', 78);
    printf("%-8s %-12s %-8s %-20s %-14s %s\n", "Numero", "Data", "Tipo", "Destino", "Valor final", "Situacao");
    imprimirLinha('-', 78);
    for (i = 0; i < qtdRes; i++) {
        posicao = resultados[i];
        textoTipo(lista[posicao].tipo, tipo);
        textoSituacao(lista[posicao].situacao, situacao);
        printf("%-8d %-12s %-8s %-20.20s R$ %-11.2f %s\n",
               lista[posicao].numero, lista[posicao].data, tipo,
               lista[posicao].destino, lista[posicao].valor_final, situacao);
    }
    imprimirLinha('-', 78);

    /* Detalhes e alteracao da situacao de pagamento */
    do {
        numero = lerInteiro("\nNumero do romaneio para ver detalhes (0 para voltar): ", 0, 99999);

        if (numero != 0) {
            posicao = buscarRomaneio(lista, qtd, numero);
            encontrado = 0;
            for (i = 0; i < qtdRes; i++) {
                if (resultados[i] == posicao) {
                    encontrado = 1;
                }
            }

            if (!encontrado) {
                printf("Erro: esse numero nao esta na lista de resultados.\n");
            } else {
                exibirRomaneio(lista[posicao]);

                if (lista[posicao].situacao == PENDENTE) {
                    opcao = lerInteiro("Marcar como PAGO? (1 - Sim, 2 - Nao): ", 1, 2);
                    if (opcao == 1) {
                        lista[posicao].situacao = PAGO;
                        if (salvarRomaneios(lista, qtd)) {
                            printf("Situacao atualizada para PAGO.\n");
                        }
                    }
                }
            }
        }
    } while (numero != 0);
}
