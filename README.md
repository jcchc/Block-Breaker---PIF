# 🧱 BLOCK BREAKER – ARKANOID REMASTERIZADO

Um jogo desenvolvido em **C + Raylib**, onde seu objetivo é **destruir todos os blocos**, **desviar a bola** e **sobreviver aos níveis cada vez mais difíceis**!

Com diferentes mapas, dificuldade progressiva, ranking local e efeitos visuais, este Arkanoid traz uma jogabilidade rápida e desafiadora.

## 👨‍💻 Membros da Equipe

* Rodrigo Vinhas Marques - rodrigovinhasmarques@gmail.com
* Julio Cesar Coutinho Holanda Cavalcanti - julioholanda.jh2007@gmail.com
* João Luiz de Lima Bacelar - joaollbacelar@gmail.com

## ✅ Pré-requisitos

Para rodar o jogo, você precisa ter instalado:
* Linux (Ubuntu/Debian), macOS ou Windows
* GCC
* Make (ou `mingw32-make` no Windows)
* Raylib

---

## 🛠️ Instalação da Raylib

### 🐧 Linux (Ubuntu/Debian)
Execute no terminal:
```bash
sudo apt update
sudo apt install build-essential git libraylib-dev
```

### 🍎 macOS
Instale o Homebrew (se ainda não tiver):
```bash
/bin/bash -c "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh](https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh))"
```

Depois instale a Raylib:
```bash
brew install raylib
```

### 🪟 Windows
Recomendado: MinGW + vcpkg.

1. Instale o vcpkg
2. Configure a variável de ambiente `VCPKG_ROOT`
3. Instale a Raylib:
```bash
vcpkg install raylib
```

Se o comando `make` não funcionar no terminal:
```powershell
Set-Alias -Name make -Value mingw32-make
```

---

## ♟️ Como Rodar o Jogo

Clone o repositório:
```bash
git clone [https://github.com/jcchc/Block-Breaker.git](https://github.com/jcchc/Block-Breaker.git)
```

Entre na pasta do projeto:
```bash
cd Block-Breaker
```

Compilando no Linux:
```
make
./jogo_final
```
Compilando no MacOS

```
gcc *.c -o jogo_final -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
./jogo_final
---
```
## 🕹️ Instruções do Jogo

### 🎮 Navegação no Menu
* **Enter** — Inicia o jogo
* **R** — Exibe o ranking
* **Esc** — Fecha o jogo

### 1️⃣ Modos de Jogo
**Modo Normal**
* 6 níveis progressivos
* Cada nível possui um layout diferente de blocos
* Destrua todos os blocos para avançar

**Modo Sobrevivência**
* Dificuldade aumenta automaticamente
* Objetivo: sobreviver o máximo possível
* Iniciado após término das 6 fases iniciais

**Rankings**
* Guarda as maiores pontuações
* Exibidas no menu de rankings

### 2️⃣ Gameplay
* `←` — mover para a esquerda
* `→` — mover para a direita
* Rebata a bola
* Destrua todos os blocos
* **Não deixe a bola cair!**

### 3️⃣ Game Over / Vitória
* **Vitória:** alcançar a maior pontuação possível
* **Derrota:** a bola caiu
* `M` — Volta ao menu
* `R` — Reinicia o jogo
