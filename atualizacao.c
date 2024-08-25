#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

// Função para buscar o nó que contém a chave
No* buscarNoParaAtualizacao(No* no, int chave) {
    int i = 0;
    while (i < no->numChaves && chave > no->chaves[i]) {
        i++;
    }
    if (i < no->numChaves && chave == no->chaves[i]) {
        return no; // Encontrou o nó que contém a chave
    }
    if (no->folha) {
        return NULL; // A chave não está na árvore
    }
    return buscarNoParaAtualizacao(no->filhos[i], chave);
}

// Função para ler a árvore do arquivo
void carregarArvoreDoArquivo(ArvoreB *arvore, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    int chave;
    while (fscanf(arquivo, "%d", &chave) != EOF) {
        inserir(arvore, chave);
    }

    fclose(arquivo);
}

// Função para salvar a árvore no arquivo
void salvarArvoreNoArquivo(No *no, FILE *arquivo) {
    if (no != NULL) {
        for (int i = 0; i < no->numChaves; i++) {
            fprintf(arquivo, "%d\n", no->chaves[i]);
        }
        if (!no->folha) {
            for (int i = 0; i <= no->numChaves; i++) {
                salvarArvoreNoArquivo(no->filhos[i], arquivo);
            }
        }
    }
}

// Função para atualizar uma chave existente
void atualizarChave(ArvoreB *arvore, int chaveAntiga, int chaveNova, const char *nomeArquivo) {
    // Carregar a árvore do arquivo
    carregarArvoreDoArquivo(arvore, nomeArquivo);

    No *no = buscarNoParaAtualizacao(arvore->raiz, chaveAntiga);

    if (no != NULL) {
        // Verifica se a nova chave já existe na árvore (para evitar duplicatas)
        No *noExistente = buscarNoParaAtualizacao(arvore->raiz, chaveNova);
        if (noExistente != NULL) {
            printf("A chave %d já existe na árvore. Atualização cancelada.\n", chaveNova);
            return;
        }

        // Remove a chave antiga
        remover(arvore, chaveAntiga);
        // Insere a nova chave
        inserir(arvore, chaveNova);

        // Salvar a árvore atualizada no arquivo
        FILE *arquivo = fopen(nomeArquivo, "w");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo para escrita!\n");
            return;
        }

        salvarArvoreNoArquivo(arvore->raiz, arquivo);
        fclose(arquivo);

        printf("Chave %d atualizada para %d com sucesso.\n", chaveAntiga, chaveNova);
    } else {
        printf("Chave %d não encontrada na árvore.\n", chaveAntiga);
    }
}

int main() {
    ArvoreB *arvore = criarArvoreB();
    const char *nomeArquivo = "arvore.txt";

    int chaveAntiga, chaveNova;

    printf("Digite a chave a ser atualizada: ");
    scanf("%d", &chaveAntiga);
    printf("Digite a nova chave: ");
    scanf("%d", &chaveNova);

    atualizarChave(arvore, chaveAntiga, chaveNova, nomeArquivo);

    // Liberar memória
    liberarNo(arvore->raiz);
    free(arvore);

    return 0;
}
