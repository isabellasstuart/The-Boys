#ifndef MUNDO
#define MUNDO

#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "defines.h"

struct coord_t {
    int x;
    int y;
};

struct heroi_t {
    int id;
    int xp;
    int base;
    int vivo;       // salva se está vivo ou morto.
    int paciencia;
    int velocidade;
    struct cjto_t *habilidades;
};

struct base_t {
    int id;
    int lotacao;
    int fila_max;   
    int missoes_cumpridas;
    struct coord_t local;
    struct fila_t *espera;
    struct cjto_t *presentes;
};

struct missao_t {
    int id;
    int cumprida;
    int tentativas;
    struct coord_t local;
    struct cjto_t *habilidades;
};

struct mundo_t {
    int relogio;
    int n_herois;
    int n_bases;
    int n_missoes;
    int n_habilidades;
    int n_compostos_v;
    int n_eventos;
    int n_mortos;
    int missoes_cumpridas;
    int missoes_min_tent;
    int missoes_max_tent;
    struct fprio_t *lef;
    struct heroi_t **herois;
    struct base_t **bases;
    struct missao_t **missoes;
    struct coord_t tamanho_mundo;
};

struct evento_t {
    int tipo;
    int tempo;
    void *dado;     // pode ser um herói ou uma missão
    struct base_t *base;
};

struct missao_dist {
    struct base_t *base;
    int distancia;
};

// aloca memória para o herói e o inicializa.
struct heroi_t *cria_heroi(int *id);

// inicializa o vetor de heróis, de tamanho N_HEROIS.
struct heroi_t **inic_herois();

// aloca memória para a base e a inicializa.
struct base_t *cria_base(int *id);

// inicializa o vetor de bases, de tamanho N_BASES.
struct base_t **inic_bases();

// aloca memória para a missão e a inicializa.
struct missao_t *cria_missao(int *id);

// inicializa o vetor de missões, de tamanho N_MISSOES.
struct missao_t **inic_missoes();

// aloca memória para o mundo e inicializa todas as estruturas de dados dele.
struct mundo_t *cria_mundo();

// libera a memória utilizada pelo vetor de heróis.
void destroi_herois(struct heroi_t **h, int tam);

// libera a memória utilizada pelo vetor de bases.
void destroi_bases(struct base_t **b, int tam);

// libera a memória utilizada pelo vetor de missões.
void destroi_missoes(struct missao_t **m, int tam);

// libera a memória utilizada por todas as estruturas de dados do mundo.
void destroi_mundo(struct mundo_t **m);

// atualiza o relógio do mundo para o tempo do evento em andamento.
void configura_relogio(struct mundo_t *mundo, int tempo);

// executa a simulação, retirando eventos da lef e os processando.
void percorre_lef(struct mundo_t *mundo);

#endif