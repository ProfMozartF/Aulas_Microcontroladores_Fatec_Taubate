#include <stdio.h>

int soma(int a, int b); // Protótiopo da função

int main() {
    int resultado = soma(3, 4);
    printf("Soma: %d\n", resultado);
    return 0;
}

int soma(int a, int b) {
    return a + b;
}