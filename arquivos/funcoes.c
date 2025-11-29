#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mundo.h"
#include "funcoes.h"
#include "conjunto.h"
#include "eventos.h"

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

struct cjto_t *reune_habilidades_base(struct mundo_t *mundo, struct base_t *base) {
    struct cjto_t *habilidades_base = NULL, *habilidades_novas = NULL;
    int i;

    if (!mundo || !base)
        return NULL;

    if (!(habilidades_base = cjto_cria(N_HABILIDADES)))
        return NULL;

    for (i = 0; i < N_HEROIS; i++) {
        if (cjto_pertence(base -> presentes, (mundo -> herois)[i] -> id)) {
            if (!(habilidades_novas = cjto_uniao(habilidades_base, (mundo -> herois)[i] -> habilidades)))
                return NULL;
            cjto_destroi(habilidades_base);
            habilidades_base = habilidades_novas;
        }
    }

    return habilidades_base;
}

// verifica se há alguma base que tenha as habilidades requeridas pela missão.
// retorna 1 se existir e 0 se não.
int missao_procura_bmp(struct mundo_t *mundo, struct missao_t *missao, struct base_t **base, struct missao_dist *distancias, int tam) {
    struct cjto_t *habilidades_base = NULL;
    struct base_t *base_atual = NULL;
    int i = 0, j, achou = 0;

    while (i < tam && !achou) {
        base_atual = distancias[i].base;

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", mundo -> relogio, missao -> id, base_atual -> id, distancias[i].distancia);
        cjto_imprime(base_atual -> presentes);
        printf(" ]\n");

        if (!(habilidades_base = reune_habilidades_base(mundo, base_atual)))
            break;             // [dúvida] rever esse break    
        
        for (j = 0; j < N_HEROIS; j++) {
            if (cjto_pertence(base_atual -> presentes, (mundo -> herois)[j] -> id)) {
                printf("%6d: MISSAO %d HAB HEROI %2d: [ ", mundo -> relogio, missao -> id, (mundo -> herois)[j] -> id);
                cjto_imprime((mundo -> herois)[j] -> habilidades);
                printf(" ]\n");
            }
        }

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
        if (cjto_pertence((base -> presentes), ((mundo -> herois)[i] -> id)))
            ((mundo -> herois)[i] -> xp)++;
    }
}

int acrescenta_habilidades_heroi(struct missao_t *missao, struct heroi_t *heroi) {
    struct cjto_t *habilidades_antigas = NULL, *habilidades_novas = NULL;

    habilidades_antigas = heroi -> habilidades;

    if (!(habilidades_novas = cjto_uniao(missao -> habilidades, heroi -> habilidades)))
        return 0;

    cjto_destroi(habilidades_antigas);
    heroi -> habilidades = habilidades_novas;

    return 1;
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
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", mundo -> missoes_min_tent, mundo -> missoes_max_tent, media_tentativas_missoes(mundo));
    printf("TAXA MORTALIDADE: %.1f%%\n", (((double)mundo -> n_mortos / (double)N_HEROIS) * 100));
}

void percorre_lef(struct mundo_t *mundo) {
    struct evento_t *ev;
    int i, fim = 0, tipo, tempo; 

    for (i = 0; i < N_HEROIS; i++) {
        if (!(evento_inic_herois(mundo, (mundo -> herois)[i]))) {
            destroi_mundo(&mundo);
            return;
        }
    }

    for (i = 0; i < N_MISSOES; i++) {
        if (!(evento_inic_missoes(mundo, (mundo -> missoes)[i]))) {
            destroi_mundo(&mundo);
            return;
        }
    }

    configura_relogio(mundo, 0);

    while (!fim && fprio_tamanho(mundo -> lef)) {
        ev = fprio_retira(mundo -> lef, &tipo, &tempo);

        (mundo -> n_eventos)++;
        configura_relogio(mundo, ev -> tempo);
        
        switch(tipo) {
            case FIM:   
                fim = 1;
		        evento_fim(mundo);
                break;
            default:
                if (ev -> dado) {
                    switch(tipo) {
                        case CHEGA: 
                            evento_chega(mundo, ev);
                            break;
                            
                        case ESPERA: 
                            evento_espera(mundo, ev);
                            break;
                            
                        case DESISTE: 
                            evento_desiste(mundo, ev);
                            break;
                            
                        case AVISA:
                            evento_avisa(mundo, ev);
                            break;
                            
                        case ENTRA: 
                            evento_entra(mundo, ev);
                            break;
                            
                        case SAI: 
                            evento_sai(mundo, ev);
                            break;
                            
                        case VIAJA: 
                            evento_viaja(mundo, ev);
                            break;
                            
                        case MORRE: 
                            evento_morre(mundo, ev);
                            break;

                        case MISSAO:
                            evento_missao(mundo, ev);
                            break;
                    }
                }
                break;
        }
        free(ev);
    }

    destroi_mundo(&mundo);
}