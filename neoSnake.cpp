#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Para funções de entrada como getch()
#include <windows.h> // Para funções específicas do Windows
#include <time.h>    // Para funções de tempo
#include <stdbool.h> // Para variáveis booleanas

// Constantes para o jogo
#define WIDTH 60
#define HEIGHT 20
#define MAX_SNAKE_LENGTH 500
#define MAX_POWERUPS 4
#define POWERUP_DURATION 10 // Duração dos power-ups em segundos
#define TIME_MODE_DURATION 60 // Duração do Modo Tempo em segundos

// Direções
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// Tipos de power-up
#define TURBO 0
#define SLOW_MOTION 1
#define IMMUNITY 2
#define DOUBLE_POINTS 3

// Modos de jogo
#define TRADITIONAL 0
#define TIME_MODE 1
#define INVERSE_MODE 2
#define MULTIPLAYER 3

// Estrutura para representar uma coordenada
typedef struct {
    int x;
    int y;
} Coordinate;

// Estrutura para representar um power-up
typedef struct {
    Coordinate position;
    int type;
    bool active;
    time_t endTime;
} PowerUp;

// Estrutura para representar uma cobra
typedef struct {
    Coordinate body[MAX_SNAKE_LENGTH];
    int length;
    int direction;
    int score;
    bool alive;
    PowerUp activePowerUps[MAX_POWERUPS];
} Snake;

// Protótipos de funções
void setup();
void drawBorder();
void draw();
void input();
void logic();
void generateFood();
void generatePowerUp();
void showMenu();
void showGameModeMenu();
void showPowerUpMenu();
void showGameOver();
void gotoxy(int x, int y);
void hideCursor();
void showCursor();
void clearScreen();
void initSnake(Snake* snake, int startX, int startY, int direction);
bool handlePowerUp(Snake* snake);
void updatePowerUps(Snake* snake);
void applyPowerUpEffects(Snake* snake);

// Variáveis globais
Snake snake1, snake2;
Coordinate food;
Coordinate powerUpPos;
int gameMode = TRADITIONAL;
bool powerUpsEnabled = false;
bool gameOver = false;
bool exitGame = false;
bool powerUpActive = false;
int powerUpType = -1;
time_t powerUpEndTime = 0;
time_t gameModeEndTime = 0;
int baseSpeed = 100; // Velocidade base em milissegundos (menor é mais rápido)

int main() {
    srand(time(NULL)); // Inicializa a semente aleatória
    hideCursor();
    
    while (!exitGame) {
        showMenu();
        
        if (exitGame) break;
        
        setup();
        
        // Loop principal do jogo
        while (!gameOver) {
            draw();
            input();
            logic();
            
            // Controla a velocidade do jogo
            int currentSpeed = baseSpeed;
            Sleep(currentSpeed);
        }
        
        showGameOver();
    }
    
    showCursor();
    return 0;
}

void setup() {
    clearScreen();
    gameOver = false;
    
    // Inicializa a primeira cobra
    initSnake(&snake1, WIDTH / 4, HEIGHT / 2, RIGHT);
    
    // Inicializa a segunda cobra para o modo multiplayer
    if (gameMode == MULTIPLAYER) {
        initSnake(&snake2, 3 * WIDTH / 4, HEIGHT / 2, LEFT);
    }
    
    generateFood();
    
    // Configura o timer para o Modo Tempo
    if (gameMode == TIME_MODE) {
        gameModeEndTime = time(NULL) + TIME_MODE_DURATION;
    }
}

void initSnake(Snake* snake, int startX, int startY, int direction) {
    snake->length = 1;
    snake->body[0].x = startX;
    snake->body[0].y = startY;
    snake->direction = direction;
    snake->score = 0;
    snake->alive = true;
    
    // Inicializa os power-ups
    for (int i = 0; i < MAX_POWERUPS; i++) {
        snake->activePowerUps[i].active = false;
    }
}

