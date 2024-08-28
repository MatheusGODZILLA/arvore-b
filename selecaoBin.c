#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

// Função recursiva para buscar o valor no arquivo binário
int buscarValorNoBinarioRecursivo(FILE *arquivo, int valorBuscado) {
    int folha, numChaves, valor;
    fread(&folha, sizeof(int), 1, arquivo);
    fread(&numChaves, sizeof(int), 1, arquivo);

    for (int i = 0; i < numChaves; i++) {
        fread(&valor, sizeof(int), 1, arquivo);
        if (valor == valorBuscado) {
            return 1;
        }
    }

    if (!folha) {
        for (int i = 0; i <= numChaves; i++) {
            if (buscarValorNoBinarioRecursivo(arquivo, valorBuscado)) {
                return 1;
            }
        }
    }

    return 0;
}

// Função para buscar o valor no arquivo binário
int buscarValorNoArquivoBinario(const char *nomeArquivo, int valorBuscado) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return -1;  // Retorna -1 para indicar erro na abertura do arquivo
    }

    int resultado = buscarValorNoBinarioRecursivo(arquivo, valorBuscado);
    fclose(arquivo);

    return resultado ? 0 : -1;  // Retorna 0 se o valor foi encontrado, -1 caso contrário
}

// Função para obter o uso de memória em sistemas Windows
size_t obterUsoMemoria() {
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))) {
        return info.WorkingSetSize / 1024;  // Retorna o uso de memória em kilobytes
    }
    return 0;  // Se houver erro, retorna 0
}

int main() {
    const char *nomeArquivo = "arvore.bin";  // Nome do arquivo binário

    int valorBuscado;
    printf("Digite o valor a ser buscado: ");
    scanf("%d", &valorBuscado);

    clock_t inicio, fim;
    double tempo_gasto;

    // Obter uso de memória antes da busca
    size_t memoriaAntes = obterUsoMemoria();

    inicio = clock();
    int resultado = buscarValorNoArquivoBinario(nomeArquivo, valorBuscado);
    fim = clock();

    tempo_gasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;

    // Obter uso de memória após a busca
    size_t memoriaDepois = obterUsoMemoria();

    printf("Tempo de execucao para buscar o valor %d: %f ms\n", valorBuscado, tempo_gasto);

    if (resultado == 0) {
        printf("Valor %d encontrado no arquivo %s.\n", valorBuscado, nomeArquivo);
    } else {
        printf("Valor %d nao encontrado no arquivo %s.\n", valorBuscado, nomeArquivo);
    }

    printf("Uso de memoria antes da busca: %zu KB\n", memoriaAntes);
    printf("Uso de memoria depois da busca: %zu KB\n", memoriaDepois);
    printf("Memoria consumida durante a busca: %zu KB\n", memoriaDepois - memoriaAntes);

    return 0;
}
