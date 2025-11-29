#ifndef FUNCOES
#define FUNCOES

#include "mundo.h"

struct missao_dist;

// sorteia valor entre min e max e retorna o resultado.
int sorteia_num(int min, int max);

// gera coordenadas x e y recebidas na entrada.
struct coord_t cria_coord(int x, int y);

// sorteia coordenadas x e y entre min e max.
struct coord_t sorteia_coord(int min, int max);

// retorna a raiz quadrada de uma entrada x.
int raiz_quadrada(int x);

// ordena vetor por insertion sort.
void ordena_vetor(struct missao_dist *v, int tam);

// retorna a distancia entre duas coordenadas.
int calcula_deslocamento(struct coord_t destino, struct coord_t origem);

// calcula as distâncias das bases até uma missão e as ordena.
void calcula_distancias(struct mundo_t *mundo, struct missao_t *missao, 
                        struct missao_dist *distancias);

// incrementa o número de tentativas de uma missão.
void missao_processa_tentativa(struct mundo_t *mundo, struct missao_t *missao);

// analisa quais heróis estão presentes na base e reúne todas as habilidades deles.
// retorna um novo conjunto com todas as habilidades ou NULL se erro.
struct cjto_t *reune_habilidades_base(struct mundo_t *mundo, struct base_t *base);

// retorna a base não vazia mais próxima de uma missão.
struct base_t *encontra_base_nao_vazia(struct missao_dist *distancias);

// retorna o herói mais experiente de uma base.
struct heroi_t *encontra_heroi_mais_exp(struct mundo_t *mundo, struct base_t *base);

// verifica se há alguma base que tenha as habilidades requeridas pela missão.
// retorna 1 se existir e 0 se não.
int missao_encontra_bmp(struct mundo_t *mundo, struct missao_t *missao, 
                        struct base_t **base, struct missao_dist *distancias);

// incrementa o xp de todos os heróis de uma base onde uma missão foi cumprida.
void incrementa_xp_herois(struct mundo_t *mundo, struct base_t *base);

// acrescenta as habilidades de uma missão a um herói que utiliza o composto V.
// retorna 1 se funcionar e 0 em caso de erro.
int acrescenta_habilidades_heroi(struct missao_t *missao, struct heroi_t *heroi);

// atualiza o status de uma missão para cumprida.
void missao_cumprida(struct mundo_t *mundo, struct missao_t *missao, 
                     struct base_t *base_atual, struct cjto_t *habilidades_base);

// retorna a quantidade média de tentativas necessárias para cumprir as missões do mundo.
double media_tentativas_missoes(struct mundo_t *mundo);

// imprime os dados de um herói no final da simulação.
void imprime_estatisticas_herois(struct mundo_t *mundo);

// imprime os dados de uma base no final da simulação.
void imprime_estatisticas_bases(struct mundo_t *mundo);

// imprime os dados do mundo no final da simulação.
void imprime_estatisticas_mundo(struct mundo_t *mundo);

#endif