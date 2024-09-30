#include <stdio.h>
#include <time.h>
//#include <video.h>
#include <intelfpgaup/video.h>
#include <time.h>
//#include <termios.h> // Para capturar entrada sem esperar enter
#include <intelfpgaup/SW.h>
#include "acell.c"


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
#define LINHAS 36 // 36 linhas de blocos
#define COLUNAS 20 // 20 colunas de blocos


// definindo matriz global
int matriz[LINHAS][COLUNAS]; //36 x 20



// definino matriz global p/ tela inicial
int telaInicialMatriz[16][19]; //16 x 19


//definindo varivael global para pontuação
int pontos = 0;


//função que gera o nome na matriz dados e repassa para matriz da tela inicial
int inicializa_matriz_telaInicial() {
   printf("Ta iniciando a tela inicial\n");

  // candy block
   int dados[16][19] = {
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}, // 1
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 2
       {0,1,1,1,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0}, // 3
       {0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}, // 4
       {0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0}, // 5
       {0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}, // 6
       {0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,0}, // 7
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 8
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 9
       {2,2,0,0,2,0,0,0,2,2,2,0,2,2,2,0,2,0,2}, // 10
       {2,0,2,0,2,0,0,0,2,0,2,0,2,0,0,0,2,0,2}, // 11
       {2,2,0,0,2,0,0,0,2,0,2,0,2,0,0,0,2,2,0}, // 12
       {2,0,2,0,2,0,0,0,2,0,2,0,2,0,0,0,2,0,2}, // 13
       {2,2,0,0,2,2,2,0,2,2,2,0,2,2,2,0,2,0,2}, // 14
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 15
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // 16
   };

   int i, j;
   // Inicializando a matriz com os valores definidos
   for (i = 0; i < 16; i++) {
       for (j = 0; j < 19; j++) {
           telaInicialMatriz[i][j] = dados[i][j];
       }
   }

    printf("matriz da tela inicial preenchida!\n");
  return 1;
}


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


int pintado_quadrado_tela_inicial_VGA(int i, int j, short color) {
  // Cada peça tem 5x5 pixels no tabuleiro real
  int x1 = 100 + (j * 5); // Adiciona o deslocamento x
  int y1 = 80 + (i * 5); // Adiciona o deslocamento y
  int x2 = x1 + 5; // Para cobrir 5 pixels
  int y2 = y1 + 5; // Para cobrir 5 pixels

  // Chama a função video_box para desenhar a peça
  video_box(x1, y1, x2, y2, color);
  return 1;
}

int pintado_quadrado_VGA(int i, int j, short color) {
  // Cada peça tem 5x5 pixels no tabuleiro real
  int x1 = X1 + (j * 5); // Adiciona o deslocamento x
  int y1 = Y1 + (i * 5); // Adiciona o deslocamento y
  int x2 = x1 + 5; // Para cobrir 5 pixels
  int y2 = y1 + 5; // Para cobrir 5 pixels


  // Chama a função video_box para desenhar a peça
  video_box(x1, y1, x2, y2, color);
  return 1;
}


void exibe_pontuacao_VGA(){
    char buffer[100];
    sprintf(buffer, "SCORE: %d", pontos);
    video_text(1, 40, buffer);
}


int pintando_matriz_VGA() {
    exibe_pontuacao_VGA(); //exibe em forma de texto

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
  return 1;
}


int pintando_matriz_tela_inicial_VGA() {
  video_clear();
  int i, j;

  for (i = 0; i < 16; i++) {
      for (j = 0; j < 19; j++) {
          //Pintando matriz de branco
          if (telaInicialMatriz[i][j] == 0) {
              pintado_quadrado_tela_inicial_VGA(i, j, video_WHITE);         
          //peça 1
          } else if (telaInicialMatriz[i][j] == 1) {
              pintado_quadrado_tela_inicial_VGA(i, j, video_MAGENTA);
          }
          else if (telaInicialMatriz[i][j] == 2) {
              pintado_quadrado_tela_inicial_VGA(i, j, video_BLUE);
          }
      }
  }

  video_show();
  return 1;
}



