#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// Constantes do jogo
#define LARGURA 40
#define ALTURA 20
#define MAX_COBRA 100
#define MAX_POWERUPS 5

// Estruturas de dados
typedef struct {
    int x, y;
} Posicao;

typedef struct {
    Posicao corpo[MAX_COBRA];
    int tamanho;
    int direcao; // 0=parado, 1=cima, 2=baixo, 3=esquerda, 4=direita
} Cobra;

typedef struct {
    Posicao pos;
    int tipo; // 1=turbo, 2=slow, 3=imunidade, 4=pontos_dobro
    int ativo;
    int tempo_restante;
} PowerUp;

typedef struct {
    int pontos;
    int nivel;
    int tempo_jogo;
    int tempo_inicio;
    int powerups_habilitados;
} EstadoJogo;

// Vari�veis globais
Cobra cobra1, cobra2;
Posicao fruta;
PowerUp powerups[MAX_POWERUPS];
EstadoJogo jogo;
int modo_jogo = 0; // 0=menu, 1=tradicional, 2=tempo, 3=inverso, 4=multiplayer
int jogo_ativo = 1;
int pausado = 0;
int velocidade_base = 150;

// Efeitos dos power-ups
int turbo_ativo = 0, turbo_tempo = 0;
int slow_ativo = 0, slow_tempo = 0;
int imunidade_ativa = 0, imunidade_tempo = 0;
int pontos_dobro_ativo = 0, pontos_dobro_tempo = 0;

// Fun��es utilit�rias
void limpar_tela() {
    system("cls");
}

void posicionar_cursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ocultar_cursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Inicializa��o do jogo
void inicializar_cobra(Cobra* cobra, int x_inicial, int y_inicial) {
    cobra->tamanho = 3;
    cobra->direcao = 0;
    for (int i = 0; i < cobra->tamanho; i++) {
        cobra->corpo[i].x = x_inicial;
        cobra->corpo[i].y = y_inicial + i;
    }
}

void gerar_fruta() {
    do {
        fruta.x = rand() % (LARGURA - 2) + 1;
        fruta.y = rand() % (ALTURA - 2) + 1;
    } while (fruta.x == cobra1.corpo[0].x && fruta.y == cobra1.corpo[0].y);
}

void inicializar_powerups() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerups[i].ativo = 0;
        powerups[i].tempo_restante = 0;
    }
}

void gerar_powerup() {
    if (!jogo.powerups_habilitados) return;
    
    // Chance de 5% de gerar power-up a cada ciclo
    if (rand() % 100 < 5) {
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!powerups[i].ativo) {
                powerups[i].pos.x = rand() % (LARGURA - 2) + 1;
                powerups[i].pos.y = rand() % (ALTURA - 2) + 1;
                powerups[i].tipo = (rand() % 4) + 1;
                powerups[i].ativo = 1;
                powerups[i].tempo_restante = 10; // 10 segundos na tela
                break;
            }
        }
    }
}

void inicializar_jogo() {
    srand(time(NULL));
    ocultar_cursor();
    
    inicializar_cobra(&cobra1, LARGURA/2, ALTURA/2);
    if (modo_jogo == 4) { // Multiplayer
        inicializar_cobra(&cobra2, LARGURA/2 + 5, ALTURA/2);
    }
    
    gerar_fruta();
    inicializar_powerups();
    
    jogo.pontos = 0;
    jogo.nivel = 1;
    jogo.tempo_inicio = time(NULL);
    
    // Reset dos efeitos
    turbo_ativo = slow_ativo = imunidade_ativa = pontos_dobro_ativo = 0;
    turbo_tempo = slow_tempo = imunidade_tempo = pontos_dobro_tempo = 0;
}

// Fun��es de desenho
void desenhar_borda() {
    // Borda superior e inferior
    for (int x = 0; x < LARGURA; x++) {
        posicionar_cursor(x, 0);
        printf("#");
        posicionar_cursor(x, ALTURA - 1);
        printf("#");
    }
    
    // Borda esquerda e direita
    for (int y = 0; y < ALTURA; y++) {
        posicionar_cursor(0, y);
        printf("#");
        posicionar_cursor(LARGURA - 1, y);
        printf("#");
    }
}

void desenhar_cobra(Cobra* cobra, char simbolo) {
    for (int i = 0; i < cobra->tamanho; i++) {
        posicionar_cursor(cobra->corpo[i].x, cobra->corpo[i].y);
        if (i == 0) {
            printf("@"); // Cabe�a
        } else {
            printf("%c", simbolo); // Corpo
        }
    }
}

void desenhar_fruta() {
    posicionar_cursor(fruta.x, fruta.y);
    printf("*");
}

