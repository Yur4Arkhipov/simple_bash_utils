#include "s21_grep.h"

int main(int argc, char** argv) {
  s21_grep(argc, argv);
  return 0;
}

void s21_grep(int argc, char** argv) {
  if (argc == 1) {
    fprintf(stderr, "Вы не ввели файлы\n");
  } else if (argc > 1) {
    char* pattern = NULL;
    OPTIONS options = {0};
    parseOptions(argc, argv, &options, &pattern);
    int files_count = argc - optind;
    for (int i = optind; i < argc; i++) {
      int line_number = 0;
      int count_of_coincidences = 0;
      FILE* file = fopen(argv[i], "r");
      if (file != NULL) {
        use_grep_fun(&options, file, argv[i], pattern, &line_number,
                     &count_of_coincidences, files_count);
        fclose(file);
      } else if (!options.flag_s) {
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      }
    }
    if (pattern) free(pattern);
  }
}

void parseOptions(int argc, char** argv, OPTIONS* options, char** pattern) {
  while (1) {
    int opt = getopt(argc, argv, "e:ivclnhsf:o");
    if (opt == -1) break;

    switch (opt) {
      case 'i':
        options->flag_i = 1;
        break;
      case 'v':
        options->flag_v = 1;
        break;
      case 'c':
        options->flag_c = 1;
        break;
      case 'l':
        options->flag_l = 1;
        break;
      case 'n':
        options->flag_n = 1;
        break;
      case 'h':
        options->flag_h = 1;
        break;
      case 's':
        options->flag_s = 1;
        break;
      case 'o':
        options->flag_o = 1;
        break;
      case 'e':
        use_e_option(pattern, optarg);
        options->flag_e = 1;
        break;
      case 'f':
        use_f_option(options, pattern, optarg);
        options->flag_f = 1;
        break;
      case '?':
      default:
        fprintf(stderr, "You can use only [-ivclnhsfe:o:] flags\n");
        exit(EXIT_FAILURE);
    }
  }
  if (!options->flag_e && !options->flag_f) {
    if (optind < argc) {
      *pattern = strdup(argv[optind]);
      optind++;
    } else {
      fprintf(stderr, "Нет паттерна\n");
      exit(EXIT_FAILURE);
    }
  }
}

void use_f_option(OPTIONS* options, char** pattern, const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file != NULL) {
    char line[2100];
    while (fgets(line, sizeof(line), file) != NULL) {
      line[strcspn(line, "\n")] = 0;
      if (*pattern == NULL) {
        *pattern = strdup(line);
      } else {
        *pattern = realloc(*pattern, strlen(*pattern) + strlen(line) + 2);
        strcat(strcat(*pattern, "|"), line);
      }
    }
    fclose(file);
  } else if (!options->flag_s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
  }
}

void use_e_option(char** pattern, const char* template) {
  if (*pattern == NULL) {
    *pattern = strdup(template);
  } else {
    *pattern = realloc(*pattern, strlen(*pattern) + strlen(template) + 2);
    strcat(strcat(*pattern, "|"), template);
  }
}

void use_grep_fun(OPTIONS* options, FILE* file, const char* filename,
                  const char* pattern, int* line_number,
                  int* count_of_coincidences, int files_count) {
  regex_t regex;
  int reti = 1;
  if (pattern != NULL) {
    reti = regcomp(&regex, pattern, options->flag_i ? REG_ICASE : REG_EXTENDED);
  } else {
    fprintf(stderr, "Ошибка компиляции regex\n");
    exit(EXIT_FAILURE);
  }
  if (reti) {
    fprintf(stderr, "Ошибка компиляции regex\n");
    exit(EXIT_FAILURE);
  }
  char line[2100];
  while (fgets(line, sizeof(line), file) != NULL) {
    (*line_number)++;
    line[strcspn(line, "\n")] = 0;
    reti = regexec(&regex, line, 0, NULL, 0);
    int match = (reti == 0) ? 1 : 0;
    if (options->flag_v) match = !match;
    if (match) {
      (*count_of_coincidences)++;
      if (options->flag_c == 0 && options->flag_l == 0) {
        outputResult(options, line, filename, *line_number, files_count,
                     pattern);
      }
    }
  }
  if (options->flag_c && !options->flag_l) {
    if (files_count > 1 && !options->flag_h) printf("%s:", filename);
    printf("%d\n", *count_of_coincidences);
  }
  if (options->flag_l && *count_of_coincidences) {
    printf("%s\n", filename);
  }
  regfree(&regex);
}

void outputResult(OPTIONS* options, const char* line, const char* filename,
                  int line_number, int files_count, const char* pattern) {
  if (options->flag_o) {
    regex_t regex;
    regcomp(&regex, pattern, options->flag_i ? REG_ICASE : REG_EXTENDED);
    regmatch_t match;
    const char* p = line;
    int first_match = 1;
    while (regexec(&regex, p, 1, &match, 0) == 0) {
      if (first_match) {
        first_match = 0;
      }
      if (files_count > 1 && !options->flag_h) printf("%s:", filename);
      if (options->flag_n) printf("%d:", line_number);
      for (int i = match.rm_so; i < match.rm_eo; i++) putchar(p[i]);
      putchar('\n');
      p += match.rm_eo;
      if (*p == '\0') break;
    }
    regfree(&regex);
  } else {
    if (files_count > 1 && !options->flag_h) printf("%s:", filename);
    if (options->flag_n) printf("%d:", line_number);
    printf("%s\n", line);
  }
}