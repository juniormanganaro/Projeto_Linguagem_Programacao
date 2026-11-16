/*
 * Sistema de Gerenciamento de Compra de Uvas por Romaneio
 * Arquivo principal: menu e chamada das funcoes dos modulos.
 */
#include <stdio.h>
#include "utils.h"
#include "produtor.h"
#include "produto.h"
#include "romaneio.h"

void exibirMenu(void) {
    printf("\n");
    imprimirLinha('=', 50);
    printf("   SISTEMA DE ROMANEIO DE UVAS\n");
    imprimirLinha('=', 50);
    printf("  1. Cadastrar Produtor\n");
    printf("  2. Cadastrar Produto\n");
    printf("  3. Registrar Compra\n");
    printf("  4. Historico de Romaneios\n");
    printf("  5. Listar Produtores\n");
    printf("  6. Listar Produtos\n");
    printf("  7. Sair\n");
    imprimirLinha('-', 50);
}

int main(void) {
    int opcao;

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ", 1, 7);

        switch (opcao) {
            case 1:
                cadastrarProdutor();
                pausar();
                break;
            case 2:
                cadastrarProduto();
                pausar();
                break;
            case 3:
                registrarRomaneio();
                pausar();
                break;
            case 4:
                historicoRomaneios();
                break;
            case 5:
                listarProdutores();
                pausar();
                break;
            case 6:
                listarProdutos();
                pausar();
                break;
            case 7:
                printf("\nEncerrando o sistema. Ate logo!\n");
                break;
        }
    } while (opcao != 7);

    return 0;
}
