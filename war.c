/*
 ============================================================================
 //         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
 // ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define MAX_NOME 50
// Missões
#define MISSAO_DESTRUIR_VERDE 1
#define MISSAO_CONQUISTAR_3 2

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_NOME];
    int numeroTropas;
} Territorio;

// --- Protótipos das Funções ---
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao);
void limparBufferEntrada();

void faseDeAtaque(Territorio* mapa, int idMissao, const char* corJogador);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador);

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro fatal: Falha na alocação de memória.\n");
        return 1;
    }

    inicializarTerritorios(mapa);
    
    // Define a cor do jogador (Azul)
    const char* corJogador = "Azul"; 
    int idMissao = sortearMissao();

    int opcao;
    int vitoria = 0;

    do {
        printf("\n--- TURNO DO JOGADOR (%s) ---\n", corJogador);
        exibirMapa(mapa);
        exibirMissao(idMissao);
        exibirMenuPrincipal();
        
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = -1; // Inválido
        } else {
             limparBufferEntrada();
        }

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, idMissao, corJogador);
                // Verifica vitória automaticamente após ataque também
                if (verificarVitoria(mapa, idMissao, corJogador)) {
                    printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO! ***\n");
                    vitoria = 1;
                }
                break;
            case 2:
                if (verificarVitoria(mapa, idMissao, corJogador)) {
                    printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO! ***\n");
                    vitoria = 1;
                } else {
                    printf("\nMissão ainda não cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0 && !vitoria);

    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

Territorio* alocarMapa() {
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa) {
    if (!mapa) return;

    // Configuração inicial fixa para teste
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].corExercito, "Azul"); // Jogador começa com 1
    mapa[0].numeroTropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].corExercito, "Verde");
    mapa[1].numeroTropas = 3;

    strcpy(mapa[2].nome, "Estados Unidos");
    strcpy(mapa[2].corExercito, "Vermelho");
    mapa[2].numeroTropas = 4;

    strcpy(mapa[3].nome, "Russia");
    strcpy(mapa[3].corExercito, "Amarelo");
    mapa[3].numeroTropas = 6;

    strcpy(mapa[4].nome, "Australia");
    strcpy(mapa[4].corExercito, "Branco");
    mapa[4].numeroTropas = 2;
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void exibirMenuPrincipal() {
    printf("\n--- MENU ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

void exibirMapa(const Territorio* mapa) {
    printf("\n%-4s | %-20s | %-10s | %s\n", "ID", "Território", "Exército", "Tropas");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-4d | %-20s | %-10s | %d\n", i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].numeroTropas);
    }
    printf("----------------------------------------------------------\n");
}

void exibirMissao(int idMissao) {
    printf("\n>>> MISSÃO ATUAL: ");
    if (idMissao == MISSAO_DESTRUIR_VERDE) {
        printf("Destruir o exército Verde.\n");
    } else if (idMissao == MISSAO_CONQUISTAR_3) {
        printf("Conquistar 3 territórios.\n");
    }
}

void faseDeAtaque(Territorio* mapa, int idMissao, const char* corJogador) {
    int origemIdx, destinoIdx;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Digite o ID do território Atacante (seu): ");
    if (scanf("%d", &origemIdx) != 1) { limparBufferEntrada(); return; }
    
    printf("Digite o ID do território Defensor (inimigo): ");
    if (scanf("%d", &destinoIdx) != 1) { limparBufferEntrada(); return; }
    limparBufferEntrada();

    // Ajustar para índice 0
    origemIdx--;
    destinoIdx--;

    if (origemIdx < 0 || origemIdx >= NUM_TERRITORIOS || destinoIdx < 0 || destinoIdx >= NUM_TERRITORIOS) {
        printf("IDs inválidos!\n");
        return;
    }

    if (origemIdx == destinoIdx) {
        printf("Não pode atacar o próprio território!\n");
        return;
    }

    // Validação de posse
    if (strcmp(mapa[origemIdx].corExercito, corJogador) != 0) {
        printf("Você só pode atacar com territórios %s!\n", corJogador);
        return;
    }

    // Regra: Não pode atacar amigo (opcional, mas faz sentido)
    if (strcmp(mapa[destinoIdx].corExercito, corJogador) == 0) {
        printf("Não pode atacar um território aliado!\n");
        return;
    }

    // Regra: Mínimo 2 tropas para atacar (1 fica, 1 ataca)
    if (mapa[origemIdx].numeroTropas < 2) {
        printf("Território %s precisa de mais de 1 tropa para atacar.\n", mapa[origemIdx].nome);
        return;
    }

    simularAtaque(&mapa[origemIdx], &mapa[destinoIdx]);
}

void simularAtaque(Territorio* atacante, Territorio* defensor) {
    printf("\nBatalha iniciada!\n");
    printf("Atacante: %s (%s) vs Defensor: %s (%s)\n", 
            atacante->nome, atacante->corExercito, defensor->nome, defensor->corExercito);

    // Sorteio de dados (1 a 6)
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dados: Atacante [%d] x Defensor [%d]\n", dadoAtk, dadoDef);

    // Regra do prompt: "Empates favorecem o atacante"
    if (dadoAtk >= dadoDef) {
        printf("-> O atacante venceu a rodada!\n");
        defensor->numeroTropas--;
        
        if (defensor->numeroTropas <= 0) {
            printf(">>> TERRITÓRIO CONQUISTADO! <<<\n");
            // Troca de dono
            strcpy(defensor->corExercito, atacante->corExercito);
            // Move tropas (simplificação: move 1 tropa)
            defensor->numeroTropas = 1;
            atacante->numeroTropas--;
        }
    } else {
        printf("-> A defesa venceu a rodada!\n");
        atacante->numeroTropas--;
    }
}

int sortearMissao() {
    return (rand() % 2) + 1;
}

int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador) {
    if (idMissao == MISSAO_DESTRUIR_VERDE) {
        // Verifica se Verde ainda existe
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].corExercito, "Verde") == 0) {
                return 0;
            }
        }
        return 1;
    } else if (idMissao == MISSAO_CONQUISTAR_3) {
        int conta = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0) {
                conta++;
            }
        }
        return (conta >= 3);
    }
    return 0;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
