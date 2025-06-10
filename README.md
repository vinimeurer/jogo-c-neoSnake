# Documentação Detalhada - Jogo da Cobra em C

## Visão Geral

Este é um jogo da cobra (Snake) implementado em C para sistemas Windows, com recursos avançados incluindo múltiplos modos de jogo, power-ups, modo multiplayer e controles invertidos. O jogo utiliza bibliotecas específicas do Windows para controle de console e entrada de dados.

## Estrutura do Projeto

### Bibliotecas Utilizadas

```c
#include <stdio.h>      // Funções básicas de entrada/saída
#include <stdlib.h>     // Funções de alocação de memória e utilidades
#include <conio.h>      // Funções de entrada específicas do Windows (getch(), kbhit())
#include <windows.h>    // Funções específicas do Windows (SetConsoleCursorPosition, Sleep)
#include <time.h>       // Funções de tempo para geração de números aleatórios e timers
#include <stdbool.h>    // Suporte para variáveis booleanas
```

### Constantes Definidas

#### Configurações do Jogo
- `WIDTH 60` - Largura da área de jogo
- `HEIGHT 20` - Altura da área de jogo
- `MAX_SNAKE_LENGTH 500` - Comprimento máximo da cobra
- `MAX_POWERUPS 4` - Número máximo de power-ups simultâneos
- `POWERUP_DURATION 10` - Duração dos power-ups em segundos
- `TIME_MODE_DURATION 60` - Duração do modo tempo em segundos

#### Direções de Movimento
- `UP 0` - Movimento para cima
- `RIGHT 1` - Movimento para direita
- `DOWN 2` - Movimento para baixo
- `LEFT 3` - Movimento para esquerda

#### Tipos de Power-ups
- `TURBO 0` - Aumenta a velocidade da cobra
- `SLOW_MOTION 1` - Diminui a velocidade da cobra
- `IMMUNITY 2` - Torna a cobra imune a colisões
- `DOUBLE_POINTS 3` - Dobra os pontos obtidos

#### Modos de Jogo
- `TRADITIONAL 0` - Modo clássico
- `TIME_MODE 1` - Jogo com tempo limitado
- `INVERSE_MODE 2` - Controles invertidos
- `MULTIPLAYER 3` - Dois jogadores

## Estruturas de Dados

### Coordinate
```c
typedef struct {
    int x;  // Coordenada horizontal
    int y;  // Coordenada vertical
} Coordinate;
```
Representa uma posição no plano cartesiano do jogo.

### PowerUp
```c
typedef struct {
    Coordinate position;  // Posição do power-up no tabuleiro
    int type;            // Tipo do power-up (TURBO, SLOW_MOTION, etc.)
    bool active;         // Se o power-up está ativo
    time_t endTime;      // Tempo de expiração do power-up
} PowerUp;
```
Representa um power-up no jogo, incluindo sua posição, tipo e estado.

### Snake
```c
typedef struct {
    Coordinate body[MAX_SNAKE_LENGTH];  // Array com as posições do corpo
    int length;                         // Comprimento atual da cobra
    int direction;                      // Direção atual de movimento
    int score;                         // Pontuação do jogador
    bool alive;                        // Se a cobra está viva
    PowerUp activePowerUps[MAX_POWERUPS]; // Power-ups ativos
} Snake;
```
Estrutura principal que representa uma cobra no jogo.

## Variáveis Globais

- `Snake snake1, snake2` - Duas cobras para suporte a multiplayer
- `Coordinate food` - Posição da comida no tabuleiro
- `Coordinate powerUpPos` - Posição do power-up atual
- `int gameMode` - Modo de jogo atual
- `bool powerUpsEnabled` - Se power-ups estão habilitados
- `bool gameOver` - Estado do jogo
- `bool exitGame` - Controle de saída do programa
- `bool powerUpActive` - Se há um power-up ativo no tabuleiro
- `int powerUpType` - Tipo do power-up atual
- `time_t powerUpEndTime` - Tempo de expiração do power-up
- `time_t gameModeEndTime` - Tempo de fim para modo tempo
- `int baseSpeed` - Velocidade base do jogo

