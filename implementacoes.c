#include "implementacoes.h"
#include "mandelbrot.h"
#include <omp.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


void mandelbrot_serial(unsigned char *img, int largura, int altura, int max_iter, int num_threads) {
    (void)num_threads;   /* a serial nao usa threads */
 
    long total = (long)largura * altura;   /* tamanho da fila de pixels */
 
    for (long i = 0; i < total; i++) {
        int px = (int)(i % largura);
        int py = (int)(i / largura);
        img[i] = mandelbrot_pixel(px, py, largura, altura, max_iter);
    }
}
 


/*
 * OPENMP - o mesmo trabalho da serial, mas dividido automaticamente.  
 *
 * A diferenca em relacao a serial -> o #pragma. Ele diz ao
 * compilador "divida as repeticoes deste for entre varias threads".
 * A biblioteca cria as threads, reparte as linhas e junta tudo no fim.
*/

void mandelbrot_openmp(unsigned char *img, int largura, int altura, int max_iter, int num_threads) {
    omp_set_num_threads(num_threads);
 
    #pragma omp parallel for schedule(dynamic)
    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            img[(long)py * largura + px] =
                mandelbrot_pixel(px, py, largura, altura, max_iter);
        }
    }
}



//Cada thread precisa saber ONDE trabalhar: cabe bem  em uma struct.

typedef struct {
    unsigned char *img;
    int largura, altura, max_iter;
    int linha_ini;   // primeira linha do bloco (inclusive) */
    int linha_fim;   /* fim do bloco (exclusive)            */
    int criada;      // 1 = thread criada; 0 = rodou em serie //
} TarefaBloco;


static void *trabalhador_bloco(void *arg) {
    TarefaBloco *t = (TarefaBloco *)arg;
    for (int py = t->linha_ini; py < t->linha_fim; py++) {
        for (int px = 0; px < t->largura; px++) {
            t->img[(long)py * t->largura + px] =
                mandelbrot_pixel(px, py, t->largura, t->altura, t->max_iter);
        }
    }
    return NULL;
}



void mandelbrot_pthreads1(unsigned char *img, int largura, int altura,
                          int max_iter, int num_threads) {
    /* nao faz sentido ter mais threads do que linhas */
    if (num_threads > altura) num_threads = altura;
    if (num_threads < 1)      num_threads = 1;
 
    pthread_t   *threads = malloc((size_t)num_threads * sizeof(pthread_t));
    TarefaBloco *tarefas = malloc((size_t)num_threads * sizeof(TarefaBloco));
 
    /* se faltar memoria pro controle das threads, faz tudo em serie */
    if (threads == NULL || tarefas == NULL) {
        TarefaBloco t = { img, largura, altura, max_iter, 0, altura, 0 };
        trabalhador_bloco(&t);
        free(threads);
        free(tarefas);
        return;
    }
}



    int base  = altura / num_threads;
    int resto = altura % num_threads;
    int linha = 0;
 
    for (int i = 0; i < num_threads; i++) {
        int qtd = base + (i < resto ? 1 : 0);
        tarefas[i].img       = img;
        tarefas[i].largura   = largura;
        tarefas[i].altura    = altura;
        tarefas[i].max_iter  = max_iter;
        tarefas[i].linha_ini = linha;
        tarefas[i].linha_fim = linha + qtd;
        linha += qtd;
 
        if (pthread_create(&threads[i], NULL, trabalhador_bloco, &tarefas[i]) == 0) {
            tarefas[i].criada = 1;
        } else {
            /* se a criacao falhar, faz o bloco aqui mesmo (o programa nao trava) */
            fprintf(stderr, "aviso: falha ao criar thread %d; rodando o bloco em serie\n", i);
            tarefas[i].criada = 0;
            trabalhador_bloco(&tarefas[i]);
        }
    }
 
    /* espera todas as threads criadas terminarem */
    for (int i = 0; i < num_threads; i++) {
        if (tarefas[i].criada) {
            pthread_join(threads[i], NULL);
        }
    }
 
    free(threads);
    free(tarefas);
}
