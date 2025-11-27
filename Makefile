
PROJETO_NOME = jogo_final

# Compilador C
CC = gcc

)
FONTES = main.c blocks.c graphics.c game_logic.c

CFLAGS = -Wpedantic -Wall -I/usr/local/include -std=c99


LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


all: $(PROJETO_NOME)

$(PROJETO_NOME): $(FONTES)
	$(CC) $(FONTES) -o $(PROJETO_NOME) $(CFLAGS) $(LDFLAGS)


clean:
	rm -f $(PROJETO_NOME)