## Funções Principais

### Função Principal (main)
```c
int main()
```
- Inicializa a semente aleatória
- Oculta o cursor do console
- Loop principal do programa que:
  - Exibe o menu principal
  - Configura o jogo
  - Executa o loop do jogo
  - Exibe a tela de game over
- Restaura o cursor antes de sair

### Configuração e Inicialização

#### setup()
```c
void setup()
```
- Limpa a tela
- Reseta o estado do jogo
- Inicializa a cobra principal
- Inicializa a segunda cobra (modo multiplayer)
- Gera a primeira comida
- Configura timer para modo tempo

#### initSnake()
```c
void initSnake(Snake* snake, int startX, int startY, int direction)
```
- Inicializa uma cobra com:
  - Posição inicial especificada
  - Direção inicial
  - Comprimento 1
  - Pontuação 0
  - Estado vivo
  - Power-ups desativados

### Sistema de Renderização

#### draw()
```c
void draw()
```
Função principal de renderização que:
- Limpa a tela
- Desenha as bordas do jogo
- Renderiza a comida (símbolo 'F')
- Renderiza power-ups com símbolos específicos:
  - 'T' para Turbo
  - 'S' para Slow Motion
  - 'I' para Immunity
  - 'D' para Double Points
- Desenha a cobra 1 ('O' para cabeça, 'o' para corpo)
- Desenha a cobra 2 ('X' para cabeça, 'x' para corpo)
- Exibe informações do jogo:
  - Pontuações
  - Power-ups ativos
  - Tempo restante (modo tempo)
  - Modo de jogo atual

#### drawBorder()
```c
void drawBorder()
```
Desenha as bordas do campo de jogo usando caracteres '#'.

### Sistema de Entrada

#### input()
```c
void input()
```
Gerencia a entrada do usuário:
- Verifica se há tecla pressionada (`_kbhit()`)
- Processa controles da cobra 1 (setas direcionais)
- Aplica inversão de controles no modo inverso
- Processa controles da cobra 2 (WASD) no modo multiplayer
- Permite sair do jogo com ESC

### Lógica Principal do Jogo

#### logic()
```c
void logic()
```
Função central que processa toda a lógica do jogo:

1. **Movimento das Cobras**: Atualiza posições baseadas na direção
2. **Atualização de Power-ups**: Verifica expiração e aplica efeitos
3. **Detecção de Colisões**:
   - Bordas do campo
   - Próprio corpo (considerando imunidade)
   - Outras cobras (modo multiplayer)
4. **Consumo de Comida**:
   - Aumenta comprimento da cobra
   - Incrementa pontuação (dobrada com power-up)
   - Gera nova comida
5. **Coleta de Power-ups**: Ativa power-ups coletados
6. **Condições de Fim de Jogo**:
   - Morte das cobras
   - Tempo esgotado (modo tempo)

### Sistema de Comida e Power-ups

#### generateFood()
```c
void generateFood()
```
Gera uma nova posição para a comida, garantindo que:
- Não apareça sobre as cobras
- Não apareça sobre power-ups existentes
- Esteja dentro dos limites do campo

#### generatePowerUp()
```c
void generatePowerUp()
```
Gera um power-up aleatório em posição válida:
- Escolhe tipo aleatório
- Garante posição livre
- Define tempo de expiração (20 segundos)

#### updatePowerUps()
```c
void updatePowerUps(Snake* snake)
```
- Remove power-ups expirados do tabuleiro
- Desativa power-ups expirados das cobras
- Aplica efeitos dos power-ups ativos

#### applyPowerUpEffects()
```c
void applyPowerUpEffects(Snake* snake)
```
Aplica os efeitos dos power-ups ativos:
- **Turbo**: Dobra a velocidade (reduz delay pela metade)
- **Slow Motion**: Reduz velocidade pela metade (dobra delay)
- **Immunity**: Aplicado na detecção de colisões
- **Double Points**: Aplicado no consumo de comida

