#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

void exibirMenu() {
  printf("\nMenu:\n");
  printf("1. Inserir elemento\n");
  printf("2. Buscar elemento\n");
  printf("3. Remover elemento\n");
  printf("4. Imprimir Árvore\n");
  printf("5. Sair\n");
  printf("Escolha uma opção: ");
}

int main() {
  ArvoreB *arvore = criarArvoreB();
  int opcao, valor;

  while (1) {
    exibirMenu();
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Digite o valor a ser inserido: ");
      scanf("%d", &valor);
      inserir(arvore, valor);
      break;

    case 2:
      printf("Digite o valor a ser buscado: ");
      scanf("%d", &valor);
      No *resultado = buscar(arvore->raiz, valor);
      if (resultado != NULL) {
        printf("Valor %d encontrado na árvore.\n", valor);
      } else {
        printf("Valor %d não encontrado na árvore.\n", valor);
      }
      break;

    case 3:
      printf("Digite o valor a ser removido: ");
      scanf("%d", &valor);
      remover(arvore, valor);
      break;

    case 4:
      imprimirArvore(arvore->raiz, 0);
      break;

    case 5:
      liberarNo(arvore->raiz);
      free(arvore);
      printf("Saindo...\n");
      exit(EXIT_SUCCESS);

    default:
      printf("Opção inválida!\n");
    }
  }
  return 0;
}