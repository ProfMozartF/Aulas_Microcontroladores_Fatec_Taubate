#include <stdio.h>

int main() {
    int vetor[5]; // Declara um vetor de 5 elementos inteiros
    int soma = 0; // Variável para armazenar a soma
    int i;

    // Solicita ao usuário os valores para preencher o vetor
    printf("Digite 5 numeros inteiros:\n");
    for (i = 0; i < 5; i++) {
        printf("Elemento %d: ", i );
        scanf("%d", &vetor[i]); // Lê o valor e armazena no vetor
    }

    // Calcula a soma dos elementos do vetor
    for (i = 0; i < 5; i++) {
        soma += vetor[i]; // Adiciona cada elemento à soma
    }

    printf("Vetor = [");

     for (i = 0; i < 5; i++) {
        printf("\t%d",vetor[i]); // Exibe o vetor
    }
    printf("\t]\n");

    // Exibe o resultado da soma
    printf("A soma dos elementos do vetor e: %d\n", soma);

    return 0;
}