void draw() {
    clearScreen();
    drawBorder();
    
    // Desenha a comida
    gotoxy(food.x, food.y);
    printf("F");
    
    // Desenha power-up se estiver ativo
    if (powerUpActive) {
        gotoxy(powerUpPos.x, powerUpPos.y);
        char powerUpChar;
        switch (powerUpType) {
            case TURBO: powerUpChar = 'T'; break;
            case SLOW_MOTION: powerUpChar = 'S'; break;
            case IMMUNITY: powerUpChar = 'I'; break;
            case DOUBLE_POINTS: powerUpChar = 'D'; break;
            default: powerUpChar = 'P';
        }
        printf("%c", powerUpChar);
    }
    
    // Desenha a primeira cobra
    for (int i = 0; i < snake1.length; i++) {
        gotoxy(snake1.body[i].x, snake1.body[i].y);
        if (i == 0) {
            printf("O"); // Cabeça
        } else {
            printf("o"); // Corpo
        }
    }
    
    // Desenha a segunda cobra no modo multiplayer
    if (gameMode == MULTIPLAYER && snake2.alive) {
        for (int i = 0; i < snake2.length; i++) {
            gotoxy(snake2.body[i].x, snake2.body[i].y);
            if (i == 0) {
                printf("X"); // Cabeça
            } else {
                printf("x"); // Corpo
            }
        }
    }
    
    // Exibe pontuações e power-ups ativos
    gotoxy(2, HEIGHT + 2);
    printf("Jogador 1: %d pts", snake1.score);
    
    if (gameMode == MULTIPLAYER) {
        gotoxy(WIDTH - 20, HEIGHT + 2);
        printf("Jogador 2: %d pts", snake2.score);
    }
    
    // Exibe power-ups ativos
    gotoxy(2, HEIGHT + 3);
    printf("Power-ups ativos: ");
    
    bool hasPowerUp = false;
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake1.activePowerUps[i].active) {
            hasPowerUp = true;
            switch (snake1.activePowerUps[i].type) {
                case TURBO: printf("Turbo "); break;
                case SLOW_MOTION: printf("SlowMo "); break;
                case IMMUNITY: printf("Imunidade "); break;
                case DOUBLE_POINTS: printf("PtsDobro "); break;
            }
        }
    }
    
    if (!hasPowerUp) {
        printf("Nenhum");
    }
    
    // Exibe o tempo restante para o Modo Tempo
    if (gameMode == TIME_MODE) {
        int timeRemaining = (int)(gameModeEndTime - time(NULL));
        gotoxy(WIDTH / 2 - 5, 0);
        printf("Tempo: %d", timeRemaining > 0 ? timeRemaining : 0);
    }
    
    // Exibe o modo de jogo atual
    gotoxy(2, HEIGHT + 4);
    printf("Modo atual: ");
    switch (gameMode) {
        case TRADITIONAL: printf("Tradicional"); break;
        case TIME_MODE: printf("Tempo"); break;
        case INVERSE_MODE: printf("Inverso"); break;
        case MULTIPLAYER: printf("Multiplayer"); break;
    }
    
    // Atualiza a exibição
    gotoxy(0, HEIGHT + 5);
}

void drawBorder() {
    for (int i = 0; i <= WIDTH + 1; i++) {
        gotoxy(i, 0);
        printf("#");
        gotoxy(i, HEIGHT + 1);
        printf("#");
    }
    
    for (int i = 0; i <= HEIGHT + 1; i++) {
        gotoxy(0, i);
        printf("#");
        gotoxy(WIDTH + 1, i);
        printf("#");
    }
}

