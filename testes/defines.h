#ifndef CONSTANTES
#define CONSTANTES

// constantes do mundo
#define T_INICIO 0  
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS /* (N_HABILIDADES * 5) */ 10
#define N_BASES /* (N_HEROIS / 5)  */ 10
#define N_MISSOES /* (T_FIM_DO_MUNDO / 100)  */ 10
#define N_COMPOSTOS_V (N_HABILIDADES * 3)

// tipos de eventos
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSÃO 9
#define FIM 10

#endif