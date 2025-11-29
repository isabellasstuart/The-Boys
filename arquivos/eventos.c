#include <stdio.h>           
#include <stdlib.h>
#include <stdbool.h>
#include "mundo.h"
#include "eventos.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "funcoes.h"
#include "defines.h"

void evento_cria(struct mundo_t *mundo, struct evento_t *ev, 
                 int tipo, int tempo, void *dado, struct base_t *b) {

    ev -> tipo = tipo;
    ev -> tempo = tempo;
    ev -> dado = dado;
    ev -> base = b;

    fprio_insere(mundo -> lef, ev, ev -> tipo, ev -> tempo);
}

int evento_inic_herois(struct mundo_t *mundo, struct heroi_t *h) {
    struct evento_t *ev_chega;
    int base, tempo;

    if (!(ev_chega = malloc(sizeof(struct evento_t))))
        return 0;

    base = sorteia_num(0, N_BASES - 1);
    tempo = sorteia_num(0, 4320);

    evento_cria(mundo, ev_chega, CHEGA, tempo, h, mundo -> bases[base]);
    return 1;
}

int evento_inic_missoes(struct mundo_t *mundo, struct missao_t *m) {
    struct evento_t *ev_missao;
    int tempo;

    if (!(ev_missao = malloc(sizeof(struct evento_t))))
        return 0;

    tempo = sorteia_num(0, T_FIM_DO_MUNDO);

    evento_cria(mundo, ev_missao, MISSAO, tempo, m, NULL);
    return 1;
}

void evento_chega(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *ev_novo;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int tempo, heroi_id, base_id, presentes, lotacao, fila_de_espera;
    bool espera;

    if (!(heroi -> vivo))       
        return;                 

    if (!(ev_novo = malloc(sizeof(struct evento_t))))
        return;

    heroi -> base = base -> id;
    tempo = mundo -> relogio;
    heroi_id = heroi -> id;
    base_id = base -> id;
    presentes = cjto_card(base -> presentes);
    lotacao = base -> lotacao;
    fila_de_espera = fila_tamanho(base -> espera);

    if (presentes < lotacao && !fila_de_espera)
	    espera = true;
    else 
        espera = ((heroi -> paciencia) > (10 * fila_de_espera));

    if (espera) {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", 
                tempo, heroi_id, base_id, presentes, lotacao);
        evento_cria(mundo, ev_novo, ESPERA, tempo, heroi, base);
    }
    else {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", 
                tempo, heroi_id, base_id, presentes, lotacao);
        evento_cria(mundo, ev_novo, DESISTE, tempo, heroi, base);
    }
}

void evento_espera(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *ev_avisa;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int tempo, heroi_id, base_id;

    if (!(heroi -> vivo))
        return;

    if (!(ev_avisa = malloc(sizeof(struct evento_t))))
        return;
    
    tempo = mundo -> relogio;
    heroi_id = heroi -> id;
    base_id = base -> id;

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", 
            tempo, heroi_id, base_id, fila_tamanho(base -> espera));
    fila_insere(base -> espera, heroi -> id);

    // atualiza o tamanho máximo que a fila da base atinge durante a simulação
    if (fila_tamanho(base -> espera) > base -> fila_max)
        base -> fila_max = fila_tamanho(base -> espera);

    evento_cria(mundo, ev_avisa, AVISA, tempo, heroi, base);
}

void evento_desiste(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *ev_viaja;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int id_nova_base, tempo, heroi_id, base_id;

    if (!(heroi -> vivo))
        return;

    if (!(ev_viaja = malloc(sizeof(struct evento_t))))
        return;
    
    id_nova_base = sorteia_num(0, N_BASES - 1);
    tempo = mundo -> relogio;
    heroi_id = heroi -> id;
    base_id = base -> id;

    printf("%6d: DESIST HEROI %2d BASE %d\n", tempo, heroi_id, base_id);
    evento_cria(mundo, ev_viaja, VIAJA, tempo, heroi, mundo -> bases[id_nova_base]);
}

void evento_avisa(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *ev_entra;
    struct heroi_t *heroi;
    struct base_t *base = ev -> base;
    int tempo, heroi_id, base_id, presentes, lotacao, fila_de_espera;

    tempo = mundo -> relogio;
    base_id = base -> id;
    presentes = cjto_card(base -> presentes);
    fila_de_espera = fila_tamanho(base -> espera);
    lotacao = base -> lotacao;

    while ((presentes < lotacao) && fila_de_espera) {
        
        if (!(ev_entra = malloc(sizeof(struct evento_t))))
            return;
        
        printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", 
                tempo, base_id, presentes, lotacao);
        fila_imprime(base -> espera);
        printf("]\n");
        fila_retira(base -> espera, &heroi_id);
        fila_de_espera = fila_tamanho(base -> espera);

        heroi = mundo -> herois[heroi_id];
        
        if (!(heroi -> vivo)) {
            free(ev_entra);
            return;
        }

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, base_id, heroi_id);
        cjto_insere(base -> presentes, heroi -> id);
        presentes = cjto_card(base -> presentes);
        evento_cria(mundo, ev_entra, ENTRA, tempo, heroi, base);
    }
}

