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

int raiz_quadrada(int x) {
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

int calcula_deslocamento(struct coord_t destino, struct coord_t origem) {
    int dx, dy;

    dx = abs((destino.x) - (origem.x));
    dy = abs((destino.y) - (origem.y));

    return raiz_quadrada(((dx * dx) + (dy * dy)));
}

void calcula_distancias(struct mundo_t *mundo, struct missao_t *missao, 
                        struct missao_dist *distancias) {
    struct coord_t missao_local = missao -> local, base_local;
    int i;

    for (i = 0; i < N_BASES; i++) {
        base_local = (mundo -> bases)[i] -> local;
        distancias[i].base = (mundo -> bases)[i];
        distancias[i].distancia = calcula_deslocamento(missao_local, base_local);
    }
    ordena_vetor(distancias, N_BASES);
}

void missao_processa_tentativa(struct mundo_t *mundo, struct missao_t *missao) {
    int tempo, missao_id, missao_tentativas;

    (missao -> tentativas)++;
    tempo = mundo -> relogio;
    missao_id = missao -> id;
    missao_tentativas = missao -> tentativas;

    /* atualiza os números mínimo e máximo de tentativas de uma missão.
       esses dados são utilizados para as estatísticas do mundo, ao fim da execução. */
    if (missao_tentativas < (mundo -> missoes_min_tent))
        mundo -> missoes_min_tent = missao_tentativas;
    else if (missao_tentativas > (mundo -> missoes_max_tent))
        mundo -> missoes_max_tent = missao_tentativas;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", tempo, missao_id, missao_tentativas);
    cjto_imprime(missao -> habilidades);
    printf(" ]\n");
}

struct cjto_t *reune_habilidades_base(struct mundo_t *mundo, struct base_t *base) {
    struct cjto_t *habilidades_base = NULL, *habilidades_novas = NULL, *heroi_habilidades;
    int i, heroi_id;

    if (!(habilidades_base = cjto_cria(N_HABILIDADES)))
        return NULL;

    for (i = 0; i < N_HEROIS; i++) {
        heroi_id = (mundo -> herois)[i] -> id;
        heroi_habilidades = (mundo -> herois)[i] -> habilidades;
        if (cjto_pertence(base -> presentes, heroi_id)) {
            if (!(habilidades_novas = cjto_uniao(habilidades_base, heroi_habilidades)))
                return NULL;
            cjto_destroi(habilidades_base);
            habilidades_base = habilidades_novas;
        }
    }
    
    return habilidades_base;
}

struct base_t *encontra_base_nao_vazia(struct missao_dist *distancias) {
    struct base_t *base_atual;
    int i = 0, achou = 0;

    while (i < N_BASES && !achou) {
        base_atual = distancias[i].base;
        if (cjto_card(base_atual -> presentes))
            achou = 1;
        i++;
    }

    if (!achou)
        return NULL;
    return base_atual;
}

struct heroi_t *encontra_heroi_mais_exp(struct mundo_t *mundo, struct base_t *base) {
    int i, maior_xp = -1, heroi_id, heroi_xp, heroi_vivo;

    for (i = 0; i < N_HEROIS; i++) {
        heroi_id = mundo -> herois[i] -> id;
        heroi_xp = mundo -> herois[i] -> xp;
        heroi_vivo = mundo -> herois[i] -> vivo;

        if (heroi_vivo && cjto_pertence(base -> presentes, heroi_id)) 
            if ((maior_xp == -1) || (heroi_xp > (mundo -> herois)[maior_xp] -> xp))
                maior_xp = i;
    }

    return (mundo -> herois)[maior_xp];
}

int missao_encontra_bmp(struct mundo_t *mundo, struct missao_t *missao, 
                        struct base_t **base, struct missao_dist *distancias) {
    struct cjto_t *habilidades_base = NULL;
    struct base_t *base_atual = NULL;
    int i = 0, j, achou = 0, 
        tempo, missao_id, base_id, base_distancia, base_presentes, heroi_id;

    tempo = mundo -> relogio;
    missao_id = missao -> id;

    while (i < N_BASES && !achou) {
        base_atual = distancias[i].base;
        base_id = base_atual -> id;
        base_distancia = distancias[i].distancia;
        base_presentes = cjto_card(base_atual -> presentes);

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", 
                tempo, missao_id, base_id, base_distancia);
        cjto_imprime(base_atual -> presentes);
        printf(" ]\n");

        if (!(habilidades_base = reune_habilidades_base(mundo, base_atual)))
            return 0;   
        
        // imprime todos os heróis de uma base e suas habilidades
        for (j = 0; j < N_HEROIS; j++) {
            heroi_id = mundo -> herois[j] -> id;

            if (cjto_pertence(base_atual -> presentes, heroi_id)) {
                printf("%6d: MISSAO %d HAB HEROI %2d: [ ", 
                        tempo, missao_id, heroi_id);
                cjto_imprime((mundo -> herois)[j] -> habilidades);
                printf(" ]\n");
            }
        }

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", tempo, missao_id, base_id);
        cjto_imprime(habilidades_base);
        printf(" ]\n");

        if (base_presentes && cjto_contem(habilidades_base, missao -> habilidades)) {
            achou = 1;
            *base = base_atual;
        }

        habilidades_base = cjto_destroi(habilidades_base);
        i++;
    }

    return achou;
}

