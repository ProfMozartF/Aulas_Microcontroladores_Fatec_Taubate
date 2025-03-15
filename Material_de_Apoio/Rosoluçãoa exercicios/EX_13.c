#include <stdio.h>

// Função para ordenar o vetor usando Selection Sort
void selectionSort(int vetor[], int tamanho) {
    int indiceMenor;
    int temp; 
    int i, j; 

    for (i = 0; i < tamanho - 1; i++) {
        indiceMenor = i; // Assume que o menor elemento está na posição i

        // Encontra o índice do menor elemento no restante do vetor
        for (j = i + 1; j < tamanho; j++) {
            if (vetor[j] < vetor[indiceMenor]) {
                indiceMenor = j; // Atualiza o índice do menor elemento
            }
        }

        // Troca o menor elemento encontrado com o elemento na posição i
        if (indiceMenor != i) {
            temp = vetor[i];
            vetor[i] = vetor[indiceMenor];
            vetor[indiceMenor] = temp;
        }
    }
}

int main() {
    int vetor[10]; // Vetor de 10 elementos
    int tamanho = 10; // Tamanho do vetor
    int i;

    // Solicita ao usuário os valores para preencher o vetor
    printf("Digite 10 numeros inteiros:\n");
    for (i = 0; i < tamanho; i++) {
        printf("Elemento %d: ", i + 1);
        scanf("%d", &vetor[i]); // Lê o valor e armazena no vetor
    }

    // Chama a função para ordenar o vetor
    selectionSort(vetor, tamanho);

    // Exibe o vetor ordenado
    printf("Vetor ordenado em ordem crescente:\n");
    for (i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    return 0;
}