void input() {
    if (_kbhit()) {
        char key = _getch();
        
        // Controles para a cobra 1
        if (gameMode == INVERSE_MODE) {
            // Controles invertidos
            if (key == 72 && snake1.direction != UP) snake1.direction = DOWN; // Seta para cima
            if (key == 80 && snake1.direction != DOWN) snake1.direction = UP; // Seta para baixo
            if (key == 75 && snake1.direction != LEFT) snake1.direction = RIGHT; // Seta para esquerda
            if (key == 77 && snake1.direction != RIGHT) snake1.direction = LEFT; // Seta para direita
        } else {
            // Controles normais
            if (key == 72 && snake1.direction != DOWN) snake1.direction = UP; // Seta para cima
            if (key == 80 && snake1.direction != UP) snake1.direction = DOWN; // Seta para baixo
            if (key == 75 && snake1.direction != RIGHT) snake1.direction = LEFT; // Seta para esquerda
            if (key == 77 && snake1.direction != LEFT) snake1.direction = RIGHT; // Seta para direita
        }
        
        // Controles para a cobra 2 no modo multiplayer
        if (gameMode == MULTIPLAYER) {
            if (key == 'w' && snake2.direction != DOWN) snake2.direction = UP;
            if (key == 's' && snake2.direction != UP) snake2.direction = DOWN;
            if (key == 'a' && snake2.direction != RIGHT) snake2.direction = LEFT;
            if (key == 'd' && snake2.direction != LEFT) snake2.direction = RIGHT;
        }
        
        // Sair do jogo
        if (key == 27) { // Tecla ESC
            gameOver = true;
        }
    }
}

