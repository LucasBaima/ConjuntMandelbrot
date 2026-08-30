#ifndef IMPLEMENTACOES_H
#define IMPLEMENTACOES_H
/*
 * As QUATRO estrategias que preenchem a matriz inteira de pixels.
 * Todas usam o nucleo de mandelbrot.h e produzem a mesma matriz.
 *
 * Aqui vao entrar as declaracoes de:
 *   - serial     : sem paralelismo (referencia)
 *   - openmp     : #pragma omp parallel for no laco dos pixels
 *   - pthreads1  : divisao estatica (blocos contiguos de linhas)
 *   - pthreads2  : divisao dinamica (fila de linhas com mutex)
 */
 
#endif /* IMPLEMENTACOES_H */
