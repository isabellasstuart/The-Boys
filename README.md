# Simulador de Eventos Discretos "The Boys"

Simulador em C inspirado levemente na série de TV _The Boys_, desenvolvido
como trabalho final da disciplina de **Programação 1 (CI1001)** da UFPR.

## Sobre

O programa simula 525.600 minutos (1 ano) da dinâmica de um mundo
onde heróis se deslocam entre bases, formam equipes e cumprem missões.
O estado do sistema é atualizado quando um evento é processado.

### Entidades do mundo

- **Herói:** possui ID, habilidades, paciência, velocidade,
  experiência e uma base atual
- **Base:** possui ID, lotação máxima, conjunto de heróis presentes,
  fila de espera e localização (X, Y)
- **Missão:** possui ID, habilidades requeridas e localização
- **Mundo:** agrega todas as entidades e mantém o relógio da simulação

### Eventos implementados

- **CHEGA:** Herói chega a uma base e decide se espera para entrar ou desiste
- **ESPERA:** Herói entra na fila de espera da base
- **AVISA:** Porteiro admite heróis da fila enquanto houver vagas
- **ENTRA:** Herói entra na base e agenda sua saída
- **SAI:** Herói sai da base e escolhe novo destino
- **VIAJA:** Herói se desloca entre bases
- **MORRE:** Herói morre (após usar Composto V) e libera vaga na sua base atual
- **MISSAO:** Missão é executada. Se não for possível, é adiada em 24h.
- **FIM:** Encerra a simulação e imprime estatísticas

## Ferramentas

- **Linguagem:** C (padrão C99)
- **Compilação:** Makefile com `-Wall -Werror -Wextra -std=c99 -g`
- **TADs implementados:** Fila, Fila de Prioridades, Conjunto e Lista
- **Programas auxiliares:** Valgrind (verificação de memória), Git

## Como compilar e executar

    make
    ./theboys

Para limpar os arquivos gerados:

    make clean

## Saída da simulação

Ao final da execução, o programa imprime:

- **Estatísticas dos heróis:** vivo/morto, paciência, velocidade,
  experiência e habilidades
- **Estatísticas das bases:** lotação, fila máxima e missões atendidas
- **Estatísticas gerais:** eventos tratados, missões cumpridas,
  tentativas por missão (números mínimo, máximo e médio) e taxa de mortalidade

## Créditos

Projeto concebido pelos professores **Fabiano Silva**, **Luis Bona** e
**Marcos Castilho** para as disciplinas CI1001 e CI067 da UFPR

Implementação desenvolvida por **Isabella dos Santos Stuart**.
