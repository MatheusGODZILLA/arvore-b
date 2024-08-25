#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

// Função para buscar o valor no arquivo
int buscarValorNoArquivo(const char *nomeArquivo, int valorBuscado) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return -1;  // Retorna -1 para indicar erro na abertura do arquivo
    }

    char linha[256];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Verifica se a linha contém o valor buscado
        char *posicao = strstr(linha, "[");
        if (posicao != NULL) {
            int valor;
            while (sscanf(posicao + 1, "%d", &valor) == 1) {
                if (valor == valorBuscado) {
                    encontrado = 1;
                    break;
                }
                // Avança para o próximo número na linha
                posicao = strchr(posicao + 1, ',');
                if (posicao == NULL) {
                    break;
                }
            }
            if (encontrado) {
                break;
            }
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
    const char *nomeArquivo = "arvore.txt";

    int valorBuscado;
    printf("Digite o valor a ser buscado: ");
    scanf("%d", &valorBuscado);

    clock_t inicio, fim;
    double tempo_gasto;

    // Obter uso de memória antes da busca
    size_t memoriaAntes = obterUsoMemoria();

    inicio = clock();
    int resultado = buscarValorNoArquivo(nomeArquivo, valorBuscado);
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

// gcc selecao.c arvore.c -o main2 -lpsapi
