#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

void InitGraphics(void);
void UnloadGraphics(void);

void DrawGameFrame(void);

void SpawnExplosion(Vector2 pos, Color color);

void TocarSomBloco(void);
void TocarSomRebatida(void);
void TocarSomGameOver(void);
void TocarSomPerderVida(void);

#endif
