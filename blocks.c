#include "blocks.h"
#include <stdlib.h>

int calcX(int col) {
    int larguraBloco = 50; 
    int espaco = 5;
    
    int larguraTotalGrid = (12 * (larguraBloco + espaco)); 
    
    return ((800 - larguraTotalGrid) / 2) + (col * (larguraBloco + espaco));
}

Bloco* criarDoMapa(Bloco *head, char *desenho[10], int level) {
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 12; j++) {
            
            if (desenho[i][j] == '1') {
                Bloco *novoBloco = (Bloco*) malloc(sizeof(Bloco));
                
                novoBloco->rect = (Rectangle){ calcX(j), i * 30 + 40, 60, 20 };
                novoBloco->vida = 1;
                novoBloco->ativo = true;
                novoBloco->tipo = (level % 6) + 1;
                
                novoBloco->prox = head;
                head = novoBloco;
            }
        }
    }
    return head;
}

Bloco* gerarBlocos(int level) {
    Bloco *head = NULL;

    if (level >= 2 && level <= 6) {

        if (level == 2) {
            char *mapa[] = {
                "000100001000",
                "000010010000",
                "000111111000",
                "001101101100",
                "011111111110",
                "101111111101", 
                "101000000101",
                "000110011000",
                "000000000000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 3) {
            char *mapa[] = {
                "000011110000",
                "000111111000",
                "001111111100",
                "001101101100",
                "001111111100",
                "001111111100",
                "001111111100",
                "001101101100",
                "001001001000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 4) {
            char *mapa[] = {
                "001111111100",
                "011111111110",
                "110111111011",
                "111111111111",
                "111111111111",
                "111000000111",
                "110111111011",
                "011011110110",
                "001111111100",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 5) {
            char *mapa[] = {
                "000000000000",
                "011000000110",
                "111100001111",
                "111110011111",
                "011111111110",
                "001111111100",
                "000111111000",
                "000011110000",
                "000001100000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 6) {
            char *mapa[] = {
                "000001100000",
                "000011110000",
                "000111111000",
                "001111111100",
                "000001100000",
                "000001100000",
                "000001100000",
                "000001100000",
                "000001100000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }

    } 
    else {
        int numLinhas;
        int alturaInicial = 40;
        int numColunas = 10;
        int paddingX = 25;

        if (level == 1) {
            numLinhas = 2;
            numColunas = 8;
            alturaInicial = 250;
            paddingX = 100;
        }
        else {
            numLinhas = 4 + (level - 6);
            if (numLinhas > 9) numLinhas = 9;
        }

        for (int i = 0; i < numLinhas; i++) {
            for (int j = 0; j < numColunas; j++) {
                Bloco *novoBloco = (Bloco*) malloc(sizeof(Bloco));
                
                novoBloco->rect = (Rectangle){ j * 75 + paddingX, i * 30 + alturaInicial, 70, 20 };
                
                novoBloco->vida = 1;
                novoBloco->ativo = true;
                novoBloco->tipo = (i % 5) + 1;
                novoBloco->prox = head;
                head = novoBloco;
            }
        }
    }

    return head;
}

void destruirLista(Bloco *head) {
    Bloco *atual = head;
    Bloco *proximo;
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

bool todosBlocosDestruidos(Bloco *head) {
    Bloco *atual = head;
    while (atual != NULL) {
        if (atual->ativo == true) return false;
        atual = atual->prox;
    }
    return true;
}