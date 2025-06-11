# Documentação Técnica - NeoSnake3

## Visão Geral

O **NeoSnake3** é uma implementação avançada do clássico jogo Snake (Jogo da Cobrinha) desenvolvida em C para sistemas Windows. O jogo oferece múltiplos modos de jogo, sistema de power-ups, multiplayer local e interface gráfica baseada em console com controles avançados.

## Arquitetura do Sistema

### Bibliotecas Utilizadas

```c
#include <stdio.h>      // Entrada/saída padrão
#include <stdlib.h>     // Funções de sistema e memória
#include <conio.h>      // Controle de console (Windows)
#include <windows.h>    // API Windows para manipulação de console
#include <time.h>       // Funções de tempo
```

### Constantes do Sistema

```c
#define LARGURA 40          // Largura da área de jogo
#define ALTURA 20           // Altura da área de jogo
#define MAX_COBRA 100       // Tamanho máximo da cobra
#define MAX_POWERUPS 5      // Número máximo de power-ups simultâneos
```

## Estruturas de Dados

### 1. Estrutura Posicao

```c
typedef struct {
    int x, y;
} Posicao;
```

**Propósito**: Representa coordenadas bidimensionais no campo de jogo.
- `x`: Posição horizontal (0 a LARGURA-1)
- `y`: Posição vertical (0 a ALTURA-1)

### 2. Estrutura Cobra

```c
typedef struct {
    Posicao corpo[MAX_COBRA];  // Array de segmentos do corpo
    int tamanho;               // Tamanho atual da cobra
    int direcao;               // Direção de movimento
} Cobra;
```

**Propósito**: Gerencia o estado completo de uma cobra no jogo.

**Campos**:
- `corpo[]`: Array contendo todas as posições dos segmentos da cobra
- `tamanho`: Número atual de segmentos (mínimo 3)
- `direcao`: Direção de movimento codificada como:
  - 0: Parada
  - 1: Cima
  - 2: Baixo
  - 3: Esquerda
  - 4: Direita

### 3. Estrutura PowerUp

```c
typedef struct {
    Posicao pos;           // Posição no campo
    int tipo;              // Tipo do power-up
    int ativo;             // Estado de ativação
    int tempo_restante;    // Tempo até desaparecer
} PowerUp;
```

**Propósito**: Gerencia os power-ups disponíveis no campo.

**Campos**:
- `pos`: Localização no campo de jogo
- `tipo`: Tipo do power-up (1-4)
  - 1: Turbo (velocidade aumentada)
  - 2: Slow (velocidade reduzida)
  - 3: Imunidade (proteção contra colisões)
  - 4: Pontos Dobro (pontuação multiplicada)
- `ativo`: Flag indicando se está presente no campo
- `tempo_restante`: Contador decrescente até remoção automática

### 4. Estrutura EstadoJogo

```c
typedef struct {
    int pontos;                 // Pontuação atual
    int nivel;                  // Nível baseado na pontuação
    int tempo_jogo;             // Duração da partida
    int tempo_inicio;           // Timestamp do início
    int powerups_habilitados;   // Flag para ativar power-ups
} EstadoJogo;
```

**Propósito**: Mantém o estado global da partida atual.

## Variáveis Globais

### Entidades do Jogo
```c
Cobra cobra1, cobra2;                    // Cobras do jogador 1 e 2
Posicao fruta;                           // Posição da fruta atual
PowerUp powerups[MAX_POWERUPS];          // Array de power-ups ativos
EstadoJogo jogo;                         // Estado geral do jogo
```

### Controle de Estado
```c
int modo_jogo = 0;          // Modo atual (0=menu, 1-4=diferentes modos)
int jogo_ativo = 1;         // Flag de execução principal
int pausado = 0;            // Estado de pausa
int velocidade_base = 150;  // Velocidade base em milissegundos
```

### Sistema de Power-ups
```c
int turbo_ativo = 0, turbo_tempo = 0;
int slow_ativo = 0, slow_tempo = 0;
int imunidade_ativa = 0, imunidade_tempo = 0;
int pontos_dobro_ativo = 0, pontos_dobro_tempo = 0;
```

## Funções Utilitárias

### 1. Manipulação de Console

#### `void limpar_tela()`
Limpa completamente a tela do console usando o comando `system("cls")`.

