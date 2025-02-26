#include <stdio.h>

struct Pessoa {
    char nome[50];
    int idade;
    float peso;
};

int main() {
    struct Pessoa pessoa_1 = {"Carlos", 25, 85.3};
    printf("Nome: %s, Idade: %d, Peso: %.2f\n", pessoa_1.nome, pessoa_1.idade,pessoa_1.peso);
    return 0;
}