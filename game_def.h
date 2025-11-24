#ifndef GAME_DEF_H
#define GAME_DEF_H

#include "raylib.h"
#include <stdbool.h>

// Definições que todo mundo usa
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

// Estados do Jogo
typedef enum { 
    MENU = 0, 
    GAMEPLAY, 
    RANKINGS, 
    GAME_OVER 
} GameScreen;

#endif