#### `void posicionar_cursor(int x, int y)`
Move o cursor para uma posição específica na tela.
- **Parâmetros**: 
  - `x`: Coluna de destino
  - `y`: Linha de destino
- **Implementação**: Utiliza a API Windows `SetConsoleCursorPosition()`

#### `void ocultar_cursor()`
Torna o cursor invisível para uma apresentação mais limpa.
- **Implementação**: Modifica `CONSOLE_CURSOR_INFO` através da API Windows

## Sistema de Inicialização

### 1. `void inicializar_cobra(Cobra* cobra, int x_inicial, int y_inicial)`

**Propósito**: Configura o estado inicial de uma cobra.

**Parâmetros**:
- `cobra`: Ponteiro para a estrutura da cobra
- `x_inicial`, `y_inicial`: Posição inicial da cabeça

**Lógica**:
1. Define tamanho inicial como 3 segmentos
2. Posiciona os segmentos verticalmente a partir da posição inicial
3. Define direção como 0 (parada)

### 2. `void gerar_fruta()`

**Propósito**: Cria uma nova fruta em posição aleatória válida.

**Lógica**:
1. Gera coordenadas aleatórias dentro dos limites
2. Verifica se não coincide com a cabeça da cobra
3. Repete até encontrar posição válida

### 3. `void inicializar_powerups()`

**Propósito**: Reseta todos os power-ups para estado inativo.

**Implementação**:
- Percorre array de power-ups
- Define `ativo = 0` e `tempo_restante = 0`

### 4. `void gerar_powerup()`

**Propósito**: Cria novos power-ups aleatoriamente durante o jogo.

**Lógica**:
1. Verifica se power-ups estão habilitados
2. 5% de chance por ciclo de gerar novo power-up
3. Encontra slot vazio no array
4. Define tipo aleatório (1-4)
5. Define posição aleatória e tempo de vida (10 segundos)

### 5. `void inicializar_jogo()`

**Propósito**: Configuração completa para início de nova partida.

**Sequência de Inicialização**:
1. Inicializa gerador de números aleatórios
2. Oculta cursor do console
3. Inicializa cobra(s) conforme o modo
4. Gera primeira fruta
5. Reseta power-ups
6. Zera pontuação e estatísticas
7. Define timestamp de início
8. Reseta todos os efeitos ativos

## Sistema de Renderização

### 1. `void desenhar_borda()`

**Propósito**: Desenha os limites do campo de jogo.

**Implementação**:
- Desenha linhas horizontais superior e inferior
- Desenha linhas verticais esquerda e direita
- Utiliza caractere '#' como delimitador

### 2. `void desenhar_cobra(Cobra* cobra, char simbolo)`

**Propósito**: Renderiza uma cobra na tela.

**Parâmetros**:
- `cobra`: Ponteiro para a estrutura da cobra
- `simbolo`: Caractere usado para o corpo

**Lógica**:
1. Percorre todos os segmentos
2. Cabeça sempre usa '@'
3. Corpo usa o símbolo fornecido
4. Posiciona cursor antes de desenhar cada segmento

### 3. `void desenhar_fruta()`

Desenha a fruta atual na posição definida usando o caractere '*'.

### 4. `void desenhar_powerups()`

**Propósito**: Renderiza todos os power-ups ativos.

**Símbolos**:
- T: Turbo
- S: Slow
- I: Imunidade  
- D: Pontos Dobro

### 5. `void desenhar_hud()`

**Propósito**: Exibe informações do jogo na lateral direita.

**Informações Exibidas**:
- Pontuação atual
- Nível atual
- Tempo restante (modo tempo)
- Power-ups ativos com contadores
- Controles disponíveis

### 6. `void desenhar_tela()`

**Propósito**: Função principal de renderização que coordena todos os elementos.

**Sequência**:
1. Limpa a tela
2. Desenha borda
3. Desenha cobra(s)
4. Desenha fruta
5. Desenha power-ups
6. Desenha HUD

## Sistema de Movimento e Física

### 1. `void mover_cobra(Cobra* cobra)`

**Propósito**: Atualiza a posição de todos os segmentos da cobra.

**Algoritmo**:
1. **Propagação do Movimento**: Cada segmento assume a posição do segmento anterior
2. **Movimento da Cabeça**: A cabeça move-se conforme a direção atual
3. **Direções Mapeadas**:
   - 1 (Cima): y--
   - 2 (Baixo): y++
   - 3 (Esquerda): x--
   - 4 (Direita): x++

