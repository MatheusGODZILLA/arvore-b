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
        int valor = rand() % (quantidade * 2);

        if (bsearch(&valor, valores, tamanho, sizeof(int), comparar) == NULL) {
            inserir(arvore, valor);
            valores[tamanho++] = valor;
            qsort(valores, tamanho, sizeof(int), comparar);
        }
    }

    free(valores);
}

void salvarArvoreNoBinario(No *no, FILE *arquivo) {
    if (no != NULL) {
        // Escreve se o nó é folha ou não
        fwrite(&no->folha, sizeof(int), 1, arquivo);

        // Escreve o número de chaves no nó
        fwrite(&no->numChaves, sizeof(int), 1, arquivo);

        // Escreve as chaves
        fwrite(no->chaves, sizeof(int), no->numChaves, arquivo);

        // Se não for folha, grava os filhos
        if (!no->folha) {
            for (int i = 0; i <= no->numChaves; i++) {
                salvarArvoreNoBinario(no->filhos[i], arquivo);
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

// Função para imprimir a árvore B
void imprimirArvoreB(No *no, int nivel) {
    if (no != NULL) {
        // Imprime a indentação correspondente ao nível
        for (int i = 0; i < nivel; i++) {
            printf("    "); // Indenta de acordo com o nível
        }

        // Imprime as chaves do nó atual
        printf("| ");
        for (int i = 0; i < no->numChaves; i++) {
            printf("%d ", no->chaves[i]);
        }
        printf("|\n");

        // Se não for folha, imprime os filhos com um nível maior de indentação
        if (!no->folha) {
            for (int i = 0; i <= no->numChaves; i++) {
                imprimirArvoreB(no->filhos[i], nivel + 1);
            }
        }
    }
}


int main() {
    ArvoreB *arvore = criarArvoreB();
    int quantidade;

    printf("Digite a quantidade de numeros a serem inseridos na arvore: ");
    scanf("%d", &quantidade);

    FILE *arquivo = fopen("arvore.bin", "wb");
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

    // Imprime a árvore B gerada na tela
    printf("\nEstrutura da Arvore B:\n");
    imprimirArvoreB(arvore->raiz, 0);

    // Salva a árvore no arquivo binário
    salvarArvoreNoBinario(arvore->raiz, arquivo);

    fclose(arquivo);

    size_t memoria_usada = calcularUsoMemoria(arvore->raiz);
    printf("Uso de memoria da arvore: %zu bytes\n", memoria_usada);

    liberarNo(arvore->raiz);
    free(arvore);

    printf("Estrutura da arvore gravada no arquivo arvore.bin\n");
    return 0;
}
