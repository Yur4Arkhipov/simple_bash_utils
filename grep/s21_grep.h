#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  int flag_e;  // -e -> шаблон
  int flag_i;  // -i -> игнорирует различия регистра
  int flag_v;  // -v -> инвертирует смысл поиска соответствий
  int flag_c;  // -c -> выводит только количество совпадающих строк
  int flag_l;  // -l -> выводит только совпадающие файлы
  int flag_n;  // -n -> предваряет каждую строку вывода номером строки из файла
  int flag_h;  // -h -> выводит совпадающие строки, не предваряя их именами
               // файлов
  int flag_s;  // -s -> подавляет сообщения об ошибках о несуществующих или
               // нечитаемых файлах
  int flag_f;  // -f file -> получает регулярные выражения из файла
  int flag_o;  // -o -> печатает только совпадающие (непустые) части совпавшей
               // строки
} OPTIONS;

void parseOptions(int argc, char** argv, OPTIONS* options, char** pattern);
void s21_grep(int argc, char** argv);
void use_grep_fun(OPTIONS* options, FILE* file, const char* filename,
                  const char* pattern, int* line_number,
                  int* count_of_coincidences, int files_count);
void use_f_option(OPTIONS* options, char** pattern, const char* filename);
void use_e_option(char** pattern, const char* template);
void outputResult(OPTIONS* options, const char* line, const char* filename,
                  int line_number, int files_count, const char* pattern);

#endif