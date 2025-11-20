#include "raylib.h"
#include "graphics.h"   // Header principal
#include "blocks.h"     // Header REAL 
#include "player_fake.h"// Header FAKE (temporário) ⚠️
#include "ball_fake.h"  // Header FAKE (temporário) ⚠️
#include <stdlib.h>     // Para GetRandomValue

const int screenWidth = 900;
const int screenHeight = 650;

// --- Blocos do Menu ---
#define BLOCK_COUNT 20 // Quantidade de blocos no fundo
typedef struct {
    Rectangle rect;
    Color color;
    float speed;
} FallingBlock;

// --- Variáveis Globais (static) ---
static GameScreen currentState; // Tela atual do jogo
static bool shouldClose = false; // Avisa quando o usuário quer sair
static int selectedOption = 0;   // 0=Iniciar, 1=Rankings, 2=Sair
static Font mainFont;            // Fonte customizada

// Variáveis do Jogo
static Player player;
static Ball ball;
static Bloco* blockList;
static int score;

// Coisas do Menu
static FallingBlock fallingBlocks[BLOCK_COUNT];
static int fakeRankings[5] = {1200, 950, 700, 400, 100}; // Ranking FAKE (será substituído)

// --- Protótipos das Funções (só desse arquivo) ---
static void InitGame(void);
static void UpdateMenu(void);
static void DrawMenu(void);
static void UpdateGameplay(void);
static void DrawGameplay(void);
static void UpdateRankings(void);
static void DrawRankings(void);
static void UpdateGameOver(void);
static void DrawGameOver(void);
static void InitMenuBlocks(void);
static void UpdateMenuBlocks(float dt);
static void DrawMenuBlocks(void);
static void DrawTextBordered(Font font, const char *text, int x, int y, int fontSize, Color color, Color borderColor);

/**
 * Função principal que RODA O JOGO
 * Contém o loop principal e a máquina de estados.
 */
void RunGame(void) {
    InitWindow(screenWidth, screenHeight, "Block Breaker (Pessoa 2)");
    
    // Diz para a Raylib não fechar o jogo com o ESC.
    SetExitKey(KEY_NULL); 
    
    // AVISO: É preciso ter um arquivo "font.ttf" na pasta
    if (FileExists("font.ttf")) mainFont = LoadFont("font.ttf");
    else mainFont = GetFontDefault();
    
    SetTargetFPS(60);
    InitGame(); // Prepara tudo

    // Loop Principal do Jogo
    while (!WindowShouldClose() && !shouldClose) {
        
        // --- ATUALIZAÇÃO (Lógica) ---
        // Roda a lógica da tela atual
        switch(currentState) {
            case MENU:      UpdateMenu();       break;
            case GAMEPLAY:  UpdateGameplay();   break;
            case RANKINGS:  UpdateRankings();   break;
            case GAME_OVER: UpdateGameOver();   break;
        }

        // --- DESENHO (Gráficos) ---
        // Desenha a tela atual
        BeginDrawing();
        ClearBackground(BLACK);
        
        switch(currentState) {
            case MENU:      DrawMenu();         break;
            case GAMEPLAY:  DrawGameplay();     break;
            case RANKINGS:  DrawRankings();     break;
            case GAME_OVER: DrawGameOver();     break;
        }
        EndDrawing();
    }

    // --- Limpeza (antes de fechar) ---
    destruirLista(blockList); // Função REAL da Pessoa 3
    UnloadFont(mainFont);     // Libera a fonte da memória
    CloseWindow();
}

/**
 * Prepara ou Reinicia o jogo.
 */
static void InitGame(void) {
    // Cria jogador e bola (usando headers fake)
    player = CreatePlayer(screenWidth / 2, screenHeight - 40, 100, 20);
    ball = CreateBall(screenWidth / 2, screenHeight / 2, 10);
    
    // Limpa a lista antiga (se existir) e cria a nova
    if (blockList != NULL) destruirLista(blockList); // Função REAL da Pessoa 3
    blockList = gerarBlocos(1);                      // Função REAL da Pessoa 3
    
    // Prepara o menu
    InitMenuBlocks();
    score = 0;
    selectedOption = 0;
    currentState = MENU; // Sempre começa no menu
}

/**
 * Desenha texto com borda preta
 */
