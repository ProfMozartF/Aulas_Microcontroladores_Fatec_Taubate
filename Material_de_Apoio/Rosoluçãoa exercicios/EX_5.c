#include <stdio.h>

int main() {
    int contador = 10; // Inicializa o contador com 10

    while (contador >= 0) { // Enquanto o contador for maior ou igual a 0
        printf("CONTAGEM ATUAL:\t %d\n", contador); // Exibe o valor atual do contador
        contador--; // Decrementa o contador
    }

    printf("FIM DA CONTAGEM!\n"); // Mensagem final

    return 0;
}