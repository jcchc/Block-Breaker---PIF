# 🧱 BLOCK BREAKER – ARKANOID REMASTERIZADO

Um jogo desenvolvido em **C + Raylib**, onde seu objetivo é **destruir todos os blocos**, **desviar a bola** e **sobreviver aos níveis cada vez mais difíceis**!  
Com diferentes mapas, dificuldade progressiva, ranking local e efeitos visuais, este Arkanoid traz uma **jogabilidade rápida e desafiadora**.

---

## 👨‍💻 Membros da Equipe

- **Rodrigo**  
- **Julio**  
- **Joao**  
- **Luis**

---

## ✅ Pré-requisitos

Para rodar o jogo, você precisa ter instalado:

- Linux (Ubuntu/Debian), macOS ou Windows  
- GCC  
- Make (ou `mingw32-make` no Windows)  
- **Raylib**  

---

## 🛠️ Instalação da Raylib

### 🐧 Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential git libraylib-dev
🍎 macOS
Instale o Homebrew (se ainda não tiver):

bash
Copiar código
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
Depois instale a Raylib:

bash
Copiar código
brew install raylib
🪟 Windows
Recomendado: MinGW + vcpkg

Instale o vcpkg

Configure a variável de ambiente VCPKG_ROOT

Instale a Raylib:

bash
Copiar código
vcpkg install raylib
Se o comando make não funcionar:

powershell
Copiar código
Set-Alias -Name make -Value mingw32-make
♟️ Como Rodar o Jogo
Clone o repositório:

bash
Copiar código
git clone https://github.com/jcchc/Block-Breaker--PIF.git
Entre na pasta do projeto:

bash
Copiar código
cd BlockBreaker
Compile e execute:

bash
Copiar código
make run
ou

bash
Copiar código
make
./jogo_final
🕹️ Instruções do Jogo
🎮 Navegação no Menu
Enter — Inicia o jogo

R — Exibe o ranking

Esc — Fecha o jogo

1️⃣ Modos de Jogo
Modo Normal
10 níveis progressivos

Cada nível possui um layout diferente de blocos

Destrua todos os blocos para avançar

Modo Infinito (opcional)
Dificuldade aumenta automaticamente

Objetivo: sobreviver o máximo possível

Rankings
Guarda as maiores pontuações

Exibidas no menu de rankings

2️⃣ Gameplay
← — mover para a esquerda

→ — mover para a direita

Rebata a bola

Destrua todos os blocos

Não deixe a bola cair!

3️⃣ Game Over / Vitória
Vitória: destrua todos os blocos

Derrota: a bola caiu

M — Volta ao menu

R — Reinicia o jogo