void desenhar_powerups() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerups[i].ativo) {
            posicionar_cursor(powerups[i].pos.x, powerups[i].pos.y);
            switch (powerups[i].tipo) {
                case 1: printf("T"); break; // Turbo
                case 2: printf("S"); break; // Slow
                case 3: printf("I"); break; // Imunidade
                case 4: printf("D"); break; // Pontos dobro
            }
        }
    }
}

void desenhar_hud() {
    posicionar_cursor(LARGURA + 2, 2);
    printf("Pontos: %d", jogo.pontos);
    
    posicionar_cursor(LARGURA + 2, 3);
    printf("Nivel: %d", jogo.nivel);
    
    if (modo_jogo == 2) { // Modo tempo
        int tempo_restante = 60 - (time(NULL) - jogo.tempo_inicio);
        posicionar_cursor(LARGURA + 2, 4);
        printf("Tempo: %d", tempo_restante > 0 ? tempo_restante : 0);
    }
    
    // Mostrar power-ups ativos
    int linha = 6;
    if (turbo_ativo) {
        posicionar_cursor(LARGURA + 2, linha++);
        printf("TURBO: %ds", turbo_tempo);
    }
    if (slow_ativo) {
        posicionar_cursor(LARGURA + 2, linha++);
        printf("SLOW: %ds", slow_tempo);
    }
    if (imunidade_ativa) {
        posicionar_cursor(LARGURA + 2, linha++);
        printf("IMUNE: %ds", imunidade_tempo);
    }
    if (pontos_dobro_ativo) {
        posicionar_cursor(LARGURA + 2, linha++);
        printf("x2 PTS: %ds", pontos_dobro_tempo);
    }
    
    posicionar_cursor(LARGURA + 2, ALTURA - 5);
    printf("ESC: Menu");
    posicionar_cursor(LARGURA + 2, ALTURA - 4);
    printf("P: Pausar");
}

void desenhar_tela() {
    limpar_tela();
    desenhar_borda();
    desenhar_cobra(&cobra1, 'o');
    
    if (modo_jogo == 4) {
        desenhar_cobra(&cobra2, 'x');
    }
    
    desenhar_fruta();
    desenhar_powerups();
    desenhar_hud();
}

// L�gica de movimento
void mover_cobra(Cobra* cobra) {
    // Mover o corpo
    for (int i = cobra->tamanho - 1; i > 0; i--) {
        cobra->corpo[i] = cobra->corpo[i - 1];
    }
    
    // Mover a cabe�a
    switch (cobra->direcao) {
        case 1: cobra->corpo[0].y--; break; // Cima
        case 2: cobra->corpo[0].y++; break; // Baixo
        case 3: cobra->corpo[0].x--; break; // Esquerda
        case 4: cobra->corpo[0].x++; break; // Direita
    }
}

int verificar_colisao_parede(Cobra* cobra) {
    if (imunidade_ativa) return 0; // Imune a colis�es
    
    return (cobra->corpo[0].x <= 0 || cobra->corpo[0].x >= LARGURA - 1 ||
            cobra->corpo[0].y <= 0 || cobra->corpo[0].y >= ALTURA - 1);
}

int verificar_colisao_corpo(Cobra* cobra) {
    if (imunidade_ativa) return 0; // Imune a colis�es
    
    for (int i = 1; i < cobra->tamanho; i++) {
        if (cobra->corpo[0].x == cobra->corpo[i].x &&
            cobra->corpo[0].y == cobra->corpo[i].y) {
            return 1;
        }
    }
    return 0;
}

int verificar_colisao_entre_cobras() {
    if (modo_jogo != 4 || imunidade_ativa) return 0;
    
    // Verificar se cobra1 colidiu com cobra2
    for (int i = 0; i < cobra2.tamanho; i++) {
        if (cobra1.corpo[0].x == cobra2.corpo[i].x &&
            cobra1.corpo[0].y == cobra2.corpo[i].y) {
            return 1;
        }
    }
    
    // Verificar se cobra2 colidiu com cobra1
    for (int i = 0; i < cobra1.tamanho; i++) {
        if (cobra2.corpo[0].x == cobra1.corpo[i].x &&
            cobra2.corpo[0].y == cobra1.corpo[i].y) {
            return 2;
        }
    }
    
    return 0;
}

void comer_fruta(Cobra* cobra) {
    cobra->tamanho++;
    int pontos_ganhos = 10;
    
    if (pontos_dobro_ativo) {
        pontos_ganhos *= 2;
    }
    
    jogo.pontos += pontos_ganhos;
    jogo.nivel = (jogo.pontos / 100) + 1;
    
    gerar_fruta();
}

