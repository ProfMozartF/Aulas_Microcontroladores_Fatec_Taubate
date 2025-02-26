#include <stdio.h>

int main() {
    int opcao;
    printf("Escolha uma opção (1 a 3): ");
    scanf("%d", &opcao);
    
    switch(opcao) {
        case 1:
            printf("Opção 1 escolhida.\n");
            break;
        case 2:
            printf("Opção 2 escolhida.\n");
            break;
        case 3:
            printf("Opção 3 escolhida.\n");
            break;
        default:
            printf("Opção inválida.\n");
    }
    return 0;
}