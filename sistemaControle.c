#include <stdio.h>
#include <stdlib.h>
#include "estruturas_e_funcoes.h"
#include <time.h>

int main(void) {
    // Declaração das estruturas vazias
    NoAguardando *inicioAguardando = NULL;
    NoAguardando *fimAguardando = NULL;
    NoComprador *inicioCompradores = NULL;
    NoComprador *fimCompradores = NULL;
    NoArquivado *topoArquivados = NULL;

    popularDados(&inicioAguardando, &fimAguardando);

    int resposta;
    int continuar = 1;

    while (continuar == 1) {
        printf("\n=== Sistema de Controle de Captação de Clientes ===\n");
        printf("1. Cadastrar cliente\n");
        printf("2. Realizar contato\n");
        printf("3. Relatorios\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &resposta);
        getchar();

        switch (resposta) {
            case 1:
                // LARA
                cadastrarCliente(&inicioAguardando, &fimAguardando);
                break;

            case 2:
                // VINICIUS = REALIZAR CONTATO E LUIZ = FINALIZAR CONTATO(OPÇÃO 4)
                realizarContato(&inicioAguardando, &fimAguardando,
                                &inicioCompradores, &fimCompradores,
                                &topoArquivados);
                break;
            case 3:
                // ANA
                relatorios(inicioAguardando);
                break;

            case 4:
                continuar = 0;
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
                break;
        }
    }
    return 0;
}
