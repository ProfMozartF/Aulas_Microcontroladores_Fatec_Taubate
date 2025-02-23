#include <stdio.h>

int main() {
	int numero;
    printf("Digite um número positivo: ");
        scanf("%d", &numero);
	
	while (numero >= 0) {
	    printf("Contagem regressiva= %d\n",numero);
	    numero--;

	}
	return 0;
}
