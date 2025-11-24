#include "raylib.h"
#include "graphics.h"
#include "game_logic.h" // Para enxergar Player, Ball, GameScreen
#include "blocks.h"     // Para enxergar Bloco
#include <stdio.h>      // Para TextFormat

// --- CONFIGURAÇÕES VISUAIS ---
const Color CORES_NIVEIS[] = {
    { 65, 105, 225, 255 },  // Round 1: Azul Royal
    { 50, 205, 50, 255 },   // Round 2: Verde Lima
    { 255, 165, 0, 255 },   // Round 3: Laranja
    { 148, 0, 211, 255 },   // Round 4: Roxo Escuro
    { 220, 20, 60, 255 }    // Round 5+: Vermelho Carmesim
};

const Color COR_FUNDO      = { 20, 20, 30, 255 };
const Color COR_BOLA       = { 245, 245, 245, 255 };
const Color COR_JOGADOR    = { 200, 200, 200, 255 };
const Color COR_BOTAO      = { 40, 40, 80, 255 };
const Color COR_DESTAQUE   = { 255, 215, 0, 255 };

// --- SISTEMA DE PARTÍCULAS (Visual) ---
#define MAX_PARTICLES 100
typedef struct {
    Vector2 pos;
    Vector2 vel;
    Color color;
    float alpha;
    float size;
    bool active;
} Particle;

static Particle particles[MAX_PARTICLES];

// --- FUNDO ANIMADO DO MENU ---
#define BLOCK_COUNT 25 
typedef struct { Rectangle rect; Color color; float speed; } FallingBlock;
static FallingBlock fallingBlocks[BLOCK_COUNT];

static Font mainFont;

// --- IMPORTANTE: Variáveis Externas ---
// Estamos dizendo: "Essas variáveis existem no game_logic.c, vou só ler."
extern Player player;
extern Ball ball;
extern Bloco *listaBlocos; // A SUA lista de blocos!
extern int pontuacao;
extern int nivel;
extern int vidas; // Assumindo que player.vidas ou variável global vidas existe
extern GameScreen currentState; // MENU, GAMEPLAY, etc.
extern int fakeRankings[5]; // Se quiser manter o ranking fake

// ---------------------------------------------------------
//               FUNÇÕES INTERNAS (Auxiliares)
// ---------------------------------------------------------

static void InitMenuBlocks(void) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        fallingBlocks[i].rect = (Rectangle){ GetRandomValue(0, 900), GetRandomValue(-600, 0), 40, 40 };
        fallingBlocks[i].speed = GetRandomValue(2, 5);
        fallingBlocks[i].color = (Color){ GetRandomValue(50, 150), GetRandomValue(50, 150), 255, 50 };
    }
}

// Atualiza apenas a animação do fundo (isso não é lógica de jogo, é efeito visual)
static void UpdateMenuBlocks(void) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        fallingBlocks[i].rect.y += fallingBlocks[i].speed;
        if (fallingBlocks[i].rect.y > 650) {
            fallingBlocks[i].rect.y = -50;
            fallingBlocks[i].rect.x = GetRandomValue(0, 900);
        }
    }
}

static void DrawMenuBlocks(void) {
    for (int i = 0; i < BLOCK_COUNT; i++) { 
        DrawRectangleRec(fallingBlocks[i].rect, fallingBlocks[i].color); 
    }
}

static void DrawTextCentered(const char *text, int y, int fontSize, Color color) {
    int textWidth = MeasureTextEx(mainFont, text, fontSize, 2).x;
    int x = (900 - textWidth) / 2;
    DrawTextEx(mainFont, text, (Vector2){x + 2, y + 2}, fontSize, 2, BLACK); // Sombra
    DrawTextEx(mainFont, text, (Vector2){x, y}, fontSize, 2, color);
}

static void DrawButton(const char *text, int y, bool isSelected) {
    // Desenha botões do menu
    Rectangle btnRect = { (900 - 300) / 2, y, 300, 50 };
    Color corBorda = isSelected ? COR_DESTAQUE : DARKGRAY;
    Color corTexto = isSelected ? COR_DESTAQUE : WHITE;
    DrawRectangleRec(btnRect, COR_BOTAO);
    DrawRectangleLinesEx(btnRect, 3, corBorda);
    int textW = MeasureTextEx(mainFont, text, 30, 2).x;
    DrawTextEx(mainFont, text, (Vector2){ btnRect.x + (300 - textW)/2, y + 10 }, 30, 2, corTexto);
}