void evento_entra(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *ev_sai;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int tempo, heroi_id, base_id, presentes, lotacao, tpb;

    if (!(heroi -> vivo))
        return;

    if (!(ev_sai = malloc(sizeof(struct evento_t))))
        return;

    tpb = 15 + ((heroi -> paciencia) * sorteia_num(1, 20));
    tempo = mundo -> relogio;
    heroi_id = heroi -> id;
    base_id = base -> id;
    presentes = cjto_card(base -> presentes);
    lotacao = base -> lotacao;

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", 
            tempo, heroi_id, base_id, presentes, lotacao, tempo + tpb);
    evento_cria(mundo, ev_sai, SAI, tempo + tpb, heroi, base);
}

void evento_sai(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *ev_viaja, *ev_avisa;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int id_nova_base, tempo, heroi_id, base_id, presentes, lotacao;

    if (!(heroi -> vivo))
        return;

    if (!(ev_viaja = malloc(sizeof(struct evento_t)))) 
        return;

    if (!(ev_avisa = malloc(sizeof(struct evento_t)))) {
        free(ev_viaja);
        return;
    }

    cjto_retira(base -> presentes, heroi -> id);

    id_nova_base = sorteia_num(0, N_BASES - 1);
    tempo = mundo -> relogio;
    heroi_id = heroi -> id;
    base_id = base -> id;
    presentes = cjto_card(base -> presentes);
    lotacao = base -> lotacao;

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", 
            tempo, heroi_id, base_id, presentes, lotacao);
    evento_cria(mundo, ev_viaja, VIAJA, tempo, heroi, mundo -> bases[id_nova_base]);
    evento_cria(mundo, ev_avisa, AVISA, tempo, heroi, base);
}

void evento_viaja(struct mundo_t *mundo, struct evento_t *ev) {
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base_atual, *base_destino = ev -> base;
    struct evento_t *ev_chega;
    int distancia, duracao, tempo, heroi_id, 
        origem, destino, velocidade, tempo_chegada;

    if (!(heroi -> vivo))
        return;

    if (!(ev_chega = malloc(sizeof(struct evento_t))))
        return;

    base_atual = mundo -> bases[heroi -> base];
    distancia = calcula_deslocamento(base_destino -> local, base_atual -> local);
    duracao = distancia / (heroi -> velocidade);
    tempo = mundo -> relogio;
    heroi_id = heroi -> id;
    origem = base_atual -> id;
    destino = base_destino -> id;
    velocidade = heroi -> velocidade;
    tempo_chegada = tempo + duracao;
    
    evento_cria(mundo, ev_chega, CHEGA, tempo_chegada, heroi, base_destino);
    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
            tempo, heroi_id, origem, destino, distancia, velocidade, tempo_chegada);
}

void evento_morre(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *ev_avisa;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int tempo, heroi_id;

    if (!(heroi -> vivo))
        return;

    if (!(ev_avisa = malloc(sizeof(struct evento_t))))
        return;

    cjto_retira(base -> presentes, heroi -> id);
    heroi -> vivo = 0;
    tempo = mundo -> relogio;
    heroi_id = heroi -> id;

    printf("%6d: MORRE HEROI %2d\n", tempo, heroi_id);
    evento_cria(mundo, ev_avisa, AVISA, tempo, heroi, base);
}

void evento_missao(struct mundo_t *mundo, struct evento_t *ev) {
    struct missao_t *missao = ev -> dado;
    struct missao_dist distancias[N_BASES];
    struct base_t *base_atual;
    struct cjto_t *habilidades_base = NULL;
    struct heroi_t *heroi;
    struct evento_t *novo;
    int tempo, missao_id;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    missao_processa_tentativa(mundo, missao);
    calcula_distancias(mundo, missao, distancias);
    tempo = mundo -> relogio;
    missao_id = missao -> id;

    if (missao_encontra_bmp(mundo, missao, &base_atual, distancias)) {
        if (!(habilidades_base = reune_habilidades_base(mundo, base_atual))) {
            free(novo);
            return;                 
        }                        

        missao_cumprida(mundo, missao, base_atual, habilidades_base);
        incrementa_xp_herois(mundo, base_atual);
        cjto_destroi(habilidades_base);
        habilidades_base = NULL;
        free(novo);
    }
    else {
        if ((mundo -> n_compostos_v) && !(tempo % 2500)) {
            if (!(base_atual = encontra_base_nao_vazia(distancias))) {
                evento_cria(mundo, novo, MISSAO, tempo + DIA, missao, NULL);
                printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao_id);
                return;
            }

            if (!(habilidades_base = reune_habilidades_base(mundo, base_atual))) {
                free(novo);
                return;
            }

            heroi = encontra_heroi_mais_exp(mundo, base_atual);

            if (!(acrescenta_habilidades_heroi(missao, heroi))) {
                free(novo);
                return;            
            }
            
            (mundo -> n_compostos_v)--;
            (mundo -> n_mortos)++;

            missao_cumprida(mundo, missao, base_atual, habilidades_base);
            cjto_retira(base_atual -> presentes, heroi -> id);   
            incrementa_xp_herois(mundo, base_atual);
            cjto_destroi(habilidades_base);
            habilidades_base = NULL;

            evento_cria(mundo, novo, MORRE, tempo, heroi, (mundo -> bases)[heroi -> base]); 
        } 
        else {
            evento_cria(mundo, novo, MISSAO, tempo + DIA, missao, NULL);
            printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao_id);
        }
    }   
}

void evento_fim(struct mundo_t *mundo) {
    printf("%6d: FIM\n", mundo -> relogio);

    printf("\n");
    imprime_estatisticas_herois(mundo);
    printf("\n");
    imprime_estatisticas_bases(mundo);
    printf("\n");
    imprime_estatisticas_mundo(mundo);
}