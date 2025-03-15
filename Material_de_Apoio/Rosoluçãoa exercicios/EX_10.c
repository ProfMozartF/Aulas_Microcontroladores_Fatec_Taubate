#include <stdio.h>

int main() {
    float numeroReal;
    int numeroInteiro;

    // Solicita ao usuário um número real
    printf("Digite um numero int: ");
    scanf("%d", &numeroInteiro);

    // Converte o número real para inteiro usando cast
    numeroReal = (float)numeroInteiro;

    // Exibe ambos os valores
    printf("Numero int digitado: %d\n", numeroInteiro);
    printf("Numero real convertido: %.2f\n", numeroReal);

    return 0;
}