void logic() {
    // Atualiza posições das cobras
    for (int i = snake1.length - 1; i > 0; i--) {
        snake1.body[i] = snake1.body[i-1];
    }
    
    // Atualiza a cabeça da cobra 1 com base na direção
    switch (snake1.direction) {
        case UP: snake1.body[0].y--; break;
        case DOWN: snake1.body[0].y++; break;
        case LEFT: snake1.body[0].x--; break;
        case RIGHT: snake1.body[0].x++; break;
    }
    
    // Atualiza a segunda cobra no modo multiplayer
    if (gameMode == MULTIPLAYER && snake2.alive) {
        for (int i = snake2.length - 1; i > 0; i--) {
            snake2.body[i] = snake2.body[i-1];
        }
        
        switch (snake2.direction) {
            case UP: snake2.body[0].y--; break;
            case DOWN: snake2.body[0].y++; break;
            case LEFT: snake2.body[0].x--; break;
            case RIGHT: snake2.body[0].x++; break;
        }
    }
    
    // Atualiza power-ups
    updatePowerUps(&snake1);
    if (gameMode == MULTIPLAYER) {
        updatePowerUps(&snake2);
    }
    
    // Verifica colisão com as bordas para a cobra 1
    if (snake1.body[0].x <= 0 || snake1.body[0].x >= WIDTH + 1 ||
        snake1.body[0].y <= 0 || snake1.body[0].y >= HEIGHT + 1) {
        snake1.alive = false;
    }
    
    // Verifica colisão com o próprio corpo para a cobra 1 se não estiver imune
    bool isImmune = false;
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake1.activePowerUps[i].active && snake1.activePowerUps[i].type == IMMUNITY) {
            isImmune = true;
            break;
        }
    }
    
    if (!isImmune) {
        for (int i = 1; i < snake1.length; i++) {
            if (snake1.body[0].x == snake1.body[i].x && snake1.body[0].y == snake1.body[i].y) {
                snake1.alive = false;
            }
        }
        
        // Verifica colisão com a cobra 2 no modo multiplayer
        if (gameMode == MULTIPLAYER && snake2.alive) {
            for (int i = 0; i < snake2.length; i++) {
                if (snake1.body[0].x == snake2.body[i].x && snake1.body[0].y == snake2.body[i].y) {
                    snake1.alive = false;
                }
            }
        }
    }
    
    // Verifica colisões para a cobra 2 no modo multiplayer
    if (gameMode == MULTIPLAYER && snake2.alive) {
        if (snake2.body[0].x <= 0 || snake2.body[0].x >= WIDTH + 1 ||
            snake2.body[0].y <= 0 || snake2.body[0].y >= HEIGHT + 1) {
            snake2.alive = false;
        }
        
        // Verifica colisão com o próprio corpo para a cobra 2 se não estiver imune
        isImmune = false;
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (snake2.activePowerUps[i].active && snake2.activePowerUps[i].type == IMMUNITY) {
                isImmune = true;
                break;
            }
        }
        
        if (!isImmune) {
            for (int i = 1; i < snake2.length; i++) {
                if (snake2.body[0].x == snake2.body[i].x && snake2.body[0].y == snake2.body[i].y) {
                    snake2.alive = false;
                }
            }
            
            // Verifica colisão com a cobra 1
            for (int i = 0; i < snake1.length; i++) {
                if (snake2.body[0].x == snake1.body[i].x && snake2.body[0].y == snake1.body[i].y) {
                    snake2.alive = false;
                }
            }
        }
    }
    
    // Verifica colisão com a comida para a cobra 1
    if (snake1.body[0].x == food.x && snake1.body[0].y == food.y) {
        // Aumenta o comprimento da cobra
        snake1.length++;
        
        // Aumenta a pontuação
        bool doublePoints = false;
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (snake1.activePowerUps[i].active && snake1.activePowerUps[i].type == DOUBLE_POINTS) {
                doublePoints = true;
                break;
            }
        }
        
        snake1.score += doublePoints ? 2 : 1;
        
        // Gera nova comida
        generateFood();
        
        // Gera power-up aleatoriamente
        if (powerUpsEnabled && !powerUpActive && rand() % 5 == 0) { // 20% de chance
            generatePowerUp();
        }
    }
    
    // Verifica colisão com a comida para a cobra 2 no modo multiplayer
    if (gameMode == MULTIPLAYER && snake2.alive) {
        if (snake2.body[0].x == food.x && snake2.body[0].y == food.y) {
            // Aumenta o comprimento da cobra
            snake2.length++;
            
            // Aumenta a pontuação
            bool doublePoints = false;
            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (snake2.activePowerUps[i].active && snake2.activePowerUps[i].type == DOUBLE_POINTS) {
                    doublePoints = true;
                    break;
                }
            }
            
            snake2.score += doublePoints ? 2 : 1;
            
            // Gera nova comida
            generateFood();
            
            // Gera power-up aleatoriamente
            if (powerUpsEnabled && !powerUpActive && rand() % 5 == 0) { // 20% de chance
                generatePowerUp();
            }
        }
    }
    
    // Verifica se a cobra 1 pegou power-up
    if (powerUpActive && snake1.body[0].x == powerUpPos.x && snake1.body[0].y == powerUpPos.y) {
        // Ativa o power-up
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!snake1.activePowerUps[i].active) {
                snake1.activePowerUps[i].active = true;
                snake1.activePowerUps[i].type = powerUpType;
                snake1.activePowerUps[i].endTime = time(NULL) + POWERUP_DURATION;
                break;
            }
        }
        
        powerUpActive = false;
    }
    
    // Verifica se a cobra 2 pegou power-up no modo multiplayer
    if (gameMode == MULTIPLAYER && snake2.alive && powerUpActive && 
        snake2.body[0].x == powerUpPos.x && snake2.body[0].y == powerUpPos.y) {
        // Ativa o power-up
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!snake2.activePowerUps[i].active) {
                snake2.activePowerUps[i].active = true;
                snake2.activePowerUps[i].type = powerUpType;
                snake2.activePowerUps[i].endTime = time(NULL) + POWERUP_DURATION;
                break;
            }
        }
        
        powerUpActive = false;
    }
    
    // Verifica se o jogo acabou
    if (gameMode == MULTIPLAYER) {
        if (!snake1.alive && !snake2.alive) {
            gameOver = true;
        }
    } else {
        if (!snake1.alive) {
            gameOver = true;
        }
    }
    
    // Verifica se o tempo acabou no Modo Tempo
    if (gameMode == TIME_MODE && time(NULL) >= gameModeEndTime) {
        gameOver = true;
    }
}