## Sistema de Detecção de Colisões

### 1. `int verificar_colisao_parede(Cobra* cobra)`

**Propósito**: Detecta colisão com as bordas do campo.

**Condições de Colisão**:
- x ≤ 0 ou x ≥ LARGURA-1
- y ≤ 0 ou y ≥ ALTURA-1

**Imunidade**: Retorna 0 se imunidade estiver ativa.

### 2. `int verificar_colisao_corpo(Cobra* cobra)`

**Propósito**: Detecta auto-colisão da cobra.

**Lógica**:
1. Compara posição da cabeça com todos os outros segmentos
2. Retorna 1 se houver sobreposição
3. Ignora se imunidade estiver ativa

### 3. `int verificar_colisao_entre_cobras()`

**Propósito**: Detecta colisões entre cobras no modo multiplayer.

**Verificações**:
1. Cabeça da cobra1 vs corpo da cobra2
2. Cabeça da cobra2 vs corpo da cobra1

**Retorno**:
- 0: Sem colisão
- 1: Cobra1 colidiu
- 2: Cobra2 colidiu

## Sistema de Pontuação e Progressão

### 1. `void comer_fruta(Cobra* cobra)`

**Propósito**: Processa o consumo de fruta pela cobra.

**Efeitos**:
1. **Crescimento**: Incrementa tamanho da cobra
2. **Pontuação**: Adiciona 10 pontos (20 se power-up dobro ativo)
3. **Progressão**: Calcula novo nível (pontos/100 + 1)
4. **Regeneração**: Gera nova fruta

## Sistema de Power-ups

### 1. `void processar_powerup(int tipo)`

**Propósito**: Ativa efeito do power-up coletado.

**Efeitos por Tipo**:
- **Tipo 1 (Turbo)**: Duplica velocidade por 5 segundos
- **Tipo 2 (Slow)**: Reduz velocidade pela metade por 5 segundos  
- **Tipo 3 (Imunidade)**: Anula colisões por 5 segundos
- **Tipo 4 (Pontos Dobro)**: Duplica pontuação por 10 segundos

### 2. `void atualizar_powerups()`

**Propósito**: Gerencia contadores de tempo dos power-ups.

**Funcionalidades**:
1. **Power-ups no Campo**: Decrementa tempo_restante, remove se expirado
2. **Efeitos Ativos**: Decrementa contadores, desativa quando zerado

## Sistema de Controles

### 1. `void processar_entrada()`

**Propósito**: Captura e interpreta entrada do teclado.

**Tipos de Entrada**:

#### Teclas Especiais (Setas)
- Código -32 ou 224 indica tecla especial
- Segundo código especifica a tecla:
  - 72: Seta Cima
  - 80: Seta Baixo  
  - 75: Seta Esquerda
  - 77: Seta Direita

#### Controles Gerais
- **ESC (27)**: Retorna ao menu
- **P**: Alterna pausa

#### Modo Multiplayer
- **WASD**: Controla cobra2
  - W: Cima
  - S: Baixo
  - A: Esquerda  
  - D: Direita

#### Modo Inverso
Inverte a direção solicitada:
- Cima → Baixo
- Baixo → Cima
- Esquerda → Direita
- Direita → Esquerda

**Validação de Movimento**: Impede inversão direta (cobra não pode voltar sobre si mesma).

## Sistema de Interface

### 1. `void mostrar_menu_principal()`

**Propósito**: Exibe menu de seleção de modo de jogo.

**Opções Disponíveis**:
1. Modo Tradicional
2. Modo Tempo (1 minuto)
3. Modo Inverso
4. Modo Multiplayer Local
5. Sair

### 2. `int menu_powerups()`

**Propósito**: Permite ao jogador habilitar/desabilitar power-ups.

**Funcionalidade**:
- Explica tipos de power-ups disponíveis
- Retorna 1 para habilitado, 0 para desabilitado

### 3. `void mostrar_game_over()`

**Propósito**: Exibe tela final com estatísticas da partida.

**Informações Mostradas**:
- Pontuação final
- Nível alcançado
- Tempo de jogo (modo tempo)

### 4. `void mostrar_pausa()`

