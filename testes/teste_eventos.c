#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../theboys.tgz/fila.h"
#include "../theboys.tgz/fprio.h"
#include "teste_mundo.h"
#include "conjunto.h"
#include "teste_funcoes.h"
#include "defines.h"

void evento_inic_herois(struct mundo_t *m, struct heroi_t *h) {
    struct evento_t *novo;
    int base, tempo;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    base = sorteia_num(0, N_BASES - 1);
    tempo = sorteia_num(0, 4320);

    novo -> tipo = CHEGA;
    novo -> tempo = tempo;
    novo -> heroi_id = h -> id;
    novo -> base_id = base;

    fprio_insere(m -> lef, novo, novo -> tipo, novo -> tempo);
}

void evento_chega(struct mundo_t *mundo, int tempo, struct heroi_t *h, struct base_t *b) {
    struct evento_t *novo;
    bool espera;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    h -> base = b -> id; 

    if ((cjto_card(b -> presentes)) < (b -> lotacao) && (!(fila_tamanho(b -> espera))))
	espera = true;
    else 
        espera = ((h -> paciencia) > (10 * fila_tamanho(b -> espera)));
    
    if (espera) {
        novo -> tipo = ESPERA;
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao);
    }
    else {
        novo -> tipo = DESISTE;
	printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao);
    }
    novo -> tempo = tempo;
    novo -> heroi_id = h -> id;
    novo -> base_id = b -> id;

    fprio_insere(mundo -> lef, novo, novo -> tipo, novo -> tempo);
}

void evento_espera(struct mundo_t *mundo, int tempo, struct heroi_t *h, struct base_t *b) {
    struct evento_t *novo;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;
    
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", mundo -> relogio, h -> id, b -> id, fila_tamanho(b -> espera));

    fila_insere(b -> espera, h -> id);

    novo -> tipo = AVISA;
    novo -> tempo = tempo;
    novo -> heroi_id = h -> id;
    novo -> base_id = b -> id;

    fprio_insere(mundo -> lef, novo, novo -> tipo, novo -> tempo);
}

void evento_desiste(struct mundo_t *mundo, int tempo, struct heroi_t *h, struct base_t *b) {
    struct evento_t *novo;
    int id_nova_base;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;
    
    printf("%6d: DESIST HEROI %2d BASE %d\n", mundo -> relogio, h -> id, b -> id);

    id_nova_base = sorteia_num(0, N_BASES - 1);

    novo -> tipo = VIAJA;
    novo -> tempo = tempo;
    novo -> heroi_id = h -> id;
    novo -> base_id = id_nova_base;

    fprio_insere(mundo -> lef, novo, novo -> tipo, novo -> tempo);
}

void evento_avisa(struct mundo_t *mundo, int tempo, struct base_t *b) {
    struct evento_t *novo;
    struct heroi_t *h;
    int heroi_id;

    while ((cjto_card(b -> presentes) < (b -> lotacao)) && fila_tamanho(b -> espera)) {
        if (!(novo = malloc(sizeof(struct evento_t))))
            return;     // [dúvida] é pra ser assim mesmo? o while parar se qualquer alocação falhar?
        
        printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", mundo -> relogio, b -> id, cjto_card(b -> presentes), b -> lotacao);
        fila_imprime(b -> espera);
        printf(" ]\n");
        fila_retira(b -> espera, &heroi_id);

        h = mundo -> herois[heroi_id];
        if (!(h -> vivo)) {
            free(novo);
            continue;
        }

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", mundo -> relogio, b -> id, heroi_id);
        cjto_insere(b -> presentes, heroi_id);
        
        novo -> tipo = ENTRA;
        novo -> tempo = tempo;
        novo -> base_id = b -> id;
        novo -> heroi_id = heroi_id;

        fprio_insere(mundo -> lef, novo, novo -> tipo, novo -> tempo);
    }
}

void evento_entra(struct mundo_t *mundo, int tempo, struct heroi_t *h, struct base_t *b) {
    struct evento_t *novo;
    int tpb = 15 + ((h -> paciencia) * sorteia_num(1, 20));

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    novo -> tipo = SAI;
    novo -> tempo = tempo + tpb;
    novo -> base_id = b -> id;
    novo -> heroi_id = h -> id;

    fprio_insere(mundo -> lef, novo, novo -> tipo, novo -> tempo);
    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao, novo -> tempo);
}

void evento_sai(struct mundo_t *mundo, int tempo, struct heroi_t *h, struct base_t *b) {
    struct evento_t *novo_1, *novo_2;
    int id_nova_base;

    if (!(h -> vivo))
        return;

    if (!(novo_1 = malloc(sizeof(struct evento_t))) || !(novo_2 = malloc(sizeof(struct evento_t))))
        return;

    cjto_retira(b -> presentes, h -> id);
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao);
    id_nova_base = sorteia_num(0, N_BASES - 1);
    
    novo_1 -> tipo = VIAJA;
    novo_1 -> tempo = tempo;
    novo_1 -> base_id = id_nova_base;
    novo_1 -> heroi_id = h -> id;

    fprio_insere(mundo -> lef, novo_1, novo_1 -> tipo, novo_1 -> tempo);

    novo_2 -> tipo = AVISA;
    novo_2 -> tempo = tempo;
    novo_2 -> base_id = b -> id;
    novo_2 -> heroi_id = h -> id;

    fprio_insere(mundo -> lef, novo_2, novo_2 -> tipo, novo_2 -> tempo);
}

