/*
pausa

OK agrupamento por cor (descer outras pecas)
OK agrupamento por cor
OK movimento para os lados
OK colisão para os lados (limite da tela)
OK *colisão para os lados (peca)
OK encerramento quando chega no topo

BUG colisao com outra peca (fundo)
*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define LINHAS 8
#define COLUNAS 5

int matriz_tela[LINHAS][COLUNAS];
int matriz_fixa[LINHAS][COLUNAS];

int queda();
void manipula_bloco(int posX, int posY, int funcao, int cor);
void move_para_lado(int posX, int *posY, int direcao, int cor);
int colisao_fundo(int posX, int posY);
void verifica_agrupamento();
void remove_agrupamento(int linha, int coluna);
void desce_blocos_acima();
int sorteia_peca();
void copia_matriz(int matriz_a[LINHAS][COLUNAS], int matriz_b[LINHAS][COLUNAS]);
void imprime_matriz(int matriz[LINHAS][COLUNAS]);

/*
Função principal do jogo.
*/
int main() {
    int encerrou = 0;
    
    while (!encerrou) {
        encerrou = queda();
    }

    return 0;
}

/*
Função:
Parâmetro: 
Retorno: 
*/
int queda() {
    int posX;
    int posY = 2;
    int cor = sorteia_peca();
    char input;

    for (posX = 0; posX < LINHAS; posX++) {

        if (posX == 0 && matriz_tela[1][posY] != 0) {
            printf("GAME OVER.\n");
            return 1;
        }
    
        if (colisao_fundo(posX, posY)) {
            manipula_bloco(posX, posY, 1, cor);
            imprime_matriz(matriz_tela);
            copia_matriz(matriz_fixa, matriz_tela);
            verifica_agrupamento();
            copia_matriz(matriz_tela, matriz_fixa);
            return 0;
        }

        manipula_bloco(posX, posY, 1, cor);
        
        scanf("%c", &input);
        if (input == 'a') {
            if ((posY-1 != -1) && (matriz_fixa[posX][posY-1] == 0)) {
                move_para_lado(posX, &posY, -1, cor);
            }
        } else if (input == 'd') {
            if ((posY+1 != COLUNAS) && (matriz_fixa[posX][posY+1] == 0)) {
                move_para_lado(posX, &posY, +1, cor);
            }
        }

        imprime_matriz(matriz_tela);
        sleep(1);
        manipula_bloco(posX, posY, 0, cor);        
    }

    return 0;
}

/*
Função:
Parâmetros:
Retorno: vazio
*/
void manipula_bloco(int posX, int posY, int funcao, int cor) {
    if (funcao == 0) {
        matriz_tela[posX][posY] = 0;
    } else if (funcao == 1) {
        matriz_tela[posX][posY] = cor;
    }  
}

/*
Função: 
Parâmetro: 
Retorno: vazio
*/
void move_para_lado(int posX, int *posY, int direcao, int cor) {
    manipula_bloco(posX, *posY, 0, cor);
    *posY = *posY+direcao;
    manipula_bloco(posX, *posY, 1, cor);
}

/*
Função: Detecta colisão com o fundo e com outras peças.
Parâmetros:
> posX: A posição X (linha) da peça na matriz do jogo.
> posY: A posição Y (coluna) da peça na matriz do jogo.
Retorno: 0 caso não aja colisão e 1 caso aja colisão.
*/
int colisao_fundo(int posX, int posY) {
    if (posX + 1 == LINHAS || matriz_fixa[posX + 1][posY] != 0){
        return 1;
    }
    return 0;
}

/*
Função: Verifica se há um agrupamento de 4 blocos da mesma cor formando um quadrado.
*/
void verifica_agrupamento() {
    for (int i = 0; i < LINHAS - 1; i++) {
        for (int j = 0; j < COLUNAS - 1; j++) {
            // Verifica se há um quadrado de 2x2 e se todos os blocos têm a mesma cor
            if (matriz_fixa[i][j] != 0 && 
                matriz_fixa[i][j] == matriz_fixa[i+1][j] && 
                matriz_fixa[i][j] == matriz_fixa[i][j+1] && 
                matriz_fixa[i][j] == matriz_fixa[i+1][j+1]) {
                
                remove_agrupamento(i, j);  // Remove o agrupamento
            }
        }
    }
}

/*
Função: Remove o agrupamento de blocos (quadrado 2x2) da matriz.
Parâmetros: Coordenadas do canto superior esquerdo do agrupamento.
*/
void remove_agrupamento(int linha, int coluna) {
    matriz_fixa[linha][coluna] = 0;
    matriz_fixa[linha + 1][coluna] = 0;
    matriz_fixa[linha][coluna + 1] = 0;
    matriz_fixa[linha + 1][coluna + 1] = 0;

    printf("Agrupamento removido\n");
    desce_blocos_acima();
}

/*
Função: Faz os blocos acima de um espaço vazio caírem.
Parâmetro: a posição do agrupamento removido (canto superior esquerdo).
*/
void desce_blocos_acima() {
    for (int j = 0; j < COLUNAS; j++) { // Percorre todas as colunas
        for (int i = LINHAS - 2; i >= 0; i--) { // De baixo para cima, exceto a última linha
            if (matriz_fixa[i][j] != 0) { // Se houver um bloco
                int k = i;
                while (k + 1 < LINHAS && matriz_fixa[k + 1][j] == 0) { // Encontra o próximo espaço vazio abaixo
                    matriz_fixa[k + 1][j] = matriz_fixa[k][j]; // Move o bloco para baixo
                    matriz_fixa[k][j] = 0; // Deixa o espaço original vazio
                    k++; // Continua descendo até encontrar um obstáculo
                }
            }
        }
    }
}

/*
Função: sorteia uma cor
Retorno: cor sorteada
*/
int sorteia_peca() {
    srand((unsigned)time(NULL));
    return rand() % 3 + 1;
}

/*
Função: faz uma cópia entre duas matrizes, onde a matriz_a recebe os valores de matriz_b.
Parâmetro: int (*matriz_a)[5] e int (*matriz_b)[5].
Retorno: vazio
*/
void copia_matriz(int matriz_a[LINHAS][COLUNAS], int matriz_b[LINHAS][COLUNAS]) {
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            matriz_a[i][j] = matriz_b[i][j];
        }
    }
}

/*
Função: Imprime uma matriz com cores.
Parâmetros: matriz LINHAS x COLUNAS de inteiros.
Retorno: vazio.
*/
void imprime_matriz(int matriz[LINHAS][COLUNAS]) {
    int i, j;
    for (i = 1; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            if (matriz[i][j] == 1) {
                // Exibe a peça com uma cor baseada no número
                printf("\033[35m%d \033[0m", matriz[i][j]); // Magenta para o valor 1
            } else if (matriz[i][j] == 2) {
                printf("\033[33m%d \033[0m", matriz[i][j]); // Amarelo para o valor 2
            } else if (matriz[i][j] == 3) {
                printf("\033[36m%d \033[0m", matriz[i][j]); // Ciano para o valor 3
            } else {
                // Se o valor for 0, mostra em branco (sem cor)
                printf("%d ", matriz[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}
