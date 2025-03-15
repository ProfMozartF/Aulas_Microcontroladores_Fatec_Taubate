#include <stdio.h>

// Função para calcular o fatorial de um número
unsigned long calcularFatorial(int num);

int main() 
{
    int numero;

    // Solicita ao usuário um número para calcular o fatorial
    printf("Digite um numero inteiro positivo: ");
    scanf("%d", &numero);

    // Verifica se o número é válido (não negativo)
    if (numero < 0) {
        printf("Erro: Fatorial de número negativo não é definido.\n");
    }
    else if (numero > 10) {
        printf("Vamos tentar um numero menor.\n");
    }
    else {
        // Chama a função e exibe o resultado
        unsigned long resultado = calcularFatorial(numero);
        printf("O fatorial de %d: %lu\n", numero, resultado);
    }

    return 0;
}


unsigned long calcularFatorial(int num) {
    unsigned long fatorial=num;
    int num_decrementado=num-1;
    int i;

    // Calcula o fatorial
    for (i = num; i >0; i--) {
        printf("Num_Dec: %d\n", num_decrementado);
        fatorial *= num_decrementado; // Multiplica o fatorial
        if(num_decrementado==1){break;}//BREAK Sai do laço executado
        num_decrementado = num_decrementado -1;
        
    }

    return fatorial; // Retorna o resultado
}