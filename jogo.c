#include <stdio.h>
#include <time.h>
//#include <video.h>
#include <intelfpgaup/video.h>
#include <time.h>
#include <intelfpgaup/SW.h>
#include <ncurses.h>

// COORDENADAS DO PLANO NO VGA
#define X1 100
#define Y1 20
#define X2 200
#define Y2 200


// matriz desenhada
#define ALTURA 180 // 20 linhas
#define LARGURA 100 // 10 colunas
#define QUADRADO 10 // tamanho do quadrado em pixel 5x5
#define PASSO 1 // passo da peça, nesse caso vai de 1 em 1


// matriz de manipulação
#define LINHAS 18 // 18 linhas de blocos
#define COLUNAS 10 // 10 colunas de blocos


// definindo matriz global
int matriz[LINHAS][COLUNAS]; //18 x 10

// definindo variavel global do pause
int pausado = 0;

// definindo variavel global da pontuação
int pontos = 0;

// inicializa a matriz com 0s
// o 5 sinaliza onde as colunas vão ficar
int inicializa_matriz() {
   int i, j;
   for (i = 0; i < LINHAS; i++) {
       for (j = 0; j < COLUNAS; j++) {
           //vendo as linhas e colunas para fazer a borda
           if(i == 0 || i == LINHAS-1){
               matriz[i][j] = 5;
           }else if((j == 0 || j == COLUNAS-1) && matriz[i][j] != 5){
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
   int x1 = X1 + (j * QUADRADO); // Adiciona o deslocamento x
   int y1 = Y1 + (i * QUADRADO); // Adiciona o deslocamento y
   int x2 = x1 + 10; // Para cobrir 10 pixels
   int y2 = y1 + 10; // Para cobrir 10 pixels

   // Chama a função video_box para desenhar a peça
   video_box(x1, y1, x2, y2, color);
}

// Verifica se a próxima posição está ocupada ou se atingiu o fundo
int pode_mover_para_baixo(int i, int j) { //no caso o nosso fundo é a linha 34
   if (i + 1 >= LINHAS-2) return 0; // Chegou ao fundo - considerando a borda - o fundo é a linha 16 da matriz
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

void pausar() {
   int chave = 0;
   printf("Jogo pausado. Pressione a chave 1 novamente para continuar.\n");
    pausado = 1; //notifica que o jogo esta pausado. Essa é uma variavel global.

   do {
       SW_read(&chave); //vai ler a chave
                  
   } while (chave != 0b0000000000); //enquanto não for igual a 0b0000000000

   SW_close();
   printf("Jogo retomado!\n");
    pausado = 0;
}

/*
Função: Faz os blocos acima de um espaço vazio caírem.
Parâmetro: a posição do agrupamento removido (canto superior esquerdo).
*/
void desce_blocos_acima() {
    int i,j;
    for (j = 0; j < COLUNAS; j++) { // Percorre todas as colunas
        for (i = LINHAS - 2; i >= 0; i--) { // De baixo para cima, exceto a última linha
            if (matriz_fixa[i][j] != 0) { // Se houver um bloco
                int k = i;
                while (k + 1 < LINHAS && matriz[k + 1][j] == 0) { // Encontra o próximo espaço vazio abaixo
                    matriza[k + 1][j] = matriz[k][j]; // Move o bloco para baixo
                    matriz[k][j] = 0; // Deixa o espaço original vazio
                    k++; // Continua descendo até encontrar um obstáculo
                }
            }
        }
    }
}

/*
Função: Remove o agrupamento de blocos a direita (quadrado 2x2) da matriz.
Parâmetros: Coordenadas do canto superior esquerdo do agrupamento.
*/
void remove_agrupamento_direita(int linha, int coluna) {
    matriz[linha][coluna] = 0;
    matriz[linha + 1][coluna] = 0;
    matriz[linha][coluna + 1] = 0;
    matriz[linha + 1][coluna + 1] = 0;

    printf("Agrupamento removido\n");

    pontos+=4;
    printf("Sua Pontuação Atual é: %d\n", pontos);

    //desce_blocos_acima();
}

/*
Função: Remove o agrupamento de blocos a esquerda (quadrado 2x2) da matriz.
Parâmetros: Coordenadas do canto superior esquerdo do agrupamento.
*/
void remove_agrupamento_esquerda(int linha, int coluna) {
    matriz[linha][coluna] = 0;
    matriz[linha - 1][coluna] = 0;
    matriz[linha][coluna + 1] = 0;
    matriz[linha - 1][coluna + 1] = 0;

    printf("Agrupamento removido\n");

    pontos+=4;
    printf("Sua Pontuação Atual é: %d\n", pontos);
    //desce_blocos_acima();
}

/*
Função: Verifica se há um agrupamento de 4 blocos da mesma cor formando um quadrado.
*/
void verifica_agrupamento() {
    for (int i = 0; i < LINHAS - 1; i++) {
        for (int j = 0; j < COLUNAS - 1; j++) {
            // Verifica se há um quadrado de 2x2 e se todos os blocos têm a mesma cor
            // Não verifica onde é 0 e 5 pois se refere a cor do tabuleiro e as bordas

            if (matriz[i][j] != 0 && matriz[i][j] != 5 && 
                matriz[i][j] == matriz[i + 1][j] &&
                matriz[i][j] == matriz[i][j + 1] &&
                matriz[i][j] == matriz[i + 1][j + 1])
            {
                remove_agrupamento_direita(i, j);  // Remove o agrupamento a direita do bloco
            }

            if (matriz[i][j] != 0 && matriz[i][j] != 5 && 
                matriz[i][j] == matriz[i - 1][j] &&
                matriz[i][j] == matriz[i][j + 1] &&
                matriz[i][j] == matriz[i - 1][j + 1])
            {
                remove_agrupamento_esquerda(i, j);  // Remove o agrupamento a esquerda do bloo
            }
        }
    }
}

int inicializa_ncurses(){
    initscr();
    cbreak();    // Desabilita o buffer de linha, captura teclas instantaneamente
    noecho();    // Não ecoa teclas digitadas na tela
    keypad(stdscr, TRUE); // Habilita captura de teclas especiais como setas
    curs_set(0); // Oculta o cursor

    return 1;
}

int main() {
    // Inicializa ncurses para usar as teclas
    inicializa_ncurses();

    // Inicializa video 
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
               // Se ela ainda pode mover para baixo, logo pode mover para os lados
               if (pode_mover_para_baixo(inicial_i, inicial_j)) {

                   // Limpa a posição anterior e move para baixo
                   matriz[inicial_i][inicial_j] = 0;
                   inicial_i += 1;

                    ch = getch(); // Captura a tecla pressionada (não bloqueia o programa)
                    // Verifica se o usuário pressionou alguma tecla
                    if (ch != ERR) {
                        switch (ch) {
                            case 'a': // Mover para a esquerda
                                if (!pausado) move_para_lado(inicial_i, inicial_j, -1, cor);
                                break;
                            case 'd': // Mover para a direita
                                if (!pausado) move_para_lado(inicial_i, inicial_j, +1, cor);
                                break;
                        }
                        //endwin(); // Fecha a janela ncurses
                    }

                    //Verifica agrupamento de um quadrado
                    verifica_agrupamento();

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

