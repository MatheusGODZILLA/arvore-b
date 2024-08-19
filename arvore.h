#ifndef ARVORE_H
#define ARVORE_H

// Ordem da Árvore B
#define ORDEM 3

typedef struct No {
  int chaves[ORDEM - 1];    // Chaves dos nós internos
  struct No *filhos[ORDEM]; // Filhos dos nós internos
  int numChaves;            // Número de chaves no nó
  int folha;                // Flag para indicar se o nó é folha
} No;

typedef struct ArvoreB {
  No *raiz;
} ArvoreB;

// Funções de manipulação da Árvore B
No *criarNo(int folha);
ArvoreB *criarArvoreB();
void dividirFilho(No *pai, int i, No *filho);
void inserirNaoCheio(No *no, int chave);
void inserir(ArvoreB *arvore, int chave);
No *buscar(No *no, int chave);
void imprimirArvore(No *no, int nivel);
void liberarNo(No *no);

void remover(ArvoreB *arvore, int chave);
void removerDeNo(No *no, int chave);
void removerDeFolha(No *no, int idx);
void removerDeNaoFolha(No *no, int idx);
int pegarAntecessor(No *no, int idx);
int pegarSucessor(No *no, int idx);
void preencher(No *no, int idx);
void pegarDoAnterior(No *no, int idx);
void pegarDoProximo(No *no, int idx);
void fundir(No *no, int idx);

#endif // ARVORE_H