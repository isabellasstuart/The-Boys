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

void evento_cria(struct mundo_t *mundo, struct evento_t *ev, int tipo, int tempo, void *dado, struct base_t *b) {
    
    if (!ev)
        return;

    ev -> tipo = tipo;
    ev -> tempo = tempo;
    ev -> dado = dado;
    ev -> base = b;

    fprio_insere(mundo -> lef, ev, ev -> tipo, ev -> tempo);
}

void evento_inic_herois(struct mundo_t *mundo, struct heroi_t *h) {
    struct evento_t *novo;
    int base, tempo;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    base = sorteia_num(0, N_BASES - 1);
    tempo = sorteia_num(0, 4320);

    evento_cria(mundo, novo, CHEGA, tempo, h, mundo -> bases[base]);
}

void evento_inic_missoes(struct mundo_t *mundo, struct missao_t *m) {
    struct evento_t *novo;
    int tempo;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    tempo = sorteia_num(0, T_FIM_DO_MUNDO);

    evento_cria(mundo, novo, MISSAO, tempo, m, NULL);
}

void evento_chega(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    bool espera;

    if (!(heroi -> vivo))       // [dúvida] precisa verificar sempre? porque, como o herói 
        return;                 // sai da base quando morre, nenhum evento criado o envolve.

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    heroi -> base = base -> id;

    if ((cjto_card(base -> presentes)) < (base -> lotacao) && (!(fila_tamanho(base -> espera))))
	    espera = true;
    else 
        espera = ((heroi -> paciencia) > (10 * fila_tamanho(base -> espera)));
    
    if (espera) {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", mundo -> relogio, heroi -> id, base -> id, cjto_card(base -> presentes), base -> lotacao);
        evento_cria(mundo, novo, ESPERA, ev->tempo, heroi, base);
    }
    else {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", mundo -> relogio, heroi -> id, base -> id, cjto_card(base -> presentes), base -> lotacao);
        evento_cria(mundo, novo, DESISTE, ev->tempo, heroi, base);
    }
}

