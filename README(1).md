# Conjunto de Mandelbrot

Trabalho de Infraestrutura de Software (CESAR School) — Implementação 2: Threads.

Gera o conjunto de Mandelbrot em C com quatro abordagens (serial, OpenMP e duas com pthreads) e mede o tempo de cada uma. A saída é uma matriz de intensidades, não uma imagem de verdade.

Troque `<login>` pelas iniciais do e-mail @cesar.school antes de entregar.

## O que faz

Para cada ponto `c` do plano, começa com `z = 0` e repete `z = z² + c` até `|z|` passar de 2 (escapou, fora do conjunto) ou bater o máximo de iterações (dentro). O número de iterações vira a intensidade do pixel, de 0 a 255.

Região: real `[-2,0; 1,0]`, imaginário `[-1,5; 1,5]`.

## Implementações

Todas geram a mesma matriz. Muda só como o trabalho é dividido.

- `serial`: referência, sem paralelismo.
- `openmp`: `#pragma omp parallel for` no laço dos pixels.
- `pthreads1`: divisão estática, blocos contíguos de linhas por thread.
- `pthreads2`: divisão dinâmica, fila de linhas com mutex.

Saída: `mandelbrot_<login>_serial.pgm`, `_openmp`, `_pthreads1`, `_pthreads2`, mais `times.txt` com o tempo de cada implementação.

## Requisitos

GCC, Make, pthreads, OpenMP. Linux, Unix ou macOS.

## Compilar

```
make          # gera ./mandelbrot
make clean    # remove binário e arquivos gerados
```

## Rodar

```
./mandelbrot <largura> <altura> <max_iteracoes> <num_threads>
```

Exemplo: `./mandelbrot 1200 800 100 4`

`largura` e `altura` são as dimensões da imagem; `max_iteracoes` é o limite por ponto; `num_threads` vale para as versões paralelas. Em execução normal não imprime nada no stdout.

## Formato da saída

Cada `.pgm` é a matriz de intensidades (0–255): valores separados por espaço, uma linha do arquivo por linha da imagem, sem cabeçalho. Os quatro arquivos são idênticos.

## Erros

Valida os argumentos antes de rodar e encerra com mensagem quando: número errado de argumentos; `largura`, `altura`, `max_iteracoes` ou `num_threads` inválidos; falha ao criar arquivo, alocar memória ou criar thread.

## Estrutura

```
README.md
.gitignore
Makefile
mandelbrot.c
```

## Autor

[Nome] — `<login>@cesar.school`
