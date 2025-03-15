#include <stdio.h>

// Definição da struct Nota
struct Nota {
    float prova1;   // Nota da primeira prova
    float prova2;   // Nota da segunda prova
    float trabalho; // Nota do trabalho
};

// Função para calcular a média ponderada
float calcularMediaPonderada(struct Nota nota) {
    float pesoProva1 = 4.0;   // Peso da prova 1
    float pesoProva2 = 4.0;   // Peso da prova 2
    float pesoTrabalho = 2.0; // Peso do trabalho

    // Cálculo da média ponderada
    float media = (nota.prova1 * pesoProva1 + nota.prova2 * pesoProva2 + nota.trabalho * pesoTrabalho) / (pesoProva1 + pesoProva2 + pesoTrabalho);
    return media;
}

int main() {
    struct Nota notaAluno; // Variável para armazenar as notas do aluno

    // Solicita as notas do aluno
    printf("Digite as notas do aluno:\n");
    printf("Prova 1: ");
    scanf("%f", &notaAluno.prova1);
    printf("Prova 2: ");
    scanf("%f", &notaAluno.prova2);
    printf("Trabalho: ");
    scanf("%f", &notaAluno.trabalho);

    // Calcula a média ponderada
    float media = calcularMediaPonderada(notaAluno);

    // Exibe a média ponderada
    printf("\nMedia ponderada do aluno: %.2f\n", media);

    return 0;
}