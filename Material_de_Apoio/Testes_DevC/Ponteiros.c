#include <stdio.h>
#include <conio.h>
int main(void)
{
  //valor é a variável que
  //será apontada pelo ponteiro
  int valor = 27;
  
  //declaração de variável ponteiro
  int *ptr;
  
  //atribuindo o endereço da variável valor ao ponteiro
  ptr = &valor;
  
  printf("Utilizando ponteiros\n\n");
  printf ("Conteudo da variavel valor: %d\n", valor);
  printf ("Endereço da variavel valor: %X \n", &valor);
  printf ("Conteudo da variavel ponteiro ptr: %X\n", ptr);
  printf ("Valor da variavel apontada pelo ponteiro: %d", *ptr);
  
  getch();
  return(0);
}