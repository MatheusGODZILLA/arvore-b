#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

// Função para criar um novo nó
No *criarNo(int folha) {
  No *no = (No *)malloc(sizeof(No));
  if (no == NULL) {
    fprintf(stderr, "Erro ao alocar memória para o nó.\n");
    exit(EXIT_FAILURE); 
  }
  no->numChaves = 0;
  no->folha = folha;

  // Inicializar os filhos e chaves
  for (int i = 0; i < ORDEM - 1; i++) {
    no->chaves[i] = 0;
  }
  for (int i = 0; i < ORDEM; i++) {
    no->filhos[i] = NULL;
  }

  return no;
}

// Função para criar uma nova Árvore B
ArvoreB *criarArvoreB() {
  ArvoreB *arvore = (ArvoreB *)malloc(sizeof(ArvoreB));
  if (arvore == NULL) {
    fprintf(stderr, "Erro ao alocar memória para a árvore.\n");
    exit(EXIT_FAILURE);
  }
  arvore->raiz = criarNo(1);
  return arvore;
}

// Função para dividir um nó filho
void dividirFilho(No *pai, int i, No *filho) {
    int t = (ORDEM+1) / 2;
    
    No *novoFilho = criarNo(filho->folha);
    novoFilho->numChaves = ORDEM - t - 1;
    
    for (int j = 0; j < t - 1; j++) {
        novoFilho->chaves[j] = filho->chaves[j + t];
    }
    
    if (!filho->folha) {
        for (int j = 0; j < t; j++) {
            novoFilho->filhos[j] = filho->filhos[j + t];
        }
    }
    
    filho->numChaves = t - 1;  // Reduzir o número de chaves em filho
    
    for (int j = pai->numChaves; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }
    
    pai->filhos[i + 1] = novoFilho;
    
    // Mover as chaves de pai para abrir espaço para a chave do meio de filho
    for (int j = pai->numChaves - 1; j >= i; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }
    
    pai->chaves[i] = filho->chaves[t - 1];
    
    pai->numChaves++;
}

// Função para inserir uma chave em um nó não cheio
void inserirNaoCheio(No *no, int chave) {
    int i = no->numChaves - 1;
    
    if (no->folha) {
        while (i >= 0 && no->chaves[i] > chave) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->numChaves++;
    } else {
        // Encontrar o filho que receberá a nova chave
        while (i >= 0 && no->chaves[i] > chave) {
            i--;
        }
        i++;        
        // Se o filho encontrado está cheio, divida-o
        if (no->filhos[i]->numChaves == ORDEM - 1) {
            dividirFilho(no, i, no->filhos[i]);
            
            if (no->chaves[i] < chave) {
                i++;
            }
        }
        inserirNaoCheio(no->filhos[i], chave);
    }
}

// Função para inserir uma chave na árvore B
void inserir(ArvoreB *arvore, int chave) {
    No *raiz = arvore->raiz;
    
    // Se a raiz está cheia, a árvore deve crescer em altura
    if (raiz->numChaves == ORDEM - 1) {
        No *novoNo = criarNo(0);
        novoNo->filhos[0] = raiz;
        arvore->raiz = novoNo;
        dividirFilho(novoNo, 0, raiz);
        inserirNaoCheio(novoNo, chave);
    } else {
        inserirNaoCheio(raiz, chave);
    }
}


No *buscar(No *no, int chave) {
  int i = 0;
  while (i < no->numChaves && chave > no->chaves[i]) {
    i++;
  }
  if (i < no->numChaves && chave == no->chaves[i]) {
    return no;
  }
  if (no->folha) {
    return NULL;
  }
  return buscar(no->filhos[i], chave);
}

// Função para imprimir a árvore B
void imprimirArvore(No *no, int nivel) {
  if (no != NULL) {
    // Imprimir as chaves no nível atual
    printf("%*s", nivel * 4, ""); // Adicionar indentação
    printf("Nivel %d: [", nivel);
    for (int i = 0; i < no->numChaves; i++) {
      printf("%d", no->chaves[i]);
      if (i < no->numChaves - 1) {
        printf(", ");
      }
    }
    printf("]\n");

    // Imprimir os filhos se não for folha
    if (!no->folha) {
      for (int i = 0; i <= no->numChaves; i++) {
        imprimirArvore(no->filhos[i], nivel + 1);
      }
    }
  } else {
    printf("A arvore esta vazia.\n");
  }
}


// Função para liberar a memória de um nó
void liberarNo(No *no) {
  if (no != NULL) {
    if (!no->folha) {
      for (int i = 0; i <= no->numChaves; i++) {
        liberarNo(no->filhos[i]);
      }
    }
    free(no);
  }
}

// Função para remover uma chave da árvore B
void remover(ArvoreB *arvore, int chave) {
  if (!arvore->raiz) {
    printf("A árvore está vazia.\n");
    return;
  }

  removerDeNo(arvore->raiz, chave);

  if (arvore->raiz->numChaves == 0) {
    No *temp = arvore->raiz;
    if (arvore->raiz->folha) {
      arvore->raiz = NULL;
    } else {
      arvore->raiz = arvore->raiz->filhos[0];
    }
    free(temp);
  }
}

