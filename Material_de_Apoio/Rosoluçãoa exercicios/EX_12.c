#include <stdio.h>

int main() {
    int vetor[10]; // Declara um vetor de 10 elementos inteiros
    int maior;     // Variável para armazenar o maior valor
    int i;

    // Solicita ao usuário os valores para preencher o vetor
    printf("Digite 10 numeros inteiros:\n");
    for (i = 0; i < 10; i++) {
        printf("Elemento %d: ", i );
        scanf("%d", &vetor[i]); // Lê o valor e armazena no vetor
    }

    // Assume que o primeiro elemento é o maior inicialmente
    maior = vetor[0];

    // Percorre o vetor para encontrar o maior valor
    for (i = 0; i < 10; i++) {
        if (vetor[i] > maior) { // Se o elemento atual for maior que "maior"
            maior = vetor[i];  // Atualiza o valor de "maior"
        }
    }

    // Exibe o maior valor encontrado
    printf("O maior valor no vetor e: %d\n", maior);

    return 0;
}