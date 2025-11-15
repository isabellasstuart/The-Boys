#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../theboys.tgz/fila.h"
#include "../theboys.tgz/fprio.h"
#include "teste_mundo.h"
#include "conjunto.h"
#include "teste_funcoes.h"
#include "defines.h"

struct evento_t {
    int tipo;
    int tempo;
    int heroi_id;
    int base_id;
};

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


// por enquanto, testando só a lef. depois, vai ser necessário passar o mundo como parâmetro
// e acessá-la como mundo -> lef.
void evento_chega(struct fprio_t *lef, int tempo, struct heroi_t *h, struct base_t *b) {
    struct evento_t *novo;
    bool espera;

    if (!(novo = malloc(sizeof(struct evento_t))))
        return;

    h -> base = b -> id; 

    if (((cjto_card(b -> presentes)) < (b -> lotacao)) && !(fila_tamanho(b -> espera))) 
        espera = true;
    else
        espera = ((h -> paciencia) > (10 * fila_tamanho(b -> espera)));
    
    if (espera) {
        novo -> tipo = ESPERA;
        fila_insere(b -> espera, h -> id);
    }
    else
        novo -> tipo = DESISTE;

    novo -> tempo = tempo;
    novo -> heroi_id = h -> id;
    novo -> base_id = b -> id;

    fprio_insere(lef, novo, novo -> tipo, novo -> tempo);
}

void processa_lef(struct mundo_t *mundo) {
    struct fprio_t *antiga_lef, *nova_lef;
    struct evento_t *ev;
    int tipo, tempo;

    if (!(nova_lef = fprio_cria()))
        return;

    while (fprio_tamanho(mundo -> lef)) {
        ev = fprio_retira(mundo -> lef, &tipo, &tempo);
        
        if (ev && tipo == CHEGA) {
            evento_chega(nova_lef, tempo, (mundo->herois)[ev->heroi_id], (mundo->bases)[ev->base_id]);
            free(ev);
        }
    }

    antiga_lef = mundo -> lef;
    mundo -> lef = nova_lef;
    fprio_destroi(antiga_lef);
}

// --------------------------------------------------- testando a implementação --------------------------------------------------

int main () {
    struct mundo_t *mundo;
    int i; 

    srand(0);   // para testes

    mundo = cria_mundo();

    for (i = 0; i < N_HEROIS; i++) {
        evento_inic_herois(mundo, (mundo -> herois)[i]);
    }

    printf("Nova LEF, impressa como (evento_tipo evento_tempo):\n");
    fprio_imprime(mundo -> lef);
    
    processa_lef(mundo);

    printf("\n");
    printf("LEF pos processamento:\n");

    fprio_imprime(mundo -> lef);

    destroi_mundo(&mundo);

    return 0;
}                  