#include <stdio.h>

int main() {
    float num1, num2, resultado;
    char operacao;

    // Solicita ao usuário que insira dois números e a operação desejada
    printf("Digite o primeiro numero: ");
    scanf("%f", &num1);

    printf("Digite o segundo numero: ");
    scanf("%f", &num2);

    printf("Escolha a operacao (+, -, *, /): ");
    scanf(" %c", &operacao); // O espaço antes do %c ignora espaços em branco

    // Realiza a operação escolhida usando switch case
    switch (operacao) {
        case '+':
            resultado = num1 + num2;
            printf("Resultado: %.2f\n", resultado);
            break;

        case '-':
            resultado = num1 - num2;
            printf("Resultado: %.2f\n", resultado);
            break;

        case '*':
            resultado = num1 * num2;
            printf("Resultado: %.2f\n", resultado);
            break;

        case '/':
            if (num2 != 0) { // Verifica se o divisor não é zero
                resultado = num1 / num2;
                printf("Resultado: %.2f\n", resultado);
            } else {
                printf("Erro: Divisao por zero nao e permitida.\n");
            }
            break;

        default:
            printf("Operacao invalida! Escolha entre +, -, *, ou /.\n");
            break;
    }

    return 0;
}