#include <stdio.h>
#include "romaneio.h"

int main(void) {
    int opcao;

    do {
        printf("\n===== ROMANEIO DE UVAS =====\n");
        printf("1. Registrar Romaneio\n");
        printf("2. Historico de Romaneios\n");
        printf("3. Sair\n");
        opcao = lerInteiro("Escolha uma opcao: ", 1, 3);

        switch (opcao) {
            case 1:
                registrarRomaneio();
                break;
            case 2:
                historicoRomaneios();
                break;
            case 3:
                printf("Encerrando o sistema.\n");
                break;
        }
    } while (opcao != 3);

    return 0;
}
