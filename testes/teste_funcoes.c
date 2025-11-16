#include "teste_funcoes.h"

int sorteia_num(int min, int max) {
    int n = rand() % (max - min + 1) + min;

    return n;
}

struct coord_t cria_coord(int x, int y) {
    struct coord_t posicao;

    posicao.x = x;
    posicao.y = y;

    return posicao;
}

struct coord_t sorteia_coord(int min, int max) {
    struct coord_t posicao;

    posicao.x = rand() % (max - min + 1) + min;
    posicao.y = rand() % (max - min + 1) + min;

    return posicao;
}

int calcula_raiz_quadrada(int x) {
    return sqrt(x);
}

// ordena vetor por insertion sort
void ordena_vetor(int *v, int tam) {
    int i, j, chave;
    for (i = 1; i < tam; i++) {
        chave = v[i];
        j = i - 1;
        while ((j >= 0) && (v[j] > chave)) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}