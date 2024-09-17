#include <stdio.h>
#include <stdlib.h.>
#include <time.h>
#include <video.h>
//#include <intelfpgaup/video.h>

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

void inicializa_video();
void exibe_peca(int posX, int posY, int peca[2][4], short cor);
void queda(int peca[2][4]);
void coloca_peca(int posX, int posY, int peca[2][4]);
void tira_peca(int posX, int posY);
int colisao(int posX, int posY, int peca[2][4]);
void salva_matriz();
int nova_peca();


/*
Função principal do jogo.
*/
int main() { 
    inicializa_video();
    
    queda(nova_peca());

    video_close();
    
    return 0;
}

/*
Função: 
Parâmetros: nenhum.
Retorno: vazio.
*/
void inicializa_video() {
    if (!video_open()) {
        printf("Erro ao abrir o video.\n");
        return 1;
    }
    video_clear();
}

/*
Função: 
Parâmetros:
Retorno: vazio.
*/
void exibe_peca(int posX, int posY, int peca[2][4], short cor) {
    int tamanho_bloco = 20;
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            if (peca[i][j] != 0) {
                int x1 = (posY + j) * tamanho_bloco;
                int y1 = (posY + i) * tamanho_bloco;
                int x2 = x1 + tamanho_bloco - 1;
                int y2 = y1 + tamanho_bloco - 1;
                video_box(x1, y1, x2, y2, cor);
            }
        }
    }
}

/*
Função: 
Parâmetros: peca[2][4]: A matriz que representa a peça atual, onde 1 indica a presença de um bloco da peça e 0 indica ausência de bloco.
Retorno: vazio
*/
void queda(int peca[2][4]) {
    int i;
    for(i = 0; i <= LINHAS - 2; i++) {
        if (colisao(i + 1, 0, peca)) { // caso fosse fosse gerar uma colisao na proxima linha
            coloca_peca(i, 0, peca); // fixa a peca na ultima posição valida
            exibe_peca(i, 0, peca, video_PINK);
            salva_matriz();
            break;
        }

        coloca_peca(i, 0, peca);
        sleep(1);
        tira_peca(i, 0); // reseta a posição anterior para 0
        exibe_peca(i, 0, peca, 000000);
    }
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
Função: 
Parâmetros: 
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
