#!/bin/sh
# 
# args: -o -n 10
#
set -e

echo -en "" > test_1.txt
for i in {0..9}; do echo $i >> test_1.txt; done
for i in {a..z}; do echo $i >> test_1.txt; done

cat > solution_1.txt << "EOF"
@@ 13,26 @@
d
e
f
g
h
i
j
k
l
m
EOF

diff <(../mid -o -n 10 test_1.txt) solution_1.txt > /dev/null
exit $?
