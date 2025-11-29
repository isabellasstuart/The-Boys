#ifndef EVENTOS
#define EVENTOS

#include "mundo.h"

// aloca um espaço na memória para um novo evento e o insere na lef.
void evento_cria(struct mundo_t *mundo, struct evento_t *ev, 
                 int tipo, int tempo, void *dado, struct base_t *b);

// aloca e insere na lef um evento "chega" para cada herói.
// retorna 1 se funcionar e 0 em caso de erro.
int evento_inic_herois(struct mundo_t *mundo, struct heroi_t *h);

// aloca e insere na lef todas as missões.
// retorna 1 se funcionar e 0 em caso de erro.
int evento_inic_missoes(struct mundo_t *mundo, struct missao_t *m);

// atualiza a base do herói e calcula se ele entrará na fila de espera dela ou não.
// se o herói decidir esperar, cria um evento "espera".
// senão, cria um evento "desiste".
void evento_chega(struct mundo_t *mundo, struct evento_t *ev);

// insere o herói na fila de espera da base e cria um evento "avisa".
void evento_espera(struct mundo_t *mundo, struct evento_t *ev);

// seleciona uma nova base para o herói e cria um evento "viaja".
void evento_desiste(struct mundo_t *mundo, struct evento_t *ev);

// insere na base o primeiro herói da sua fila de espera e cria um evento "entra".
void evento_avisa(struct mundo_t *mundo, struct evento_t *ev);

// calcula o tempo de permanência do herói na base e cria um evento "sai".
void evento_entra(struct mundo_t *mundo, struct evento_t *ev);

// retira um herói de sua base atual e sorteia uma base destino para ele
// cria dois eventos: "viaja" e "avisa".
void evento_sai(struct mundo_t *mundo, struct evento_t *ev);

// calcula distância e tempo de deslocamento para base destino.
// cria um evento "chega".
void evento_viaja(struct mundo_t *mundo, struct evento_t *ev);

// retira um herói de sua base atual e atualiza seu status para morto.
// cria um evento "avisa".
void evento_morre(struct mundo_t *mundo, struct evento_t *ev);

// verifica se uma missão pode ser cumprida em um dado momento.
// se sim, atualiza seu status para cumprida.
// pode criar dois eventos: evento "morre" ou outro evento "missão".
void evento_missao(struct mundo_t *mundo, struct evento_t *ev);

// imprime as estatísticas do mundo.
void evento_fim(struct mundo_t *mundo);

#endif