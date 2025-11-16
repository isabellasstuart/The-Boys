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

int raiz_quadrada(int x) {
    return sqrt(x);
}

