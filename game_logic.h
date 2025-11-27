#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "raylib.h"
#include "game_def.h"
#include "blocks.h"
#include <stdbool.h>

extern Player player;
extern Ball ball;
extern Bloco *listaBlocos;

extern int pontuacao;
extern int nivel;
extern int vidas;

extern GameScreen currentState;

extern int topScores[5];

extern float roundTimer;

void IniciarJogo(Player *p, Ball *b, Bloco **l);
void AtualizarLogica(Player *p, Ball *b, Bloco **l);

#endif