// Verifica se a próxima posição está ocupada ou se atingiu o fundo
int pode_mover_para_baixo(int i, int j) { //no caso o nosso fundo é a linha 34
  if (i + 1 >= LINHAS-1) return 0; // Chegou ao fundo - considerando a borda
  if (matriz[i + 1][j] != 0) return 0; // Há uma peça abaixo
  return 1; // Pode mover para baixo
}


//sorteador de valores de 1 a 4
int sorteia_cor() {
  srand((unsigned)time(NULL));  // Inicializa o gerador de números aleatórios
  int tipo_peca = (rand() % 4) + 1;   // Sorteia um número entre 1 e 4
  return tipo_peca;  // Retorna o número sorteado (1, 2, 3 ou 4)
}


//função que move para os lados
//direteção recebe 1, -1 ou 0 -> valores vão vim do acelerometro
int move_para_lado(int posX, int posY, int direcao, int cor) {
    //nesse codigo ele verifica se os lados não são bordas
    if(matriz[posX][posY-1] != 5 && matriz[posX][posY+1] != 5 ){
        if(direcao == -1 && matriz[posX][posY-1] == 0 ){
            matriz[posX][posY] = 0;
            posY = posY+direcao;
            matriz[posX][posY] = cor;
        } else if(direcao == 1 && matriz[posX][posY+1] == 0){
            matriz[posX][posY] = 0;
            posY = posY+direcao;
            matriz[posX][posY] = cor;
        }
    }

  return posY;
}