### Sistema de Menus

#### showMenu()
```c
void showMenu()
```
Exibe o menu principal com opções:
1. Iniciar jogo
2. Selecionar modo
3. Ativar/desativar power-ups
4. Sair

#### showGameModeMenu()
```c
void showGameModeMenu()
```
Menu de seleção de modo de jogo:
- Modo Tradicional
- Modo Tempo (60 segundos)
- Modo Inverso (controles invertidos)
- Modo Multiplayer

#### showGameOver()
```c
void showGameOver()
```
Tela de fim de jogo que exibe:
- Pontuações finais
- Vencedor (modo multiplayer)
- Opção de retornar ao menu

### Funções Utilitárias

#### gotoxy()
```c
void gotoxy(int x, int y)
```
Move o cursor do console para posição específica usando a API do Windows.

#### hideCursor() / showCursor()
```c
void hideCursor()
void showCursor()
```
Controla a visibilidade do cursor do console.

#### clearScreen()
```c
void clearScreen()
```
Limpa a tela do console usando `system("cls")`.

## Fluxo de Execução

### Inicialização
1. Programa inicia com `main()`
2. Inicializa gerador de números aleatórios
3. Oculta cursor do console
4. Entra no loop principal

### Loop Principal
1. **Menu**: Exibe opções e aguarda seleção
2. **Configuração**: Chama `setup()` para inicializar jogo
3. **Game Loop**: Enquanto não for game over:
   - `draw()`: Renderiza estado atual
   - `input()`: Processa entrada do usuário
   - `logic()`: Atualiza estado do jogo
   - `Sleep()`: Controla velocidade do jogo
4. **Game Over**: Exibe resultados finais

### Lógica de Colisão
- **Bordas**: Verifica se cabeça saiu dos limites
- **Auto-colisão**: Compara cabeça com segmentos do corpo
- **Colisão entre cobras**: No modo multiplayer
- **Imunidade**: Power-up que desabilita colisões temporariamente

### Sistema de Pontuação
- **Básico**: 1 ponto por comida
- **Power-up**: 2 pontos por comida (com Double Points ativo)
- **Multiplayer**: Pontuação independente para cada jogador

## Recursos Especiais

### Modos de Jogo
1. **Tradicional**: Jogo clássico sem limitações
2. **Tempo**: 60 segundos para maior pontuação
3. **Inverso**: Controles invertidos para maior desafio
4. **Multiplayer**: Dois jogadores simultâneos

### Power-ups
- **Turbo**: Velocidade dobrada por 10 segundos
- **Slow Motion**: Velocidade reduzida por 10 segundos
- **Immunity**: Imunidade a colisões por 10 segundos
- **Double Points**: Pontos dobrados por 10 segundos

### Características Técnicas
- **Detecção de teclas**: Usa `_kbhit()` e `_getch()`
- **Controle de velocidade**: `Sleep()` para frame rate
- **Posicionamento de cursor**: API Windows para desenho preciso
- **Geração aleatória**: Posições de comida e power-ups
- **Timers**: Controle de duração de power-ups e modo tempo

## Limitações e Considerações

### Dependências de Plataforma
- Código específico para Windows
- Usa bibliotecas não-padrão (`conio.h`, `windows.h`)
- Não é portável para outros sistemas operacionais

### Possíveis Melhorias
- Implementar níveis de dificuldade
- Adicionar mais tipos de power-ups
- Salvar recordes de pontuação
- Adicionar efeitos sonoros
- Implementar sistema de vidas
- Criar modo de jogo cooperativo

### Bugs Conhecidos
- Velocidade pode ficar inconsistente com múltiplos power-ups
- Não há validação de entrada em alguns menus
- Power-ups podem ocasionalmente aparecer em posições inválidas

## Compilação e Execução

### Requisitos
- Compilador C compatível com Windows (ex: MinGW, MSVC)
- Sistema operacional Windows
- Console com suporte a códigos de escape ANSI

### Comando de Compilação
```bash
gcc -o snake4.exe snake4.cpp -luser32 -lkernel32
```

