#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

// Função para carregar um nó do arquivo binário
No* carregarNoDeBinario(FILE *arquivo) {
    No *no = (No *)malloc(sizeof(No));
    if (no == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó.\n");
        exit(EXIT_FAILURE);
    }

    // Lê se o nó é uma folha
    fread(&no->folha, sizeof(int), 1, arquivo);

    // Lê o número de chaves no nó
    fread(&no->numChaves, sizeof(int), 1, arquivo);

    // Lê as chaves
    fread(no->chaves, sizeof(int), no->numChaves, arquivo);

    // Inicializa os filhos
    for (int i = 0; i < ORDEM; i++) {
        no->filhos[i] = NULL;
    }

    // Se não for folha, lê os filhos
    if (!no->folha) {
        for (int i = 0; i <= no->numChaves; i++) {
            no->filhos[i] = carregarNoDeBinario(arquivo);
        }
    }

    return no;
}

// Função para carregar a árvore do arquivo binário
void carregarArvoreDoArquivoBinario(ArvoreB *arvore, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo binário!\n");
        return;
    }

    arvore->raiz = carregarNoDeBinario(arquivo);
    fclose(arquivo);
}

// Função para salvar a árvore no arquivo binário
void salvarArvoreNoArquivoBinario(No *no, FILE *arquivo) {
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
                salvarArvoreNoArquivoBinario(no->filhos[i], arquivo);
            }
        }
    }
}

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

// Função para atualizar uma chave existente
void atualizarChaveBinario(ArvoreB *arvore, int chaveAntiga, int chaveNova, const char *nomeArquivo) {
    // Carregar a árvore do arquivo binário
    carregarArvoreDoArquivoBinario(arvore, nomeArquivo);

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

        // Salvar a árvore atualizada no arquivo binário
        FILE *arquivo = fopen(nomeArquivo, "wb");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo binário para escrita!\n");
            return;
        }

        salvarArvoreNoArquivoBinario(arvore->raiz, arquivo);
        fclose(arquivo);

        printf("Chave %d atualizada para %d com sucesso.\n", chaveAntiga, chaveNova);
    } else {
        printf("Chave %d não encontrada na árvore.\n", chaveAntiga);
    }
}

int main() {
    ArvoreB *arvore = criarArvoreB();
    const char *nomeArquivo = "arvore.bin";

    int chaveAntiga, chaveNova;

    printf("Digite a chave a ser atualizada: ");
    scanf("%d", &chaveAntiga);
    printf("Digite a nova chave: ");
    scanf("%d", &chaveNova);

    atualizarChaveBinario(arvore, chaveAntiga, chaveNova, nomeArquivo);

    // Liberar memória
    liberarNo(arvore->raiz);
    free(arvore);

    return 0;
}
