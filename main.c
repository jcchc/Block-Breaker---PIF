#include "raylib.h"
#include "game_def.h"
#include "game_logic.h"
#include "graphics.h"
#include "blocks.h"

int main(void) {
    InitWindow(900, 650, "Block Breaker Final");
    InitAudioDevice();
    
    SetExitKey(KEY_NULL); 
    SetTargetFPS(60);    

    InitGraphics();
    
    IniciarJogo(&player, &ball, &listaBlocos); 
    currentState = MENU; 

    while (!WindowShouldClose()) {
        if (currentState == MENU && IsKeyPressed(KEY_ESCAPE)) {
            break; 
        }

        AtualizarLogica(&player, &ball, &listaBlocos);

        BeginDrawing();
            ClearBackground((Color){10, 10, 15, 255});
            DrawGameFrame(); 
        EndDrawing();
    }

    destruirLista(listaBlocos);
    UnloadGraphics();
    CloseWindow();
    CloseAudioDevice();
    
    return 0;
}
