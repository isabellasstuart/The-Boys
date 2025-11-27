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

void evento_missao(struct mundo_t *mundo, struct evento_t *ev);

void evento_fim(struct mundo_t *mundo, struct evento_t *ev);

#endif