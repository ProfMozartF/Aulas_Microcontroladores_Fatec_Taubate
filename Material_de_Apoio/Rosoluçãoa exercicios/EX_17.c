#include <stdio.h>

int main() {
    int vetor[5] = {10, 20, 30, 40, 50}; // Vetor de 5 elementos
    int *ptr; // Ponteiro para inteiro
    int i;

    // Inicializa o ponteiro para apontar para o início do vetor
    ptr = vetor;

    // Usa o ponteiro para acessar e imprimir cada elemento do vetor
    printf("Elementos do vetor:\n");
    for (i = 0; i < 5; i++) {
        printf("Elemento %d: %d\n", i + 1, *(ptr + i)); // Acessa o elemento usando ponteiros
    }
    
    //Acessa o endereco do vetor
    for (i = 0; i < 5; i++) {
        printf("End. Vetor indice [%d]: %X\n",i, &vetor[i]); 
    }

     //Acessa o valor armazenado no ponteiro
     for (i = 0; i < 5; i++) {
        printf("valor armazenado no ponteiro [%d]: %X\n",i+1, ptr); 
        ptr++;
    }

    

    return 0;
}