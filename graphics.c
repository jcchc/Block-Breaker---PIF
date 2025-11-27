#include "raylib.h"
#include "graphics.h"
#include "game_logic.h" 
#include "blocks.h"     
#include <stdio.h>      

const Color COR_FUNDO      = { 10, 10, 15, 255 };      
const Color COR_BOTAO      = { 20, 20, 80, 200 };      
const Color COR_BORDA      = { 0, 200, 255, 255 };     
const Color COR_TITULO     = { 255, 200, 0, 255 };     
const Color COR_TEXTO      = { 255, 255, 255, 255 };   

const Color CORES_NIVEIS[] = {
    { 230, 41, 55, 255 },   
    { 255, 161, 0, 255 },   
    { 253, 249, 0, 255 },   
    { 0, 228, 48, 255 },    
    { 0, 121, 241, 255 }    
};

static Sound musicaMenu;
static Sound musicaFase;
static Sound somBloco;
static Sound somRebatida;
static Sound somGameOver;
static Sound somPerderVida;

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

#define BLOCK_COUNT 30
typedef struct { Rectangle rect; Color color; float speed; } FallingBlock;
static FallingBlock fallingBlocks[BLOCK_COUNT];

extern Player player;
extern Ball ball;
extern Bloco *listaBlocos;
extern int pontuacao;
extern int nivel;
extern int vidas;
extern GameScreen currentState;
extern int topScores[5];
extern float roundTimer;

static void InitMenuBlocks(void) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        fallingBlocks[i].rect = (Rectangle){ GetRandomValue(0, 900), GetRandomValue(-600, 0), GetRandomValue(20, 50), GetRandomValue(20, 50) };
        fallingBlocks[i].speed = GetRandomValue(1, 4);
        fallingBlocks[i].color = (Color){ GetRandomValue(50, 255), GetRandomValue(50, 255), 255, 30 };
    }
}

static void UpdateMenuBlocks(void) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        fallingBlocks[i].rect.y += fallingBlocks[i].speed;
        if (fallingBlocks[i].rect.y > 650) {
            fallingBlocks[i].rect.y = -50;
            fallingBlocks[i].rect.x = GetRandomValue(0, 900);
        }
    }
}

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

static void DrawTextCentered(const char *text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int x = (900 - textWidth) / 2;
    DrawText(text, x + 2, y + 2, fontSize, BLACK);
    DrawText(text, x, y, fontSize, color);
}

static void DrawArcadeButton(int y, const char* text, Color highlight) {
    int btnWidth = 350;
    int btnHeight = 50;
    int x = (900 - btnWidth) / 2;
    DrawRectangle(x, y, btnWidth, btnHeight, COR_BOTAO);
    DrawRectangleLines(x, y, btnWidth, btnHeight, COR_BORDA);
    int textW = MeasureText(text, 25);
    DrawText(text, x + (btnWidth - textW)/2, y + 12, 25, highlight);
}

static void DrawMenu(void) {
    ClearBackground(COR_FUNDO);
    UpdateMenuBlocks();
    for (int i = 0; i < BLOCK_COUNT; i++) {
        DrawRectangleRec(fallingBlocks[i].rect, fallingBlocks[i].color);
    }
    
    DrawTextCentered("BLOCK BREAKER", 80, 80, RED);   
    DrawTextCentered("BLOCK BREAKER", 75, 80, BLUE);  
    DrawTextCentered("BLOCK BREAKER", 70, 80, COR_TITULO); 

    DrawArcadeButton(300, "START GAME  [ENTER]", GOLD);
    DrawArcadeButton(370, "RANKING  [R]", GOLD);
    DrawArcadeButton(440, "EXIT  [ESC]", GOLD);
}

static void DrawGameplay(void) {
    ClearBackground(COR_FUNDO);
    DrawRectangleRec(player.rect, BLUE);
    DrawRectangleLinesEx(player.rect, 2, SKYBLUE);
    DrawRectangle(player.rect.x, player.rect.y+5, 10, 10, RED);
    DrawRectangle(player.rect.x+player.rect.width-10, player.rect.y+5, 10, 10, RED);

    DrawCircleV(ball.posicao, ball.raio, WHITE);

    Bloco *atual = listaBlocos;
    while (atual != NULL) {
        if (atual->ativo) {
            int indiceCor = (atual->tipo > 0) ? (atual->tipo - 1) % 5 : (nivel - 1) % 5;
            DrawRectangleRec(atual->rect, CORES_NIVEIS[indiceCor]);
            DrawRectangleLinesEx(atual->rect, 2, BLACK);
            DrawRectangle(atual->rect.x+2, atual->rect.y+2, atual->rect.width-4, 5, (Color){255,255,255,80});
        }
        atual = atual->prox;
    }
    UpdateDrawParticles();
    DrawRectangle(0, 0, 900, 40, (Color){0,0,0,150});
    DrawText(TextFormat("SCORE %05d", pontuacao), 20, 10, 25, GOLD);
    DrawText(TextFormat("ROUND %02d", nivel), 410, 10, 25, COR_BORDA);
    DrawText(TextFormat("LIVES %d", player.vidas), 780, 10, 25, RED);

    if (roundTimer > 0) {
        DrawRectangle(0, 250, 900, 100, (Color){0,0,0,200});
        DrawTextCentered(TextFormat("ROUND %d", nivel), 270, 40, COR_TITULO);
        DrawTextCentered("READY!", 320, 30, WHITE);
    }
}