void evento_espera(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;

    if (!(heroi -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;
    
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", mundo -> relogio, heroi -> id, base -> id, fila_tamanho(base -> espera));

    fila_insere(base -> espera, heroi -> id);

    if (fila_tamanho(base -> espera) > base -> fila_max)
        base -> fila_max = fila_tamanho(base -> espera);

    evento_cria(mundo, novo, AVISA, ev->tempo, heroi, base);
}

void evento_desiste(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int id_nova_base;

    if (!(heroi -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;
    
    printf("%6d: DESIST HEROI %2d BASE %d\n", mundo -> relogio, heroi -> id, base -> id);

    id_nova_base = sorteia_num(0, N_BASES - 1);

    evento_cria(mundo, novo, VIAJA, ev->tempo, heroi, mundo -> bases[id_nova_base]);
}

void evento_avisa(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *heroi;
    struct base_t *base = ev -> base;
    int heroi_id;

    while ((cjto_card(base -> presentes) < (base -> lotacao)) && fila_tamanho(base -> espera)) {
        
        if (!(novo = malloc(sizeof(struct evento_t))))
            return;
        
        printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", mundo -> relogio, base -> id, cjto_card(base -> presentes), base -> lotacao);
        fila_imprime(base -> espera);
        printf("]\n");
        fila_retira(base -> espera, &heroi_id);

        heroi = mundo -> herois[heroi_id];
        
        if (!(heroi -> vivo)) {
            free(novo);
            return;
        }

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", mundo -> relogio, base -> id, heroi_id);
        cjto_insere(base -> presentes, heroi -> id);
        
        evento_cria(mundo, novo, ENTRA, ev->tempo, heroi, base);
    }
}

void evento_entra(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int tpb;

    if (!(heroi -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    tpb = 15 + ((heroi -> paciencia) * sorteia_num(1, 20));

    evento_cria(mundo, novo, SAI, ev -> tempo + tpb, heroi, base);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", mundo -> relogio, heroi -> id, base -> id, cjto_card(base -> presentes), base -> lotacao, novo -> tempo);
}

void evento_sai(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo_1, *novo_2;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;
    int id_nova_base;

    if (!(heroi -> vivo))
        return;

    if (!(novo_1 = malloc(sizeof(struct evento_t))) || !(novo_2 = malloc(sizeof(struct evento_t)))) {
        if (novo_1)
            free(novo_1);
        if (novo_2)
            free(novo_2);
        return;
    }

    cjto_retira(base -> presentes, heroi -> id);
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", mundo -> relogio, heroi -> id, base -> id, cjto_card(base -> presentes), base -> lotacao);
    id_nova_base = sorteia_num(0, N_BASES - 1);
    
    evento_cria(mundo, novo_1, VIAJA, ev->tempo, heroi, mundo -> bases[id_nova_base]);
    evento_cria(mundo, novo_2, AVISA, ev->tempo, heroi, base);
}

void evento_viaja(struct mundo_t *mundo, struct evento_t *ev) {
    struct base_t *base_atual, *base_destino, *base = ev -> base;
    struct evento_t *novo;
    struct heroi_t *heroi = ev -> dado;
    int distancia, duracao, dx, dy;

    if (!(heroi -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    base_atual = mundo -> bases[heroi -> base];
    base_destino = base;

    dx = abs(((base_destino -> local).x) - ((base_atual -> local).x));
    dy = abs(((base_destino -> local).y) - ((base_atual -> local).y));

    distancia = calcula_raiz_quadrada(((dx * dx) + (dy * dy)));
    duracao = distancia / (heroi -> velocidade);
    
    evento_cria(mundo, novo, CHEGA, ev->tempo + duracao, heroi, base);

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", mundo -> relogio, heroi -> id, base_atual -> id, base_destino -> id, distancia, heroi -> velocidade, ev->tempo + duracao);
}

void evento_morre(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *heroi = ev -> dado;
    struct base_t *base = ev -> base;

    if (!(heroi -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    cjto_retira(base -> presentes, heroi -> id);
    heroi -> vivo = 0;
    printf("%6d: MORRE HEROI %2d\n", mundo -> relogio, heroi -> id);

    evento_cria(mundo, novo, AVISA, ev -> tempo, heroi, base);
}

void evento_missao(struct mundo_t *mundo, struct evento_t *ev) {
    struct missao_t *missao;
    struct missao_dist distancias[N_BASES];
    struct base_t *base_atual = NULL;
    struct cjto_t *habilidades_base = NULL;
    struct heroi_t *heroi;
    struct evento_t *novo;
    int i, j, maior_xp = -1;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    missao = ev -> dado;
    (missao -> tentativas)++;

    // atualiza os números mínimo e máximo de tentativas de uma missão no mundo, se for necessário.
    if ((missao -> tentativas) < (mundo -> missoes_min_tent))
        mundo -> missoes_min_tent = missao -> tentativas;
    else if ((missao -> tentativas) > (mundo -> missoes_max_tent))
        mundo -> missoes_max_tent = missao -> tentativas;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", mundo -> relogio, missao -> id, missao -> tentativas);
    cjto_imprime(missao -> habilidades);
    printf(" ]\n");

    // verifica se há alguma base não vazia no mundo
    i = 0;
    int achou = 0;
    while (i < N_BASES && !achou) {
        base_atual = (mundo -> bases)[i];
        if (cjto_card(base_atual -> presentes))
            achou = 1;
        i++;
    }

    if (!achou) {
        evento_cria(mundo, novo, MISSAO, ((ev -> tempo) + 24*60), missao, NULL);
        printf("%6d: MISSAO %d IMPOSSIVEL\n", mundo -> relogio, missao -> id);
        return;
    }

    // calcula as distâncias das bases em relação à missão
    for (i = 0; i < N_BASES; i++) {
        distancias[i].base = (mundo -> bases)[i];
        distancias[i].distancia = calcula_distancia(missao, (mundo -> bases)[i]);
    }

    ordena_vetor(distancias, N_BASES);

    if (missao_procura_bmp(mundo, missao, &base_atual, distancias, N_BASES)) {
        
        if (!(habilidades_base = reune_habilidades_base(mundo, missao, base_atual))) {
            free(novo);             // [dúvida] aqui, eu chamo reune_habilidades_base 2 vezes: 
            return;                 // dentro da funcao procura bmp (como variável local) e aqui.
        }                           // pode deixar assim mesmo?

        missao -> cumprida = 1;
        (base_atual -> missoes_cumpridas)++;
        (mundo -> missoes_cumpridas)++;
        
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", mundo -> relogio, missao -> id, base_atual -> id);
        cjto_imprime(habilidades_base);
        printf(" ]\n");

        incrementa_xp_herois(mundo, base_atual);
        
        cjto_destroi(habilidades_base);
        habilidades_base = NULL;

        free(novo);
    }
    else {
        if ((mundo -> n_compostos_v) && !((mundo -> relogio) % 2500)) {

            // procura base não vazia mais próxima
            i = 0;
            int achou = 0;
            while (i < N_BASES && !achou) {
                base_atual = distancias[i].base;
                if (cjto_card(base_atual -> presentes))
                    achou = 1;
                i++;
            }
            
            (mundo -> n_compostos_v)--;
            missao -> cumprida = 1;
            (base_atual -> missoes_cumpridas)++;
            (mundo -> missoes_cumpridas)++;

            habilidades_base = reune_habilidades_base(mundo, missao, base_atual);

            printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", mundo -> relogio, missao -> id, base_atual -> id);
            cjto_imprime(habilidades_base);
            printf(" ]\n");

            // encontra o heroi mais experiente da base
            for (j = 0; j < N_HEROIS; j++) {
                if ((mundo -> herois[j] -> vivo) && cjto_pertence(base_atual -> presentes, mundo->herois[j] -> id)) 
                    if ((maior_xp == -1) || ((mundo -> herois)[j] -> xp > (mundo -> herois)[maior_xp] -> xp))
                        maior_xp = j;
            }
            
            heroi = (mundo -> herois)[maior_xp];
            cjto_retira(base_atual -> presentes, heroi -> id);   
            
            incrementa_xp_herois(mundo, base_atual);

            evento_cria(mundo, novo, MORRE, ev -> tempo, heroi, (mundo -> bases)[heroi -> base]); 
            (mundo -> n_mortos)++;
        } 
        else {
            evento_cria(mundo, novo, MISSAO, ev -> tempo + 24*60, missao, NULL);
            printf("%6d: MISSAO %d IMPOSSIVEL\n", mundo -> relogio, missao -> id);
        }
    }
    
    if (habilidades_base) {
        cjto_destroi(habilidades_base);
        habilidades_base = NULL;
    }
}

void evento_fim(struct mundo_t *mundo, struct evento_t *ev) {
    printf("%6d: FIM\n", mundo -> relogio);

    printf("\n");
    imprime_estatisticas_herois(mundo);
    printf("\n");
    imprime_estatisticas_bases(mundo);
    printf("\n");
    imprime_estatisticas_mundo(mundo);
}