#include <stdio.h>
#include <time.h>
//#include <video.h>
#include <intelfpgaup/video.h>
#include <time.h>
#include <termios.h> // Para capturar entrada sem esperar enter
#include <intelfpgaup/SW.h>


// COORDENADAS DO PLANO NO VGA
#define X1 100
#define Y1 20
#define X2 200
#define Y2 200

// matriz desenhada
#define ALTURA 180 // 20 linhas
#define LARGURA 100 // 10 colunas
#define QUADRADO 5 // tamanho do quadrado em pixel 5x5
#define PASSO 1 // passo da peça, nesse caso vai de 1 em 1

// matriz de manipulação
#define LINHAS 36 // 36 linhas de blocos
#define COLUNAS 20 // 20 colunas de blocos

// definindo matriz global
int matriz[LINHAS][COLUNAS]; //36 x 20

// inicializa a matriz com 0s
// o 5 sinaliza onde as colunas vão ficar
int inicializa_matriz() {
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            //vendo as linhas e colunas para fazer a borda
            if(i == 0 || i == 35){
                matriz[i][j] = 5;
            }else if((j == 0 || j == 19) && matriz[i][j] != 5){
                matriz[i][j] = 5;
            //o restante é o tabuleiro branco
            }else{
                matriz[i][j] = 0;
            }
        }
    }
    return 1;
}

int inicializa_video() {
    if (!video_open()) {
        printf("Erro ao abrir o vídeo.\n");
        return 0;
    } else {
        video_clear(); // sempre bom limpar
        return 1;
    }
}

void pintando_matriz_VGA() {
    video_clear();
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            //Pintando matriz de branco
            if (matriz[i][j] == 0) {
                pintado_quadrado_VGA(i, j, video_WHITE);            
            //peça 1
            } else if (matriz[i][j] == 1) {
                pintado_quadrado_VGA(i, j, video_MAGENTA);
            //peça 2
            } else if (matriz[i][j] == 2) {
                pintado_quadrado_VGA(i, j, video_YELLOW);
            //peça 3
            } else if (matriz[i][j] == 3) {
                pintado_quadrado_VGA(i, j, video_GREEN);
            //peça 4
            } else if (matriz[i][j] == 4) {
                pintado_quadrado_VGA(i, j, video_BLUE);
            //pintando bordas 
            } else if (matriz[i][j] == 5) {
                pintado_quadrado_VGA(i, j, video_ORANGE);
            }
            
        }
    }
    video_show();
}

void pintado_quadrado_VGA(int i, int j, short color) {
    // Cada peça tem 5x5 pixels no tabuleiro real
    int x1 = X1 + (j * 5); // Adiciona o deslocamento x
    int y1 = Y1 + (i * 5); // Adiciona o deslocamento y
    int x2 = x1 + 5; // Para cobrir 5 pixels
    int y2 = y1 + 5; // Para cobrir 5 pixels

    // Chama a função video_box para desenhar a peça
    video_box(x1, y1, x2, y2, color);
}

// Verifica se a próxima posição está ocupada ou se atingiu o fundo
int pode_mover_para_baixo(int i, int j) { //no caso o nosso fundo é a linha 34
    if (i + 1 >= LINHAS-2) return 0; // Chegou ao fundo - considerando a borda
    if (matriz[i + 1][j] != 0) return 0; // Há uma peça abaixo
    return 1; // Pode mover para baixo
}

//sorteador de valores de 1 a 4
int nova_peca() {
    srand((unsigned)time(NULL));  // Inicializa o gerador de números aleatórios
    int tipo_peca = (rand() % 4) + 1;   // Sorteia um número entre 1 e 4
    return tipo_peca;  // Retorna o número sorteado (1, 2, 3 ou 4)
}

//função que move para os lados (até o momento com o teclado)
int move_para_lado(int posX, int posY, int direcao, int cor) {
    matriz[posX][posY] = 0;
    posY = posY+direcao;
    matriz[posX][posY] = cor;

    return posY;
}

// Função para detectar uma tecla pressionada sem precisar do 'Enter'
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void pausar() {
    int chave = 0;
    printf("Jogo pausado. Pressione a chave 1 novamente para continuar.\n");

    do {
        SW_read(&chave); //vai ler a chave
                    
    } while (chave != 0b0000000000); //enquanto não for igual a 0b0000000000

    SW_close();
    printf("Jogo retomado!\n");

}

int main() {
    int video_ok = inicializa_video();
    int cor = 0;

    if (video_ok == 1) {
        inicializa_matriz();

        while (1) { // Loop infinito para novas peças (depois tem que rever isso!!)
            // Coordenadas iniciais da nova peça
            int inicial_i = 1; // linha inicial na matriz
            int inicial_j = 10; // coluna inicial na matriz
            int fim = 0; // flag para controlar o fim de movimento de uma peça

            while (fim == 0) {
                //se não teve pause, prossegue
                cor = nova_peca();
                matriz[inicial_i][inicial_j] = cor;
                pintando_matriz_VGA();

                usleep(500000); // Intervalo para a peça descer

                //verifica se teve pause
                int valor_chaves = 0; //valor das chaves
                int retorno_chaves = 0;
                if(SW_open()){ // tudo certo para abrir
                    retorno_chaves = SW_read(&valor_chaves); //Leitura ok, retorna 1
                    if(retorno_chaves == 1){
                        if(valor_chaves == 0b0000000010){ //so executa se o valor armazenado nas chaves for igual aos bits correspondentes apenas a SW01 
                            pausar();
                        }
                    }
                }

                // Verifica se pode mover para baixo
                if (pode_mover_para_baixo(inicial_i, inicial_j)) {
                    // Limpa a posição anterior e move para baixo
                    matriz[inicial_i][inicial_j] = 0;
                    inicial_i += 1;

                    //verifica se as teclas a, d ou p foram pressionadas
                    if (kbhit()) {
                        input = getchar();

                        if (input == 'a') { //esquerda
                            if ((inicial_j-1 != -1) && (matriz[inicial_i][inicial_j-1] == 0)) {
                                inicial_j = move_para_lado(inicial_i, inicial_j, -1, cor);
                            }
                        } else if (input == 'd') { //direita
                            if ((inicial_j+1 != COLUNAS) && (matriz[inicial_i][inicial_j+1] == 0)) {
                                inicial_j = move_para_lado(inicial_i, inicial_j, +1, cor);
                            }
                        } 
                    }

                } else {
                    // A peça atingiu o fundo ou outra peça, fixa ela
                    fim = 1; // Termina o movimento da peça atual
                }
            }

            // Ao sair do loop de uma peça, começa uma nova
        }
    }

    return 1;
}