static void DrawTextBordered(Font font, const char *text, int x, int y, int fontSize, Color color, Color borderColor) {
    // Desenha a "sombra" (borda)
    DrawTextEx(font, text, (Vector2){x-2, y-2}, fontSize, 2, borderColor);
    DrawTextEx(font, text, (Vector2){x+2, y-2}, fontSize, 2, borderColor);
    DrawTextEx(font, text, (Vector2){x-2, y+2}, fontSize, 2, borderColor);
    DrawTextEx(font, text, (Vector2){x+2, y+2}, fontSize, 2, borderColor);
    
    // Desenha o texto principal
    DrawTextEx(font, text, (Vector2){x, y}, fontSize, 2, color);
}

// --- Funções da Tela de MENU ---

static void UpdateMenu(void) {
    UpdateMenuBlocks(GetFrameTime()); // Atualiza o fundo animado

    // Controla a seleção dos botões
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
        if (selectedOption > 2) selectedOption = 0; // 3 opções (0, 1, 2)
    }
    if (IsKeyPressed(KEY_UP)) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = 2;
    }

    // Checa se o Enter foi pressionado
    if (IsKeyPressed(KEY_ENTER)) {
        switch(selectedOption) {
            case 0: currentState = GAMEPLAY;    break; // Vai para o Jogo
            case 1: currentState = RANKINGS;    break; // Vai para o Ranking
            case 2: shouldClose = true;         break; // Fecha o Jogo
        }
    }
}

static void DrawMenu(void) {
    DrawMenuBlocks(); // Desenha o fundo animado
    
    // Título
    const char *titulo = "BLOCK BREAKER"; 
    int xTitulo = (screenWidth - MeasureTextEx(mainFont, titulo, 60, 2).x) / 2; // Centraliza
    DrawTextBordered(mainFont, titulo, xTitulo, 100, 60, WHITE, BLACK);

    // Define os retângulos dos botões
    Rectangle btnStart    = { (float)screenWidth/2 - 150, 250, 300, 50 };
    Rectangle btnRankings = { (float)screenWidth/2 - 150, 320, 300, 50 };
    Rectangle btnQuit     = { (float)screenWidth/2 - 150, 390, 300, 50 };
    
    // Desenha os fundos dos botões
    DrawRectangleRec(btnStart, (Color){0, 121, 241, 200});    // Azul
    DrawRectangleRec(btnRankings, (Color){0, 200, 121, 200}); // Verde
    DrawRectangleRec(btnQuit, (Color){255, 109, 109, 200});   // Vermelho

    // Desenha o destaque amarelo
    if (selectedOption == 0) DrawRectangleLinesEx(btnStart, 4, YELLOW);
    if (selectedOption == 1) DrawRectangleLinesEx(btnRankings, 4, YELLOW);
    if (selectedOption == 2) DrawRectangleLinesEx(btnQuit, 4, YELLOW);

    // Desenha os textos dos botões
    DrawTextBordered(mainFont, "Start Game", btnStart.x + 65, btnStart.y + 10, 30, WHITE, BLACK);
    DrawTextBordered(mainFont, "Rankings", btnRankings.x + 80, btnRankings.y + 10, 30, WHITE, BLACK);
    DrawTextBordered(mainFont, "Quit", btnQuit.x + 110, btnQuit.y + 10, 30, WHITE, BLACK);
}

// --- Funções da Tela de RANKINGS ---

static void UpdateRankings(void) {
    if (IsKeyPressed(KEY_ESCAPE)) currentState = MENU; 
}

static void DrawRankings(void) {
    DrawMenuBlocks(); // Reusa o fundo animado
    
    // Título
    const char *titulo = "RANKINGS"; 
    int xTitulo = (screenWidth - MeasureTextEx(mainFont, titulo, 50, 2).x) / 2;
    DrawTextBordered(mainFont, titulo, xTitulo, 80, 50, YELLOW, BLACK);
    
    // Mostra os scores FAKE (será substituído)
    DrawTextBordered(mainFont, TextFormat("1. %d", fakeRankings[0]), 350, 200, 30, WHITE, BLACK);
    DrawTextBordered(mainFont, TextFormat("2. %d", fakeRankings[1]), 350, 240, 30, WHITE, BLACK);
    DrawTextBordered(mainFont, TextFormat("3. %d", fakeRankings[2]), 350, 280, 30, WHITE, BLACK);
    DrawTextBordered(mainFont, TextFormat("4. %d", fakeRankings[3]), 350, 320, 30, WHITE, BLACK);
    DrawTextBordered(mainFont, TextFormat("5. %d", fakeRankings[4]), 350, 360, 30, WHITE, BLACK);
    
    // Instrução
    DrawTextBordered(mainFont, "Press ESC to go back", 260, 550, 25, GRAY, BLACK);
}


// --- Funções da Tela de GAMEPLAY ---

