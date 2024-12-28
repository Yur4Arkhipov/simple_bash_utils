#!/bin/bash

./s21_cat -T -b -E files/album.txt files/lesson.txt > test_s21_cat.log 
cat cat -T -b -E files/album.txt files/lesson.txt > test_cat.log
if cmp -s test_s21_cat.log test_cat.log
then
(( SUCCESS++ ))
(( COUNTER++ ))

echo "Test  №$COUNTER SUCCESS $var";
else
(( FAIL++ ))
(( COUNTER++ ))

echo "Test  №$COUNTER FAIL $var"
fi
# rm test_s21_cat.log test_cat.log
