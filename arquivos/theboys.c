#include <stdio.h>           
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mundo.h"
#include "eventos.h"
#include "funcoes.h"

int main () {
    struct mundo_t *mundo;

    srand(time(NULL));  

    if (!(mundo = cria_mundo()))
        return 1;

    percorre_lef(mundo);

    return 0;
}