#ifndef GRAPHICS_H_
#define GRAPHICS_H_

// Define as telas do jogo (para a máquina de estados)
typedef enum GameScreen {
    MENU,
    GAMEPLAY,
    RANKINGS,
    GAME_OVER
} GameScreen;

void RunGame(void);

#endif 