static void DrawRankings(void) {
    ClearBackground(COR_FUNDO);
    UpdateMenuBlocks();
    for (int i = 0; i < BLOCK_COUNT; i++) DrawRectangleRec(fallingBlocks[i].rect, fallingBlocks[i].color);

    DrawRectangle(200, 50, 500, 550, (Color){0, 0, 0, 230});
    DrawRectangleLines(200, 50, 500, 550, COR_BORDA);
    DrawTextCentered("TOP SCORES", 80, 50, COR_TITULO);

    for (int i = 0; i < 5; i++) {
        int yPos = 180 + (i * 60);
        Color c = (i == 0) ? COR_TITULO : WHITE;
        DrawRectangle(220, yPos, 460, 40, (Color){255,255,255,10});
        DrawText(TextFormat("#%d", i + 1), 240, yPos + 5, 30, c);
        if (topScores[i] == 0) DrawText("-------", 480, yPos + 5, 30, GRAY);
        else DrawText(TextFormat("%06d", topScores[i]), 480, yPos + 5, 30, c);
    }
    DrawTextCentered("PRESS [ESC] TO RETURN", 560, 20, GRAY);
}

static void DrawGameOver(void) {
    DrawGameplay(); 
    DrawRectangle(0, 0, 900, 650, (Color){0, 0, 0, 200}); 
    DrawTextCentered("GAME OVER", 150, 80, RED);
    DrawArcadeButton(300, TextFormat("SCORE: %d", pontuacao), WHITE);
    if (pontuacao > 0 && pontuacao >= topScores[4]) DrawTextCentered("NEW HIGH SCORE!", 260, 30, COR_TITULO);
    DrawTextCentered("[R] TRY AGAIN", 450, 25, COR_BORDA);
    DrawTextCentered("[M] MAIN MENU", 500, 25, GRAY);
}

void InitGraphics(void) {
    musicaMenu = LoadSound("SomMenu.wav");
    musicaFase = LoadSound("SomFase.wav");
    somBloco   = LoadSound("SomBloco.wav");
    somRebatida = LoadSound("SomRebatida.wav");
    somGameOver = LoadSound("SomGameOver.wav");
    somPerderVida = LoadSound("SomPerderVida.wav");
    
    for(int i=0; i<MAX_PARTICLES; i++) particles[i].active = false;
    InitMenuBlocks();
}

void UnloadGraphics(void) {
    UnloadSound(musicaMenu);
    UnloadSound(musicaFase);
    UnloadSound(somBloco);
    UnloadSound(somRebatida);
    UnloadSound(somGameOver);
    UnloadSound(somPerderVida);
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

void TocarSomBloco(void) { PlaySound(somBloco); }
void TocarSomRebatida(void) { PlaySound(somRebatida); }
void TocarSomGameOver(void) { PlaySound(somGameOver); }
void TocarSomPerderVida(void) { PlaySound(somPerderVida); }

void DrawGameFrame(void) {
    if (currentState == MENU || currentState == RANKINGS) {
        if (!IsSoundPlaying(musicaMenu)) PlaySound(musicaMenu);
        if (IsSoundPlaying(musicaFase)) StopSound(musicaFase);
    } 
    else if (currentState == GAMEPLAY) {
        if (!IsSoundPlaying(musicaFase)) PlaySound(musicaFase);
        if (IsSoundPlaying(musicaMenu)) StopSound(musicaMenu);
    }
    else if (currentState == GAME_OVER) {
        StopSound(musicaFase);
        StopSound(musicaMenu);
    }

    switch(currentState) {
        case MENU:      DrawMenu();     break;
        case GAMEPLAY:  DrawGameplay(); break;
        case RANKINGS:  DrawRankings(); break;
        case GAME_OVER: DrawGameOver(); break;
    }
}