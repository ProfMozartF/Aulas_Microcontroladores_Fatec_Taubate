#include <stdio.h>

void trocarValores(int *a, int *b);

int main() {
    int num1, num2;

    // Solicita ao usuário dois números inteiros
    printf("Digite o primeiro numero: ");
    scanf("%d", &num1);
    printf("Digite o segundo numero: ");
    scanf("%d", &num2);

    // Exibe os valores antes da troca
    printf("Antes da troca: num1 = %d, num2 = %d\n", num1, num2);

    // Chama a função para trocar os valores
    trocarValores(&num1, &num2);

    // Exibe os valores após a troca
    printf("Apos a troca: num1 = %d, num2 = %d\n", num1, num2);

    return 0;
}

// Função para trocar os valores de dois números inteiros
void trocarValores(int *a, int *b) {
    int temp = *a; // Armazena o valor de a em uma variável temporária
    *a = *b;       // Atribui o valor de b a a
    *b = temp;     // Atribui o valor temporário (antigo valor de a) a b
}