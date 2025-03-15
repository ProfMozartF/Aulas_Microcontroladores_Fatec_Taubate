#include <stdio.h>
#include <string.h>

// Definição da struct Aluno
struct Aluno {
    char nome[50]; // Campo para armazenar o nome do aluno
    int idade;     // Campo para armazenar a idade do aluno
    float nota;    // Campo para armazenar a nota do aluno
};

int main() {
    struct Aluno alunos[3]; // Vetor para armazenar 3 alunos
    int i;

    // Cadastro dos alunos
    printf("Cadastro de alunos:\n");
    for (i = 0; i < 3; i++) {
        printf("\nAluno %d:\n", i + 1);

        // Solicita o nome do aluno
        printf("Nome: ");
        scanf(" %[^\n]", alunos[i].nome); // Lê uma string com espaços

        // Solicita a idade do aluno
        printf("Idade: ");
        scanf("%d", &alunos[i].idade);

        // Solicita a nota do aluno
        printf("Nota: ");
        scanf("%f", &alunos[i].nota);
    }

    // Exibição dos alunos cadastrados
    printf("\nInformacoes dos alunos cadastrados:\n");
    for (i = 0; i < 3; i++) {
        printf("\nAluno %d:\n", i + 1);
        printf("Nome: %s\n", alunos[i].nome);
        printf("Idade: %d anos\n", alunos[i].idade);
        printf("Nota: %.2f\n", alunos[i].nota);
    }

    return 0;
}