void desce_blocos_acima() {
    int i, j;
    for (j = 1; j <= (COLUNAS-2); j++) { // Percorre todas as colunas
        for (i = (LINHAS-2); i >= 1; i--) { // De baixo para cima, exceto a última linha
            if (matriz[i][j] != 0) { // Se houver uma peça
                int k = i;
                while ((k + 1 < LINHAS) && (matriz[k + 1][j] == 0)) {
                    matriz[k + 1][j] = matriz[k][j]; // Move o bloco para baixo
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
    pintando_matriz_VGA();

    usleep(5000);
  
    desce_blocos_acima();
    pintando_matriz_VGA();
}


/*
Função: Remove o agrupamento de blocos a esquerda (quadrado 2x2) da matriz.
Parâmetros: Coordenadas do canto superior esquerdo do agrupamento.
*/
void remove_agrupamento_esquerda(int linha, int coluna) {
    matriz[linha][coluna] = 0;
    matriz[linha + 1][coluna] = 0;
    matriz[linha][coluna - 1] = 0;
    matriz[linha + 1][coluna - 1] = 0;

    printf("Agrupamento removido\n");

    pontos+=4;
    printf("Sua Pontuação Atual é: %d\n", pontos);
    pintando_matriz_VGA();

    usleep(5000);

    desce_blocos_acima();
    pintando_matriz_VGA();
}


/*
Função: Verifica se há um agrupamento de 4 blocos da mesma cor formando um quadrado.
*/
int verifica_agrupamento() {
  int i,j;
  for (i = 0; i < LINHAS - 1; i++) {
      for (j = 0; j < COLUNAS - 1; j++) {
          // Verifica se há um quadrado de 2x2 e se todos os blocos têm a mesma cor
          // Não verifica onde é 0 e 5 pois se refere a cor do tabuleiro e as bordas
          if (matriz[i][j] != 0 && matriz[i][j] != 5 &&
               matriz[i][j] == matriz[i + 1][j] &&
               matriz[i][j] == matriz[i][j + 1] &&
               matriz[i][j] == matriz[i + 1][j + 1])
           {        
               printf("Opa, agrupamento a direita\n");
               remove_agrupamento_direita(i, j);  // Remove o agrupamento a direita do bloco
               return 1;
          }


          if (matriz[i][j] != 0 && matriz[i][j] != 5 &&
              matriz[i][j] == matriz[i + 1][j] &&
              matriz[i][j] == matriz[i][j - 1] &&
              matriz[i][j] == matriz[i + 1][j - 1])
          {     
                printf("Opa, agrupamento a esquerda\n");
              remove_agrupamento_esquerda(i, j);  // Remove o agrupamento a esquerda do bloco
              return 1;
          }
      }
  }




  return 0;
}



void pausar() {
  int chave = 0;
  printf("Jogo pausado. Pressione a chave 1 novamente para continuar.\n");


  do {
      SW_read(&chave); //vai ler a chave
               
  } while (chave != 0b0000000001); //enquanto não for igual a 0b0000000001 0b0000000011


  printf("Jogo retomado!\n");


}


int main() {
   printf("começou!");
  int video_ok = inicializa_video();
  int cor = 0;
  int encerrado = 1;

   configurar_acelerometro();

  if (video_ok == 1) {
       inicializa_matriz_telaInicial();
       pintando_matriz_tela_inicial_VGA();

       //Aciona jogo com chave SW00
       int valor_chaves = 0; //valor das chaves
       int retorno_chaves = 0;

       if(SW_open()){ // tudo certo para abrir
           retorno_chaves = SW_read(&valor_chaves); // Leitura ok, retorna 1

           if(retorno_chaves == 1){
               //fica no loop enquanto a chave 0b0000000001 não for acionada
               printf("aperte a chave para iniciar!\n");
               do {
                   SW_read(&valor_chaves); //vai ler a chave
                                 
               } while (valor_chaves != 0b0000000001); //enquanto não for igual a 0b0000000001
           }
       }
       video_clear();
       video_erase();
       video_show();     
     
       //se sair do loop, inicia matriz
       inicializa_matriz();

      while (encerrado) { // Loop que inicia com a chave

          int inicial_i = 0; // linha inicial na matriz
          int inicial_j = 0; // coluna inicial na matriz

          // Coordenadas iniciais da nova peça
           inicial_i = 1; // linha inicial na matriz
           inicial_j = 10; // coluna inicial na matriz


          int fim = 0; // flag para controlar o fim de movimento de uma peça
          cor = sorteia_cor();
          int verifica = 0;
       
          while (fim == 0) {

                if (inicial_i == 1 && matriz[2][inicial_j] != 0) {
                    printf("GAME OVER.\n");
                    video_clear();
                   video_erase();
                   video_show();
                   sleep(1);
                   encerrado = 0;
                   break;
                }

              matriz[inicial_i][inicial_j] = cor;
              pintando_matriz_VGA();


              usleep(50000); // Intervalo para a peça descer


              //verifica se teve pause com a chave
              SW_read(&valor_chaves);
               if(valor_chaves == 0b0000000011){ //so executa se o valor armazenado nas chaves for igual aos bits correspondentes apenas a SW01
                   pausar();
               }
             
              //verifica se usuario não encerrou o jogo com a chave
               SW_read(&valor_chaves);
               //printf("lendo pra ver se não ta encerrado:\n");
               if(valor_chaves != 0b0000000001 || valor_chaves == 0b0000000011){
                   encerrado = 0;
                   printf("Jogo Encerrado!\n");
                   printf("Sua Pontuação total: %d\n", pontos);

                   video_clear();
                   video_erase();
                   video_show();
                   sleep(1);
                   break;
               }
           
              //Verifica agrupamento de um quadrado
              verifica = verifica_agrupamento();


               // Verifica se pode mover para baixo
              if (verifica != 1 && pode_mover_para_baixo(inicial_i, inicial_j)) {
                  // Limpa a posição anterior e move para baixo
                  matriz[inicial_i][inicial_j] = 0;
                  inicial_i += 1;

                   //verifica o movimento para os lados
                   //aq vai verificar a função que brenda vai fazer, retornando 1, 0 ou -1
                  
                   int direcao = get_direcao_movimento();
                    inicial_j = move_para_lado(inicial_i, inicial_j, direcao, cor);
             
              } else {
                  // A peça atingiu o fundo ou outra peça, fixa ela
                  fim = 1; // Termina o movimento da peça atual
              }
          }
          // Ao sair do loop de uma peça, começa uma nova
      }
       desmapear_memoria();
  }
  return 1;
}
