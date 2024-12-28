#include "s21_cat.h"

int main(int argc, char **argv) {
  s21_cat(argc, argv);

  return 0;
}

void s21_cat(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "You are not entering files.\n");
  } else if (argc > 1) {
    OPTIONS options = {0};
    parseOptions(argc, argv, &options);
    int countFiles = argc - optind;
    for (int i = argc - countFiles; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if (file != NULL) {
        concatenate_files(&options, file);
        fclose(file);
      } else {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      }
    }
  }
}

void parseOptions(int argc, char **argv, OPTIONS *options) {
  static struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  int option_index = 0;
  while (1) {
    int opt = getopt_long(argc, argv, "benstvTE", long_options, &option_index);
    if (opt == -1) break;

    switch (opt) {
      case 'b':
        options->flag_b = 1;
        options->flag_n = 0;
        break;
      case 'E':
        options->flag_e = 1;
        options->flag_v = 0;
        break;
      case 'T':
        options->flag_t = 1;
        options->flag_v = 0;
        break;
      case 'n':
        options->flag_n = 1;
        if (options->flag_b) options->flag_n = 0;
        break;
      case 's':
        options->flag_s = 1;
        break;
      case 't':
        options->flag_t = 1;
        options->flag_v = 1;
        break;
      case 'e':
        options->flag_e = 1;
        options->flag_v = 1;
        break;
      case 'v':
        options->flag_v = 1;
        break;
      case '?':
      default:
        fprintf(stderr, "You can use only [-benstv] flags\n");
        exit(EXIT_FAILURE);
    }
  }
}

void concatenate_files(OPTIONS *options, FILE *file) {
  char ch;
  int count_endl = 0;
  static int line_number = 1;
  int is_new_line = 1;  // 0 - false, 1 - true
  while ((ch = fgetc(file)) != EOF) {
    if (ch != '\n' && is_new_line == 1 &&
        (options->flag_n || options->flag_b)) {
      printf("%6d\t", line_number++);
    }
    if ((ch == '\n') && is_new_line == 1 && options->flag_n) {
      printf("%6d\t", line_number++);
      is_new_line = 0;
    }
    if ((options->flag_v && (((ch) >= 0 && (ch) <= 8) ||
                             ((ch) >= 11 && (ch) <= 31) || ((ch) >= 127))) ||
        (ch == '\t' && options->flag_t)) {
      is_new_line = 0;
      count_endl = 0;
      printf("^%c", ch >= 32 ? ch - '@' : ch + '@');
    } else if (ch == '\n') {
      if (count_endl < 2) {
        is_new_line = 1;
        if (options->flag_e) {
          printf("$\n");
        } else {
          printf("\n");
        }
      }
      if (options->flag_s) {
        count_endl++;
      }
    } else {
      is_new_line = 0;
      count_endl = 0;
      printf("%c", ch);
    }
  }
}