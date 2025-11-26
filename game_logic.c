#include "game_logic.h"
#include <stdlib.h>
#include <stdio.h> // Necessário para arquivos

Player player;
Ball ball;
Bloco *listaBlocos = NULL;

int pontuacao = 0;
int nivel = 1;
int vidas = 3;
GameScreen currentState = MENU;

// SUA PARTE: Ranking Real (Top 5)
int topScores[5] = {0, 0, 0, 0, 0}; 

// PARTE DELE: Timers de controle (Câmera lenta e Pausa)
float roundTimer = 0.0f;
float respawnTimer = 0.0f;

const int LARGURA_TELA = 900;
const int ALTURA_TELA = 650;

// --- FUNÇÕES DE ARQUIVO (Ranking) ---
void CarregarRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    if (arquivo != NULL) {
        for(int i = 0; i < 5; i++) {
            if(fscanf(arquivo, "%d", &topScores[i]) == EOF) break;
        }
        fclose(arquivo);
    }
}

void SalvarRanking(int novaPontuacao) {
    CarregarRanking(); 
    if (novaPontuacao > topScores[4]) {
        topScores[4] = novaPontuacao;
        for (int i = 0; i < 5; i++) {
            for (int j = i + 1; j < 5; j++) {
                if (topScores[j] > topScores[i]) {
                    int temp = topScores[i];
                    topScores[i] = topScores[j];
                    topScores[j] = temp;
                }
            }
        }
    }
    FILE *arquivo = fopen("ranking.txt", "w");
    if (arquivo != NULL) {
        for(int i = 0; i < 5; i++) {
            fprintf(arquivo, "%d\n", topScores[i]);
        }
        fclose(arquivo);
    }
}

// --- FUNÇÃO DE RESET ---
void IniciarJogo(Player *p, Ball *b, Bloco **l) {
    p->rect = (Rectangle){ LARGURA_TELA/2 - 50, ALTURA_TELA - 40, 100, 20 };
    p->velocidade = 7.0f;
    p->vidas = 3;

    b->posicao = (Vector2){ LARGURA_TELA/2, ALTURA_TELA/2 };
    // Velocidade ajustada do seu amigo
    b->velocidade = (Vector2){ 3.2f, -3.2f };
    b->raio = 8.0f;
    b->ativa = true;

    pontuacao = 0;
    nivel = 1;
    vidas = 3;
    
    // Zera os timers dele
    roundTimer = 0.0f;
    respawnTimer = 0.0f;

    CarregarRanking();

    if (*l != NULL) destruirLista(*l);
    *l = gerarBlocos(nivel);
}

void AtualizarLogica(Player *p, Ball *b, Bloco **l) {
    
    // --- MÁQUINA DE ESTADOS ---

    // 1. MENU
    if (currentState == MENU) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
            IniciarJogo(p, b, l);
            currentState = GAMEPLAY;
        }
        if (IsKeyPressed(KEY_R)) {
            CarregarRanking();
            currentState = RANKINGS;
        }
        return;
    }

    // 2. RANKING
    if (currentState == RANKINGS) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_B)) {
            currentState = MENU;
        }
        return;
    }

    // 3. GAME OVER
    if (currentState == GAME_OVER) {
        if (IsKeyPressed(KEY_R)) {
             IniciarJogo(p, b, l);
             currentState = GAMEPLAY;
        }
        if (IsKeyPressed(KEY_M)) {
             currentState = MENU;
        }
        return;
    }

    // --- TIMERS (Lógica dele) ---
    if (roundTimer > 0) {
        roundTimer -= GetFrameTime();
        return;
    }

    if (respawnTimer > 0) {
        respawnTimer -= GetFrameTime();
    }

    // --- JOGABILIDADE ---

    if (IsKeyDown(KEY_LEFT)) p->rect.x -= p->velocidade;
    if (IsKeyDown(KEY_RIGHT)) p->rect.x += p->velocidade;
    if (p->rect.x < 0) p->rect.x = 0;
    if (p->rect.x > LARGURA_TELA - p->rect.width) p->rect.x = LARGURA_TELA - p->rect.width;

    float mult = 1.0f + (nivel * 0.1f);

    // Efeito Câmera Lenta no Respawn (Lógica dele)
    if (respawnTimer > 0) {
        b->posicao.x += b->velocidade.x * mult * 0.35f;
        b->posicao.y += b->velocidade.y * mult * 0.35f;
    } else {
        b->posicao.x += b->velocidade.x * mult;
        b->posicao.y += b->velocidade.y * mult;
    }

    // Paredes
    if (b->posicao.x <= b->raio || b->posicao.x >= LARGURA_TELA - b->raio) b->velocidade.x *= -1;
    if (b->posicao.y <= b->raio) b->velocidade.y *= -1;

    // Colisão Player
    if (CheckCollisionCircleRec(b->posicao, b->raio, p->rect)) {
        b->velocidade.y *= -1;
        b->posicao.y = p->rect.y - b->raio - 1;
    }

    // Colisão Blocos
    Bloco *atual = *l;
    while (atual != NULL) {
        if (atual->ativo) {
            if (CheckCollisionCircleRec(b->posicao, b->raio, atual->rect)) {
                b->velocidade.y *= -1;
                atual->vida--;
                if (atual->vida <= 0) {
                    atual->ativo = false;
                    pontuacao += 100;
                }
                break;
            }
        }
        atual = atual->prox;
    }

    // Passar de Nível
    if (todosBlocosDestruidos(*l)) {
        nivel++;
        destruirLista(*l);
        *l = gerarBlocos(nivel);

        b->posicao = (Vector2){ LARGURA_TELA/2, ALTURA_TELA/2 };
        b->velocidade = (Vector2){ 3.2f, -3.2f };

        roundTimer = 3.0f;
        respawnTimer = 1.5f;
    }

    // Morreu
    if (b->posicao.y > ALTURA_TELA) {
        p->vidas--;
        vidas = p->vidas;

        if (p->vidas <= 0) {
            SalvarRanking(pontuacao); // Salva arquivo
            currentState = GAME_OVER;
        } else {
            b->posicao = (Vector2){ LARGURA_TELA/2, ALTURA_TELA/2 };
            b->velocidade = (Vector2){ 3.2f, -3.2f };
            respawnTimer = 1.5f;
        }
    }
}
