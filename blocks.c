#include "blocks.h"
#include <stdlib.h>

int calcX(int col) {
    int larguraBloco = 60;
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

    if (level >= 2 && level <= 11) {

        if (level == 2) {
            char *mapa[] = {
                "100000000001",
                "010000000010",
                "001000000100",
                "000100001000",
                "000010010000",
                "000010010000",
                "000100001000",
                "001000000100",
                "010000000010",
                "100000000001"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 3) {
            char *mapa[] = {
                "000100001000",
                "000010010000",
                "000111111000",
                "001101101100",
                "011111111110",
                "010111111010",
                "010100001010",
                "000011110000",
                "000000000000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 4) {
            char *mapa[] = {
                "001100001100",
                "011110011110",
                "010010010010",
                "010010010010",
                "011111111110",
                "010000000010",
                "011000000110",
                "001111111100",
                "000111111000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 5) {
            char *mapa[] = {
                "001100001100",
                "011110011110",
                "111111111111",
                "111111111111",
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
                "000011110000",
                "000011110000",
                "000011110000",
                "000011110000",
                "000011110000",
                "000011110000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 7) {
            char *mapa[] = {
                "000001100000",
                "000011110000",
                "000111111000",
                "001111111100",
                "011111111110",
                "111111111111",
                "000000000000",
                "000000000000",
                "000000000000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 8) {
            char *mapa[] = {
                "000001100000",
                "000011110000",
                "000111111000",
                "001111111100",
                "011111111110",
                "001111111100",
                "000111111000",
                "000011110000",
                "000001100000",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 9) {
            char *mapa[] = {
                "000011110000",
                "000111111000",
                "001111111100",
                "001101101100",
                "001111111100",
                "001111111100",
                "001111111100",
                "001010010100",
                "001010010100",
                "000000000000"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 10) {
            char *mapa[] = {
                "110011001100",
                "110011001100",
                "111111111111",
                "111111111111",
                "110000000011",
                "110000000011",
                "111111111111",
                "111111111111",
                "110000000011",
                "110000000011"
            };
            head = criarDoMapa(head, mapa, level);
        }
        else if (level == 11) {
            char *mapa[] = {
                "111111111111",
                "011111111110",
                "001111111100",
                "000111111000",
                "000011110000",
                "000001100000",
                "000001100000",
                "000001100000",
                "000111111000",
                "000111111000"
            };
            head = criarDoMapa(head, mapa, level);
        }

    } else {
        int numLinhas;
        if (level == 1) numLinhas = 3;
        else numLinhas = 4 + (level - 12);

        if (numLinhas > 9) numLinhas = 9;

        for (int i = 0; i < numLinhas; i++) {
            for (int j = 0; j < 10; j++) {
                Bloco *novoBloco = (Bloco*) malloc(sizeof(Bloco));
                novoBloco->rect = (Rectangle){ j * 75 + 25, i * 30 + 40, 70, 20 };
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