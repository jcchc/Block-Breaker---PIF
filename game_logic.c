#include "game_logic.h"
#include <stdlib.h> // Para NULL

// =========================================================
// 1. DEFINIÇÃO DAS VARIÁVEIS GLOBAIS
// (Aqui elas nascem de verdade. Sem 'static', sem 'extern')
// =========================================================
Player player;
Ball ball;
Bloco *listaBlocos = NULL;

int pontuacao = 0;
int nivel = 1;
int vidas = 3;
GameScreen currentState = MENU;
int fakeRankings[5] = {1200, 950, 700, 400, 100};
bool jogoFinalizado = false;

// Constantes locais
const int LARGURA_TELA = 900;
const int ALTURA_TELA = 650;

// =========================================================
// 2. FUNÇÕES DE LÓGICA
// =========================================================

void IniciarJogo(Player *p, Ball *b, Bloco **l) {
    // Configura o Player (Raquete)
    p->rect = (Rectangle){ LARGURA_TELA/2 - 50, ALTURA_TELA - 40, 100, 20 };
    p->velocidade = 7.0f;
    p->vidas = 3; // Reseta vidas

    // Configura a Bola
    b->posicao = (Vector2){ LARGURA_TELA/2, ALTURA_TELA/2 };
    b->velocidade = (Vector2){ 4.0f, -4.0f };
    b->raio = 8.0f;
    b->ativa = true;

    // Reseta Globais
    pontuacao = 0;
    nivel = 1;
    vidas = 3;
    jogoFinalizado = false;
    currentState = GAMEPLAY; // Vai direto pro jogo ao iniciar

    // Reseta Blocos (Integração com seu blocks.c)
    if (*l != NULL) {
        destruirLista(*l);
    }
    *l = gerarBlocos(nivel); 
}

void AtualizarLogica(Player *p, Ball *b, Bloco **l) {
    
    // Se estiver no menu ou game over, a lógica de jogo não roda
    if (currentState != GAMEPLAY) {
        // Lógica simples de transição (Enter para jogar)
        if (currentState == MENU && IsKeyPressed(KEY_ENTER)) {
            IniciarJogo(p, b, l);
        }
        if (currentState == GAME_OVER && IsKeyPressed(KEY_R)) {
            currentState = MENU;
        }
        return;
    }

    // --- 1. Movimento da Raquete ---
    if (IsKeyDown(KEY_LEFT) && p->rect.x > 0) 
        p->rect.x -= p->velocidade;
    if (IsKeyDown(KEY_RIGHT) && p->rect.x < LARGURA_TELA - p->rect.width) 
        p->rect.x += p->velocidade;

    // --- 2. Movimento da Bola ---
    // Velocidade aumenta levemente com o nível
    float mult = 1.0f + (nivel * 0.1f);
    b->posicao.x += b->velocidade.x * mult;
    b->posicao.y += b->velocidade.y * mult;

    // --- 3. Colisão com Paredes ---
    if (b->posicao.x <= b->raio || b->posicao.x >= LARGURA_TELA - b->raio)
        b->velocidade.x *= -1;
    if (b->posicao.y <= b->raio)
        b->velocidade.y *= -1;

    // --- 4. Colisão com Raquete ---
    if (CheckCollisionCircleRec(b->posicao, b->raio, p->rect)) {
        b->velocidade.y *= -1;
        b->posicao.y = p->rect.y - b->raio - 1; // Desgruda
    }

    // --- 5. Colisão com Blocos (SUA LISTA ENCADEADA) ---
    Bloco *atual = *l;
    while (atual != NULL) {
        if (atual->ativo) {
            if (CheckCollisionCircleRec(b->posicao, b->raio, atual->rect)) {
                b->velocidade.y *= -1; // Rebate
                atual->vida--;
                
                if (atual->vida <= 0) {
                    atual->ativo = false;
                    pontuacao += 100;
                }
                break; // Só bate em um bloco por vez
            }
        }
        atual = atual->prox;
    }

    // --- 6. Passar de Fase ---
    if (todosBlocosDestruidos(*l)) {
        nivel++;
        destruirLista(*l);
        *l = gerarBlocos(nivel); // Carrega o próximo desenho (Alien, etc)
        
        // Reseta bola
        b->posicao = (Vector2){ LARGURA_TELA/2, ALTURA_TELA/2 };
        b->velocidade = (Vector2){ 4.0f, -4.0f };
    }

    // --- 7. Game Over / Perder Vida ---
    if (b->posicao.y > ALTURA_TELA) {
        p->vidas--;
        vidas = p->vidas; // Atualiza global
        
        if (p->vidas <= 0) {
            currentState = GAME_OVER;
        } else {
            // Reseta bola
            b->posicao = (Vector2){ LARGURA_TELA/2, ALTURA_TELA/2 };
            b->velocidade = (Vector2){ 4.0f, -4.0f };
        }
    }
}