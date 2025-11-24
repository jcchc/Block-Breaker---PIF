#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h" // <--- ADICIONE ESTA LINHA (Resolve o erro de Vector2 e Color)

// Inicializa fontes, texturas e sistemas de partículas
void InitGraphics(void);

// Descarrega da memória ao fechar o jogo
void UnloadGraphics(void);

// Função principal que desenha o quadro atual
void DrawGameFrame(void);

// Função para criar explosão
void SpawnExplosion(Vector2 pos, Color color);

#endif