void generateFood() {
    bool validPosition;
    do {
        validPosition = true;
        food.x = 1 + rand() % WIDTH;
        food.y = 1 + rand() % HEIGHT;
        
        // Verifica se a comida apareceu na cobra 1
        for (int i = 0; i < snake1.length; i++) {
            if (food.x == snake1.body[i].x && food.y == snake1.body[i].y) {
                validPosition = false;
                break;
            }
        }
        
        // Verifica se a comida apareceu na cobra 2 no modo multiplayer
        if (validPosition && gameMode == MULTIPLAYER) {
            for (int i = 0; i < snake2.length; i++) {
                if (food.x == snake2.body[i].x && food.y == snake2.body[i].y) {
                    validPosition = false;
                    break;
                }
            }
        }
        
        // Verifica se a comida apareceu sobre um power-up
        if (validPosition && powerUpActive) {
            if (food.x == powerUpPos.x && food.y == powerUpPos.y) {
                validPosition = false;
            }
        }
    } while (!validPosition);
}

void generatePowerUp() {
    bool validPosition;
    do {
        validPosition = true;
        powerUpPos.x = 1 + rand() % WIDTH;
        powerUpPos.y = 1 + rand() % HEIGHT;
        
        // Verifica se o power-up apareceu na comida
        if (powerUpPos.x == food.x && powerUpPos.y == food.y) {
            validPosition = false;
            continue;
        }
        
        // Verifica se o power-up apareceu na cobra 1
        for (int i = 0; i < snake1.length; i++) {
            if (powerUpPos.x == snake1.body[i].x && powerUpPos.y == snake1.body[i].y) {
                validPosition = false;
                break;
            }
        }
        
        // Verifica se o power-up apareceu na cobra 2 no modo multiplayer
        if (validPosition && gameMode == MULTIPLAYER) {
            for (int i = 0; i < snake2.length; i++) {
                if (powerUpPos.x == snake2.body[i].x && powerUpPos.y == snake2.body[i].y) {
                    validPosition = false;
                    break;
                }
            }
        }
    } while (!validPosition);
    
    powerUpType = rand() % MAX_POWERUPS;
    powerUpActive = true;
    powerUpEndTime = time(NULL) + 20; // Power-up desaparecerá após 20 segundos se não for coletado
}

void updatePowerUps(Snake* snake) {
    // Verifica se o power-up deve desaparecer
    if (powerUpActive && time(NULL) > powerUpEndTime) {
        powerUpActive = false;
    }
    
    // Atualiza os power-ups ativos
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake->activePowerUps[i].active && time(NULL) > snake->activePowerUps[i].endTime) {
            snake->activePowerUps[i].active = false;
        }
    }
    
    // Aplica os efeitos dos power-ups
    applyPowerUpEffects(snake);
}

void applyPowerUpEffects(Snake* snake) {
    // Reseta a velocidade para a base
    int speed = baseSpeed;
    
    // Aplica efeitos de velocidade dos power-ups
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (snake->activePowerUps[i].active) {
            switch (snake->activePowerUps[i].type) {
                case TURBO:
                    speed /= 2; // Dobra a velocidade (metade do atraso)
                    break;
                case SLOW_MOTION:
                    speed *= 2; // Metade da velocidade (dobro do atraso)
                    break;
                // Outros efeitos são aplicados em outros lugares
            }
        }
    }
    
    // Aplica a velocidade
    baseSpeed = speed;
}

