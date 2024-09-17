#include <stdio.h>
#include <stdlib.h.>
#include <time.h>

#define LINHAS 8
#define COLUNAS 5

int matriz_tela[LINHAS][COLUNAS]; 
int matriz_fixa[LINHAS][COLUNAS]; 
int peca_0[2][4] = {
        {1, 0, 0, 0},
        {1, 0, 0, 0},
    };
int peca_1[2][4] = {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
    };
int peca_2[2][4] = {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
    };
int peca_3[2][4] = {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
    };

void imprime_matriz(int matriz[LINHAS][COLUNAS]);
int queda(int peca[2][4]);
void coloca_peca(int posX, int posY, int peca[2][4]);
void tira_peca(int posX, int posY);
int colisao(int posX, int posY, int peca[2][4]);
void salva_matriz();
int nova_peca();
void verifica_agrupamento();
void remove_linha(int linha);


/*
Função principal do jogo.
*/
int main() {
    int perdeu = 0;

    queda(nova_peca());

    /* 
    //PARA COMPARAR MATRIZES
    printf("DINAMICA \n");
    imprime_matriz(matriz_tela);
    printf("FIXA \n");
    imprime_matriz(matriz_fixa);
    */
    return 0;
}

/*
Função: Imprime uma matriz.
Parâmetros: matriz LINHAS x COLUNAS de inteiros.
Retorno: vazio.
*/
void imprime_matriz(int matriz[LINHAS][COLUNAS]) {
    int i, j;
    for(i = 2; i < LINHAS; i++) {
        for(j = 0; j < COLUNAS; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
Função: 
Parâmetros: peca[2][4]: A matriz que representa a peça atual, onde 1 indica a presença de um bloco da peça e 0 indica ausência de bloco.
Retorno: vazio
*/
int queda(int peca[2][4]) {
    int i;
    for(i = 0; i <= LINHAS - 2; i++) {
        if (colisao(i + 1, 0, peca)) { // caso fosse fosse gerar uma colisão na proxima linha
            coloca_peca(i, 0, peca); // fixa a peca na ultima posição valida
            imprime_matriz(matriz_tela);
            salva_matriz();
            verifica_agrupamento();
            break;
        }

        coloca_peca(i, 0, peca);
        imprime_matriz(matriz_tela);
        sleep(1);
        tira_peca(i, 0); // reseta a posição anterior para 0
    }

    return 0;
}

/*
Função: 
Parâmetros: 
> posX: A posição X (linha) da peça na matriz do jogo.
> posY: A posição Y (coluna) da peça na matriz do jogo.
> peca[2][4]: A matriz que representa a peça atual, onde 1 indica a presença de um bloco da peça e 0 indica ausência de bloco.
Retorno: vazio.
*/
void coloca_peca(int posX, int posY, int peca[2][4]) {
    int i, j;
    for(i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++){
            matriz_tela[posX + i][posY + j] = peca[i][j];
        } 
    }
}

/*
Função: 
Parâmetros: 
Retorno: vazio.
*/
void tira_peca(int posX, int posY) {
    int i, j;
    for(i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++){
            matriz_tela[posX + i][posY + j] = 0;
        }
    }
}

/*
Função: Faz uma cópia da matriz dinâmica para a matriz fixa.
Parâmetros: nenhum.
Retorno: vazio.
*/
void salva_matriz() {
    int i, j;
    for(i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++){
            matriz_fixa[i][j] = matriz_tela[i][j];
        }
    }
}

/*
Função: Detecta colisão com o fundo e com outras peças.
Parâmetros: 
> posX: A posição X (linha) da peça na matriz do jogo.
> posY: A posição Y (coluna) da peça na matriz do jogo.
> peca[2][4]: A matriz que representa a peça atual, onde 1 indica a presença de um bloco da peça e 0 indica ausência de bloco.
Retorno: 0 caso não aja colisão e 1 caso aja colisão.
*/
int colisao(int posX, int posY, int peca[2][4]) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            if (peca[i][j] != 0) {
                if (posX + i >= LINHAS || matriz_fixa[posX + i][posY + 1] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/*
Função: 
Parâmetros: nenhum.
Retorno:
*/
int nova_peca() {
    srand((unsigned)time(NULL));
    int tipo_peca = rand() % 4;
    switch (tipo_peca) {
        case 0: return peca_0; break;
        case 1: return peca_1; break;
        case 2: return peca_2; break;
        case 3: return peca_3; break;
    }
}

void verifica_agrupamento() {
    printf("verificando...\n");
    int i, j, agrupamento;
    
    for (i = 0; i < LINHAS; i++) {
        agrupamento = 1;
        for (j = 0; j < COLUNAS; j++) {
            if (matriz_fixa[i][j] == 0) {
                agrupamento = 0;
                break;
            }
        }

        if (agrupamento) {
            remove_linha(i);
            i--; // é necessário verificar novamente a linha i pois a matriz mudou pela retirada de uma linha
        }
    }
}

void remove_linha(int linha) {
    int i, j;

    // limpa a linha agrupada
    for (j = 0; j < COLUNAS; j++) {
        matriz_fixa[linha][j] = 0;
    }
    
    // "move" as linhas para baixo
    for (i = linha; i > 0; i--) {
        for (j = 0; j < COLUNAS; j++) {
            matriz_fixa[i][j] = matriz_fixa[i - 1][j];
        }
    }
}
