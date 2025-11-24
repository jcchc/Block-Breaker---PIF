#include "raylib.h"
#include "game_logic.h" // Lógica
#include "graphics.h"   // Desenho
#include "blocks.h"     // Seus blocos

int main(void) {
    // Vamos usar o tamanho que estava no graphics.c para não quebrar o layout
    const int LARGURA = 900; 
    const int ALTURA = 650;

    InitWindow(LARGURA, ALTURA, "Block Breaker - Versão Final");
    SetTargetFPS(60);

    // Inicializa os Módulos
    InitGraphics();
    
    // Precisamos criar as variáveis do jogo que o game_logic espera
    // (Player, Ball e Lista de Blocos são iniciados dentro de IniciarJogo)
    // Mas precisamos passar os endereços delas.
    
    // Como as variáveis são globais no game_logic.c (sem static), 
    // a função IniciarJogo vai resetá-las.
    
    // Vamos chamar a função de reset da lógica para começar tudo
    // Nota: No seu game_logic.h, verifique se a função pede parâmetros ou usa globais.
    // Baseado no último código que fizemos, vamos assumir o uso das globais com extern.
    
    // Se o seu game_logic.c usa variáveis globais 'player', 'ball', etc:
    // Apenas chamamos um reset ou configuramos manualmente aqui se necessário.
    // Mas espere! No código que fizemos antes, IniciarJogo pedia ponteiros.
    // Vamos simplificar e chamar as globais direto se você definiu elas no game_logic.c
    
    // --- CORREÇÃO RÁPIDA PARA O MAIN ---
    // Vamos declarar as variáveis aqui e passar para a lógica, 
    // OU confiar que game_logic.c já as tem.
    
    // Para garantir que funcione com o código que você tem agora:
    extern Player player;
    extern Ball ball;
    extern Bloco *listaBlocos;
    
    // Inicializa o jogo (Chama a função da lógica)
    IniciarJogo(&player, &ball, &listaBlocos);

    // Loop Principal
    while (!WindowShouldClose()) {
        
        // 1. Atualiza a Lógica (Matemática)
        AtualizarLogica(&player, &ball, &listaBlocos);

        // 2. Desenha o Quadro (Gráfico)
        BeginDrawing();
        ClearBackground((Color){20, 20, 30, 255}); // Cor de fundo escura
        
        DrawGameFrame(); // A função mágica do graphics.c

        EndDrawing();
    }

    // Limpeza
    destruirLista(listaBlocos);
    UnloadGraphics();
    CloseWindow();

    return 0;
}