**Propósito**: Exibe indicador visual de jogo pausado.

## Loop Principal do Jogo

### 1. `void executar_jogo()`

**Propósito**: Loop principal que coordena toda a lógica do jogo.

**Ciclo de Execução**:

#### Fase 1: Verificações Iniciais
- Verifica fim do tempo (modo tempo)
- Processa entrada do usuário
- Verifica estado de pausa

#### Fase 2: Atualizações de Estado
- Atualiza power-ups (contadores e geração)
- Move cobra(s) se estiverem em movimento

#### Fase 3: Detecção de Colisões
- Verifica colisões com parede e corpo próprio
- Verifica colisões entre cobras (multiplayer)
- Termina jogo se colisão detectada

#### Fase 4: Interações
- Verifica se fruta foi coletada
- Verifica se power-up foi coletado
- Processa efeitos correspondentes

#### Fase 5: Renderização
- Desenha estado atual do jogo

#### Fase 6: Controle de Velocidade
- Calcula velocidade baseada em nível e power-ups
- Aplica delay correspondente usando Sleep()

**Cálculo de Velocidade**:
```c
int velocidade = velocidade_base - (jogo.nivel * 10);
if (turbo_ativo) velocidade /= 2;      // Mais rápido
if (slow_ativo) velocidade *= 2;       // Mais lento  
if (velocidade < 50) velocidade = 50;  // Velocidade mínima
```

## Função Principal

### 1. `int main()`

**Propósito**: Ponto de entrada e loop de controle geral.

**Fluxo**:
1. **Loop Infinito**: Mantém programa ativo
2. **Exibição de Menu**: Mostra opções disponíveis
3. **Seleção de Modo**: Processa escolha do usuário
4. **Configuração**: Define power-ups se aplicável
5. **Execução**: Chama executar_jogo()
6. **Retorno**: Volta ao menu após cada partida

**Modos de Jogo**:
- **Caso 1-4**: Define modo correspondente e executa
- **Caso 5**: Encerra programa
- **Padrão**: Trata opção inválida

## Modos de Jogo Detalhados

### Modo 1: Tradicional
- Jogo clássico sem limite de tempo
- Velocidade aumenta com o nível
- Termina apenas com colisão

### Modo 2: Tempo
- Duração fixa de 60 segundos
- Objetivo: máxima pontuação no tempo
- Termina automaticamente ao fim do tempo

### Modo 3: Inverso  
- Controles invertidos
- Aumenta dificuldade significativamente
- Todas as outras regras iguais ao tradicional

### Modo 4: Multiplayer
- Duas cobras simultâneas
- Cobra1: Setas do teclado
- Cobra2: Teclas WASD
- Termina se qualquer cobra colidir

## Considerações Técnicas

### Dependências de Plataforma
- **Windows Específico**: Uso extensivo da API Windows
- **conio.h**: Biblioteca não-padrão para controle de console
- **Portabilidade**: Código não portável para outros sistemas

### Gerenciamento de Memória
- **Estruturas Estáticas**: Todos os arrays têm tamanho fixo
- **Sem Alocação Dinâmica**: Não usa malloc/free
- **Segurança**: Sem riscos de vazamento de memória

### Performance
- **Sleep()**: Controla framerate do jogo
- **Renderização Completa**: Redesenha tela inteira a cada frame
- **Otimização**: Possível otimizar com renderização diferencial

### Limitações
- **Tamanho Fixo**: Campo e cobra limitados pelas constantes
- **Power-ups Limitados**: Máximo de 5 simultâneos
- **Console**: Interface limitada pelas capacidades do console

## Possíveis Melhorias

1. **Portabilidade**: Abstração da camada de sistema
2. **Otimização**: Renderização apenas de mudanças
3. **Configurabilidade**: Parâmetros ajustáveis via arquivo
4. **Persistência**: Sistema de save/load e high scores
5. **Audio**: Efeitos sonoros e música
6. **IA**: Modo single-player com IA controlando segunda cobra
7. **Rede**: Multiplayer via rede local/internet

## Conclusão

O NeoSnake3 representa uma implementação robusta e feature-rich do clássico Snake, demonstrando uso avançado de estruturas de dados em C, manipulação de console Windows, e design de arquitetura de jogos. O código é bem estruturado, com separação clara de responsabilidades e sistema modular que facilita manutenção e extensão.