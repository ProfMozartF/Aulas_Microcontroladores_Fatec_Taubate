#include <stdio.h>

int main() {
    float nota;

    // Solicita ao usuário que insira uma nota
    printf("Digite a nota (0 a 10): ");
    scanf("%f", &nota);

    // Verifica se a nota está dentro do intervalo válido
    if (nota >= 0 && nota <= 10) {
        // Classifica a nota
        if (nota > 7) {
            printf("Aprovado!\n");
        } else if (nota >=5 && nota<=7) {
            printf("Recuperação.\n");
        } else {
            printf("Reprovado.\n");
        }
    } else {
        // Mensagem de erro se a nota estiver fora do intervalo válido
        printf("Nota invalida! A nota deve estar entre 0 e 10.\n");
    }

    return 0;
}