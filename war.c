/*
 ============================================================================
 //         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
 // ============================================================================
 */
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// --- Constantes Globais ---
#define MAX_NOME 50
// Missões (simplificadas para o exemplo)
#define MISSAO_DESTRUIR_VERDE 1
#define MISSAO_CONQUISTAR_3 2

// --- Estrutura de Dados ---
typedef struct {
  char nome[MAX_NOME];
  char corExercito[MAX_NOME];
  int numeroTropas;
} Territorio;

// --- Protótipos das Funções ---
Territorio *alocarMapa(int quantidade);
void cadastrarTerritorios(Territorio *mapa, int quantidade);
void liberarMemoria(Territorio *mapa);

void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa, int quantidade);
void exibirMissao(int idMissao);
void limparBufferEntrada();

void faseDeAtaque(Territorio *mapa, int quantidade, const char *corJogador);
void atacar(Territorio *atacante, Territorio *defensor);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int quantidade, int idMissao,
                     const char *corJogador);

// --- Função Principal ---
int main() {
  setlocale(LC_ALL, "Portuguese");
  srand(time(NULL));

  int qtdTerritorios;
  printf("Bem-vindo ao War Estruturado!\n");
  printf("Digite a quantidade de territórios para o jogo: ");
  if (scanf("%d", &qtdTerritorios) != 1 || qtdTerritorios <= 0) {
    printf("Quantidade inválida! Encerrando.\n");
    return 1;
  }
  limparBufferEntrada();

  Territorio *mapa = alocarMapa(qtdTerritorios);
  if (mapa == NULL) {
    printf("Erro fatal: Falha na alocação de memória.\n");
    return 1;
  }

  cadastrarTerritorios(mapa, qtdTerritorios);

  // Define a cor do jogador (Vamos assumir que o jogador é o primeiro a ser
  // cadastrado ou fixo "Azul" para simplificar a lógica de jogo solo) Para este
  // exercício, mantemos o jogador como "Azul" e assumimos que ele deve
  // controlar os territórios que cadastrar como "Azul".
  const char *corJogador = "Azul";
  int idMissao = sortearMissao();

  int opcao;
  int vitoria = 0;

  do {
    printf("\n--- TURNO DO JOGADOR (%s) ---\n", corJogador);
    exibirMapa(mapa, qtdTerritorios);
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
      faseDeAtaque(mapa, qtdTerritorios, corJogador);
      if (verificarVitoria(mapa, qtdTerritorios, idMissao, corJogador)) {
        printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO! ***\n");
        vitoria = 1;
      }
      break;
    case 2:
      if (verificarVitoria(mapa, qtdTerritorios, idMissao, corJogador)) {
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

Territorio *alocarMapa(int quantidade) {
  return (Territorio *)calloc(quantidade, sizeof(Territorio));
}

void cadastrarTerritorios(Territorio *mapa, int quantidade) {
  printf("\n--- CADASTRO DE TERRITÓRIOS ---\n");
  for (int i = 0; i < quantidade; i++) {
    printf("Território %d:\n", i + 1);
    printf("Nome: ");
    // fgets é mais seguro que scanf para strings com espaços
    fgets(mapa[i].nome, MAX_NOME, stdin);
    mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0; // Remove \n

    printf("Cor do Exército (ex: Azul, Verde): ");
    fgets(mapa[i].corExercito, MAX_NOME, stdin);
    mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = 0;

    printf("Número de Tropas: ");
    while (scanf("%d", &mapa[i].numeroTropas) != 1 ||
           mapa[i].numeroTropas < 0) {
      printf("Inválido. Digite um número positivo: ");
      limparBufferEntrada();
    }
    limparBufferEntrada(); // Limpa buffer após ler int
  }
}

void liberarMemoria(Territorio *mapa) {
  if (mapa != NULL) {
    free(mapa);
    printf("\nMemória liberada com sucesso.\n");
  }
}

void exibirMenuPrincipal() {
  printf("\n--- MENU ---\n");
  printf("1 - Atacar\n");
  printf("2 - Verificar Missão\n");
  printf("0 - Sair\n");
}

void exibirMapa(const Territorio *mapa, int quantidade) {
  printf("\n%-4s | %-20s | %-10s | %s\n", "ID", "Território", "Exército",
         "Tropas");
  printf("----------------------------------------------------------\n");
  for (int i = 0; i < quantidade; i++) {
    printf("%-4d | %-20s | %-10s | %d\n", i + 1, mapa[i].nome,
           mapa[i].corExercito, mapa[i].numeroTropas);
  }
  printf("----------------------------------------------------------\n");
}

void exibirMissao(int idMissao) {
  printf("\n>>> MISSÃO ATUAL: ");
  if (idMissao == MISSAO_DESTRUIR_VERDE) {
    printf("Destruir o exército Verde.\n");
  } else if (idMissao == MISSAO_CONQUISTAR_3) {
    printf("Conquistar pelo menos 3 territórios.\n");
  }
}

void faseDeAtaque(Territorio *mapa, int quantidade, const char *corJogador) {
  int origemIdx, destinoIdx;

  printf("\n--- FASE DE ATAQUE ---\n");
  printf("Digite o ID do território Atacante (seu %s): ", corJogador);
  if (scanf("%d", &origemIdx) != 1) {
    limparBufferEntrada();
    return;
  }

  printf("Digite o ID do território Defensor (inimigo): ");
  if (scanf("%d", &destinoIdx) != 1) {
    limparBufferEntrada();
    return;
  }
  limparBufferEntrada();

  // Ajustar para índice 0
  origemIdx--;
  destinoIdx--;

  if (origemIdx < 0 || origemIdx >= quantidade || destinoIdx < 0 ||
      destinoIdx >= quantidade) {
    printf("IDs inválidos! Escolha entre 1 e %d.\n", quantidade);
    return;
  }

  if (origemIdx == destinoIdx) {
    printf("Não pode atacar o próprio território!\n");
    return;
  }

  // Validação de posse do atacante
  /*
     Nota: Como o cadastro é livre, o jogador deve digitar "Azul" (ou a cor
     escolhida) corretamente no cadastro para ser reconhecido aqui.
  */
  if (strcmp(mapa[origemIdx].corExercito, corJogador) != 0) {
    printf("Você só pode atacar com territórios %s!\n", corJogador);
    return;
  }

  // Validação de posse do defensor (não pode atacar aliados)
  if (strcmp(mapa[destinoIdx].corExercito, corJogador) == 0) {
    printf("Não pode atacar um território aliado (%s)!\n",
           mapa[destinoIdx].corExercito);
    return;
  }

  // Regra: Mínimo 2 tropas para atacar (1 fica, 1 ataca)
  if (mapa[origemIdx].numeroTropas < 2) {
    printf("Território %s precisa de mais de 1 tropa para atacar.\n",
           mapa[origemIdx].nome);
    return;
  }

  atacar(&mapa[origemIdx], &mapa[destinoIdx]);
}

void atacar(Territorio *atacante, Territorio *defensor) {
  printf("\n>>> BATALHA INICIADA <<<\n");
  printf("Atacante: %s (%s) [%d tropas] vs Defensor: %s (%s) [%d tropas]\n",
         atacante->nome, atacante->corExercito, atacante->numeroTropas,
         defensor->nome, defensor->corExercito, defensor->numeroTropas);

  // Simulação de dados (1 a 6)
  int dadoAtk = (rand() % 6) + 1;
  int dadoDef = (rand() % 6) + 1;

  printf("Rolagem de Dados: Atacante [%d] x Defensor [%d]\n", dadoAtk, dadoDef);

  if (dadoAtk > dadoDef) {
    printf("-> Vitoria do ATACANTE!\n");

    // Atacante vence: Território defensor muda de dono
    printf("O território %s agora pertence ao exército %s!\n", defensor->nome,
           atacante->corExercito);

    strcpy(defensor->corExercito, atacante->corExercito);

    // Transfere metade das tropas do atacante para o defensor
    int tropasMovidas = atacante->numeroTropas / 2;
    if (tropasMovidas < 1)
      tropasMovidas = 1; // Garante pelo menos 1 tropa se possível, mas lógico
                         // que < 2 não ataca

    defensor->numeroTropas = tropasMovidas;
    atacante->numeroTropas -= tropasMovidas;

    printf("%d tropas foram movidas para o novo território.\n", tropasMovidas);

  } else {
    printf("-> Vitoria da DEFESA!\n");
    // Atacante perde: Perde 1 tropa
    atacante->numeroTropas--;
    printf("O atacante perdeu 1 tropa. Restam %d tropas.\n",
           atacante->numeroTropas);
  }
}

int sortearMissao() { return (rand() % 2) + 1; }

int verificarVitoria(const Territorio *mapa, int quantidade, int idMissao,
                     const char *corJogador) {
  if (idMissao == MISSAO_DESTRUIR_VERDE) {
    // Verifica se Verde ainda existe
    for (int i = 0; i < quantidade; i++) {
      if (strcmp(mapa[i].corExercito, "Verde") == 0) {
        return 0; // Ainda existe Verde
      }
    }
    return 1; // Verde eliminado
  } else if (idMissao == MISSAO_CONQUISTAR_3) {
    int conta = 0;
    for (int i = 0; i < quantidade; i++) {
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
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}