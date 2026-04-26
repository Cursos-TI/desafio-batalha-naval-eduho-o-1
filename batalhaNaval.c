/*
 * =====================================================================
 * BATALHA NAVAL - COMPLETO (Novato + Aventureiro + Mestre)
 * =====================================================================
 * Executa os três níveis em sequência, mostrando a evolução do jogo:
 *   - Nível Novato     : dois navios com exibição de coordenadas
 *   - Nível Aventureiro: tabuleiro 10x10 com quatro navios (+ diagonais)
 *   - Nível Mestre     : habilidades especiais (cone, cruz, octaedro)
 *
 * Legenda do tabuleiro:
 *   0 = água        3 = navio        5 = área afetada por habilidade
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>   /* abs() */

/* ------------------------------------------------------------------ */
/* Constantes globais                                                   */
/* ------------------------------------------------------------------ */
#define LINHAS      10
#define COLUNAS     10
#define TAM_NAVIO    3
#define TAM_HAB      5   /* lado da matriz de habilidade (ímpar) */

/* ================================================================== */
/* FUNÇÕES AUXILIARES COMPARTILHADAS                                   */
/* ================================================================== */

/* Retorna 1 se (l, c) está dentro do tabuleiro */
int dentro_do_tabuleiro(int l, int c) {
    return (l >= 0 && l < LINHAS && c >= 0 && c < COLUNAS);
}

/* Exibe o tabuleiro 10x10 com cabeçalho de índices */
void exibir_tabuleiro(int tab[LINHAS][COLUNAS]) {
    printf("    ");
    for (int c = 0; c < COLUNAS; c++) printf("%2d", c);
    printf("\n    ");
    for (int c = 0; c < COLUNAS; c++) printf("--");
    printf("\n");
    for (int i = 0; i < LINHAS; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < COLUNAS; j++)
            printf("%d ", tab[i][j]);
        printf("\n");
    }
}

/* Exibe uma matriz de habilidade 5x5 */
void exibir_habilidade(int hab[TAM_HAB][TAM_HAB], const char *nome) {
    printf("\n  Padrao - %s:\n", nome);
    for (int i = 0; i < TAM_HAB; i++) {
        printf("  ");
        for (int j = 0; j < TAM_HAB; j++)
            printf("%d ", hab[i][j]);
        printf("\n");
    }
}

/* ================================================================== */
/* FUNÇÕES DE POSICIONAMENTO DE NAVIOS                                 */
/* ================================================================== */

void posicionar_horizontal(int tab[LINHAS][COLUNAS], int l, int c) {
    for (int k = 0; k < TAM_NAVIO; k++)
        if (dentro_do_tabuleiro(l, c + k))
            tab[l][c + k] = 3;
}

void posicionar_vertical(int tab[LINHAS][COLUNAS], int l, int c) {
    for (int k = 0; k < TAM_NAVIO; k++)
        if (dentro_do_tabuleiro(l + k, c))
            tab[l + k][c] = 3;
}

/* dir = +1 diagonal direita | dir = -1 diagonal esquerda */
void posicionar_diagonal(int tab[LINHAS][COLUNAS], int l, int c, int dir) {
    for (int k = 0; k < TAM_NAVIO; k++)
        if (dentro_do_tabuleiro(l + k, c + k * dir))
            tab[l + k][c + k * dir] = 3;
}

/* ================================================================== */
/* FUNÇÕES DE GERAÇÃO DE HABILIDADES                                   */
/* ================================================================== */

