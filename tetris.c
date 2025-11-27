#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================================
// CONSTANTES E DEFINIÇÕES DE ESTRUTURAS
// ============================================================

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Estrutura para a Fila Circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int total; // Contador de elementos atuais
} FilaCircular;

// Estrutura para a Pilha
typedef struct {
    Peca itens[TAM_PILHA];
    int topo; // Índice do topo (-1 se vazia)
} Pilha;

// Variável global para controle do ID único (simulando um autoincrement)
int contadorGlobalId = 0;

// ============================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================

Peca gerarPeca();
void inicializarFila(FilaCircular *f);
void inicializarPilha(Pilha *p);
int filaCheia(FilaCircular *f);
int filaVazia(FilaCircular *f);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void enfileirar(FilaCircular *f, Peca peca);
Peca desenfileirar(FilaCircular *f);
void empilhar(Pilha *p, Peca peca);
Peca desempilhar(Pilha *p);
void mostrarEstado(FilaCircular *f, Pilha *p);
void jogarPeca(FilaCircular *f);
void reservarPeca(FilaCircular *f, Pilha *p);
void usarPecaReservada(Pilha *p);
void trocarPecaAtual(FilaCircular *f, Pilha *p);
void trocaMultipla(FilaCircular *f, Pilha *p);

// ============================================================
// FUNÇÃO PRINCIPAL (MAIN)
// ============================================================

int main() {
    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    FilaCircular fila;
    Pilha pilha;
    int opcao;

    // 1. Inicializar estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    printf("Inicializando o sistema e gerando peças iniciais...\n");
    while (!filaCheia(&fila)) {
        enfileirar(&fila, gerarPeca());
    }

    // Loop do menu principal
    do {
        mostrarEstado(&fila, &pilha);

        printf("\n-------------------------------------------------------\n");
        printf("| Codigo | Acao                                       |\n");
        printf("|--------|--------------------------------------------|\n");
        printf("|   1    | Jogar peca da frente da fila               |\n");
        printf("|   2    | Enviar peca da fila para a pilha de reserva|\n");
        printf("|   3    | Usar peca da pilha de reserva              |\n");
        printf("|   4    | Trocar peca da frente da fila com o topo   |\n");
        printf("|   5    | Trocar bloco (3 da fila <-> 3 da pilha)    |\n");
        printf("|   0    | Sair                                       |\n");
        printf("-------------------------------------------------------\n");
        printf("Opcao escolhida: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf(">> Opcao invalida! Tente novamente.\n");
        }
        
        // Pequena pausa visual antes de redesenhar o estado
        if(opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            setbuf(stdin, NULL); // Limpa buffer do teclado
            getchar();
            // system("cls"); // Pode usar system("clear") no Linux/Mac para limpar a tela
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DE LÓGICA DO JOGO
// ============================================================

// Gera uma peça aleatória
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = contadorGlobalId++;
    return p;
}

// Ação 1: Remove da fila e gera uma nova no final (Manter fila cheia)
void jogarPeca(FilaCircular *f) {
    if (filaVazia(f)) {
        printf(">> Erro: Fila vazia!\n");
        return;
    }
    Peca p = desenfileirar(f);
    printf(">> Acao: Peca [ %c %d ] foi JOGADA.\n", p.nome, p.id);
    
    // Gera nova peça para repor
    Peca nova = gerarPeca();
    enfileirar(f, nova);
    printf(">> Reposicao: Nova peca [ %c %d ] adicionada ao fim da fila.\n", nova.nome, nova.id);
}

// Ação 2: Tira da fila e põe na pilha. Fila repõe automaticamente.
void reservarPeca(FilaCircular *f, Pilha *p) {
    if (pilhaCheia(p)) {
        printf(">> Acao Negada: A pilha de reserva esta cheia! Use uma peca reservada antes.\n");
        return;
    }
    
    Peca temp = desenfileirar(f);
    empilhar(p, temp);
    printf(">> Acao: Peca [ %c %d ] movida para a RESERVA.\n", temp.nome, temp.id);

    // Repõe a fila
    Peca nova = gerarPeca();
    enfileirar(f, nova);
    printf(">> Reposicao: Nova peca [ %c %d ] adicionada ao fim da fila.\n", nova.nome, nova.id);
}

// Ação 3: Tira da pilha e descarta (simula uso)
void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf(">> Acao Negada: Nao ha pecas na reserva.\n");
        return;
    }
    Peca temp = desempilhar(p);
    printf(">> Acao: Peca reservada [ %c %d ] foi USADA.\n", temp.nome, temp.id);
}

