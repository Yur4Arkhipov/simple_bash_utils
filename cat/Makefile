CC=gcc
CC_FLAGS=-c -std=c11 -Wall -Werror -Wextra
BUILD_NAME=s21_cat

all: build

build: s21_cat

rebuild: clean build

s21_cat: s21_cat.o
	${CC} ${BUILD_NAME}.o -o ${BUILD_NAME}

s21_cat.o: s21_cat.c
	${CC} ${BUILD_NAME}.c ${CC_FLAGS} -o ${BUILD_NAME}.o

clean:
	rm -rf ./*.o

.PHONY: test

test: test/s21_cat_test.sh
	bash test/s21_cat_test.sh

	