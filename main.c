
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "implementacoes.h"
#include "imagem.h"
 

#define LOGIN "jlogb"   // <---Substituir por um id 
 
/*
 * "Atalho para uma funcao de calculo": as quatro implementacoes tem a
 * mesma assinatura, entao um mesmo ponteiro consegue apontar para qualquer
 * uma delas. 
 */
typedef void (*FuncCalculo)(unsigned char *, int, int, int, int);
 
/*
 * Relogio de PAREDE (tempo real), nao tempo de CPU.
 * Isso importa: quando formos medir o ganho das threads, so o tempo real
 * mostra a aceleracao (o tempo de CPU somaria o trabalho de todas as threads).
 */
static double agora_segundos(void) {
    struct timespec t;
    timespec_get(&t, TIME_UTC);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}
 
/*
 * Le um inteiro estritamente positivo vindo do argv.
 * Devolve 1 e guarda em *destino se for valido; 0 caso contrario.
 * (strtol pega sujeira que o atoi deixaria passar
 *
 */
static int ler_positivo(const char *texto, int *destino) {
    char *fim;
    long v = strtol(texto, &fim, 10);
    if (fim == texto || *fim != '\0' || v <= 0 || v > 100000) {
        return 0;
    }
    *destino = (int)v;
    return 1;
}
 
/*
 * Roda UMA implementacao de ponta a ponta:
 *   cronometra o calculo, grava o .pgm dela e anota o tempo no times.txt.
 * Devolve 1 em sucesso, 0 se a gravacao do arquivo falhar.
 */
static int rodar(const char *sufixo,const char *rotulo, FuncCalculo calcular, //<<-- conteúdo interno sem dependencias externas(lembrar)
                 unsigned char *img, int W, int H, int mi, int nt,
                 FILE *times) {
    char caminho[128];
    snprintf(caminho, sizeof caminho, "mandelbrot_%s_%s.pgm", LOGIN, sufixo);
 
    double t0 = agora_segundos();
    calcular(img, W, H, mi, nt);          /* <-- o calculo que estou medindo */
    double dt = agora_segundos() - t0;
 
    if (gravar_pgm(caminho, img, W, H) != 0) {
        fprintf(stderr, "erro: nao consegui criar o arquivo %s\n", caminho);
        return 0;
    }
    fprintf(times, "%s: %.6fs\n", rotulo, dt);
    return 1;
}


 
int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr,
                "uso: %s largura altura max_iteracoes num_threads\n", argv[0]);
        return 1;
    }
 
    int largura, altura, max_iter, num_threads;
    if (!ler_positivo(argv[1], &largura)     ||
        !ler_positivo(argv[2], &altura)      ||
        !ler_positivo(argv[3], &max_iter)    ||
        !ler_positivo(argv[4], &num_threads)) {
        fprintf(stderr,
                "erro: largura, altura, max_iteracoes e num_threads devem ser "
                "inteiros positivos\n");
        return 1;
    }
 
    unsigned char *img = criar_matriz(largura, altura);
    if (img == NULL) {
        fprintf(stderr, "erro: falha ao alocar memoria para a imagem\n");
        return 1;
    }
 
    FILE *times = fopen("times.txt", "w");
    if (times == NULL) {
        fprintf(stderr, "erro: nao consegui criar o times.txt\n");
        liberar_matriz(img);
        return 1;
    }
 
    rodar("serial",    "Serial",    mandelbrot_serial,    img, largura, altura, max_iter, num_threads, times);
    rodar("openmp",    "OpenMP",    mandelbrot_openmp,    img, largura, altura, max_iter, num_threads, times);
    rodar("pthreads1", "Pthreads1", mandelbrot_pthreads1, img, largura, altura, max_iter, num_threads, times);
    rodar("pthreads2", "Pthreads2", mandelbrot_pthreads2, img, largura, altura, max_iter, num_threads, times);
 
    fclose(times);
    liberar_matriz(img);
    return 0;
}