// Ação 4: Troca a peça da frente da fila com a do topo da pilha
void trocarPecaAtual(FilaCircular *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf(">> Acao Negada: A pilha esta vazia, nao ha como trocar.\n");
        return;
    }
    if (filaVazia(f)) {
        printf(">> Acao Negada: Fila vazia.\n");
        return;
    }

    // Acesso direto aos arrays para troca (sem remover e inserir, apenas swap de valores)
    // Na fila circular, o índice da frente é f->frente
    // Na pilha, o índice do topo é p->topo
    
    Peca tempFila = f->itens[f->frente];
    Peca tempPilha = p->itens[p->topo];

    f->itens[f->frente] = tempPilha;
    p->itens[p->topo] = tempFila;

    printf(">> Acao: Troca simples realizada entre [ %c %d ] e [ %c %d ].\n", 
           tempFila.nome, tempFila.id, tempPilha.nome, tempPilha.id);
}

// Ação 5: Troca as 3 primeiras da fila com as 3 da pilha
void trocaMultipla(FilaCircular *f, Pilha *p) {
    // Verifica se a pilha tem 3 peças (capacidade máxima é 3)
    // Verifica se a fila tem pelo menos 3 peças
    if (p->topo != (TAM_PILHA - 1)) { // Topo deve ser 2 (0, 1, 2)
        printf(">> Acao Negada: A pilha precisa ter %d pecas para a troca em bloco.\n", TAM_PILHA);
        return;
    }
    if (f->total < 3) {
        printf(">> Acao Negada: A fila precisa ter pelo menos 3 pecas.\n");
        return;
    }

    printf(">> Acao: Realizando troca em bloco (3 pecas)...\n");

    // Realiza a troca dos 3 primeiros elementos
    for (int i = 0; i < 3; i++) {
        // Calcula o índice na fila circular: (frente + i) % TAM
        int idxFila = (f->frente + i) % TAM_FILA;
        
        // Na pilha, vamos trocar com os elementos de cima para baixo (topo, topo-1, topo-2)
        // Ou podemos trocar direto com os índices 2, 1, 0.
        // Vamos assumir a troca do 1º da fila com o Topo, 2º da fila com Topo-1...
        int idxPilha = p->topo - i; 

        Peca aux = f->itens[idxFila];
        f->itens[idxFila] = p->itens[idxPilha];
        p->itens[idxPilha] = aux;
    }
    printf(">> Sucesso: Troca multipla concluida.\n");
}

// Exibe o estado atual visualmente
void mostrarEstado(FilaCircular *f, Pilha *p) {
    printf("\n=== ESTADO ATUAL ===\n");
    
    // Exibir Fila
    printf("Fila de pecas (Frente -> Tras): ");
    if (filaVazia(f)) {
        printf("[ Vazia ]");
    } else {
        int count = 0;
        int i = f->frente;
        while (count < f->total) {
            printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
            i = (i + 1) % TAM_FILA; // Lógica circular
            count++;
        }
    }
    printf("\n");

    // Exibir Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[ Vazia ]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n====================\n");
}

// ============================================================
// FUNÇÕES DE MANIPULAÇÃO DE ESTRUTURAS DE DADOS
// ============================================================

void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = 0;
    f->total = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1; // -1 indica pilha vazia
}

int filaCheia(FilaCircular *f) {
    return (f->total == TAM_FILA);
}

int filaVazia(FilaCircular *f) {
    return (f->total == 0);
}

int pilhaCheia(Pilha *p) {
    return (p->topo == TAM_PILHA - 1);
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

void enfileirar(FilaCircular *f, Peca peca) {
    if (filaCheia(f)) return;
    f->itens[f->tras] = peca;
    f->tras = (f->tras + 1) % TAM_FILA; // Incremento circular
    f->total++;
}

Peca desenfileirar(FilaCircular *f) {
    Peca p = {' ', -1}; // Peça vazia/erro
    if (filaVazia(f)) return p;
    
    p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA; // Incremento circular
    f->total--;
    return p;
}

void empilhar(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = peca;
}

Peca desempilhar(Pilha *p) {
    Peca peca = {' ', -1};
    if (pilhaVazia(p)) return peca;
    
    peca = p->itens[p->topo];
    p->topo--;
    return peca;
}