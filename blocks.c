#include "blocks.h"
#include <stdlib.h>

Bloco* gerarBlocos(int level) {
    Bloco *head = NULL; 
    int numLinhas = 5;
    int numColunas = 10;

    for (int i = 0; i < numLinhas; i++) {
        for (int j = 0; j < numColunas; j++) {
            
            Bloco *novoBloco = (Bloco*) malloc(sizeof(Bloco));

            novoBloco->rect.x = j * 80 + 10;  //isso veio implicito na biblioteca raylib
            novoBloco->rect.y = i * 30 + 50; 
            novoBloco->rect.width = 75;
            novoBloco->rect.height = 25;
            novoBloco->ativo = true;
            novoBloco->vida = 1 + level;
            novoBloco->tipo = 1;

      
            novoBloco->prox = head;
            head = novoBloco;
        }
    }
    return head;
}

void destruirLista(Bloco *head) {
Bloco *atual = head;
    Bloco *proximo;

    while (atual != NULL) {
        proximo = atual->prox; // Guarda o endereço do próximo antes de destruir o atual
        free(atual);           // Devolve o "quarto" (memória) para o sistema
        atual = proximo;       // Avança para o próximo
    }
}

bool todosBlocosDestruidos(Bloco *head) {
    Bloco *atual = head;
    
    while (atual != NULL) {
        // Se encontrar APENAS UM bloco que ainda esteja ativo (vivo)...
        if (atual->ativo == true) {
            return false; // ...então o nível NÃO acabou.
        }
        atual = atual->prox; // Continua procurando
    }
    
    // Se passou por todos e não retornou false, é porque todos morreram.
    return true; 
}