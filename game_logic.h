#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "raylib.h"
#include <stdbool.h>
#include "blocks.h"

typedef enum {
    MENU = 0,
    GAMEPLAY,
    RANKINGS,
    GAME_OVER
} GameScreen;

typedef struct {
    Rectangle rect;
    float velocidade;
    int vidas;
} Player;

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    float raio;
    bool ativa;
} Ball;

// --- EXPORTANDO AS GLOBAIS (Para main.c e graphics.c verem) ---
extern Player player;
extern Ball ball;
extern Bloco *listaBlocos; // IMPORTANTE
extern int pontuacao;
extern int nivel;
extern int vidas;
extern GameScreen currentState;
extern int fakeRankings[5];
extern bool jogoFinalizado;

// Funções
void IniciarJogo(Player *p, Ball *b, Bloco **l);
void AtualizarLogica(Player *p, Ball *b, Bloco **l);

#endif