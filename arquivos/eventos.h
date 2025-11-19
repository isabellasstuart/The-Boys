#ifndef EVENTOS
#define EVENTOS

#include "mundo.h"

void evento_cria(struct mundo_t *mundo, struct evento_t *ev, int tipo, int tempo, void *dado, struct base_t *b);

void evento_inic_herois(struct mundo_t *mundo, struct heroi_t *h);

void evento_inic_missoes(struct mundo_t *mundo, struct missao_t *m);

void evento_chega(struct mundo_t *mundo, struct evento_t *ev);

void evento_espera(struct mundo_t *mundo, struct evento_t *ev);

void evento_desiste(struct mundo_t *mundo, struct evento_t *ev);

void evento_avisa(struct mundo_t *mundo, struct evento_t *ev);

void evento_entra(struct mundo_t *mundo, struct evento_t *ev);

void evento_sai(struct mundo_t *mundo, struct evento_t *ev);

void evento_viaja(struct mundo_t *mundo, struct evento_t *ev);

void evento_morre(struct mundo_t *mundo, struct evento_t *ev);

// [editar] deixar em teste_funcoes.c
// retorna a distancia entre uma base e uma missao -- [editar] criar uma para calcular a distância entre bases também
int calcula_distancia(struct missao_t *missao, struct base_t *base);

// [editar] deixar mais modularizada + acrescentar prints de depuração
void evento_missao(struct mundo_t *mundo, struct evento_t *ev);

void imprime_estatisticas_herois(struct mundo_t *mundo);

void imprime_estatisticas_bases(struct mundo_t *mundo);

void imprime_estatisticas_mundo(struct mundo_t *mundo);

void evento_fim(struct mundo_t *mundo, struct evento_t *ev);

#endif