void processar_powerup(int tipo) {
    switch (tipo) {
        case 1: // Turbo
            turbo_ativo = 1;
            turbo_tempo = 5;
            break;
        case 2: // Slow
            slow_ativo = 1;
            slow_tempo = 5;
            break;
        case 3: // Imunidade
            imunidade_ativa = 1;
            imunidade_tempo = 5;
            break;
        case 4: // Pontos dobro
            pontos_dobro_ativo = 1;
            pontos_dobro_tempo = 10;
            break;
    }
}

void atualizar_powerups() {
    // Atualizar tempo dos power-ups na tela
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerups[i].ativo) {
            powerups[i].tempo_restante--;
            if (powerups[i].tempo_restante <= 0) {
                powerups[i].ativo = 0;
            }
        }
    }
    
    // Atualizar efeitos ativos
    if (turbo_ativo) {
        turbo_tempo--;
        if (turbo_tempo <= 0) turbo_ativo = 0;
    }
    
    if (slow_ativo) {
        slow_tempo--;
        if (slow_tempo <= 0) slow_ativo = 0;
    }
    
    if (imunidade_ativa) {
        imunidade_tempo--;
        if (imunidade_tempo <= 0) imunidade_ativa = 0;
    }
    
    if (pontos_dobro_ativo) {
        pontos_dobro_tempo--;
        if (pontos_dobro_tempo <= 0) pontos_dobro_ativo = 0;
    }
}

// Controles
void processar_entrada() {
    if (_kbhit()) {
        char tecla = _getch();
        
        // Verificar se � uma tecla especial (setas)
        if (tecla == -32 || tecla == 224) { // C�digo de tecla especial
            tecla = _getch(); // Pegar o segundo c�digo
            
            // Controles da cobra1 (setas)
            int nova_direcao = cobra1.direcao;
            switch (tecla) {
                case 72: // Seta cima
                    if (cobra1.direcao != 2) nova_direcao = 1;
                    break;
                case 80: // Seta baixo
                    if (cobra1.direcao != 1) nova_direcao = 2;
                    break;
                case 75: // Seta esquerda
                    if (cobra1.direcao != 4) nova_direcao = 3;
                    break;
                case 77: // Seta direita
                    if (cobra1.direcao != 3) nova_direcao = 4;
                    break;
            }
            
            // Modo inverso
            if (modo_jogo == 3) {
                switch (nova_direcao) {
                    case 1: nova_direcao = 2; break; // Cima vira baixo
                    case 2: nova_direcao = 1; break; // Baixo vira cima
                    case 3: nova_direcao = 4; break; // Esquerda vira direita
                    case 4: nova_direcao = 3; break; // Direita vira esquerda
                }
            }
            
            cobra1.direcao = nova_direcao;
            return;
        }
        
        // Controles gerais
        if (tecla == 27) { // ESC
            modo_jogo = 0;
            return;
        }
        
        if (tecla == 'p' || tecla == 'P') {
            pausado = !pausado;
            return;
        }
        
        // Controles da cobra2 (multiplayer) - apenas para teclas normais
        if (modo_jogo == 4) {
            switch (tecla) {
                case 'w': case 'W':
                    if (cobra2.direcao != 2) cobra2.direcao = 1;
                    break;
                case 's': case 'S':
                    if (cobra2.direcao != 1) cobra2.direcao = 2;
                    break;
                case 'a': case 'A':
                    if (cobra2.direcao != 4) cobra2.direcao = 3;
                    break;
                case 'd': case 'D':
                    if (cobra2.direcao != 3) cobra2.direcao = 4;
                    break;
            }
        }
    }
}

// Menus
void mostrar_menu_principal() {
    limpar_tela();
    printf("\n");
    printf("  #######################################\n");
    printf("  #            JOGO DA COBRINHA         #\n");
    printf("  #######################################\n");
    printf("  #                                     #\n");
    printf("  #  1. Modo Tradicional                #\n");
    printf("  #  2. Modo Tempo (1 minuto)           #\n");
    printf("  #  3. Modo Inverso                    #\n");
    printf("  #  4. Modo Multiplayer Local          #\n");
    printf("  #  5. Sair                            #\n");
    printf("  #                                     #\n");
    printf("  #######################################\n");
    printf("\n  Escolha uma opcao: ");
}

int menu_powerups() {
    limpar_tela();
    printf("\n");
    printf("  #######################################\n");
    printf("  #            POWER-UPS                #\n");
    printf("  #######################################\n");
    printf("  #                                     #\n");
    printf("  #  T - Turbo temporario               #\n");
    printf("  #  S - Slow motion                    #\n");
    printf("  #  I - Imunidade temporaria           #\n");
    printf("  #  D - Pontos em dobro                #\n");
    printf("  #                                     #\n");
    printf("  #  Habilitar power-ups?               #\n");
    printf("  #  1. Sim                             #\n");
    printf("  #  2. Nao                             #\n");
    printf("  #                                     #\n");
    printf("  #######################################\n");
    printf("\n  Escolha uma opcao: ");
    
    int opcao;
    scanf("%d", &opcao);
    return (opcao == 1);
}

