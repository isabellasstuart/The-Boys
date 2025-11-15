#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------fprio.h----------------------------------------------- */

// descreve um nodo da fila de prioridades
struct fpnodo_t
{
	void *item ;			// item associado ao nodo
	int   tipo ;			// tipo do item
	int   prio ;			// prioridade do item
	struct fpnodo_t *prox;	// próximo nodo
};

// descreve uma fila de prioridades
struct fprio_t
{
	struct fpnodo_t *prim ;	// primeiro nodo da fila
	struct fpnodo_t *fim;		// último nodo da fila
	int num ;			// número de itens na fila
} ;

/* -----------------------------------------------fprio.c------------------------------------------------- */

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria () {
    struct fprio_t *fila;

    if (!(fila = malloc(sizeof(struct fprio_t))))
        return NULL;

    fila -> prim = NULL;
    fila -> fim = NULL;
    fila -> num = 0;

    return fila;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f) {
    if (f)
        return f -> num;
    return -1;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {  // [dúvida] ver se essa função faz sentido
    struct fpnodo_t *inserido, *p; 

    if ((!f) || (!item))
        return -1;

    if (!(inserido = malloc(sizeof(struct fpnodo_t))))
        return -1;

    // Verifica se o item ja existe na fila 
    p = f -> prim;
    while (p) {
        if (p -> item == item) {
            free(inserido);
            return -1;  
        } 
        p = p -> prox;
    }

    inserido -> item = item;
    inserido -> tipo = tipo;
    inserido -> prio = prio;
    inserido -> prox = NULL;

    // Insere no início
    if (!(fprio_tamanho(f)) || (f -> prim -> prio > prio)) {
        inserido -> prox = f -> prim;
        f -> prim = inserido;
    }
    else {
        p = f -> prim;

        // Encontra valor anterior à posição correta de inserido
        while ((p -> prox) && ((p -> prox -> prio) <= (prio))) 
            p = p -> prox;

        inserido -> prox = p -> prox;
        p -> prox = inserido;
    }

    // Atualiza f->fim se item for inserido no fim da fprio
    if (!(fprio_tamanho(f)) || (f -> fim -> prox))
        f -> fim = inserido;

    (f -> num)++;

    return f -> num;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
    struct fpnodo_t *removido;
    void *item;

    if ((!f) || (!tipo) || (!prio) || !fprio_tamanho(f))
        return NULL;

    removido = f -> prim;
    *tipo = removido -> tipo;
    *prio = removido -> prio;
    item = removido -> item;

    f -> prim = removido -> prox;
    free(removido);
    removido = NULL;
    (f -> num)--;

    return item;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f) {
    struct fpnodo_t *p;

    if ((f) && (p = f -> prim)) {
        while (p -> prox) {
            printf("(%d %d) ", p -> tipo, p -> prio);
            p = p -> prox;
        }
        printf("(%d %d)", p -> tipo, p -> prio);
    }
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f) {
    void *item;
    int tipo, prio;

    if (!f)
        return NULL;

    while (fprio_tamanho(f)) {
        item = fprio_retira(f, &tipo, &prio);
        free(item);
    }
    free(f);
    f = NULL;

    return f;
}

/* --------------------------------------------- testando tad fprio ------------------------------------------------- */

// item a guardar na FPRIO
struct item_t
{
  int dado1 ;
  int dado2 ;
} ;

//------------------------------------------------------------------------------

// cria um novo item
struct item_t *item_cria (int a, int b)
{
  struct item_t *item ;

  item = malloc (sizeof (struct item_t)) ;
  if (!item)
    abort () ;

  item->dado1  = a ;
  item->dado2  = b ;

  return (item) ;
}

//------------------------------------------------------------------------------

// destroi um item
struct item_t *item_destroi (struct item_t *item)
{
  if (item)
    free (item) ;
  return (NULL) ;
}

//------------------------------------------------------------------------------

// imprime a FP
void fprio_print (struct fprio_t *fp)
{
  int tam ;

  if (!fp)
    return ;

  // imprime a FP
  printf ("Fila: ") ;
  fprio_imprime (fp) ;
  tam = fprio_tamanho (fp) ;

  // imprime o tamanho da FP
  switch (tam)
  {
    case 0:
      printf ("(0 itens)\n") ;
      break ;
    case 1:
      printf (" (1 item)\n") ;
      break ;
    default:
      printf (" (%d itens)\n", tam) ;
  }
}

//------------------------------------------------------------------------------

// imprime mensagem de erro e encerra execução
void erro (char *msg)
{
  fprintf (stderr, "ERRO: %s\n", msg) ;
  exit (1) ;
}

//------------------------------------------------------------------------------

/* int main ()
{
  struct fprio_t *fp ;
  struct item_t *item ;
  int tipo, prio, status ;
  int tipo_ant, prio_ant ;

  // cria uma FP vazia
  printf ("Cria a fila:\n") ;
  fp = fprio_cria () ;
  if (!fp)
    erro ("não criou a fila") ;
  fprio_print (fp) ;
  printf ("\n") ;

  // cria e insere itens na FP
  printf ("Insere itens com prioridades crescentes:\n") ;
  for (int i = 0; i < 3; i++)
  {
    item = item_cria (0, 0) ;
    if (!item)
      erro ("não criou o item") ;
    printf ("Insere item [%d %d] com tipo %d e prioridade %d\n",
              item->dado1, item->dado2, 0, i) ;
    status = fprio_insere (fp, item, 0, i) ;
    if (status < 0)
      erro ("não inseriu na lista") ;
    fprio_print (fp) ;
  }
  printf ("\n") ;

  // insere mais eventos na LEF
  printf ("Insere itens com prioridades decrescentes:\n") ;
  for (int i = 3; i >= 0; i--)
  {
    item = item_cria (1, 1) ;
    if (!item)
      erro ("não criou o item") ;
    printf ("Insere item [%d %d] com tipo %d e prioridade %d\n",
              item->dado1, item->dado2, 1, i) ;
    status = fprio_insere (fp, item, 1, i) ;
    if (status < 0)
      erro ("não inseriu na lista") ;
    fprio_print (fp) ;
  }
  printf ("\n") ;

  printf ("Retira os 4 primeiros itens:\n") ;
  for (int i = 0; i < 4; i++)
  {
    //   retira o primeiro item da FP
    item = fprio_retira (fp, &tipo, &prio) ;
    if (item)
    {
      printf ("retirou item [%d %d] com tipo %d e prioridade %d\n",
              item->dado1, item->dado2, tipo, prio) ;
      item = item_destroi (item) ;
    }
    else
      erro ("não retirou item") ;

    fprio_print (fp) ;
  }
  printf ("\n") ;

  // insere mais eventos na LEF
  printf ("Insere itens com prioridades crescentes:\n") ;
  for (int i = 1; i < 5; i++)
  {
    item = item_cria (2, 2) ;
    if (!item)
      erro ("não criou o item") ;
    printf ("Insere item [%d %d] com tipo %d e prioridade %d\n",
              item->dado1, item->dado2, 2, i) ;
    status = fprio_insere (fp, item, 2, i) ;
    if (status < 0)
      erro ("não inseriu na lista") ;
    fprio_print (fp) ;
  }
  printf ("\n") ;

  // enquanto a FP não estiver vazia
  printf ("Retira itens verificando a ordem:\n") ;
  tipo_ant = -1 ;
  prio_ant = -1 ;
  while (fprio_tamanho (fp) > 0)
  {
    //   retira o primeiro item da FP
    item = fprio_retira (fp, &tipo, &prio) ;
    if (item)
    {
      printf ("retirou item [%d %d] com tipo %d e prioridade %d\n",
              item->dado1, item->dado2, tipo, prio) ;
      item = item_destroi (item) ;

      // verifica se itens estão saindo na ordem correta
      if ((prio > prio_ant) || ((prio == prio_ant) && (tipo > tipo_ant)))
      {
        tipo_ant = tipo ;
        prio_ant = prio ;
      }
      else
        if (prio < prio_ant)
          erro ("fila não respeita ordem de prioridades") ;
        else
          erro ("fila não respeita ordem FIFO para a mesma prioridade") ;
    }
    else
      erro ("não retirou item") ;

    fprio_print (fp) ;
  }
  printf ("\n") ;

  // testa operações inválidas sobre lista ou valor inexistentes
  printf ("Testa operações com parâmetros inválidos:\n") ;

  item = item_cria (0, 0) ;

  // insere com fila NULL
  status = fprio_insere (NULL, item, 0, 0) ;
  if (status != -1)
    erro ("insere com fila NULL não retornou erro") ;

  // insere com item NULL
  status = fprio_insere (fp, NULL, 0, 0) ;
  if (status != -1)
    erro ("insere com item NULL não retornou erro") ;

  // insere com item repetido
  status = fprio_insere (fp, item, 0, 0) ;
  if (status == -1)
    erro ("insere item falhou") ;
  status = fprio_insere (fp, item, 0, 0) ;
  if (status != -1)
    erro ("insere item repetido não retornou erro") ;

  // retira com fila NULL
  item = fprio_retira (NULL, &tipo, &prio) ;
  if (item)
    erro ("retira com fila NULL não retornou erro") ;

  // retira com tipo NULL
  item = fprio_retira (fp, NULL, &prio) ;
  if (item)
    erro ("retira com tipo NULL não retornou erro") ;

  // retira com prioridade NULL
  item = fprio_retira (fp, &tipo, NULL) ;
  if (item)
    erro ("retira com prioridade NULL não retornou erro") ;

  // tamanho com fila NULL
  status = fprio_tamanho (NULL) ;
  if (status != -1)
    erro ("tamanho em fila NULL não retornou erro") ;

  printf ("\n") ;

  // destroi a FP
  printf ("Destroi a fila:\n") ;
  fp = fprio_destroi (fp) ;
  fprio_print (fp) ;
  printf ("\n") ;

  // encerra
  return (0) ;
} */