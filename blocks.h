#ifndef BLOCKS_H
#define BLOCKS_H

#include <raylib.h>
#include <stdbool.h>

typedef struct Bloco {
    Rectangle rect;
    int vida;
    int tipo;
    bool ativo;
    struct Bloco *prox;
} Bloco;

Bloco* gerarBlocos(int level);
void destruirLista(Bloco *head);
bool todosBlocosDestruidos(Bloco *head);

#endif
