#include <stdio.h>

int main() {
    int numero;
    printf("TENTE ADVINHAR O NUMERO...\n\n");
    printf("Digite um numero positivo de 0 a 10: \n");

    do {
        scanf("%d", &numero); // Lê o número digitado pelo usuário

        if (numero != 7) { // Verifica se o número é inválido
            printf("Tente outro Numero.\n");
        }
    } while (numero != 7); // Repete enquanto o número for inválido

    printf("Você encontrou o numero: %d\n", numero);

    return 0;
}