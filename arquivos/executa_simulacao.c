#include <stdio.h>           
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mundo.h"
#include "eventos.h"
#include "funcoes.h"

int percorre_lef(struct mundo_t *mundo) {
    struct evento_t *ev;
    int i, fim = 0, tipo, tempo; 

    for (i = 0; i < N_HEROIS; i++) {
        if (mundo -> herois[i])
            evento_inic_herois(mundo, (mundo -> herois)[i]);
    }

    for (i = 0; i < N_MISSOES; i++) {
        if (mundo -> missoes[i])
            evento_inic_missoes(mundo, (mundo -> missoes)[i]);
    }

    configura_relogio(mundo, 0);

    while (!fim && fprio_tamanho(mundo -> lef)) {
        ev = fprio_retira(mundo -> lef, &tipo, &tempo);
        if (!ev)
            return 1;

        (mundo -> n_eventos)++;
        configura_relogio(mundo, ev -> tempo);
        
        switch(tipo) {
            case FIM:   
                fim = 1;
		        evento_fim(mundo, ev);
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

    return 0;
}

int main () {
    struct mundo_t *mundo;

    srand(42);   // para testes

    mundo = cria_mundo();
    if (!mundo)
        return 1;

    percorre_lef(mundo);

    return 0;
}