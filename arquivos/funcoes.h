#ifndef FUNCOES
#define FUNCOES

#include "mundo.h"

struct missao_dist;

// sorteia valor entre min e max
int sorteia_num(int min, int max);

// gera coordenada com x e y recebidos na entrada
struct coord_t cria_coord(int x, int y);

// sorteia coordenadas x e y entre min e max
struct coord_t sorteia_coord(int min, int max);

// calcula a raiz quadrada de um valor
int calcula_raiz_quadrada(int x);

// ordena vetor por insertion sort
void ordena_vetor(struct missao_dist *v, int tam);

// [editar] deixar em teste_funcoes.c
// retorna a distancia entre uma base e uma missao -- [editar] criar uma para calcular a distância entre bases também
int calcula_distancia(struct missao_t *missao, struct base_t *base);

struct cjto_t *reune_habilidades_base(struct mundo_t *mundo, struct missao_t *missao, struct base_t *base);

// verifica se há alguma base que tenha as habilidades requeridas pela missão.
// retorna 1 se existir e 0 se não.
int missao_procura_bmp(struct mundo_t *mundo, struct missao_t *missao, struct base_t **base, struct missao_dist *distancias, int tam);

void incrementa_xp_herois(struct mundo_t *mundo, struct base_t *base);

void imprime_estatisticas_herois(struct mundo_t *mundo);

void imprime_estatisticas_bases(struct mundo_t *mundo);

void imprime_estatisticas_mundo(struct mundo_t *mundo);

#endif