static void UpdateGameplay(void) {
    // ------------------------------------------------------------------
    // ATENÇÃO: LÓGICA FAKE (Pessoa 1 precisa fazer o real)
    // ------------------------------------------------------------------
    
    // Mover jogador (fake)
    if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.velocidade;
    if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.velocidade;

    // Mover bola (fake)
    ball.posicao.x += ball.velocidade.x;
    ball.posicao.y += ball.velocidade.y;
    
    // Colisão com paredes (fake)
    if (ball.posicao.x <= 0 || ball.posicao.x >= screenWidth) ball.velocidade.x *= -1;
    if (ball.posicao.y <= 0) ball.velocidade.y *= -1;
    
    // Colisão com jogador (fake)
    if (CheckCollisionCircleRec(ball.posicao, ball.raio, player.rect)) ball.velocidade.y *= -1;
    
    // Colisão com blocos (usando a lista REAL da Pessoa 3)
    Bloco* atual = blockList;
    while (atual != NULL) {
        if (atual->ativo && CheckCollisionCircleRec(ball.posicao, ball.raio, atual->rect)) {
            ball.velocidade.y *= -1;
            atual->ativo = false;    
            score += 10;
            break; // Sai do loop para evitar bugs
        }
        atual = atual->prox;
    }
    
    // Game Over (fake)
    if (ball.posicao.y > screenHeight) {
        player.vidas--;
        if (player.vidas <= 0) currentState = GAME_OVER;
        else ball = CreateBall(screenWidth / 2, screenHeight / 2, 10);
    }
    
    // Atalho para voltar ao menu
    if (IsKeyPressed(KEY_ESCAPE)) currentState = MENU;
}

static void DrawGameplay(void) {
    // Desenho da tela de jogo
    
    DrawRectangleRec(player.rect, BLUE);
    DrawCircleV(ball.posicao, ball.raio, WHITE);
    
    // Desenha os blocos (lendo a lista REAL da Pessoa 3)
    Bloco* atual = blockList;
    while (atual != NULL) {
        if (atual->ativo) DrawRectangleRec(atual->rect, MAROON); 
        atual = atual->prox;
    }
    
    // Desenha o HUD
    DrawTextBordered(mainFont, TextFormat("Score: %d", score), 20, 10, 25, WHITE, BLACK);
    DrawTextBordered(mainFont, TextFormat("Lives: %d", player.vidas), screenWidth - 120, 10, 25, WHITE, BLACK);
}

// --- Funções da Tela de GAME OVER ---

static void UpdateGameOver(void) {
    if (IsKeyPressed(KEY_R)) {
        InitGame(); // Reinicia o jogo
        currentState = MENU; // Volta para o menu
    }
    // Também permite voltar com ESC, por consistência
    if (IsKeyPressed(KEY_ESCAPE)) {
        InitGame(); 
        currentState = MENU;
    }
}
static void DrawGameOver(void) {
    DrawGameplay(); // Desenha o jogo "congelado"
    
    // Centraliza as mensagens
    DrawTextBordered(mainFont, "GAME OVER", (screenWidth - MeasureTextEx(mainFont, "GAME OVER", 60, 2).x)/2, 200, 60, RED, BLACK); 
    DrawTextBordered(mainFont, "Press 'R' to return to Menu", (screenWidth - MeasureTextEx(mainFont, "Press 'R' to return to Menu", 25, 2).x)/2, 280, 25, WHITE, BLACK);
}

// --- Funções do Fundo Animado do Menu ---

static void InitMenuBlocks() {
    Color cols[5] = { RED, ORANGE, YELLOW, GREEN, BLUE };
    for (int i = 0; i < BLOCK_COUNT; i++) {
        fallingBlocks[i].rect.width = 60;
        fallingBlocks[i].rect.height = 20;
        fallingBlocks[i].rect.x = GetRandomValue(0, screenWidth - 60);
        fallingBlocks[i].rect.y = GetRandomValue(-600, 0);
        fallingBlocks[i].color = cols[i % 5];
        fallingBlocks[i].speed = 50 + GetRandomValue(0, 50); 
    }
}
static void UpdateMenuBlocks(float dt) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        fallingBlocks[i].rect.y += fallingBlocks[i].speed * dt;
        // Reposiciona o bloco quando ele sai da tela
        if (fallingBlocks[i].rect.y > screenHeight) {
            fallingBlocks[i].rect.y = GetRandomValue(-500, 0);
            fallingBlocks[i].rect.x = GetRandomValue(0, screenWidth - 60);
        }
    }
}
static void DrawMenuBlocks() {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        DrawRectangleRec(fallingBlocks[i].rect, fallingBlocks[i].color);
    }
}
