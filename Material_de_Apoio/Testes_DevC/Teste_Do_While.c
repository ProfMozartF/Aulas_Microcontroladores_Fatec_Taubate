#include <stdio.h>

int main() {
    int numero;
    do {
        printf("Digite um número positivo: ");
        scanf("%d", &numero);
    } while (numero < 0);
    return 0;
}