#include <stdio.h>

int main() 
{
    int numero;
    printf("Digite um numero inteiro: ");
    scanf("%d", &numero);//aguarda o valor ser informado

    // Verifica se o número é par ou ímpar
    if (numero % 2 == 0) // usar o operador que retorna o resto da divisão
    {
        printf("O numero %d e par.\n", numero);
    } else 
    {
        printf("O numero %d e impar.\n", numero);
    }

    return 0;
}