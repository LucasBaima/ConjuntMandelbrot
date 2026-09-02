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



//pthreads2 -> 


/** Aqui a ideia muda: existe UM contador compartilhado - a "proxima linha
 * a vim". Cada thread, quando fica livre, PEGA a proxima linha da fila,
 * processa, e volta pra pegar mais. Assim quem termina rapido pega mais
 * trabalho e ninguem fica parado - a carga se equilibra sozinha.*/

 typedef struct {
    unsigned char *img;
    int largura, altura, max_iter;
    int proxima_linha;          // contador compartilhado: proxima linha a fazer //
    pthread_mutex_t trava;      // protege o acesso a proxima_linha //
} FilaLinhas;


static void *trabalhador_fila(void *arg) {
    FilaLinhas *f = (FilaLinhas *)arg;
 
    for (;;) {
        /* --- regiao critica: pega e reserva a proxima linha --- */
        pthread_mutex_lock(&f->trava);
        int py = f->proxima_linha;
        f->proxima_linha++;
        pthread_mutex_unlock(&f->trava);
        /* --- fim da regiao critica --- */
 
        if (py >= f->altura) {
            break;              /* fila vazia -> essa thread termina */
        }
 
        /* o trabalho pesado fica FORA da trava (roda em paralelo) */
        for (int px = 0; px < f->largura; px++) {
            f->img[(long)py * f->largura + px] =
                mandelbrot_pixel(px, py, f->largura, f->altura, f->max_iter);
        }
    }
    return NULL;
}


void mandelbrot_pthreads2(unsigned char *img, int largura, int altura,
                          int max_iter, int num_threads) {
    if (num_threads > altura) num_threads = altura;
    if (num_threads < 1)      num_threads = 1;
 
    /* a fila compartilhada, com o contador zerado e a trava pronta */
    FilaLinhas fila;
    fila.img           = img;
    fila.largura       = largura;
    fila.altura        = altura;
    fila.max_iter      = max_iter;
    fila.proxima_linha = 0;
    pthread_mutex_init(&fila.trava, NULL);
 


    pthread_t *threads = malloc((size_t)num_threads * sizeof(pthread_t));
    if (threads == NULL) {
        /* sem memoria pras threads: esvazia a fila em serie e sai */
        trabalhador_fila(&fila);
        pthread_mutex_destroy(&fila.trava);
        return;
    }
 
    /* todas as threads compartilham a MESMA fila (passamos &fila pra todas) */
    int criadas = 0;
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, trabalhador_fila, &fila) == 0) {
            criadas++;
        } else {
            fprintf(stderr, "aviso: falha na criação da thread %d; seguindo com %d\n", i, criadas);
            break;   /* as threads ja criadas dao conta de esvaziar a fila */
        }
    }
 
    /* se nenhuma foi criada, a propria main esvazia a fila */
    if (criadas == 0) {
        trabalhador_fila(&fila);
    }
 
    for (int i = 0; i < criadas; i++) {
        pthread_join(threads[i], NULL);
    }
 
    pthread_mutex_destroy(&fila.trava);
    free(threads);
}
