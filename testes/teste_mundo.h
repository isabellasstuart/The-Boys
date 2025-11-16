#ifndef MUNDO
#define MUNDO

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../theboys.tgz/fila.h"
#include "../theboys.tgz/fprio.h"
#include "conjunto.h"
#include "teste_funcoes.h"
#include "defines.h"

struct coord_t;

struct heroi_t {
    int id;
    int xp;
    int base;
    int paciencia;
    int velocidade;
    int vivo;
    struct cjto_t *habilidades;
};

struct base_t {
    int id;
    struct coord_t local;
    int lotacao;
    struct cjto_t *presentes;
    struct fila_t *espera;
};

struct missao_t {
    int id;
    struct coord_t local;
    struct cjto_t *habilidades;
};

struct mundo_t {
    int n_herois;
    int n_bases;
    int n_missoes;
    int n_habilidades;
    int n_compostos_v;
    struct fprio_t *lef;
    struct heroi_t **herois;
    struct base_t **bases;
    struct missao_t **missoes;
    struct coord_t tamanho_mundo;
    int relogio;
};

struct evento_t {
    int tipo;
    int tempo;
    struct heroi_t *heroi;
    struct base_t *base;
};

struct heroi_t *cria_heroi(int *id);

struct heroi_t **inic_herois();

struct base_t *cria_base(int *id);

struct base_t **inic_bases();

struct missao_t *cria_missao(int *id);

struct missao_t **inic_missoes();

struct mundo_t *cria_mundo();

void destroi_herois(struct heroi_t **h, int tam);

void destroi_bases(struct base_t **b, int tam);

void destroi_missoes(struct missao_t **m, int tam);

void destroi_mundo(struct mundo_t **m);

void imprime_herois(struct heroi_t **h);

void imprime_bases(struct base_t **b);

void imprime_missoes(struct missao_t **m);

void configura_relogio(struct mundo_t *mundo, int tempo);

#endif