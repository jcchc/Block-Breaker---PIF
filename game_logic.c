#include "raylib.h"
#include <stdio.h>

// --------------------------------------------------
// Estruturas principais (podem ser separadas em outros arquivos depois)
// --------------------------------------------------

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    float raio;
    bool ativa;
} Bola;

typedef struct {
    Vector2 posicao;
    Vector2 tamanho;
    float velocidade;
} Raquete;

// --------------------------------------------------
// Variáveis globais do jogo
// --------------------------------------------------

const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;

int pontuacao = 0;
int nivel = 1;
bool jogoFinalizado = false;
bool pausado = false;

// --------------------------------------------------
// Protótipos das funções
// --------------------------------------------------

void AtualizarJogo(Bola *bola, Raquete *raquete, float *dificuldade);
void ReiniciarJogo(Bola *bola, Raquete *raquete);
void DesenharJogo(Bola bola, Raquete raquete, float dificuldade);

// --------------------------------------------------
// Função principal (main)
// --------------------------------------------------

int main(void) {
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Block Breaker - Fase Única com Dificuldade Progressiva");
    SetTargetFPS(60);

    // Cria os objetos principais
    Bola bola = { {LARGURA_TELA / 2, ALTURA_TELA / 2}, {4, -4}, 8, true };
    Raquete raquete = { {LARGURA_TELA / 2 - 50, ALTURA_TELA - 30}, {100, 15}, 7 };

    float dificuldade = 1.0f; // começa fácil

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        if (!jogoFinalizado && !pausado)
            AtualizarJogo(&bola, &raquete, &dificuldade);

        // Teclas de controle geral
        if (IsKeyPressed(KEY_P)) pausado = !pausado;
        if (IsKeyPressed(KEY_R)) ReiniciarJogo(&bola, &raquete);

        // Desenho na tela
        BeginDrawing();
        ClearBackground(BLACK);

        DesenharJogo(bola, raquete, dificuldade);

        if (pausado)
            DrawText("PAUSADO", LARGURA_TELA / 2 - 80, ALTURA_TELA / 2, 30, YELLOW);

        if (jogoFinalizado)
            DrawText("GAME OVER! Pressione R para Reiniciar", LARGURA_TELA / 2 - 200, ALTURA_TELA / 2, 20, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// --------------------------------------------------
// Atualização da lógica principal do jogo
// --------------------------------------------------

void AtualizarJogo(Bola *bola, Raquete *raquete, float *dificuldade) {
    // --- Movimento da raquete ---
    if (IsKeyDown(KEY_LEFT) && raquete->posicao.x > 0)
        raquete->posicao.x -= raquete->velocidade;
    if (IsKeyDown(KEY_RIGHT) && raquete->posicao.x + raquete->tamanho.x < LARGURA_TELA)
        raquete->posicao.x += raquete->velocidade;

    // --- Movimento da bola ---
    bola->posicao.x += bola->velocidade.x * (*dificuldade);
    bola->posicao.y += bola->velocidade.y * (*dificuldade);

    // --- Colisões com as bordas da tela ---
    if (bola->posicao.x <= bola->raio || bola->posicao.x >= LARGURA_TELA - bola->raio)
        bola->velocidade.x *= -1;

    if (bola->posicao.y <= bola->raio)
        bola->velocidade.y *= -1;

    // --- Colisão com a raquete ---
    if (CheckCollisionCircleRec(bola->posicao, bola->raio,
                                (Rectangle){raquete->posicao.x, raquete->posicao.y, raquete->tamanho.x, raquete->tamanho.y})) {
        bola->velocidade.y *= -1;
        pontuacao += 10;
    }

    // --- Se a bola cair ---
    if (bola->posicao.y > ALTURA_TELA) {
        jogoFinalizado = true;
    }

    // --- Aumenta a dificuldade conforme a pontuação ---
    if (pontuacao > 0 && pontuacao % 100 == 0) {
        *dificuldade += 0.05f;
        nivel++;
    }
}

// --------------------------------------------------
// Reinicia o jogo
// --------------------------------------------------

void ReiniciarJogo(Bola *bola, Raquete *raquete) {
    bola->posicao = (Vector2){LARGURA_TELA / 2, ALTURA_TELA / 2};
    bola->velocidade = (Vector2){4, -4};
    raquete->posicao = (Vector2){LARGURA_TELA / 2 - 50, ALTURA_TELA - 30};
    pontuacao = 0;
    nivel = 1;
    jogoFinalizado = false;
}

// --------------------------------------------------
// Desenha todos os elementos do jogo
// --------------------------------------------------

void DesenharJogo(Bola bola, Raquete raquete, float dificuldade) {
    DrawText("BLOCK BREAKER", LARGURA_TELA / 2 - 100, 10, 30, LIGHTGRAY);
    DrawText(TextFormat("Pontuação: %d", pontuacao), 10, 50, 20, WHITE);
    DrawText(TextFormat("Nível: %d", nivel), 10, 75, 20, WHITE);
    DrawText(TextFormat("Dificuldade: %.2f", dificuldade), 10, 100, 20, WHITE);

    DrawCircleV(bola.posicao, bola.raio, RAYWHITE);
    DrawRectangleV(raquete.posicao, raquete.tamanho, BLUE);
}
