#include <stdio.h>
#include "arvore.h"
#include <stdlib.h>

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