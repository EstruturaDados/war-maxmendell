/*
 ============================================================================
 //         PROJETO WAR ESTRUTURADO - VERSÃO COM MISSÕES
 // ============================================================================
 */
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// --- Constantes Globais ---
#define MAX_NOME 50
// Quantidade de missões disponíveis
#define TOTAL_MISSOES 5

    // --- Estrutura de Dados ---
    typedef struct {
  char nome[MAX_NOME];
  char corExercito[MAX_NOME];
  int numeroTropas;
} Territorio;

// --- Vetor de Missões (Descrições) ---
const char *MISSOES[TOTAL_MISSOES] = {
    "Destruir o exército Verde", "Conquistar 3 territórios",
    "Conquistar 5 territórios", "Eliminar o exército Vermelho",
    "Eliminar o exército Azul" // Caso raro em solo, mas possível
};

// --- Protótipos das Funções ---
Territorio *alocarMapa(int quantidade);
void cadastrarTerritorios(Territorio *mapa, int quantidade);
void liberarMemoria(Territorio *mapa, char *missao);

void atribuirMissao(char **destino, const char *missoes[], int totalMissoes);
int verificarMissao(const char *missao, const Territorio *mapa, int quantidade,
                    const char *corJogador);

void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa, int quantidade);
void exibirMissao(const char *missao);
void limparBufferEntrada();

void faseDeAtaque(Territorio *mapa, int quantidade, const char *corJogador);
void atacar(Territorio *atacante, Territorio *defensor);

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
    printf("Erro fatal: Falha na alocação de memória do mapa.\n");
    return 1;
  }

  cadastrarTerritorios(mapa, qtdTerritorios);

  // Configuração do Jogador
  const char *corJogador = "Azul";

  // Atribuição de Missão
  char *missaoJogador = NULL;
  atribuirMissao(&missaoJogador, MISSOES, TOTAL_MISSOES);

  if (missaoJogador == NULL) {
    printf("Erro fatal: Falha na alocação de memória da missão.\n");
    free(mapa);
    return 1;
  }

  printf("\n>>> MISSÃO SORTEADA! <<<\n");
  printf("Sua missão é: %s\n", missaoJogador);

  int opcao;
  int vitoria = 0;

  do {
    printf("\n--- TURNO DO JOGADOR (%s) ---\n", corJogador);
    exibirMapa(mapa, qtdTerritorios);
    exibirMissao(missaoJogador); // Passagem por valor (ponteiro é valor)
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
      // Verificação silenciosa pós-ataque
      if (verificarMissao(missaoJogador, mapa, qtdTerritorios, corJogador)) {
        printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO: %s ***\n",
               missaoJogador);
        vitoria = 1;
      }
      break;
    case 2:
      // Verificação explícita a pedido do usuário
      if (verificarMissao(missaoJogador, mapa, qtdTerritorios, corJogador)) {
        printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO: %s ***\n",
               missaoJogador);
        vitoria = 1;
      } else {
        printf("\nMissão ainda não cumprida: %s\nContinua lutando!\n",
               missaoJogador);
      }
      break;
    case 0:
      printf("Saindo do jogo...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }

  } while (opcao != 0 && !vitoria);

  liberarMemoria(mapa, missaoJogador);

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
    fgets(mapa[i].nome, MAX_NOME, stdin);
    mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0; // Remove \n

    printf("Cor do Exército (ex: Azul, Verde, Vermelho): ");
    fgets(mapa[i].corExercito, MAX_NOME, stdin);
    mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = 0;

    printf("Número de Tropas: ");
    while (scanf("%d", &mapa[i].numeroTropas) != 1 ||
           mapa[i].numeroTropas < 0) {
      printf("Inválido. Digite um número positivo: ");
      limparBufferEntrada();
    }
    limparBufferEntrada();
  }
}

void atribuirMissao(char **destino, const char *missoes[], int totalMissoes) {
  int indice = rand() % totalMissoes;
  int tamanho = strlen(missoes[indice]) + 1;

  *destino = (char *)malloc(tamanho * sizeof(char));
  if (*destino != NULL) {
    strcpy(*destino, missoes[indice]);
  }
}

int verificarMissao(const char *missao, const Territorio *mapa, int quantidade,
                    const char *corJogador) {
  // Lógica baseada em comparação de strings

  if (strstr(missao, "Destruir o exército Verde") != NULL) {
    for (int i = 0; i < quantidade; i++) {
      if (strcmp(mapa[i].corExercito, "Verde") == 0)
        return 0; // Ainda existe
    }
    return 1;
  }

  if (strstr(missao, "Eliminar o exército Vermelho") != NULL) {
    for (int i = 0; i < quantidade; i++) {
      if (strcmp(mapa[i].corExercito, "Vermelho") == 0)
        return 0;
    }
    return 1;
  }

  if (strstr(missao, "Eliminar o exército Azul") != NULL) {
    for (int i = 0; i < quantidade; i++) {
      if (strcmp(mapa[i].corExercito, "Azul") == 0)
        return 0;
    }
    return 1;
  }

  // Missões de conquista
  int territoriosConquistados = 0;
  for (int i = 0; i < quantidade; i++) {
    if (strcmp(mapa[i].corExercito, corJogador) == 0) {
      territoriosConquistados++;
    }
  }

  if (strstr(missao, "Conquistar 3 territórios") != NULL) {
    return (territoriosConquistados >= 3);
  }

  if (strstr(missao, "Conquistar 5 territórios") != NULL) {
    return (territoriosConquistados >= 5);
  }

  return 0; // Missão desconhecida ou não cumprida
}

void liberarMemoria(Territorio *mapa, char *missao) {
  if (mapa != NULL) {
    free(mapa);
  }
  if (missao != NULL) {
    free(missao);
  }
  printf("\nMemória (mapa e missões) liberada com sucesso.\n");
}

void exibirMenuPrincipal() {
  printf("\n--- MENU ---\n");
  printf("1 - Atacar\n");
  printf("2 - Verificar Status da Missão\n");
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

void exibirMissao(const char *missao) {
  if (missao) {
    printf("\n>>> OBJETIVO: %s\n", missao);
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

  if (strcmp(mapa[origemIdx].corExercito, corJogador) != 0) {
    printf("Você só pode atacar com territórios %s!\n", corJogador);
    return;
  }

  if (strcmp(mapa[destinoIdx].corExercito, corJogador) == 0) {
    printf("Não pode atacar um território aliado (%s)!\n",
           mapa[destinoIdx].corExercito);
    return;
  }

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

  int dadoAtk = (rand() % 6) + 1;
  int dadoDef = (rand() % 6) + 1;

  printf("Rolagem de Dados: Atacante [%d] x Defensor [%d]\n", dadoAtk, dadoDef);

  if (dadoAtk > dadoDef) {
    printf("-> Vitoria do ATACANTE!\n");
    printf("O território %s agora pertence ao exército %s!\n", defensor->nome,
           atacante->corExercito);

    strcpy(defensor->corExercito, atacante->corExercito);

    int tropasMovidas = atacante->numeroTropas / 2;
    if (tropasMovidas < 1)
      tropasMovidas = 1;

    defensor->numeroTropas = tropasMovidas;
    atacante->numeroTropas -= tropasMovidas;

    printf("%d tropas foram movidas para o novo território.\n", tropasMovidas);
  } else {
    printf("-> Vitoria da DEFESA!\n");
    atacante->numeroTropas--;
    printf("O atacante perdeu 1 tropa. Restam %d tropas.\n",
           atacante->numeroTropas);
  }
}

void limparBufferEntrada() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
```