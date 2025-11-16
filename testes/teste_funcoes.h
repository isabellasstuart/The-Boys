#ifndef FUNCOES
#define FUNCOES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct coord_t {
    int x;
    int y;
};

// sorteia valor entre min e max
int sorteia_num(int min, int max);

// gera coordenada com x e y recebidos na entrada
struct coord_t cria_coord(int x, int y);

// sorteia coordenadas x e y entre min e max
struct coord_t sorteia_coord(int min, int max);

// calcula a raiz quadrada de um valor
int calcula_raiz_quadrada(int x);

#endif