/* Cone: alarga uma célula por linha a partir do centro superior */
void gerar_cone(int hab[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;
    for (int i = 0; i < TAM_HAB; i++)
        for (int j = 0; j < TAM_HAB; j++)
            hab[i][j] = (abs(j - centro) <= i) ? 1 : 0;
}

/* Octaedro (losango): distância Manhattan ao centro <= raio */
void gerar_octaedro(int hab[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;
    for (int i = 0; i < TAM_HAB; i++)
        for (int j = 0; j < TAM_HAB; j++)
            hab[i][j] = (abs(i - centro) + abs(j - centro) <= centro) ? 1 : 0;
}

/* Cruz: linha central + coluna central */
void gerar_cruz(int hab[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;
    for (int i = 0; i < TAM_HAB; i++)
        for (int j = 0; j < TAM_HAB; j++)
            hab[i][j] = (i == centro || j == centro) ? 1 : 0;
}

/* Aplica padrão de habilidade ao tabuleiro centrado em (orig_l, orig_c) */
void aplicar_habilidade(int tab[LINHAS][COLUNAS],
                         int hab[TAM_HAB][TAM_HAB],
                         int orig_l, int orig_c) {
    int meio = TAM_HAB / 2;
    for (int i = 0; i < TAM_HAB; i++)
        for (int j = 0; j < TAM_HAB; j++)
            if (hab[i][j] == 1) {
                int tl = orig_l + (i - meio);
                int tc = orig_c + (j - meio);
                /* Não sobrescreve navio (3) */
                if (dentro_do_tabuleiro(tl, tc) && tab[tl][tc] != 3)
                    tab[tl][tc] = 5;
            }
}

/* ================================================================== */
/* NÍVEIS                                                              */
/* ================================================================== */

/* ------------------------------------------------------------------ */
/* NÍVEL NOVATO                                                         */
/* ------------------------------------------------------------------ */
void nivel_novato() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║     NIVEL NOVATO  🏅                 ║\n");
    printf("╚══════════════════════════════════════╝\n\n");

    /* Dois navios definidos como vetores de coordenadas [parte][linha/coluna] */
    int navio_vertical  [TAM_NAVIO][2] = {{0,2},{1,2},{2,2}};
    int navio_horizontal[TAM_NAVIO][2] = {{5,0},{5,1},{5,2}};

    printf("Navio Vertical (tamanho %d):\n", TAM_NAVIO);
    for (int i = 0; i < TAM_NAVIO; i++)
        printf("  Parte %d -> Linha: %d, Coluna: %d\n",
               i + 1, navio_vertical[i][0], navio_vertical[i][1]);

    printf("\nNavio Horizontal (tamanho %d):\n", TAM_NAVIO);
    for (int i = 0; i < TAM_NAVIO; i++)
        printf("  Parte %d -> Linha: %d, Coluna: %d\n",
               i + 1, navio_horizontal[i][0], navio_horizontal[i][1]);

    printf("\nPosicionamento concluido com sucesso!\n");
}

/* ------------------------------------------------------------------ */
/* NÍVEL AVENTUREIRO                                                    */
/* ------------------------------------------------------------------ */
void nivel_aventureiro() {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║     NIVEL AVENTUREIRO  🏅🏅           ║\n");
    printf("╚══════════════════════════════════════╝\n\n");

    int tabuleiro[LINHAS][COLUNAS] = {0};

    /* Quatro navios: horizontal, vertical e dois diagonais */
    posicionar_horizontal(tabuleiro, 1, 0);       /* Navio 1 */
    posicionar_vertical  (tabuleiro, 3, 5);       /* Navio 2 */
    posicionar_diagonal  (tabuleiro, 6, 0, +1);   /* Navio 3 - diagonal direita */
    posicionar_diagonal  (tabuleiro, 6, 9, -1);   /* Navio 4 - diagonal esquerda */

    printf("Legenda: 0 = agua | 3 = navio\n\n");
    exibir_tabuleiro(tabuleiro);

    printf("\nNavios posicionados:\n");
    printf("  Navio 1 (Horizontal)        - Linha 1, Colunas 0-2\n");
    printf("  Navio 2 (Vertical)          - Coluna 5, Linhas 3-5\n");
    printf("  Navio 3 (Diagonal direita)  - Inicia em (6,0)\n");
    printf("  Navio 4 (Diagonal esquerda) - Inicia em (6,9)\n");
}

/* ------------------------------------------------------------------ */
/* NÍVEL MESTRE                                                         */
/* ------------------------------------------------------------------ */
void nivel_mestre() {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║     NIVEL MESTRE  🏅🏅🏅              ║\n");
    printf("╚══════════════════════════════════════╝\n\n");

    /* Tabuleiro com os mesmos quatro navios do Aventureiro */
    int tabuleiro[LINHAS][COLUNAS] = {0};

    posicionar_horizontal(tabuleiro, 1, 0);
    posicionar_vertical  (tabuleiro, 3, 5);
    posicionar_diagonal  (tabuleiro, 6, 0, +1);
    posicionar_diagonal  (tabuleiro, 6, 9, -1);

    /* Gera os três padrões de habilidade */
    int cone    [TAM_HAB][TAM_HAB];
    int octaedro[TAM_HAB][TAM_HAB];
    int cruz    [TAM_HAB][TAM_HAB];

    gerar_cone    (cone);
    gerar_octaedro(octaedro);
    gerar_cruz    (cruz);

    /* Exibe os padrões antes de aplicar */
    printf("--- Padroes de Habilidade (5x5) ---\n");
    exibir_habilidade(cone,     "CONE");
    exibir_habilidade(octaedro, "OCTAEDRO");
    exibir_habilidade(cruz,     "CRUZ");

    /* Aplica habilidades em posições distintas do tabuleiro */
    aplicar_habilidade(tabuleiro, cone,     0, 4);   /* centro (0,4)  */
    aplicar_habilidade(tabuleiro, octaedro, 5, 2);   /* centro (5,2)  */
    aplicar_habilidade(tabuleiro, cruz,     8, 7);   /* centro (8,7)  */

    /* Exibe tabuleiro final */
    printf("\n--- Tabuleiro Final ---\n");
    printf("Legenda: 0 = agua | 3 = navio | 5 = area afetada\n\n");
    exibir_tabuleiro(tabuleiro);
}

/* ================================================================== */
/* MAIN                                                                */
/* ================================================================== */
int main() {
    printf("\n=== BATALHA NAVAL ===\n\n");

    nivel_novato();
    nivel_aventureiro();
    nivel_mestre();

    printf("\n=== FIM DO JOGO ===\n");
    return 0;
}
