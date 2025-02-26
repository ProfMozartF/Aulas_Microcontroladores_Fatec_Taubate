#include <stdio.h>

// Função recursiva para calcular o n-ésimo termo da sequência de Fibonacci
int fibonacci(int n) {
    // Caso base: F(0) = 0
    if (n == 0) {
        return 0;
    }
    // Caso base: F(1) = 1
    if (n == 1) {
        return 1;
    }
    // Chamada recursiva para calcular F(n) = F(n-1) + F(n-2)
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n;

    // Solicita ao usuário o valor de n
    printf("Digite o valor de n para calcular o n-ésimo termo da sequência de Fibonacci: ");
    scanf("%d", &n);

    // Verifica se o valor de n é válido
    if (n < 0) {
        printf("O valor de n deve ser um número inteiro não negativo.\n");
    } else {
        // Chama a função fibonacci e exibe o resultado
        printf("O %d-ésimo termo da sequência de Fibonacci é: %d\n", n, fibonacci(n));
    }

    return 0;
}