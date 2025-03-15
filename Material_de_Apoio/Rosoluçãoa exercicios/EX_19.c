#include <stdio.h>

// Definição do enum Status_do_Pedido
typedef enum {
    PENDENTE,
    EM_ANDAMENTO,
    CONCLUIDO
} Status_do_Pedido;

// Função para exibir mensagem com base no status do pedido
void exibirMensagem(Status_do_Pedido status) {
    switch (status) {
        case PENDENTE:
            printf("Seu pedido esta pendente. Aguarde o processamento.\n");
            break;
        case EM_ANDAMENTO:
            printf("Seu pedido esta sendo processado.\n");
            break;
        case CONCLUIDO:
            printf("Seu pedido foi concluido com sucesso!\n");
            break;
        default:
            printf("Status desconhecido.\n");
            break;
    }
}

int main() {
    Status_do_Pedido status = EM_ANDAMENTO;
    exibirMensagem(status);
    return 0;
}