void evento_viaja(struct mundo_t *mundo, int tempo, struct heroi_t *h, struct base_t *b) {
    struct base_t *base_atual, *base_destino;
    struct evento_t *novo;
    int distancia, duracao, dx, dy;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    base_atual = mundo -> bases[h-> base];
    base_destino = b;

    dx = abs(((base_destino -> local).x) - ((base_atual -> local).x));
    dy = abs(((base_destino -> local).y) - ((base_atual -> local).y));

    distancia = raiz_quadrada(((dx * dx) + (dy * dy)));
    duracao = distancia / (h -> velocidade);
    
    novo -> tipo = CHEGA;
    novo -> tempo = tempo + duracao;
    novo -> base_id = b -> id;
    novo -> heroi_id = h -> id;

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", mundo -> relogio, h -> id, base_atual -> id, base_destino -> id, distancia, h -> velocidade, novo -> tempo);
    fprio_insere(mundo -> lef, novo, novo -> tipo, novo -> tempo);
}

void evento_morre(struct mundo_t *mundo, int tempo, struct heroi_t *h, struct base_t *b) {
    struct evento_t *novo;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    cjto_retira(b -> presentes, h -> id);
    h -> vivo = 0;
    printf("%6d: MORRE HEROI %2d\n", mundo -> relogio, h -> id);

    novo -> tipo = AVISA;
    novo -> tempo = tempo;
    novo -> heroi_id = h -> id;
    novo -> base_id = b -> id;

    fprio_insere(mundo -> lef, novo, novo -> tipo, novo -> tempo);
}

/* void processa_lef(struct mundo_t *mundo) {
    struct fprio_t *antiga_lef, *nova_lef;
    struct evento_t *ev;
    int i, tipo, tempo;

    if (!(nova_lef = fprio_cria()))
        return;

    for (i = 1, i <= 3, i++) {
        ev = fprio_retira(mundo -> lef, &tipo, &tempo);
        configura_relogio(mundo, ev -> tempo);
        if (ev) {
            if (tipo == CHEGA) {
                evento_chega(mundo, tempo, (mundo->herois)[ev->heroi_id], (mundo->bases)[ev->base_id]);
            }
            else if (tipo == ESPERA) {
                evento_espera(mundo, tempo, (mundo->herois)[ev->heroi_id], (mundo->bases)[ev->base_id]);  
            }
            else if (tipo == DESISTE) {
                evento_desiste(mundo, tempo, (mundo->herois)[ev->heroi_id], (mundo->bases)[ev->base_id]);
            }
            else if (tipo == AVISA) {

            }
        free(ev)
        }
    }

    antiga_lef = mundo -> lef;
    mundo -> lef = nova_lef;
    fprio_destroi(antiga_lef);
} */

// --------------------------------------------------- testando a implementação --------------------------------------------------

int main () {
    struct mundo_t *mundo;
    struct evento_t *ev;
    struct heroi_t *h;
    struct base_t *b;
    int i, fim = 0, tipo, tempo; 

    srand(0);   // para testes

    mundo = cria_mundo();

    for (i = 0; i < N_HEROIS; i++) {
        evento_inic_herois(mundo, (mundo -> herois)[i]);
    }

    configura_relogio(mundo, 0);

    while (!fim && fprio_tamanho(mundo -> lef)) {
        ev = fprio_retira(mundo -> lef, &tipo, &tempo);
        configura_relogio(mundo, ev -> tempo);
        h = mundo->herois[ev->heroi_id];
        b = mundo->bases[ev->base_id];
        switch(tipo) {
            case CHEGA:
                if (h && b) 
                    evento_chega(mundo, tempo, h, b);
                break;
                
            case ESPERA:
                if (h && b) 
                    evento_espera(mundo, tempo, h, b);
                break;
                
            case DESISTE:
                if (h && b) 
                    evento_desiste(mundo, tempo, h, b);
                break;
                
            case AVISA:
                if (b) 
                    evento_avisa(mundo, tempo, b);
                break;
                
            case ENTRA:
                if (h && b) 
                    evento_entra(mundo, tempo, h, b);
                break;
                
            case SAI:
                if (h && b) 
                    evento_sai(mundo, tempo, h, b);
                break;
                
            case VIAJA:
                if (h && b) 
                    evento_viaja(mundo, tempo, h, b);
                break;
                
            case MORRE:
                if (h && b) 
                    evento_morre(mundo, tempo, h, b);
                break;
            case FIM:
                fim = 1;
                break;
            }
        free(ev);
    }

    printf("\n");
    printf("LEF que sobrou:\n");
    fprio_imprime(mundo -> lef);

    destroi_mundo(&mundo);

    return 0;
}                  
