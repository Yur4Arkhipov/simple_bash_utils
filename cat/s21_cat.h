#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int flag_b;  // -b (GNU: --number-nonblank) -> нумерует только непустые строки
  int flag_e;  // -e предполагает и -v (GNU only: -E то же самое, но без
               // применения -v) -> также отображает символы конца строки как $
  int flag_n;  // -n (GNU: --number) -> нумерует все выходные строки
  int flag_s;  // -s (GNU: --squeeze-blank) -> сжимает несколько смежных пустых
               // строк
  int flag_t;  // -t предполагает и -v (GNU: -T то же самое, но без применения
               // -v) -> также отображает табы как ^I
  int flag_v;
} OPTIONS;

void parseOptions(int argc, char **argv, OPTIONS *options);
void s21_cat(int argc, char **argv);
void concatenate_files(OPTIONS *options, FILE *file);