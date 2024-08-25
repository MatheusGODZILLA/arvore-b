#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

// Função para buscar o valor no arquivo binário
int buscarValorNoArquivoBinario(const char *nomeArquivo, int valorBuscado) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return -1;  // Retorna -1 para indicar erro na abertura do arquivo
    }

    int folha, numChaves, valor;
    int encontrado = 0;

    // Percorre o arquivo lendo os nós da árvore
    while (fread(&folha, sizeof(int), 1, arquivo) == 1) {
        fread(&numChaves, sizeof(int), 1, arquivo);
        for (int i = 0; i < numChaves; i++) {
            fread(&valor, sizeof(int), 1, arquivo);
            if (valor == valorBuscado) {
                encontrado = 1;
                break;
            }
        }

        if (encontrado) {
            break;
        }

        // Pula os filhos (se não for folha)
        if (!folha) {
            fseek(arquivo, (numChaves + 1) * sizeof(int*), SEEK_CUR);
        }
    }

    fclose(arquivo);

    return encontrado ? 0 : -1;  // Retorna 0 se o valor foi encontrado, -1 caso contrário
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