// Função para remover uma chave de um nó específico
void removerDeNo(No *no, int chave) {
  int idx = 0;
  while (idx < no->numChaves && no->chaves[idx] < chave) {
    idx++;
  }

  if (idx < no->numChaves && no->chaves[idx] == chave) {
    if (no->folha) {
      removerDeFolha(no, idx);
    } else {
      removerDeNaoFolha(no, idx);
    }
  } else {
    if (no->folha) {
      printf("A chave %d não está na árvore.\n", chave);
      return;
    }

    int flag = (idx == no->numChaves);

    if (no->filhos[idx]->numChaves < ORDEM / 2) {
      preencher(no, idx);
    }

    if (flag && idx > no->numChaves) {
      removerDeNo(no->filhos[idx - 1], chave);
    } else {
      removerDeNo(no->filhos[idx], chave);
    }
  }
}

// Função para remover uma chave de um nó folha
void removerDeFolha(No *no, int idx) {
  for (int i = idx + 1; i < no->numChaves; ++i) {
    no->chaves[i - 1] = no->chaves[i];
  }
  no->numChaves--;
}

// Função para remover uma chave de um nó não folha
void removerDeNaoFolha(No *no, int idx) {
    int chave = no->chaves[idx];

    if (no->filhos[idx + 1]->numChaves >= ORDEM / 2) {
        int sucessor = pegarSucessor(no, idx);
        no->chaves[idx] = sucessor;
        removerDeNo(no->filhos[idx + 1], sucessor);
    } else if (no->filhos[idx]->numChaves >= ORDEM / 2) {
        int antecessor = pegarAntecessor(no, idx);
        no->chaves[idx] = antecessor;
        removerDeNo(no->filhos[idx], antecessor);
    } else {
        fundir(no, idx);
        removerDeNo(no->filhos[idx], chave);
    }
}


// Função para pegar o antecessor de uma chave
int pegarAntecessor(No *no, int idx) {
  No *atual = no->filhos[idx];
  while (!atual->folha) {
    atual = atual->filhos[atual->numChaves];
  }
  return atual->chaves[atual->numChaves - 1];
}

// Função para pegar o sucessor de uma chave
int pegarSucessor(No *no, int idx) {
  No *atual = no->filhos[idx + 1];
  while (!atual->folha) {
    atual = atual->filhos[0];
  }
  return atual->chaves[0];
}

// Função para preencher um nó filho
void preencher(No *no, int idx) {
  if (idx != 0 && no->filhos[idx - 1]->numChaves >= ORDEM / 2) {
    pegarDoAnterior(no, idx);
  } else if (idx != no->numChaves &&
             no->filhos[idx + 1]->numChaves >= ORDEM / 2) {
    pegarDoProximo(no, idx);
  } else {
    if (idx != no->numChaves) {
      fundir(no, idx);
    } else {
      fundir(no, idx - 1);
    }
  }
}

// Função para pegar uma chave do nó anterior
void pegarDoAnterior(No *no, int idx) {
  No *filho = no->filhos[idx];
  No *irmao = no->filhos[idx - 1];

  for (int i = filho->numChaves - 1; i >= 0; --i) {
    filho->chaves[i + 1] = filho->chaves[i];
  }

  if (!filho->folha) {
    for (int i = filho->numChaves; i >= 0; --i) {
      filho->filhos[i + 1] = filho->filhos[i];
    }
  }

  filho->chaves[0] = no->chaves[idx - 1];

  if (!filho->folha) {
    filho->filhos[0] = irmao->filhos[irmao->numChaves];
  }

  no->chaves[idx - 1] = irmao->chaves[irmao->numChaves - 1];

  filho->numChaves += 1;
  irmao->numChaves -= 1;
}

// Função para pegar uma chave do próximo nó
void pegarDoProximo(No *no, int idx) {
  No *filho = no->filhos[idx];
  No *irmao = no->filhos[idx + 1];

  filho->chaves[filho->numChaves] = no->chaves[idx];

  if (!filho->folha) {
    filho->filhos[filho->numChaves + 1] = irmao->filhos[0];
  }

  no->chaves[idx] = irmao->chaves[0];

  for (int i = 1; i < irmao->numChaves; ++i) {
    irmao->chaves[i - 1] = irmao->chaves[i];
  }

  if (!irmao->folha) {
    for (int i = 1; i <= irmao->numChaves; ++i) {
      irmao->filhos[i - 1] = irmao->filhos[i];
    }
  }

  filho->numChaves += 1;
  irmao->numChaves -= 1;
}

// Função para fundir dois nós filhos
void fundir(No *no, int idx) {
  No *filho = no->filhos[idx];
  No *irmao = no->filhos[idx + 1];

  filho->chaves[ORDEM / 2 - 1] = no->chaves[idx];

  for (int i = 0; i < irmao->numChaves; ++i) {
    filho->chaves[i + ORDEM / 2] = irmao->chaves[i];
  }

  if (!filho->folha) {
    for (int i = 0; i <= irmao->numChaves; ++i) {
      filho->filhos[i + ORDEM / 2] = irmao->filhos[i];
    }
  }

  for (int i = idx + 1; i < no->numChaves; ++i) {
    no->chaves[i - 1] = no->chaves[i];
  }

  for (int i = idx + 2; i <= no->numChaves; ++i) {
    no->filhos[i - 1] = no->filhos[i];
  }

  filho->numChaves += irmao->numChaves + 1;
  no->numChaves--;

  free(irmao);
}