void showMenu() {
    clearScreen();
    gotoxy(WIDTH / 2 - 10, 5);
    printf("===== JOGO DA COBRA =====");
    gotoxy(WIDTH / 2 - 10, 7);
    printf("1. Iniciar Jogo");
    gotoxy(WIDTH / 2 - 10, 8);
    printf("2. Selecionar Modo de Jogo");
    gotoxy(WIDTH / 2 - 10, 9);
    printf("3. Ativar/Desativar Power-ups (Atual: %s)", powerUpsEnabled ? "ATIVO" : "DESATIVADO");
    gotoxy(WIDTH / 2 - 10, 10);
    printf("4. Sair");
    
    // Mostrar o modo de jogo atual
    gotoxy(WIDTH / 2 - 10, 11);
    printf("Modo atual: ");
    switch (gameMode) {
        case TRADITIONAL: printf("Tradicional"); break;
        case TIME_MODE: printf("Modo Tempo"); break;
        case INVERSE_MODE: printf("Modo Inverso"); break;
        case MULTIPLAYER: printf("Multiplayer"); break;
    }
    
    gotoxy(WIDTH / 2 - 10, 13);
    printf("Digite sua escolha: ");
    
    char choice;
    do {
        choice = _getch();
        
        switch (choice) {
            case '1':
                return; // Inicia o jogo com as configurações atuais
            case '2':
                showGameModeMenu();
                return showMenu();
            case '3':
                powerUpsEnabled = !powerUpsEnabled;
                return showMenu();
            case '4':
                exitGame = true;
                return;
        }
    } while (true);
}

void showGameModeMenu() {
    clearScreen();
    gotoxy(WIDTH / 2 - 15, 5);
    printf("===== SELECIONAR MODO DE JOGO =====");
    gotoxy(WIDTH / 2 - 15, 7);
    printf("1. Modo Tradicional");
    gotoxy(WIDTH / 2 - 15, 8);
    printf("2. Modo Tempo (60 segundos)");
    gotoxy(WIDTH / 2 - 15, 9);
    printf("3. Modo Inverso (Controles invertidos)");
    gotoxy(WIDTH / 2 - 15, 10);
    printf("4. Modo Multiplayer");
    gotoxy(WIDTH / 2 - 15, 11);
    printf("5. Voltar");
    gotoxy(WIDTH / 2 - 15, 13);
    printf("Modo Atual: ");
    
    switch (gameMode) {
        case TRADITIONAL: printf("Tradicional"); break;
        case TIME_MODE: printf("Modo Tempo"); break;
        case INVERSE_MODE: printf("Modo Inverso"); break;
        case MULTIPLAYER: printf("Multiplayer"); break;
    }
    
    gotoxy(WIDTH / 2 - 15, 15);
    printf("Digite sua escolha: ");
    
    char choice;
    do {
        choice = _getch();
        
        switch (choice) {
            case '1':
                gameMode = TRADITIONAL;
                return;
            case '2':
                gameMode = TIME_MODE;
                return;
            case '3':
                gameMode = INVERSE_MODE;
                return;
            case '4':
                gameMode = MULTIPLAYER;
                return;
            case '5':
                return;
        }
    } while (true);
}

void showGameOver() {
    clearScreen();
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 2);
    printf("===== FIM DE JOGO =====");
    
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2);
    printf("Pontuação Jogador 1: %d", snake1.score);
    
    if (gameMode == MULTIPLAYER) {
        gotoxy(WIDTH / 2 - 10, HEIGHT / 2 + 1);
        printf("Pontuação Jogador 2: %d", snake2.score);
        
        gotoxy(WIDTH / 2 - 10, HEIGHT / 2 + 3);
        if (snake1.score > snake2.score) {
            printf("Jogador 1 venceu!");
        } else if (snake2.score > snake1.score) {
            printf("Jogador 2 venceu!");
        } else {
            printf("Empate!");
        }
    }
    
    gotoxy(WIDTH / 2 - 15, HEIGHT / 2 + 5);
    printf("Pressione qualquer tecla para voltar ao menu...");
    
    _getch(); // Espera por uma tecla
}

// Funções utilitárias
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void showCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void clearScreen() {
    system("cls");
}
