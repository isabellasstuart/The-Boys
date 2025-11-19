#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mundo.h"
#include "funcoes.h"
#include "conjunto.h"

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

void ordena_vetor(struct missao_dist *v, int tam) {
    int i, j;
    struct missao_dist chave;
    for (i = 1; i < tam; i++) {
        chave = v[i];
        j = i - 1;
        while ((j >= 0) && (v[j].distancia > chave.distancia)) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

// retorna a distancia entre uma base e uma missao -- [editar] criar uma para calcular a distância entre bases também
int calcula_distancia(struct missao_t *missao, struct base_t *base) {
    int dx, dy;

    dx = abs(((missao -> local).x) - ((base -> local).x));
    dy = abs(((missao -> local).y) - ((base -> local).y));

    return calcula_raiz_quadrada(((dx * dx) + (dy * dy)));
}

struct cjto_t *reune_habilidades_base(struct mundo_t *mundo, struct missao_t *missao, struct base_t *base) {
    struct cjto_t *habilidades_base = NULL, *habilidades_nova_base = NULL;
    int i;

    if (!mundo || !base)
        return NULL;

    if (!(habilidades_base = cjto_cria(N_HABILIDADES)))
        return NULL;

    for (i = 0; i < N_HEROIS; i++) {
        if (cjto_pertence(base -> presentes, (mundo -> herois)[i] -> id)) {
            habilidades_nova_base = cjto_uniao(habilidades_base, (mundo -> herois)[i] -> habilidades);
            cjto_destroi(habilidades_base);
            habilidades_base = habilidades_nova_base;

            printf("%6d: MISSAO %d HAB HEROI %2d: [ ", mundo -> relogio, missao -> id, (mundo -> herois)[i] -> id);
            cjto_imprime((mundo -> herois)[i] -> habilidades);
            printf(" ]\n");
        }
    }

    return habilidades_base;
}

// verifica se há alguma base que tenha as habilidades requeridas pela missão.
// retorna 1 se existir e 0 se não.
int missao_procura_bmp(struct mundo_t *mundo, struct missao_t *missao, struct base_t **base, struct missao_dist *distancias, int tam) {
    struct cjto_t *habilidades_base = NULL;
    struct base_t *base_atual = NULL;
    int i = 0, /* j, */ achou = 0;

    while (i < tam && !achou) {
        base_atual = distancias[i].base;

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", mundo -> relogio, missao -> id, base_atual -> id, distancias[i].distancia);
        cjto_imprime(base_atual -> presentes);
        printf(" ]\n");

        /* habilidades_base = reune_habilidades_base(mundo, missao, base_atual); */

        if (!(habilidades_base = reune_habilidades_base(mundo, missao, base_atual)))
            break;             // [dúvida] rever esse break           

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", mundo -> relogio, missao -> id, base_atual -> id);
        cjto_imprime(habilidades_base);
        printf(" ]\n");

        if (cjto_contem(habilidades_base, missao -> habilidades)) {
            achou = 1;
            *base = base_atual;
        }

        cjto_destroi(habilidades_base);
        habilidades_base = NULL;

        i++;
    }

    return achou;
}

void incrementa_xp_herois(struct mundo_t *mundo, struct base_t *base) {
    int i;

    for (i = 0; i < N_HEROIS; i++) {
        if (!mundo -> herois[i])
            break;          // [dúvida] rever esse break
        if (cjto_pertence((base -> presentes), ((mundo -> herois)[i] -> id)))
            ((mundo -> herois)[i] -> xp)++;
    }
}

void imprime_estatisticas_herois(struct mundo_t *mundo) {
    struct heroi_t *h;
    int i;
    for (i = 0; i < N_HEROIS; i++) {
        if (!mundo -> herois[i])
            continue;

        h = (mundo -> herois)[i];

        if (h -> vivo)
            printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS [ ", h -> id, h -> paciencia, h -> velocidade, h -> xp);
        else 
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", h -> id, h -> paciencia, h -> velocidade, h -> xp);
        
        cjto_imprime(h -> habilidades);
        printf(" ]\n");
    }
}

void imprime_estatisticas_bases(struct mundo_t *mundo) {
    struct base_t *b;
    int i;

    for (i = 0; i < N_BASES; i++) {
        if (!mundo -> bases[i])
            continue;

        b = (mundo -> bases)[i];
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", b -> id, b -> lotacao, b -> fila_max, b -> missoes_cumpridas);
    }
}

void imprime_estatisticas_mundo(struct mundo_t *mundo) {
    printf("EVENTOS TRATADOS: %d\n", mundo -> n_eventos);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", mundo -> missoes_cumpridas, N_MISSOES, (((double)mundo -> missoes_cumpridas / (double)N_MISSOES) * 100));
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d\n", mundo -> missoes_min_tent, mundo -> missoes_max_tent);
    printf("TAXA MORTALIDADE: %.1f%%\n", (((double)mundo -> n_mortos / (double)N_HEROIS) * 100));
}