#!/bin/bash

COUNTER=0
SUCCESS=0
DIFF=""

cd test
pwd
s21_command=(
    "./../s21_grep"
    )

sys_command=(
    "grep"
    )
flags=(
    "v"
    "l"
    "n"
    "i"
    "c"
    "h"
    "s"
    "o"
)

flags2=(
    "c"
    "h"
    "s"
    "o"
)

tests=(
"FLAGS Test test1.txt test2.txt"
"FLAGS ae test1.txt test2.txt"
"FLAGS data test1.txt test2.txt"
)
manual=(
"-ce l test1.txt test2.txt"
"-e o -e test test1.txt test2.txt"
)
simple_test=(
    "-e "came" test1.txt test2.txt"
    "-e "[0-9]" test1.txt test2.txt"
    "-f pattern.txt test1.txt test2.txt"
    "-i "came" test1.txt test2.txt"
    "-v "[0-9]" test1.txt test2.txt"
    "-c "[0-9]" test1.txt test2.txt"
    "-l "[0-9]" test1.txt test2.txt"
    "-n "[0-9]" test1.txt test2.txt"
    "-h "[0-9]" test1.txt test2.txt"
    "-s "[0-9]" test12.txt"
    "-o "me" test1.txt test2.txt"
)

run_test() {
    let "COUNTER++"
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param >> s21_grep.log
    "${sys_command[@]}" $param >> grep.log
    DIFF="$(diff -s s21_grep.log grep.log)"
    echo $COUNTER " ${s21_command[@]} $param" >> all_comm.log
    if [ "$DIFF" != "Files s21_grep.log and grep.log are identical" ]
    then
        echo $(diff -u s21_grep.log grep.log | grep -e '-')
        echo "$COUNTER ${s21_command[@]} $param" >> s21_grep_fail.log
        echo "$COUNTER ${sys_command[@]} $param" >> grep_fail.log
        echo "$COUNTER - Fail $param"
        exit
    else
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
        rm -f grep.log s21_grep.log
    fi
}

rm -f s21_grep.log grep.log s21_grep_fail.log grep_fail.log
for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
# for var1 in "${flags[@]}"
# do
#     for var2 in "${flags[@]}"
#     do
#         for var3 in "${flags[@]}"
#         do
#             for var4 in "${flags[@]}"
#             do
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1$var2$var3$var4"
#                     run_test "$i"
#                 done
#             done
#         done
#     done
# done

# for var1 in "${flags2[@]}"
# do
#     for var2 in "${flags2[@]}"
#     do
#         for var3 in "${flags2[@]}"
#         do
#             for var4 in "${flags2[@]}"
#             do
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1$var2$var3$var4"
#                     run_test "$i"
#                 done
#             done
#         done
#     done
# done

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do 
        for i in "${tests[@]}"
        do
            var="-$var1$var2"
            run_test "$i"
        done
    done
done


for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

for param in "${simple_test[@]}"
do
    eval "${s21_command[@]} $param" > s21_grep.log
    eval "${sys_command[@]} $param" > grep.log

    if diff s21_grep.log grep.log > /dev/null; then
        echo "Test passed: $param"
    else
        echo "Test failed: $param"
        diff s21_grep.log grep.log
    fi
    
    rm s21_grep.log grep.log
done

echo "ALL - ${COUNTER}"
echo "SUCCESS - ${SUCCESS}"
