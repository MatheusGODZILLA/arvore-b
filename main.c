#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void exibirMenu() {
  printf("\nMenu:\n");
  printf("1. Inserir elemento\n");
  printf("2. Buscar elemento\n");
  printf("3. Remover elemento\n");
  printf("4. Imprimir Árvore\n");
  printf("5. Sair\n");
  printf("Escolha uma opção: ");
}

void medirTempoInsercao(ArvoreB *arvore, int valor) {
  clock_t inicio, fim;
  double tempo_gasto;

  inicio = clock();
  inserir(arvore, valor);
  fim = clock();

  tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de execução para inserção: %f segundos\n", tempo_gasto);
}

void medirTempoBusca(ArvoreB *arvore, int valor) {
  clock_t inicio, fim;
  double tempo_gasto;

  inicio = clock();
  No *resultado = buscar(arvore->raiz, valor);
  fim = clock();

  tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de execução para busca: %f segundos\n", tempo_gasto);

  if (resultado != NULL) {
    printf("Valor %d encontrado na árvore.\n", valor);
  } else {
    printf("Valor %d não encontrado na árvore.\n", valor);
  }
}

void medirTempoRemocao(ArvoreB *arvore, int valor) {
  clock_t inicio, fim;
  double tempo_gasto;

  inicio = clock();
  remover(arvore, valor);
  fim = clock();

  tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de execução para remoção: %f segundos\n", tempo_gasto);
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
      medirTempoInsercao(arvore, valor);
      break;

    case 2:
      printf("Digite o valor a ser buscado: ");
      scanf("%d", &valor);
      medirTempoBusca(arvore, valor);
      break;

    case 3:
      printf("Digite o valor a ser removido: ");
      scanf("%d", &valor);
      medirTempoRemocao(arvore, valor);
      break;

    case 4:
      imprimirArvore(arvore->raiz, 0);
      break;

    case 5:
      liberarNo(arvore->raiz);
      free(arvore);
      printf("Saindo...\n");
      exit(EXIT_SUCCESS);
      break;

    default:
      printf("Opção inválida!\n");
    }
  }
  return 0;
}