### Execução
```bash
./snake4.exe
```

O jogo abrirá no console e exibirá o menu principal para configuração e início da partida.

## Análise de Performance

### Complexidade Algorítmica das Principais Operações

#### Sistema de Detecção de Colisões
- **Bordas**: `O(1)` - Verificação constante com 4 comparações simples
- **Auto-colisão**: `O(n)` - Onde n é o comprimento da cobra (até MAX_SNAKE_LENGTH)
- **Colisão entre cobras**: `O(n + m)` - Onde n e m são os comprimentos das duas cobras
- **Colisão com comida**: `O(1)` - Comparação direta de coordenadas
- **Colisão com power-ups**: `O(1)` - Comparação direta de coordenadas

#### Sistema de Renderização
- **Limpeza de tela**: `O(W × H)` - Onde W é WIDTH e H é HEIGHT
- **Desenho de bordas**: `O(W + H)` - Perímetro do campo de jogo
- **Desenho da cobra**: `O(n)` - Onde n é o comprimento da cobra
- **Renderização total**: `O(W × H + n)` - Dominado pela limpeza de tela

#### Geração de Posições Aleatórias
- **Comida**: `O(n)` no pior caso - Pode precisar várias tentativas para encontrar posição livre
- **Power-ups**: `O(n + m)` no pior caso - Considera ambas as cobras no modo multiplayer
- **Média esperada**: `O(1)` - Assumindo campo com espaço suficiente

#### Sistema de Power-ups
- **Atualização**: `O(MAX_POWERUPS)` = `O(4)` = `O(1)` - Constante
- **Aplicação de efeitos**: `O(MAX_POWERUPS)` = `O(1)` - Constante
- **Verificação de expiração**: `O(MAX_POWERUPS)` = `O(1)` - Constante

### Análise de Memória

#### Uso Estático de Memória
```c
// Estruturas principais
Snake snake1, snake2;  // ~2KB cada (500 coordenadas + metadados)
Coordinate food;       // 8 bytes
Coordinate powerUpPos; // 8 bytes
PowerUp structures;    // ~100 bytes por cobra
```

**Total aproximado**: ~4-5KB de memória estática

#### Características de Memória
- **Sem alocação dinâmica**: Todo o código usa arrays estáticos
- **Previsível**: Uso de memória constante durante execução
- **Eficiente**: Não há overhead de malloc/free
- **Limitado**: Comprimento máximo da cobra fixado em compile-time

### Análise de Tempo de Resposta

#### Frame Rate e Responsividade
```c
int currentSpeed = baseSpeed; // Padrão: 100ms
Sleep(currentSpeed);          // Controle de velocidade
```

- **FPS base**: ~10 FPS (100ms por frame)
- **Com Turbo**: ~20 FPS (50ms por frame)
- **Com Slow Motion**: ~5 FPS (200ms por frame)

#### Latência de Entrada
- **Detecção**: `_kbhit()` verifica buffer de teclado - O(1)
- **Processamento**: Mudança de direção imediata - O(1)
- **Resposta**: Aplicada no próximo frame (máximo 100ms de delay)

### Gargalos de Performance Identificados

#### 1. Limpeza de Tela Desnecessária
```c
void draw() {
    clearScreen();  // system("cls") - Custoso!
    // ... resto da renderização
}
```
**Problema**: `system("cls")` é lento e causa flickering
**Impacto**: Maior gargalo de performance do jogo
**Solução sugerida**: Usar buffer duplo ou atualização seletiva

#### 2. Detecção de Colisão Linear
```c
// Auto-colisão O(n)
for (int i = 1; i < snake1.length; i++) {
    if (snake1.body[0].x == snake1.body[i].x && 
        snake1.body[0].y == snake1.body[i].y) {
        snake1.alive = false;
    }
}
```
**Problema**: Verifica todo o corpo da cobra a cada frame
**Impacto**: Performance degrada com cobras longas
**Solução sugerida**: Hash table para posições ocupadas

