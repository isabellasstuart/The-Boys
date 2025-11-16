#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../theboys.tgz/fila.h"
#include "../theboys.tgz/fprio.h"
#include "teste_mundo.h"
#include "conjunto.h"
#include "teste_funcoes.h"
#include "defines.h"

void evento_cria(struct mundo_t *mundo, struct evento_t *ev, int tipo, int tempo, void *dado, struct base_t *b) {
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
    struct heroi_t *h = ev -> dado;
    struct base_t *b = ev -> base;
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
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao);
        evento_cria(mundo, novo, ESPERA, ev->tempo, h, b);
    }
    else {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao);
        evento_cria(mundo, novo, DESISTE, ev->tempo, h, b);
    }
}

void evento_espera(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *h = ev -> dado;
    struct base_t *b = ev -> base;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;
    
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", mundo -> relogio, h -> id, b -> id, fila_tamanho(b -> espera));

    fila_insere(b -> espera, h -> id);

    evento_cria(mundo, novo, AVISA, ev->tempo, h, b);
}

void evento_desiste(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *h = ev -> dado;
    struct base_t *b = ev -> base;
    int id_nova_base;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;
    
    printf("%6d: DESIST HEROI %2d BASE %d\n", mundo -> relogio, h -> id, b -> id);

    id_nova_base = sorteia_num(0, N_BASES - 1);

    evento_cria(mundo, novo, VIAJA, ev->tempo, h, mundo -> bases[id_nova_base]);
}

void evento_avisa(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *h;
    struct base_t *b = ev -> base;
    int heroi_id;

    while ((cjto_card(b -> presentes) < (b -> lotacao)) && fila_tamanho(b -> espera)) {
        if (!(novo = malloc(sizeof(struct evento_t))))
            return;     // [dúvida] é pra ser assim mesmo? o while parar se qualquer alocação falhar?
        
        printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", mundo -> relogio, b -> id, cjto_card(b -> presentes), b -> lotacao);
        fila_imprime(b -> espera);
        printf("]\n");
        fila_retira(b -> espera, &heroi_id);

        h = mundo -> herois[heroi_id];
        if (!(h -> vivo)) {
            free(novo);
            continue;
        }

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", mundo -> relogio, b -> id, heroi_id);
        cjto_insere(b -> presentes, h -> id);
        
        evento_cria(mundo, novo, ENTRA, ev->tempo, h, b);
    }
}

void evento_entra(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *h = ev -> dado;
    struct base_t *b = ev -> base;
    int tpb = 15 + ((h -> paciencia) * sorteia_num(1, 20));

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    evento_cria(mundo, novo, SAI, ev -> tempo + tpb, h, b);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao, novo -> tempo);
}

void evento_sai(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo_1, *novo_2;
    struct heroi_t *h = ev -> dado;
    struct base_t *b = ev -> base;
    int id_nova_base;

    if (!(h -> vivo))
        return;

    if (!(novo_1 = malloc(sizeof(struct evento_t))) || !(novo_2 = malloc(sizeof(struct evento_t))))
        return;

    cjto_retira(b -> presentes, h -> id);
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", mundo -> relogio, h -> id, b -> id, cjto_card(b -> presentes), b -> lotacao);
    id_nova_base = sorteia_num(0, N_BASES - 1);
    
    evento_cria(mundo, novo_1, VIAJA, ev->tempo, h, mundo -> bases[id_nova_base]);
    evento_cria(mundo, novo_2, AVISA, ev->tempo, h, b);
}

void evento_viaja(struct mundo_t *mundo, struct evento_t *ev) {
    struct base_t *base_atual, *base_destino, *b = ev->base;
    struct evento_t *novo;
    struct heroi_t *h = ev -> dado;
    int distancia, duracao, dx, dy;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    base_atual = mundo -> bases[h-> base];
    base_destino = b;

    dx = abs(((base_destino -> local).x) - ((base_atual -> local).x));
    dy = abs(((base_destino -> local).y) - ((base_atual -> local).y));

    distancia = calcula_raiz_quadrada(((dx * dx) + (dy * dy)));
    duracao = distancia / (h -> velocidade);
    
    evento_cria(mundo, novo, CHEGA, ev->tempo + duracao, h, b);

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", mundo -> relogio, h -> id, base_atual -> id, base_destino -> id, distancia, h -> velocidade, ev->tempo + duracao);
}

void evento_morre(struct mundo_t *mundo, struct evento_t *ev) {
    struct evento_t *novo;
    struct heroi_t *h = ev -> dado;
    struct base_t *b = ev -> base;

    if (!(h -> vivo))
        return;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    cjto_retira(b -> presentes, h -> id);
    h -> vivo = 0;
    printf("%6d: MORRE HEROI %2d\n", mundo -> relogio, h -> id);

    evento_cria(mundo, novo, AVISA, ev->tempo, h, b);
}

// [editar] deixar em teste_funcoes.c
// retorna a distancia entre uma base e uma missao -- [editar] criar uma para calcular a distância entre bases também
int calcula_distancia(struct missao_t *missao, struct base_t *base) {
    int dx, dy;

    dx = abs(((missao -> local).x) - ((base -> local).x));
    dy = abs(((missao -> local).y) - ((base -> local).y));

    return calcula_raiz_quadrada(((dx * dx) + (dy * dy)));
}