void incrementa_xp_herois(struct mundo_t *mundo, struct base_t *base) {
    int i;

    for (i = 0; i < N_HEROIS; i++) {
        if (cjto_pertence((base -> presentes), ((mundo -> herois)[i] -> id)))
            ((mundo -> herois)[i] -> xp)++;
    }
}

int acrescenta_habilidades_heroi(struct missao_t *missao, struct heroi_t *heroi) {
    struct cjto_t *habilidades_antigas = NULL, *habilidades_novas = NULL;

    habilidades_antigas = heroi -> habilidades;

    if (!(habilidades_novas = cjto_uniao(missao -> habilidades, heroi -> habilidades)))
        return 0;

    habilidades_antigas = cjto_destroi(habilidades_antigas);
    heroi -> habilidades = habilidades_novas;

    return 1;
}

void missao_cumprida(struct mundo_t *mundo, struct missao_t *missao, 
                     struct base_t *base_atual, struct cjto_t *habilidades_base) {
    int tempo, missao_id, base_id;

    missao -> cumprida = 1;
    (base_atual -> missoes_cumpridas)++;
    (mundo -> missoes_cumpridas)++;
    tempo = mundo -> relogio;
    missao_id = missao -> id;
    base_id = base_atual -> id;
    
    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", tempo, missao_id, base_id);
    cjto_imprime(habilidades_base);
    printf(" ]\n");
}

double media_tentativas_missoes(struct mundo_t *mundo) {
    int i;
    double soma = 0;

    for (i = 0; i < N_MISSOES; i++) {
        soma += ((mundo -> missoes)[i] -> tentativas);
    }

    return soma / (double)N_MISSOES;
}

void imprime_estatisticas_herois(struct mundo_t *mundo) {
    struct heroi_t *heroi;
    int i;

    for (i = 0; i < N_HEROIS; i++) {
        if (!mundo -> herois[i])
            continue;

        heroi = (mundo -> herois)[i];

        if (heroi -> vivo)
            printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS [ ", 
                    heroi -> id, heroi -> paciencia, heroi -> velocidade, heroi -> xp);
        else 
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", 
                    heroi -> id, heroi -> paciencia, heroi -> velocidade, heroi -> xp);
        
        cjto_imprime(heroi -> habilidades);
        printf(" ]\n");
    }
}

void imprime_estatisticas_bases(struct mundo_t *mundo) {
    struct base_t *base;
    int i;

    for (i = 0; i < N_BASES; i++) {
        if (!mundo -> bases[i])
            continue;

        base = (mundo -> bases)[i];
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", 
                base -> id, base -> lotacao, base -> fila_max, base -> missoes_cumpridas);
    }
}

void imprime_estatisticas_mundo(struct mundo_t *mundo) {
    int missoes_cumpridas, min_tentativas, max_tentativas;
    double percentual_cumprido, percentual_mortes;

    missoes_cumpridas = mundo -> missoes_cumpridas;
    percentual_cumprido = ((double)missoes_cumpridas / (double)N_MISSOES) * 100;
    min_tentativas = mundo -> missoes_min_tent;
    max_tentativas = mundo -> missoes_max_tent;
    percentual_mortes = ((double)mundo -> n_mortos / (double)N_HEROIS) * 100;

    printf("EVENTOS TRATADOS: %d\n", mundo -> n_eventos);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", 
            missoes_cumpridas, N_MISSOES, percentual_cumprido);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", 
            min_tentativas, max_tentativas, media_tentativas_missoes(mundo));
    printf("TAXA MORTALIDADE: %.1f%%\n", percentual_mortes);
}