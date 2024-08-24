#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void inserirLote(ArvoreB *arvore, int quantidade) {
    srand(time(NULL));

    for (int i = 0; i < quantidade; i++) {
        int valor = rand() % 50000;
        inserir(arvore, valor);
    }
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

int main() {
    ArvoreB *arvore = criarArvoreB();
    int quantidade = 25000;

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

    liberarNo(arvore->raiz);
    free(arvore);

    printf("Estrutura da arvore gravada no arquivo arvore.txt\n");
    return 0;
}
