# Nome do seu executável final
PROJETO_NOME = jogo_final

# Compilador C
CC = gcc

# Arquivos fonte .c (só temos o main.c por enquanto)
FONTES = main.c blocks.c graphics.c game_logic.c
# Flags de Compilação
CFLAGS = -Wpedantic -Wall -I/usr/local/include -std=c99

# Flags de Linkagem (A MÁGICA está aqui)
# Diz ao compilador para usar a 'raylib' e suas dependências
LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Comando para criar o executável
all: $(PROJETO_NOME)

$(PROJETO_NOME): $(FONTES)
	$(CC) $(FONTES) -o $(PROJETO_NOME) $(CFLAGS) $(LDFLAGS)

# Comando para limpar
clean:
	rm -f $(PROJETO_NOME)
