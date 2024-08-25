#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int comparar(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void inserirLote(ArvoreB *arvore, int quantidade) {
    int *valores = (int *)malloc(quantidade * sizeof(int));
    int tamanho = 0;

    srand(time(NULL));

    while (tamanho < quantidade) {
        int valor = rand() % 100;

        if (bsearch(&valor, valores, tamanho, sizeof(int), comparar) == NULL) {
            inserir(arvore, valor);
            valores[tamanho++] = valor;
            qsort(valores, tamanho, sizeof(int), comparar);
        }
    }

    free(valores);
}

void imprimirArvoreNoArquivo(No *no, int nivel, FILE *arquivo) {
    if (no != NULL) {
        fprintf(arquivo, "%*sNivel %d: [", nivel * 4, "", nivel);
        for (int i = 0; i < no->numChaves; i++) {
            fprintf(arquivo, "%d", no->chaves[i]);
            if (i < no->numChaves - 1) {
                fprintf(arquivo, ", ");
            }
        }
        fprintf(arquivo, "]\n");

        if (!no->folha) {
            for (int i = 0; i <= no->numChaves; i++) {
                imprimirArvoreNoArquivo(no->filhos[i], nivel + 1, arquivo);
            }
        }
    }
}

size_t calcularUsoMemoria(No *no) {
    if (no == NULL)
        return 0;

    size_t memoria = sizeof(No) + (no->numChaves * sizeof(int)) + (ORDEM * sizeof(No *));

    if (!no->folha) {
        for (int i = 0; i <= no->numChaves; i++) {
            memoria += calcularUsoMemoria(no->filhos[i]);
        }
    }
    return memoria;
}

int main() {
    ArvoreB *arvore = criarArvoreB();
    int quantidade;

    printf("Digite a quantidade de números a serem inseridos na árvore: ");
    scanf("%d", &quantidade);

    FILE *arquivo = fopen("arvore.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    clock_t inicio, fim;
    double tempo_gasto;

    inicio = clock();
    inserirLote(arvore, quantidade);
    fim = clock();

    tempo_gasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo de execucao para insercao de %d numeros: %f ms\n", quantidade, tempo_gasto);

    imprimirArvoreNoArquivo(arvore->raiz, 0, arquivo);

    fclose(arquivo);

    size_t memoria_usada = calcularUsoMemoria(arvore->raiz);
    printf("Uso de memoria da arvore: %zu bytes\n", memoria_usada);

    liberarNo(arvore->raiz);
    free(arvore);

    printf("Estrutura da arvore gravada no arquivo arvore.txt\n");
    return 0;
}