// [editar] deixar mais modularizada + acrescentar prints de depuração
void evento_missao(struct mundo_t *mundo, struct evento_t *ev) {
    struct missao_t *missao = ev -> dado;
    struct base_t *base_atual, *bmp = NULL;
    struct heroi_t *h;
    struct cjto_t *habilidades_base, *habs_usadas = cjto_cria(N_HABILIDADES);
    struct evento_t *novo;
    int i, j, maior_xp = -1;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    (missao -> tentativas)++;
    printf("%6d: MISSAO %d TENT %d HAB REQ: [", mundo -> relogio, missao -> id, missao -> tentativas);
    cjto_imprime(missao -> habilidades);
    printf("]\n");

    // verifica se existe bmp que atenda às habilidades da missão
    for (i = 0; i < N_BASES; i++) {
        base_atual = (mundo -> bases)[i];
        habilidades_base = cjto_cria(N_HABILIDADES);    // [dúvida] se não der certo a alocação, tem que ser return também?

        // analisa quais habilidades a base tem
        for (j = 0; j < N_HEROIS; j++) 
            if (cjto_pertence(base_atual -> presentes, (mundo -> herois)[j] -> id))
                habilidades_base = cjto_uniao(habilidades_base, (mundo -> herois)[j] -> habilidades);

        // se tiver todas as necessárias e for a mais próxima, é bmp
        if (cjto_contem(habilidades_base, missao -> habilidades))
            if (!(bmp) || calcula_distancia(missao, base_atual) < calcula_distancia(missao, bmp)) {
                bmp = base_atual;
                habs_usadas = cjto_copia(habilidades_base);
            }

        cjto_destroi(habilidades_base);
    }

    if (bmp) {
        missao -> cumprida = 1;
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", mundo -> relogio, missao -> id, bmp -> id);
        cjto_imprime(habs_usadas);
        printf("]\n");

        // incrementa xp de todos os herois da base
        for (j = 0; j < N_HEROIS; j++) 
            if (cjto_pertence((bmp -> presentes), ((mundo -> herois)[j] -> id)))
                ((mundo -> herois)[j] -> xp)++;
    }
    else {
        if (mundo -> n_compostos_v) {
            if (!((mundo -> relogio) % 2500)) {
                (mundo -> n_compostos_v)--;
                missao -> cumprida = 1;
                
                // encontra a base não vazia mais próxima à missão, ainda que não atenda aos requisitos
                bmp = (mundo -> bases)[0];
                for (i = 0; i < N_BASES; i++)
                    if (cjto_card((mundo -> bases)[i] -> presentes) && calcula_distancia(missao, (mundo -> bases)[i]) < calcula_distancia(missao, bmp))
                        bmp = (mundo -> bases)[i];

                base_atual = bmp;

                // encontra o heroi mais experiente da base
                for (j = 0; j < N_HEROIS; j++)
                    if (cjto_pertence((base_atual -> presentes), ((mundo -> herois)[j] -> id)))
                        if ((maior_xp == -1) || ((mundo -> herois)[j] -> xp >= (mundo -> herois)[maior_xp] -> xp))
                            maior_xp = j;

                h = (mundo -> herois)[maior_xp];
                cjto_retira(base_atual -> presentes, h -> id);   
                
                // incrementa xp dos restantes
                for (j = 0; j < N_HEROIS; j++) 
                    if (cjto_pertence((base_atual -> presentes), ((mundo -> herois)[j] -> id)))
                        ((mundo -> herois)[j] -> xp)++;

                evento_cria(mundo, novo, MORRE, ev -> tempo, h, (mundo -> bases)[h -> base]);       // [editar] fazer heroi guardar a base em si, e não o id
            }
            else {
                evento_cria(mundo, novo, MISSAO, ev -> tempo + 24*60, missao, NULL);
                printf("%6d: MISSAO %d IMPOSSIVEL", mundo -> relogio, missao -> id);
            }
        }
    }
    cjto_destroi(habs_usadas);
}

// --------------------------------------------------- testando a implementação --------------------------------------------------

int main () {
    struct mundo_t *mundo;
    struct evento_t *ev;
    int i, fim = 0, tipo, tempo; 

    srand(0);   // para testes

    mundo = cria_mundo();

    for (i = 0; i < N_HEROIS; i++) {
        evento_inic_herois(mundo, (mundo -> herois)[i]);
    }

    for (i = 0; i < N_MISSOES; i++) {
        evento_inic_missoes(mundo, (mundo -> missoes)[i]);
    }

    configura_relogio(mundo, 0);

    while (!fim && fprio_tamanho(mundo -> lef)) {
        ev = fprio_retira(mundo -> lef, &tipo, &tempo);
        configura_relogio(mundo, ev -> tempo);
        
        switch(tipo) {
            case FIM:
                fim = 1;
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
    printf("\n");
    printf("LEF que sobrou:\n");
    fprio_imprime(mundo -> lef);

    destroi_mundo(&mundo);

    return 0;
}