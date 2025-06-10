# Documentação Técnica Completa - Jogo da Cobra Avançado em C

## Índice
1. [Visão Geral do Projeto](#visão-geral-do-projeto)
2. [Arquitetura e Design](#arquitetura-e-design)
3. [Análise Detalhada das Bibliotecas](#análise-detalhada-das-bibliotecas)
4. [Sistema de Constantes e Configurações](#sistema-de-constantes-e-configurações)
5. [Estruturas de Dados Detalhadas](#estruturas-de-dados-detalhadas)
6. [Variáveis Globais e Gestão de Estado](#variáveis-globais-e-gestão-de-estado)
7. [Análise Função por Função](#análise-função-por-função)
8. [Fluxo de Execução Detalhado](#fluxo-de-execução-detalhado)
9. [Sistemas Especializados](#sistemas-especializados)
10. [Algoritmos e Lógica Complexa](#algoritmos-e-lógica-complexa)
11. [Gestão de Memória e Performance](#gestão-de-memória-e-performance)
12. [Tratamento de Erros e Edge Cases](#tratamento-de-erros-e-edge-cases)
13. [Extensibilidade e Manutenção](#extensibilidade-e-manutenção)

---

## Visão Geral do Projeto

### Contexto e Propósito
Este projeto implementa uma versão avançada do clássico jogo Snake (Cobra) em linguagem C, desenvolvido especificamente para ambiente Windows. O jogo transcende a implementação básica tradicional, incorporando múltiplos modos de jogo, sistema de power-ups, suporte a multiplayer e mecânicas de gameplay inovadoras.

### Características Técnicas Principais
- **Linguagem**: C (padrão C99/C11)
- **Plataforma**: Windows (específico)
- **Paradigma**: Programação procedural com estruturas
- **Interface**: Console/Terminal com controle direto de cursor
- **Entrada**: Teclado com detecção de teclas em tempo real
- **Renderização**: Baseada em caracteres ASCII no console

### Objetivos de Design
1. **Modularidade**: Código organizado em funções especializadas
2. **Extensibilidade**: Fácil adição de novos modos e power-ups
3. **Performance**: Execução eficiente em tempo real
4. **Usabilidade**: Interface intuitiva e responsiva
5. **Robustez**: Tratamento adequado de casos extremos

---

## Arquitetura e Design

### Padrão Arquitetural
O projeto segue um padrão **Game Loop** clássico com separação clara de responsabilidades:

```
┌─────────────────┐
│   Inicialização │
└─────────┬───────┘
          │
    ┌─────▼─────┐
    │   Setup   │
    └─────┬─────┘
          │
    ┌─────▼─────┐
    │Game Loop  │◄─────┐
    │           │      │
    │ ┌───────┐ │      │
    │ │ Draw  │ │      │
    │ └───┬───┘ │      │
    │ ┌───▼───┐ │      │
    │ │ Input │ │      │
    │ └───┬───┘ │      │
    │ ┌───▼───┐ │      │
    │ │ Logic │ │      │
    │ └───┬───┘ │      │
    │ ┌───▼───┐ │      │
    │ │ Sleep │ │      │
    │ └───────┘ │      │
    └─────┬─────┘      │
          │            │
    ┌─────▼─────┐      │
    │Game Over? │──────┘
    └─────┬─────┘
          │ Sim
    ┌─────▼─────┐
    │  Cleanup  │
    └───────────┘
```

### Separação de Responsabilidades

#### Camada de Apresentação
- **Renderização**: `draw()`, `drawBorder()`
- **Interface**: `showMenu()`, `showGameModeMenu()`, `showGameOver()`
- **Utilitários de Display**: `gotoxy()`, `clearScreen()`, `hideCursor()`

#### Camada de Controle
- **Entrada**: `input()`
- **Lógica Principal**: `logic()`
- **Configuração**: `setup()`, `initSnake()`

#### Camada de Dados
- **Estruturas**: `Snake`, `PowerUp`, `Coordinate`
- **Geração**: `generateFood()`, `generatePowerUp()`
- **Gestão de Estado**: `updatePowerUps()`, `applyPowerUpEffects()`

---

## Análise Detalhada das Bibliotecas

### stdio.h - Entrada/Saída Padrão
```c
#include <stdio.h>
```
**Funções Utilizadas:**
- `printf()`: Formatação e exibição de texto
- Macros de formatação para pontuações e informações

**Justificativa**: Essencial para comunicação com o usuário através do console.

### stdlib.h - Utilitários Padrão
```c
#include <stdlib.h>
```
**Funções Utilizadas:**
- `srand()`: Inicialização do gerador de números aleatórios
- `rand()`: Geração de números pseudo-aleatórios
- `system()`: Execução de comandos do sistema (cls)

**Análise Crítica**: O uso de `system("cls")` pode ser considerado inseguro em ambientes de produção, mas é adequado para este contexto educacional.

### conio.h - Console I/O (Windows)
```c
#include <conio.h>
```
**Funções Críticas:**
- `_kbhit()`: Detecção não-bloqueante de teclas pressionadas
- `_getch()`: Leitura de caractere sem echo

**Importância**: Permite entrada de dados em tempo real sem interromper o fluxo do jogo.

**Limitação**: Biblioteca não-padrão, específica para Windows.

### windows.h - API Windows
```c
#include <windows.h>
```
**Funcionalidades Utilizadas:**
- `SetConsoleCursorPosition()`: Posicionamento preciso do cursor
- `GetStdHandle()`: Obtenção de handle do console
- `SetConsoleCursorInfo()`: Controle de visibilidade do cursor
- `Sleep()`: Pausa de execução para controle de frame rate

**Estruturas Utilizadas:**
- `COORD`: Coordenadas do console
- `CONSOLE_CURSOR_INFO`: Informações do cursor

### time.h - Funções de Tempo
```c
#include <time.h>
```
**Aplicações:**
- `time()`: Timestamp atual para timers
- `time_t`: Tipo para armazenamento de tempo
- Controle de duração de power-ups
- Implementação do modo tempo limitado

### stdbool.h - Tipos Booleanos
```c
#include <stdbool.h>
```
**Benefícios:**
- Código mais legível com `true`/`false`
- Tipo `bool` explícito
- Melhor expressão de intenções no código

---

## Sistema de Constantes e Configurações

### Constantes de Dimensionamento
```c
#define WIDTH 60        // Largura do campo de jogo
#define HEIGHT 20       // Altura do campo de jogo
#define MAX_SNAKE_LENGTH 500  // Limite teórico da cobra
```

**Análise de Design:**
- `WIDTH 60`: Escolhido para caber em consoles padrão (80 colunas)
- `HEIGHT 20`: Proporciona gameplay equilibrado
- `MAX_SNAKE_LENGTH 500`: Limite generoso que raramente será atingido

**Cálculo de Área Total:**
```
Área jogável = WIDTH × HEIGHT = 60 × 20 = 1200 posições
Área total com bordas = (WIDTH+2) × (HEIGHT+2) = 62 × 22 = 1364 posições
```

### Constantes de Gameplay
```c
#define MAX_POWERUPS 4          // Power-ups simultâneos por cobra
#define POWERUP_DURATION 10     // Duração em segundos
#define TIME_MODE_DURATION 60   // Duração do modo tempo
```

**Balanceamento:**
- `MAX_POWERUPS 4`: Permite combinações interessantes sem overwhelming
- `POWERUP_DURATION 10`: Tempo suficiente para impacto sem permanência
- `TIME_MODE_DURATION 60`: Desafio temporal apropriado

### Enumerações Implícitas

#### Direções de Movimento
```c
#define UP 0      // ↑
#define RIGHT 1   // →
#define DOWN 2    // ↓
#define LEFT 3    // ←
```

**Lógica de Oposição:**
```c
// Direções opostas diferem por 2
UP (0) ↔ DOWN (2)      // |0 - 2| = 2
RIGHT (1) ↔ LEFT (3)   // |1 - 3| = 2
```

#### Tipos de Power-ups
```c
#define TURBO 0         // Velocidade aumentada
#define SLOW_MOTION 1   // Velocidade reduzida
#define IMMUNITY 2      // Imunidade a colisões
#define DOUBLE_POINTS 3 // Pontuação dobrada
```

**Balanceamento de Poder:**
- **TURBO**: Vantagem de velocidade, risco de controle
- **SLOW_MOTION**: Controle preciso, menor eficiência
- **IMMUNITY**: Proteção absoluta, temporária
- **DOUBLE_POINTS**: Vantagem de pontuação, sem mudança de mecânica

#### Modos de Jogo
```c
#define TRADITIONAL 0   // Modo clássico
#define TIME_MODE 1     // Contra o tempo
#define INVERSE_MODE 2  // Controles invertidos
#define MULTIPLAYER 3   // Dois jogadores
```

---

## Estruturas de Dados Detalhadas

### Coordinate - Representação Espacial
```c
typedef struct {
    int x;  // Coordenada horizontal (0 a WIDTH+1)
    int y;  // Coordenada vertical (0 a HEIGHT+1)
} Coordinate;
```

**Análise Detalhada:**
- **Tipo**: `int` permite valores negativos para detecção de saída de bounds
- **範囲**: x ∈ [0, WIDTH+1], y ∈ [0, HEIGHT+1]
- **Origem**: (0,0) no canto superior esquerdo (padrão console)
- **Uso de Memória**: 8 bytes por coordenada (2 × 4 bytes)

**Operações Implícitas:**
```c
// Movimento em direções
switch(direction) {
    case UP:    coord.y--; break;  // Decrementa Y
    case DOWN:  coord.y++; break;  // Incrementa Y
    case LEFT:  coord.x--; break;  // Decrementa X
    case RIGHT: coord.x++; break;  // Incrementa X
}
```

### PowerUp - Sistema de Melhorias Temporárias
```c
typedef struct {
    Coordinate position;  // Localização no tabuleiro
    int type;            // Tipo específico do power-up
    bool active;         // Estado de ativação
    time_t endTime;      // Timestamp de expiração
} PowerUp;
```

**Análise da Estrutura:**
- **position**: Localização física no jogo (8 bytes)
- **type**: Identificador do efeito (4 bytes)  
- **active**: Flag de estado booleano (1 byte, alinhado para 4)
- **endTime**: Timestamp absoluto de expiração (8 bytes)
- **Total**: ~24 bytes por power-up

**Estados do Power-up:**
1. **Inativo**: `active = false`, aguardando ativação
2. **Ativo**: `active = true`, `endTime` no futuro
3. **Expirado**: `active = true`, `endTime` no passado (pendente limpeza)

**Lógica de Expiração:**
```c
// Verificação de expiração
if (powerUp.active && time(NULL) > powerUp.endTime) {
    powerUp.active = false;  // Desativa power-up
}
```

### Snake - Entidade Principal do Jogo
```c
typedef struct {
    Coordinate body[MAX_SNAKE_LENGTH];  // Segmentos do corpo
    int length;                         // Comprimento atual
    int direction;                      // Direção de movimento
    int score;                         // Pontuação acumulada
    bool alive;                        // Estado vital
    PowerUp activePowerUps[MAX_POWERUPS]; // Power-ups ativos
} Snake;
```

#### Análise Detalhada dos Campos

**body[MAX_SNAKE_LENGTH]:**
- **Propósito**: Array de coordenadas representando cada segmento
- **Indexação**: `body[0]` = cabeça, `body[length-1]` = cauda
- **Uso de Memória**: 500 × 8 bytes = 4KB por cobra
- **Crescimento**: Novos segmentos adicionados ao final

**Algoritmo de Movimento:**
```c
// Shift dos segmentos (cauda para cabeça)
for (int i = length - 1; i > 0; i--) {
    body[i] = body[i-1];  // Cada segmento assume posição do anterior
}
// Atualiza apenas a cabeça
updateHead(&body[0], direction);
```

**length:**
- **Inicial**: 1 (apenas cabeça)
- **Máximo**: 500 (limitado por MAX_SNAKE_LENGTH)
- **Crescimento**: Incrementa ao consumir comida
- **Uso**: Controla loops de renderização e colisão

**direction:**
- **Valores**: 0-3 (UP, RIGHT, DOWN, LEFT)
- **Restrição**: Não pode ser oposto à direção atual
- **Atualização**: Apenas via input do usuário
- **Persistência**: Mantém última direção válida

**score:**
- **Inicial**: 0
- **Incremento**: +1 por comida (ou +2 com DOUBLE_POINTS)
- **Tipo**: `int` (suporta até ~2 bilhões)
- **Uso**: Comparação em multiplayer, exibição final

**alive:**
- **Inicial**: `true`
- **Falso**: Colisão detectada
- **Uso**: Controle de game over, renderização condicional

**activePowerUps[MAX_POWERUPS]:**
- **Capacidade**: 4 power-ups simultâneos
- **Gestão**: Array circular ou busca linear
- **Memória**: 4 × 24 bytes = 96 bytes por cobra

---

## Variáveis Globais e Gestão de Estado

### Entidades Principais
```c
Snake snake1, snake2;  // Duas cobras para multiplayer
```
**Análise:**
- **snake1**: Jogador principal (sempre ativo)
- **snake2**: Segundo jogador (ativo apenas em MULTIPLAYER)
- **Memória total**: ~8KB para ambas as cobras
- **Inicialização**: Apenas snake1 sempre inicializada

### Elementos do Jogo
```c
Coordinate food;        // Posição da comida atual
Coordinate powerUpPos;  // Posição do power-up no tabuleiro
```

**food:**
- **Regeneração**: A cada consumo
- **Algoritmo**: Posição aleatória válida
- **Validação**: Não pode sobrepor cobras ou power-ups

**powerUpPos:**
- **Ativação**: Probabilística (20% após comer)
- **Duração**: 20 segundos no tabuleiro
- **Exclusividade**: Apenas um power-up por vez

### Controle de Estado do Jogo
```c
int gameMode = TRADITIONAL;  // Modo de jogo atual
bool powerUpsEnabled = false; // Habilitação de power-ups
bool gameOver = false;       // Estado de fim de jogo
bool exitGame = false;       // Controle de saída do programa
```

**Máquina de Estados do Jogo:**
```
[MENU] → [PLAYING] → [GAME_OVER] → [MENU]
   ↓
[EXIT]
```

### Controle de Power-ups
```c
bool powerUpActive = false;   // Power-up presente no tabuleiro
int powerUpType = -1;         // Tipo do power-up atual
time_t powerUpEndTime = 0;    // Tempo de expiração do power-up
```

**Ciclo de Vida do Power-up:**
1. **Geração**: 20% chance após comer
2. **Ativo**: Visível no tabuleiro
3. **Coletado**: Transferido para cobra
4. **Expirado**: Removido após 20s ou coleta

### Controle Temporal
```c
time_t gameModeEndTime = 0;  // Fim do modo tempo
int baseSpeed = 100;         // Velocidade base (ms)
```

**baseSpeed:**
- **Valor**: 100ms (10 FPS)
- **Modificação**: Power-ups alteram temporariamente
- **Efeitos**: 
  - TURBO: 50ms (20 FPS)
  - SLOW_MOTION: 200ms (5 FPS)

---

## Análise Função por Função

### main() - Ponto de Entrada Principal
```c
int main() {
    srand(time(NULL));  // Inicialização aleatória
    hideCursor();       // Oculta cursor para melhor visual
    
    while (!exitGame) {
        showMenu();     // Exibe menu principal
        if (exitGame) break;
        
        setup();        // Configura novo jogo
        
        // Game loop principal
        while (!gameOver) {
            draw();     // Renderiza estado atual
            input();    // Processa entrada
            logic();    // Atualiza lógica
            Sleep(currentSpeed);  // Controla FPS
        }
        
        showGameOver(); // Exibe resultados
    }
    
    showCursor();       // Restaura cursor
    return 0;
}
```

**Análise de Fluxo:**
1. **Inicialização única**: Semente aleatória, configuração de console
2. **Loop principal**: Menu → Jogo → Resultado → Repetir
3. **Game loop**: Ciclo render-input-logic-delay
4. **Cleanup**: Restauração do estado original do console

**Considerações de Design:**
- **Robustez**: Sempre restaura cursor antes de sair
- **Flexibilidade**: Permite múltiplos jogos sem reiniciar programa
- **Performance**: Sleep() evita uso excessivo de CPU

### setup() - Inicialização de Partida
```c
void setup() {
    clearScreen();     // Limpa tela para novo jogo
    gameOver = false;  // Reseta flag de fim de jogo
    
    // Inicializa cobra principal
    initSnake(&snake1, WIDTH / 4, HEIGHT / 2, RIGHT);
    
    // Inicializa segunda cobra (multiplayer)
    if (gameMode == MULTIPLAYER) {
        initSnake(&snake2, 3 * WIDTH / 4, HEIGHT / 2, LEFT);
    }
    
    generateFood();    // Gera primeira comida
    
    // Configura timer para modo tempo
    if (gameMode == TIME_MODE) {
        gameModeEndTime = time(NULL) + TIME_MODE_DURATION;
    }
}
```

**Posicionamento Inicial das Cobras:**
- **Snake1**: (WIDTH/4, HEIGHT/2) = (15, 10), direção RIGHT
- **Snake2**: (3*WIDTH/4, HEIGHT/2) = (45, 10), direção LEFT
- **Estratégia**: Posições simétricas, direções opostas para evitar colisão imediata

**Inicialização Condicional:**
- **Snake2**: Apenas no modo MULTIPLAYER
- **Timer**: Apenas no modo TIME_MODE
- **Otimização**: Evita inicializações desnecessárias

### initSnake() - Configuração Individual de Cobra
```c
void initSnake(Snake* snake, int startX, int startY, int direction) {
    snake->length = 1;               // Comprimento inicial
    snake->body[0].x = startX;       // Posição X inicial
    snake->body[0].y = startY;       // Posição Y inicial
    snake->direction = direction;     // Direção inicial
    snake->score = 0;                // Pontuação zerada
    snake->alive = true;             // Estado vivo
    
    // Limpa todos os power-ups
    for (int i = 0; i < MAX_POWERUPS; i++) {
        snake->activePowerUps[i].active = false;
    }
}
```

**Análise de Inicialização:**
- **Memória**: Apenas primeiro elemento do array body é definido
- **Estado**: Cobra começa no estado mais simples possível
- **Power-ups**: Explicitamente desativados para evitar estados indefinidos
- **Parâmetros**: Posição e direção configuráveis permitem diferentes starts

### draw() - Sistema de Renderização Principal
```c
void draw() {
    clearScreen();    // Limpa frame anterior
    drawBorder();     // Desenha bordas do jogo
    
    // Renderiza comida
    gotoxy(food.x, food.y);
    printf("F");
    
    // Renderiza power-up se ativo
    if (powerUpActive) {
        gotoxy(powerUpPos.x, powerUpPos.y);
        char powerUpChar = getPowerUpChar(powerUpType);
        printf("%c", powerUpChar);
    }
    
    // Renderiza cobra 1
    renderSnake(&snake1, 'O', 'o');
    
    // Renderiza cobra 2 (multiplayer)
    if (gameMode == MULTIPLAYER && snake2.alive) {
        renderSnake(&snake2, 'X', 'x');
    }
    
    // Renderiza HUD
    renderHUD();
}
```

**Ordem de Renderização (Z-Index):**
1. **Fundo**: Limpeza de tela
2. **Bordas**: Quadro do jogo
3. **Comida**: Elemento coletável
4. **Power-ups**: Elementos especiais
5. **Cobras**: Entidades principais
6. **HUD**: Informações overlay

**Otimizações Possíveis:**
- **Dirty Rectangles**: Apenas redesenhar áreas modificadas
- **Double Buffering**: Render off-screen para evitar flicker
- **Sprite Caching**: Pré-calcular representações

### Renderização de Cobra
```c
void renderSnake(Snake* snake, char headChar, char bodyChar) {
    for (int i = 0; i < snake->length; i++) {
        gotoxy(snake->body[i].x, snake->body[i].y);
        if (i == 0) {
            printf("%c", headChar);  // Cabeça distinta
        } else {
            printf("%c", bodyChar);  // Corpo uniforme
        }
    }
}
```

**Diferenciação Visual:**
- **Snake1**: 'O' (cabeça), 'o' (corpo)
- **Snake2**: 'X' (cabeça), 'x' (corpo)
- **Contraste**: Facilita identificação em multiplayer

### drawBorder() - Renderização de Bordas
```c
void drawBorder() {
    // Bordas horizontais
    for (int i = 0; i <= WIDTH + 1; i++) {
        gotoxy(i, 0);              // Borda superior
        printf("#");
        gotoxy(i, HEIGHT + 1);     // Borda inferior
        printf("#");
    }
    
    // Bordas verticais
    for (int i = 0; i <= HEIGHT + 1; i++) {
        gotoxy(0, i);              // Borda esquerda
        printf("#");
        gotoxy(WIDTH + 1, i);      // Borda direita
        printf("#");
    }
}
```

**Análise Geométrica:**
- **Dimensões**: (WIDTH+2) × (HEIGHT+2) = 62 × 22
- **Caracteres**: 2×62 + 2×22 - 4 = 164 caracteres '#'
- **Sobreposição**: Cantos são desenhados duas vezes (não afeta visual)

### input() - Sistema de Entrada de Dados
```c
void input() {
    if (_kbhit()) {  // Verifica se há tecla pressionada
        char key = _getch();  // Lê tecla sem echo
        
        // Controles Snake1 (condicionais por modo)
        if (gameMode == INVERSE_MODE) {
            // Controles invertidos
            handleInverseInput(&snake1, key);
        } else {
            // Controles normais
            handleNormalInput(&snake1, key);
        }
        
        // Controles Snake2 (apenas multiplayer)
        if (gameMode == MULTIPLAYER) {
            handlePlayer2Input(&snake2, key);
        }
        
        // Controles globais
        if (key == 27) {  // ESC
            gameOver = true;
        }
    }
}
```

**Análise de Entrada:**
- **Non-blocking**: `_kbhit()` permite execução contínua
- **Immediate**: `_getch()` não requer Enter
- **Condicional**: Processamento baseado no modo de jogo
- **Hierárquica**: Controles globais têm precedência

**Mapeamento de Teclas:**
```c
// Snake1 (Setas direcionais)
72 (↑) → UP
80 (↓) → DOWN  
75 (←) → LEFT
77 (→) → RIGHT

// Snake2 (WASD)
'w' → UP
's' → DOWN
'a' → LEFT
'd' → RIGHT

// Global
27 (ESC) → Sair do jogo
```

**Validação de Movimento:**
```c
// Previne movimento reverso instantâneo
if (key == UP_KEY && currentDirection != DOWN) {
    newDirection = UP;
}
```

### logic() - Núcleo da Lógica do Jogo
```c
void logic() {
    // 1. Atualiza movimento das cobras
    updateSnakeMovement(&snake1);
    if (gameMode == MULTIPLAYER && snake2.alive) {
        updateSnakeMovement(&snake2);
    }
    
    // 2. Atualiza power-ups
    updatePowerUps(&snake1);
    if (gameMode == MULTIPLAYER) {
        updatePowerUps(&snake2);
    }
    
    // 3. Detecta colisões
    checkCollisions(&snake1);
    if (gameMode == MULTIPLAYER && snake2.alive) {
        checkCollisions(&snake2);
    }
    
    // 4. Processa consumo de comida
    processFoodConsumption();
    
    // 5. Gerencia power-ups no tabuleiro
    manageBoardPowerUps();
    
    // 6. Verifica condições de fim de jogo
    checkGameOverConditions();
}
```

**Ordem de Execução Crítica:**
1. **Movimento**: Primeiro para posições atualizadas
2. **Power-ups**: Segundo para efeitos aplicados
3. **Colisões**: Terceiro com posições/efeitos atuais
4. **Consumo**: Quarto para crescimento da cobra
5. **Gestão**: Quinto para novos power-ups
6. **Verificação**: Último para estado final

### Algoritmo de Movimento da Cobra
```c
void updateSnakeMovement(Snake* snake) {
    // Shift de todos os segmentos (do fim para o início)
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i-1];
    }
    
    // Atualiza apenas a cabeça baseada na direção
    switch (snake->direction) {
        case UP:    snake->body[0].y--; break;
        case DOWN:  snake->body[0].y++; break;
        case LEFT:  snake->body[0].x--; break;
        case RIGHT: snake->body[0].x++; break;
    }
}
```

**Complexidade Temporal:** O(n) onde n é o comprimento da cobra
**Complexidade Espacial:** O(1) - modificação in-place

**Otimização Possível:**
```c
// Circular buffer approach
int head = (snake->head + 1) % MAX_SNAKE_LENGTH;
snake->body[head] = newHeadPosition;
snake->head = head;
if (growing) {
    snake->length++;
} else {
    snake->tail = (snake->tail + 1) % MAX_SNAKE_LENGTH;
}
```

### Sistema de Detecção de Colisões
```c
void checkCollisions(Snake* snake) {
    Coordinate head = snake->body[0];
    
    // 1. Colisão com bordas
    if (head.x <= 0 || head.x >= WIDTH + 1 ||
        head.y <= 0 || head.y >= HEIGHT + 1) {
        snake->alive = false;
        return;
    }
    
    // 2. Auto-colisão (se não imune)
    if (!hasImmunity(snake)) {
        for (int i = 1; i < snake->length; i++) {
            if (head.x == snake->body[i].x && 
                head.y == snake->body[i].y) {
                snake->alive = false;
                return;
            }
        }
    }
    
    // 3. Colisão com outra cobra (multiplayer)
    if (gameMode == MULTIPLAYER) {
        Snake* other = (snake == &snake1) ? &snake2 : &snake1;
        if (other->alive && !hasImmunity(snake)) {
            for (int i = 0; i < other->length; i++) {
                if (head.x == other->body[i].x && 
                    head.y == other->body[i].y) {
                    snake->alive = false;
                    return;
                }
            }
        }
    }
}
```

**Análise de Performance:**
- **Auto-colisão**: O(n) onde n é o comprimento da cobra
- **Colisão entre cobras**: O(n×m) onde n e m são os comprimentos das cobras
- **Otimização**: Spatial hashing poderia reduzir para O(1) em casos médios

**Casos Extremos:**
- **Cobra de comprimento 1**: Não há auto-colisão possível
- **Cobras de comprimento máximo**: 500×500 = 250.000 comparações por frame
- **Imunidade ativa**: Pula verificações de colisão, mantendo apenas detecção de bordas

### generateFood() - Geração Inteligente de Comida
```c
void generateFood() {
    bool validPosition;
    do {
        validPosition = true;
        food.x = 1 + rand() % WIDTH;   // Range: [1, WIDTH]
        food.y = 1 + rand() % HEIGHT;  // Range: [1, HEIGHT]
        
        // Validação contra cobra 1
        for (int i = 0; i < snake1.length; i++) {
            if (food.x == snake1.body[i].x && food.y == snake1.body[i].y) {
                validPosition = false;
                break;
            }
        }
        
        // Validação contra cobra 2 (multiplayer)
        if (validPosition && gameMode == MULTIPLAYER) {
            for (int i = 0; i < snake2.length; i++) {
                if (food.x == snake2.body[i].x && food.y == snake2.body[i].y) {
                    validPosition = false;
                    break;
                }
            }
        }
        
        // Validação contra power-up ativo
        if (validPosition && powerUpActive) {
            if (food.x == powerUpPos.x && food.y == powerUpPos.y) {
                validPosition = false;
            }
        }
    } while (!validPosition);
}
```

**Análise Algorítmica:**
- **Complexidade Média**: O(k×n) onde k é número de tentativas e n é comprimento total das cobras
- **Complexidade Pior Caso**: Infinita (teoricamente) se não há posições válidas
- **Probabilidade de Sucesso**: (Área Total - Área Ocupada) / Área Total

**Cálculo de Probabilidade:**
```
Área Total = WIDTH × HEIGHT = 60 × 20 = 1200
Área Ocupada Máxima = 2 × MAX_SNAKE_LENGTH + 1 (power-up) = 1001
Probabilidade Mínima = (1200 - 1001) / 1200 = 16.6%
```

**Otimizações Possíveis:**
1. **Lista de Posições Livres**: Manter array de posições disponíveis
2. **Rejection Sampling Limitado**: Máximo de tentativas antes de usar busca exaustiva
3. **Spatial Partitioning**: Dividir área em regiões para busca mais eficiente

### generatePowerUp() - Sistema de Power-ups no Tabuleiro
```c
void generatePowerUp() {
    bool validPosition;
    do {
        validPosition = true;
        powerUpPos.x = 1 + rand() % WIDTH;
        powerUpPos.y = 1 + rand() % HEIGHT;
        
        // Mesmo algoritmo de validação que generateFood()
        // ... código de validação ...
    } while (!validPosition);
    
    powerUpType = rand() % 4;  // 0-3 (TURBO, SLOW_MOTION, IMMUNITY, DOUBLE_POINTS)
    powerUpActive = true;
    powerUpEndTime = time(NULL) + 20;  // Expira em 20 segundos
}
```

**Distribuição de Power-ups:**
- **TURBO (0)**: 25% de probabilidade
- **SLOW_MOTION (1)**: 25% de probabilidade  
- **IMMUNITY (2)**: 25% de probabilidade
- **DOUBLE_POINTS (3)**: 25% de probabilidade

**Ciclo de Vida Temporal:**
```
Geração → Ativo (20s) → Expiração/Coleta
    ↓         ↓              ↓
Aparece   Visível        Desaparece
```

### updatePowerUps() - Gerenciamento de Estado dos Power-ups
```c
void updatePowerUps(Snake* snake) {
    // 1. Gerencia power-up no tabuleiro
    if (powerUpActive && time(NULL) > powerUpEndTime) {
        powerUpActive = false;
    }
    
    // 2. Atualiza power-ups ativos na cobra
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake->activePowerUps[i].active && 
            time(NULL) > snake->activePowerUps[i].endTime) {
            snake->activePowerUps[i].active = false;
        }
    }
    
    // 3. Aplica efeitos dos power-ups ativos
    applyPowerUpEffects(snake);
}
```

**Responsabilidades Múltiplas:**
1. **Limpeza Temporal**: Remove power-ups expirados
2. **Estado Ativo**: Mantém consistência de estados
3. **Aplicação de Efeitos**: Chama sistema de efeitos

**Análise de Performance:**
- **Complexidade**: O(MAX_POWERUPS) = O(4) = O(1)
- **Chamadas por Frame**: 1 por cobra ativa
- **Operações de Tempo**: Múltiplas chamadas `time(NULL)`

### applyPowerUpEffects() - Sistema de Efeitos Temporários
```c
void applyPowerUpEffects(Snake* snake) {
    int speed = baseSpeed;  // 100ms base
    
    // Aplica modificadores de velocidade
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake->activePowerUps[i].active) {
            switch (snake->activePowerUps[i].type) {
                case TURBO:
                    speed /= 2;  // 50ms (20 FPS)
                    break;
                case SLOW_MOTION:
                    speed *= 2;  // 200ms (5 FPS)
                    break;
                // IMMUNITY e DOUBLE_POINTS são aplicados contextualmente
            }
        }
    }
    
    baseSpeed = speed;  // Atualiza velocidade global
}
```

**Problemas de Design Identificados:**
1. **Efeito Global**: `baseSpeed` afeta ambas as cobras em multiplayer
2. **Acumulação de Efeitos**: Múltiplos TURBO podem causar divisão excessiva
3. **Aplicação Assimétrica**: IMMUNITY/DOUBLE_POINTS verificados em outros locais

**Sistema de Efeitos Corrigido (Proposta):**
```c
int calculateSnakeSpeed(Snake* snake) {
    int speed = 100;  // Base local
    int turboCount = 0, slowCount = 0;
    
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake->activePowerUps[i].active) {
            switch (snake->activePowerUps[i].type) {
                case TURBO: turboCount++; break;
                case SLOW_MOTION: slowCount++; break;
            }
        }
    }
    
    // Net effect calculation
    int netEffect = turboCount - slowCount;
    if (netEffect > 0) {
        speed /= (1 + netEffect);  // Faster
    } else if (netEffect < 0) {
        speed *= (1 - netEffect);  // Slower
    }
    
    return max(25, min(400, speed));  // Clamping
}
```

---

## Sistemas Especializados

### Sistema de Menus Hierárquicos

#### showMenu() - Menu Principal
```c
void showMenu() {
    clearScreen();
    
    // Título centralizado
    gotoxy(WIDTH / 2 - 10, 5);
    printf("===== JOGO DA COBRA =====");
    
    // Opções numeradas
    gotoxy(WIDTH / 2 - 10, 7);  printf("1. Iniciar Jogo");
    gotoxy(WIDTH / 2 - 10, 8);  printf("2. Selecionar Modo de Jogo");
    gotoxy(WIDTH / 2 - 10, 9);  printf("3. Ativar/Desativar Power-ups (Atual: %s)", 
                                       powerUpsEnabled ? "ATIVO" : "DESATIVADO");
    gotoxy(WIDTH / 2 - 10, 10); printf("4. Sair");
    
    // Estado atual
    gotoxy(WIDTH / 2 - 10, 11);
    printf("Modo atual: %s", getModeString(gameMode));
    
    // Prompt de entrada
    gotoxy(WIDTH / 2 - 10, 13);
    printf("Digite sua escolha: ");
    
    // Loop de entrada com validação
    char choice;
    do {
        choice = _getch();
        switch (choice) {
            case '1': return;  // Inicia jogo
            case '2': showGameModeMenu(); return showMenu();  // Submenu
            case '3': powerUpsEnabled = !powerUpsEnabled; return showMenu();  // Toggle
            case '4': exitGame = true; return;  // Sair
        }
    } while (true);
}
```

**Análise de UX:**
- **Centralização**: Cálculos baseados em WIDTH para responsividade
- **Estado Dinâmico**: Exibe configurações atuais
- **Navegação Recursiva**: `return showMenu()` para refresh após mudanças
- **Validação de Entrada**: Loop infinito até entrada válida

#### showGameModeMenu() - Submenu de Modos
```c
void showGameModeMenu() {
    // Layout similar ao menu principal
    // 5 opções: 4 modos + voltar
    
    char choice;
    do {
        choice = _getch();
        switch (choice) {
            case '1': gameMode = TRADITIONAL; return;
            case '2': gameMode = TIME_MODE; return;
            case '3': gameMode = INVERSE_MODE; return;
            case '4': gameMode = MULTIPLAYER; return;
            case '5': return;  // Volta ao menu principal
        }
    } while (true);
}
```

**Padrão de Navegação:**
```
Menu Principal
    ├── 1. Iniciar Jogo → Game Loop
    ├── 2. Modos de Jogo → Submenu
    │   ├── 1-4. Seleção → Volta ao Principal
    │   └── 5. Voltar → Volta ao Principal
    ├── 3. Toggle Power-ups → Refresh Principal
    └── 4. Sair → Termina Programa
```

### Sistema de Exibição de Resultados

#### showGameOver() - Tela de Fim de Jogo
```c
void showGameOver() {
    clearScreen();
    
    // Título
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 2);
    printf("===== FIM DE JOGO =====");
    
    // Pontuação principal
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2);
    printf("Pontuação Jogador 1: %d", snake1.score);
    
    // Multiplayer específico
    if (gameMode == MULTIPLAYER) {
        gotoxy(WIDTH / 2 - 10, HEIGHT / 2 + 1);
        printf("Pontuação Jogador 2: %d", snake2.score);
        
        // Determinação de vencedor
        gotoxy(WIDTH / 2 - 10, HEIGHT / 2 + 3);
        if (snake1.score > snake2.score) {
            printf("Jogador 1 venceu!");
        } else if (snake2.score > snake1.score) {
            printf("Jogador 2 venceu!");
        } else {
            printf("Empate!");
        }
    }
    
    // Prompt de continuação
    gotoxy(WIDTH / 2 - 15, HEIGHT / 2 + 5);
    printf("Pressione qualquer tecla para voltar ao menu...");
    
    _getch();  // Blocking wait
}
```

**Lógica de Vitória:**
1. **Single Player**: Sempre exibe pontuação do Jogador 1
2. **Multiplayer**: Compara pontuações e declara vencedor
3. **Empate**: Tratado explicitamente
4. **Modo Tempo**: Vencedor por pontuação no tempo limite

---

## Algoritmos e Lógica Complexa

### Algoritmo de Movimento da Cobra (Análise Detalhada)

#### Movimento Base
```c
// Shift Algorithm - O(n)
for (int i = snake1.length - 1; i > 0; i--) {
    snake1.body[i] = snake1.body[i-1];
}

// Head Update - O(1)
switch (snake1.direction) {
    case UP:    snake1.body[0].y--; break;
    case DOWN:  snake1.body[0].y++; break;
    case LEFT:  snake1.body[0].x--; break;
    case RIGHT: snake1.body[0].x++; break;
}
```

**Análise Matemática:**
- **Operações por Movimento**: n-1 cópias + 1 atualização = n operações
- **Bytes Transferidos**: (n-1) × 8 bytes por movimento
- **Frequência**: 10-20 vezes por segundo dependendo da velocidade

**Alternativa Otimizada - Circular Buffer:**
```c
typedef struct {
    Coordinate body[MAX_SNAKE_LENGTH];
    int head;    // Índice da cabeça
    int tail;    // Índice da cauda
    int length;  // Comprimento atual
} OptimizedSnake;

void moveSnake(OptimizedSnake* snake, int direction) {
    // Calcula nova posição da cabeça
    int newHead = (snake->head + 1) % MAX_SNAKE_LENGTH;
    snake->body[newHead] = snake->body[snake->head];
    
    // Atualiza posição baseada na direção
    updatePosition(&snake->body[newHead], direction);
    
    // Atualiza índices
    snake->head = newHead;
    if (snake->length < MAX_SNAKE_LENGTH) {
        snake->length++;
    } else {
        snake->tail = (snake->tail + 1) % MAX_SNAKE_LENGTH;
    }
}
```

**Vantagens do Circular Buffer:**
- **Complexidade**: O(1) vs O(n)
- **Performance**: Constante independente do tamanho
- **Memória**: Sem operações de cópia desnecessárias

### Algoritmo de Validação de Posições

#### Geração com Rejection Sampling
```c
void generateValidPosition(Coordinate* pos, bool (*validator)(Coordinate)) {
    int attempts = 0;
    const int MAX_ATTEMPTS = 1000;
    
    do {
        pos->x = 1 + rand() % WIDTH;
        pos->y = 1 + rand() % HEIGHT;
        attempts++;
        
        if (attempts > MAX_ATTEMPTS) {
            // Fallback: busca exaustiva
            findFirstValidPosition(pos, validator);
            return;
        }
    } while (!validator(*pos));
}

bool isValidFoodPosition(Coordinate pos) {
    // Verifica contra todas as entidades
    return !occupiedBySnake(pos) && 
           !occupiedByPowerUp(pos) && 
           !occupiedByBorder(pos);
}
```

**Análise Estatística:**
- **Tentativas Esperadas**: 1 / P(sucesso)
- **Caso Médio**: ~1.2 tentativas (quando 83% do espaço está livre)
- **Caso Extremo**: 1000 tentativas → busca exaustiva

### Sistema de Detecção de Colisões Otimizado

#### Spatial Hashing (Proposta)
```c
#define GRID_SIZE 10
#define GRID_WIDTH (WIDTH / GRID_SIZE + 1)
#define GRID_HEIGHT (HEIGHT / GRID_SIZE + 1)

typedef struct {
    int entities[GRID_WIDTH][GRID_HEIGHT][10];  // Máximo 10 entidades por célula
    int count[GRID_WIDTH][GRID_HEIGHT];
} SpatialGrid;

int getGridX(int x) { return x / GRID_SIZE; }
int getGridY(int y) { return y / GRID_SIZE; }

bool checkCollisionOptimized(Coordinate pos, SpatialGrid* grid) {
    int gx = getGridX(pos.x);
    int gy = getGridY(pos.y);
    
    // Verifica apenas células adjacentes (3x3)
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int cgx = gx + dx;
            int cgy = gy + dy;
            
            if (cgx >= 0 && cgx < GRID_WIDTH && 
                cgy >= 0 && cgy < GRID_HEIGHT) {
                // Verifica entidades na célula
                for (int i = 0; i < grid->count[cgx][cgy]; i++) {
                    if (grid->entities[cgx][cgy][i] == positionToId(pos)) {
                        return true;  // Colisão detectada
                    }
                }
            }
        }
    }
    return false;
}
```

**Vantagens do Spatial Hashing:**
- **Complexidade**: O(1) amortizada vs O(n)
- **Escalabilidade**: Performance constante mesmo com cobras grandes
- **Flexibilidade**: Suporta múltiplas entidades facilmente

---

## Gestão de Memória e Performance

### Análise de Uso de Memória

#### Estruturas Principais
```c
// Snake structure analysis
sizeof(Snake) = 
    sizeof(Coordinate) * MAX_SNAKE_LENGTH +  // 8 * 500 = 4000 bytes
    sizeof(int) * 4 +                        // 16 bytes (length, direction, score, alive)
    sizeof(PowerUp) * MAX_POWERUPS           // 24 * 4 = 96 bytes
    = 4112 bytes per snake

// Global memory usage
Snake snake1, snake2;                        // 8224 bytes
Coordinate food, powerUpPos;                 // 16 bytes  
Various int/bool globals;                    // ~50 bytes
Total static memory: ~8.3 KB
```

#### Stack vs Heap Usage
```
Stack Memory:
- Variáveis locais nas funções: ~100 bytes por chamada
- Call stack depth: ~5 níveis máximo
- Total stack usage: <1 KB

Heap Memory: 
- Nenhuma alocação dinâmica (malloc/calloc)
- Tudo em memória estática/stack

Total RAM footprint: <10 KB
```

### Análise de Performance por Frame

#### Operações por Frame (60x20 = 1200 posições)
```c
// Render phase - O(n + m + c)
clearScreen();          // ~1ms (system call)
drawBorder();           // 164 printf calls
draw();                 // n + m + 2 printf calls (n,m = snake lengths)

// Input phase - O(1)
input();                // 1 system call if key pressed

// Logic phase - O(n² + m²) worst case
logic();                // Movement: O(n + m)
                        // Collision: O(n² + m² + nm) worst case
                        // Food/PowerUp: O(n + m)

Total time per frame: 100-200ms (controlled by Sleep())
```

#### Bottlenecks Identificados
1. **clearScreen()**: System call custosa
2. **Collision Detection**: O(n²) para cobras grandes
3. **Console Output**: Múltiplas chamadas printf()
4. **Validação de Posições**: Pode ter muitas tentativas

#### Otimizações Implementáveis

**1. Double Buffering**
```c
char screenBuffer[HEIGHT+2][WIDTH+3];

void renderToBuffer() {
    // Renderiza para buffer em memória
    memset(screenBuffer, ' ', sizeof(screenBuffer));
    
    // Render entities to buffer
    screenBuffer[food.y][food.x] = 'F';
    for (int i = 0; i < snake1.length; i++) {
        screenBuffer[snake1.body[i].y][snake1.body[i].x] = (i == 0) ? 'O' : 'o';
    }
}

void displayBuffer() {
    gotoxy(0, 0);
    for (int y = 0; y < HEIGHT+2; y++) {
        printf("%.*s\n", WIDTH+2, screenBuffer[y]);
    }
}
```

**2. Dirty Rectangle System**
```c
typedef struct {
    int minX, minY, maxX, maxY;
    bool dirty;
} DirtyRegion;

void markDirty(int x, int y) {
    if (!dirtyRegion.dirty) {
        dirtyRegion.minX = dirtyRegion.maxX = x;
        dirtyRegion.minY = dirtyRegion.maxY = y;
        dirtyRegion.dirty = true;
    } else {
        dirtyRegion.minX = min(dirtyRegion.minX, x);
        dirtyRegion.maxX = max(dirtyRegion.maxX, x);
        dirtyRegion.minY = min(dirtyRegion.minY, y);
        dirtyRegion.maxY = max(dirtyRegion.maxY, y);
    }
}
```

---

## Tratamento de Erros e Edge Cases

### Casos Extremos Identificados

#### 1. Overflow de Pontuação
```c
// Problema atual
snake->score += 1;  // int pode overflow em ~2 bilhões

// Solução
void addScore(Snake* snake, int points) {
    if (snake->score > INT_MAX - points) {
        snake->score = INT_MAX;  // Clamp to maximum
    } else {
        snake->score += points;
    }
}
```

#### 2. Cobra de Comprimento Máximo
```c
// Problema: Array bounds
if (snake->length >= MAX_SNAKE_LENGTH) {
    // Não pode crescer mais - comportamento indefinido
}

// Solução: Verificação preventiva
void growSnake(Snake* snake) {
    if (snake->length < MAX_SNAKE_LENGTH) {
        snake->length++;
    } else {
        // Win condition ou special behavior
        triggerMaxLengthEvent(snake);
    }
}
```

#### 3. Múltiplos Power-ups do Mesmo Tipo
```c
// Problema: Efeitos podem se acumular incorretamente
// TURBO + TURBO = speed / 4 (muito rápido)

// Solução: Stack de efeitos
int calculateSpeedModifier(Snake* snake) {
    int turboCount = 0, slowCount = 0;
    
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake->activePowerUps[i].active) {
            switch (snake->activePowerUps[i].type) {
                case TURBO: turboCount++; break;
                case SLOW_MOTION: slowCount++; break;
            }
        }
    }
    
    int netEffect = turboCount - slowCount;
    return clamp(50, 400, baseSpeed * pow(2, -netEffect));
}
```

#### 4. Console Resize/Invalid State
```c
// Problema: Console redimensionado durante execução
// Solução: Verificação de dimensões
bool validateConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    return (consoleWidth >= WIDTH + 2) && (consoleHeight >= HEIGHT + 8);
}
```

### Sistema de Validação Robusto

#### Validação de Estados
```c
bool validateGameState() {
    // Verifica integridade das cobras
    if (snake1.length < 1 || snake1.length > MAX_SNAKE_LENGTH) return false;
    if (gameMode == MULTIPLAYER && snake2.length < 1) return false;
    
    // Verifica posições válidas
    for (int i = 0; i < snake1.length; i++) {
        if (!isValidPosition(snake1.body[i])) return false;
    }
    
    // Verifica consistência de power-ups
    int activePowerUps = 0;
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake1.activePowerUps[i].active) {
            activePowerUps++;
            if (snake1.activePowerUps[i].endTime < time(NULL)) {
                // Power-up expirado não deveria estar ativo
                return false;
            }
        }
    }
    
    return true;
}

bool isValidPosition(Coordinate pos) {
    return pos.x >= 1 && pos.x <= WIDTH && 
           pos.y >= 1 && pos.y <= HEIGHT;
}
```

---

## Extensibilidade e Manutenção

### Arquitetura para Extensões

#### Sistema de Modos Plugável
```c
typedef struct {
    char name[32];
    void (*init)(void);
    void (*update)(void);
    void (*cleanup)(void);
    bool (*isGameOver)(void);
} GameMode;

GameMode gameModes[] = {
    {"Traditional", initTraditional, updateTraditional, cleanupTraditional, isGameOverTraditional},
    {"Time Mode", initTimeMode, updateTimeMode, cleanupTimeMode, isGameOverTimeMode},
    {"Inverse", initInverse, updateInverse, cleanupInverse, isGameOverInverse},
    {"Multiplayer", initMultiplayer, updateMultiplayer, cleanupMultiplayer, isGameOverMultiplayer}
};

void runGameMode(int modeIndex) {
    GameMode* mode = &gameModes[modeIndex];
    mode->init();
    
    while (!mode->isGameOver()) {
        draw();
        input();
        mode->update();
        Sleep(calculateFrameDelay());
    }
    
    mode->cleanup();
}
```

#### Sistema de Power-ups Extensível
```c
typedef struct {
    char name[16];
    char displayChar;
    void (*onActivate)(Snake* snake);
    void (*onUpdate)(Snake* snake);
    void (*onDeactivate)(Snake* snake);
    int duration;
} PowerUpType;

PowerUpType powerUpTypes[] = {
    {"Turbo", 'T', activateTurbo, updateTurbo, deactivateTurbo, 10},
    {"Slow Motion", 'S', activateSlowMotion, updateSlowMotion, deactivateSlowMotion, 10},
    {"Immunity", 'I', activateImmunity, updateImmunity, deactivateImmunity, 10},
    {"Double Points", 'D', activateDoublePoints, updateDoublePoints, deactivateDoublePoints, 10},
    // Facilmente extensível
    {"Phase Walk", 'P', activatePhaseWalk, updatePhaseWalk, deactivatePhaseWalk, 5},
    {"Magnet", 'M', activateMagnet, updateMagnet, deactivateMagnet, 15}
};
```

### Sugestões de Melhorias

#### 1. Sistema de Configuração
```c
typedef struct {
    int width, height;
    int maxSnakeLength;
    int baseSpeed;
    bool soundEnabled;
    char playerName[32];
} GameConfig;

GameConfig loadConfig(const char* filename);
void saveConfig(const char* filename, GameConfig* config);
```

#### 2. Sistema de Alto Score
```c
typedef struct {
    char playerName[32];
    int score;
    int gameMode;
    time_t timestamp;
} HighScore;

void saveHighScore(HighScore* score);
HighScore* loadHighScores(int* count);
void displayLeaderboard();
```

#### 3. Sistema de Som (Proposta)
```c
// Usando Windows API
void playSound(const char* soundName) {
    char filename[64];
    sprintf(filename, "sounds/%s.wav", soundName);
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}

// Chamadas no jogo
playSound("eat");      // Ao comer comida
playSound("powerup");  // Ao pegar power-up
playSound("gameover"); // Ao morrer
```

#### 4. Melhorias Visuais
```c
// Cores usando ANSI escape codes
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

void printColored(const char* text, const char* color) {
    printf("%s%s%s", color, text, COLOR_RESET);
}
```

---

## Conclusão

Este jogo da cobra representa uma implementação avançada que vai muito além do conceito básico. A arquitetura modular, sistema de power-ups, múltiplos modos de jogo e tratamento robusto de estados demonstram princípios sólidos de engenharia de software.