void mostrar_game_over() {
    limpar_tela();
    printf("\n");
    printf("  #######################################\n");
    printf("  #              GAME OVER              #\n");
    printf("  #######################################\n");
    printf("  #                                     #\n");
    printf("  #  Pontuacao Final: %-8d         #\n", jogo.pontos);
    printf("  #  Nivel Alcancado: %-8d         #\n", jogo.nivel);
    
    if (modo_jogo == 2) {
        printf("  #  Tempo de Jogo: 60 segundos         #\n");
    }
    
    printf("  #                                     #\n");
    printf("  #  Pressione qualquer tecla para      #\n");
    printf("  #  retornar ao menu...                #\n");
    printf("  #                                     #\n");
    printf("  #######################################\n");
    
    _getch();
    modo_jogo = 0;
}

void mostrar_pausa() {
    posicionar_cursor(LARGURA/2 - 5, ALTURA/2);
    printf("JOGO PAUSADO");
    posicionar_cursor(LARGURA/2 - 8, ALTURA/2 + 1);
    printf("Pressione P para continuar");
}

// Loop principal do jogo
void executar_jogo() {
    inicializar_jogo();
    
    while (jogo_ativo && modo_jogo != 0) {
        // Verificar fim do modo tempo
        if (modo_jogo == 2 && (time(NULL) - jogo.tempo_inicio) >= 60) {
            mostrar_game_over();
            break;
        }
        
        processar_entrada();
        
        if (pausado) {
            mostrar_pausa();
            Sleep(100);
            continue;
        }
        
        // Atualizar power-ups
        atualizar_powerups();
        gerar_powerup();
        
        // Mover cobras
        if (cobra1.direcao != 0) {
            mover_cobra(&cobra1);
        }
        
        if (modo_jogo == 4 && cobra2.direcao != 0) {
            mover_cobra(&cobra2);
        }
        
        // Verificar colis�es
        if (verificar_colisao_parede(&cobra1) || verificar_colisao_corpo(&cobra1)) {
            mostrar_game_over();
            break;
        }
        
        if (modo_jogo == 4) {
            if (verificar_colisao_parede(&cobra2) || verificar_colisao_corpo(&cobra2)) {
                mostrar_game_over();
                break;
            }
            
            int colisao_cobras = verificar_colisao_entre_cobras();
            if (colisao_cobras > 0) {
                mostrar_game_over();
                break;
            }
        }
        
        // Verificar se comeu fruta
        if (cobra1.corpo[0].x == fruta.x && cobra1.corpo[0].y == fruta.y) {
            comer_fruta(&cobra1);
        }
        
        if (modo_jogo == 4 && cobra2.corpo[0].x == fruta.x && cobra2.corpo[0].y == fruta.y) {
            comer_fruta(&cobra2);
        }
        
        // Verificar power-ups
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (powerups[i].ativo) {
                if (cobra1.corpo[0].x == powerups[i].pos.x && 
                    cobra1.corpo[0].y == powerups[i].pos.y) {
                    processar_powerup(powerups[i].tipo);
                    powerups[i].ativo = 0;
                }
                
                if (modo_jogo == 4 && cobra2.corpo[0].x == powerups[i].pos.x && 
                    cobra2.corpo[0].y == powerups[i].pos.y) {
                    processar_powerup(powerups[i].tipo);
                    powerups[i].ativo = 0;
                }
            }
        }
        
        desenhar_tela();
        
        // Calcular velocidade baseada nos power-ups
        int velocidade = velocidade_base - (jogo.nivel * 10);
        if (turbo_ativo) velocidade /= 2;
        if (slow_ativo) velocidade *= 2;
        if (velocidade < 50) velocidade = 50;
        
        Sleep(velocidade);
    }
}

// Fun��o principal
int main() {
    while (1) {
        mostrar_menu_principal();
        
        int opcao;
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                modo_jogo = 1;
                jogo.powerups_habilitados = menu_powerups();
                executar_jogo();
                break;
                
            case 2:
                modo_jogo = 2;
                jogo.powerups_habilitados = menu_powerups();
                executar_jogo();
                break;
                
            case 3:
                modo_jogo = 3;
                jogo.powerups_habilitados = menu_powerups();
                executar_jogo();
                break;
                
            case 4:
                modo_jogo = 4;
                jogo.powerups_habilitados = menu_powerups();
                executar_jogo();
                break;
                
            case 5:
                printf("\nObrigado por jogar!\n");
                return 0;
                
            default:
                printf("\nOpcao invalida! Pressione qualquer tecla...\n");
                _getch();
                break;
        }
    }
    
    return 0;
}