// Atualiza e desenha partículas
static void UpdateDrawParticles(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            particles[i].pos.x += particles[i].vel.x;
            particles[i].pos.y += particles[i].vel.y;
            particles[i].alpha -= 0.02f;
            if (particles[i].alpha <= 0) particles[i].active = false;

            Color finalColor = particles[i].color;
            finalColor.a = (unsigned char)(particles[i].alpha * 255);
            DrawRectangle(particles[i].pos.x, particles[i].pos.y, particles[i].size, particles[i].size, finalColor);
        }
    }
}

// ---------------------------------------------------------
//               FUNÇÕES DE DESENHO DE ESTADO
// ---------------------------------------------------------

static void DrawMenu(void) {
    UpdateMenuBlocks(); // Anima o fundo
    DrawMenuBlocks();
    
    DrawTextCentered("BLOCK BREAKER", 100, 80, COR_DESTAQUE);
    
    // Instruções simples (já que a lógica de seleção está no game_logic)
    DrawTextCentered("Pressione ENTER para Jogar", 300, 30, WHITE);
    DrawTextCentered("Pressione R para Ranking", 350, 20, GRAY);
    DrawTextCentered("Pressione ESC para Sair", 400, 20, GRAY);
}

static void DrawGameplay(void) {
    // 1. Desenha Player
    DrawRectangleRec(player.rect, COR_JOGADOR);
    
    // 2. Desenha Bola
    DrawCircleV(ball.posicao, ball.raio, COR_BOLA);

    // 3. Desenha Blocos (DA SUA LISTA!)
    Color corDoNivel = CORES_NIVEIS[(nivel - 1) % 5];
    
    Bloco *atual = listaBlocos;
    while (atual != NULL) {
        if (atual->ativo) {
            // Usa a cor do nível ou uma cor especial baseada no tipo do bloco
            Color corBloco = corDoNivel;
            if (atual->tipo > 0) {
                 // Se quiser variar a cor por linha (tipo 1, 2, 3...)
                 // corBloco = CORES_NIVEIS[(atual->tipo) % 5];
            }
            
            DrawRectangleRec(atual->rect, corBloco);
            DrawRectangleLinesEx(atual->rect, 2, BLACK); 
        }
        atual = atual->prox;
    }

    // 4. Partículas
    UpdateDrawParticles();

    // 5. HUD
    DrawText(TextFormat("SCORE: %d", pontuacao), 20, 20, 20, COR_DESTAQUE);
    DrawText(TextFormat("LEVEL: %d", nivel), 450 - 40, 20, 20, WHITE);
    // Acessando vidas (assumindo que está na struct player ou global)
    DrawText(TextFormat("LIVES: %d", player.vidas), 900 - 120, 20, 20, RED);
}

static void DrawRankings(void) {
    DrawMenuBlocks(); // Fundo
    DrawTextCentered("TOP SCORES", 80, 60, COR_DESTAQUE);
    // Desenha o ranking fake (extern)
    for (int i = 0; i < 5; i++) {
        DrawTextCentered(TextFormat("%d.  %05d", i + 1, fakeRankings[i]), 200 + (i * 50), 40, WHITE);
    }
    DrawTextCentered("Pressione ESC para voltar", 550, 20, GRAY);
}

static void DrawGameOver(void) {
    DrawGameplay(); // Fundo do jogo congelado
    DrawRectangle(0, 0, 900, 650, (Color){0, 0, 0, 150}); // Escurece
    
    DrawTextCentered("GAME OVER", 200, 80, RED);
    DrawTextCentered(TextFormat("Final Score: %d", pontuacao), 300, 40, WHITE);
    DrawTextCentered("Pressione [R] para Reiniciar", 450, 20, GRAY);
}

// ---------------------------------------------------------
//               FUNÇÕES PÚBLICAS (API)
// ---------------------------------------------------------

void InitGraphics(void) {
    if (FileExists("font.ttf")) mainFont = LoadFont("font.ttf");
    else mainFont = GetFontDefault();
    
    for(int i=0; i<MAX_PARTICLES; i++) particles[i].active = false;
    InitMenuBlocks();
}

void UnloadGraphics(void) {
    UnloadFont(mainFont);
}

void SpawnExplosion(Vector2 pos, Color color) {
    int count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) {
            particles[i].active = true;
            particles[i].pos = pos;
            particles[i].vel.x = (float)GetRandomValue(-50, 50) / 10.0f;
            particles[i].vel.y = (float)GetRandomValue(-50, 50) / 10.0f;
            particles[i].color = color;
            particles[i].alpha = 1.0f;
            particles[i].size = (float)GetRandomValue(3, 6);
            count++;
            if (count >= 8) break;
        }
    }
}

void DrawGameFrame(void) {
    switch(currentState) {
        case MENU:      DrawMenu();     break;
        case GAMEPLAY:  DrawGameplay(); break;
        case RANKINGS:  DrawRankings(); break;
        case GAME_OVER: DrawGameOver(); break;
    }
}