#### 3. Geração de Posições Aleatórias
```c
do {
    validPosition = true;
    food.x = 1 + rand() % WIDTH;
    food.y = 1 + rand() % HEIGHT;
    // Verifica colisões...
} while (!validPosition);
```
**Problema**: Pode fazer muitas tentativas em campos cheios
**Impacto**: Lag ocasional quando o campo está congestionado
**Solução sugerida**: Lista de posições livres

### Otimizações Implementadas

#### 1. Power-ups com Limite Fixo
- Máximo de 4 power-ups simultâneos por cobra
- Evita crescimento descontrolado de arrays
- Garante performance constante O(1)

#### 2. Validação Eficiente de Posições
- Verifica primeiro condições mais prováveis
- Para loops assim que encontra colisão
- Evita verificações desnecessárias

#### 3. Estados Booleanos para Controle
```c
bool gameOver = false;
bool exitGame = false;
bool powerUpActive = false;
```
- Evita cálculos desnecessários
- Controle de fluxo eficiente
- Reduz chamadas de função

### Benchmarks Estimados

#### Em Sistema Típico (Windows 10, CPU moderna)

| Operação | Tempo Médio | Complexidade |
|----------|-------------|--------------|
| Frame completo | ~50-100ms | O(W×H + n) |
| Detecção colisão | ~0.1ms | O(n) |
| Entrada usuário | ~0.01ms | O(1) |
| Atualização lógica | ~0.5ms | O(n) |
| Renderização | ~40-90ms | O(W×H) |

#### Escalabilidade

| Tamanho Cobra | FPS Esperado | Uso Memória |
|---------------|--------------|-------------|
| 10 segmentos | ~10 FPS | ~4KB |
| 100 segmentos | ~9 FPS | ~4KB |
| 500 segmentos | ~8 FPS | ~4KB |

**Observação**: FPS limitado principalmente pela limpeza de tela, não pela lógica do jogo

### Recomendações de Otimização

#### 1. Renderização Otimizada
```c
// Em vez de clearScreen() completo
void updateDisplay() {
    // Apagar apenas posições que mudaram
    // Usar coordenadas absolutas
    // Implementar buffer duplo
}
```

#### 2. Estrutura de Dados Otimizada
```c
// Hash set para posições ocupadas
typedef struct {
    bool occupied[WIDTH][HEIGHT];
    int count;
} PositionMap;
```

#### 3. Cache de Validação
```c
// Cache de última posição válida
static Coordinate lastValidFood = {-1, -1};
// Tentar posições próximas primeiro
```

#### 4. Limitação de Frame Rate Adaptativa
```c
void adaptiveDelay() {
    static clock_t lastFrame = 0;
    clock_t current = clock();
    int elapsed = (current - lastFrame) * 1000 / CLOCKS_PER_SEC;
    
    if (elapsed < targetFrameTime) {
        Sleep(targetFrameTime - elapsed);
    }
    lastFrame = clock();
}
```

### Consumo de Recursos do Sistema

#### CPU
- **Baixo a médio**: Principalmente limitado por Sleep()
- **Picos**: Durante limpeza de tela e renderização
- **Escalabilidade**: Degrada linearmente com tamanho da cobra

#### Memória
- **RAM**: ~4-5KB (insignificante)
- **Stack**: Profundidade baixa de chamadas
- **Heap**: Não utilizado (apenas stack)

#### I/O
- **Console**: Intensivo durante renderização
- **Teclado**: Polling eficiente com _kbhit()
- **Sistema**: Chamadas system("cls") custosas

### Limitações de Performance

#### Hardware
- **CPU mínima**: Qualquer processador moderno
- **RAM mínima**: <1MB
- **Limitação real**: Velocidade do console do Windows

#### Software
- **Windows específico**: APIs não-portáveis
- **Console**: Limitado pela implementação do terminal
- **Sincronização**: Sem controle preciso de timing

Esta análise revela que o jogo é bem otimizado para suas necessidades, com o principal gargalo sendo a renderização do console, não a lógica do jogo em si.