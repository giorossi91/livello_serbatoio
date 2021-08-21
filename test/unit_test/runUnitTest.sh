#!/bin/bash

cd $1

rm -f *.gcda
rm -f ../test_result/*

./unit_test

gcovr -s -r $2 --object-directory $1 --html --html-title "Livello Serbatoio Coverage Report" --html-details -o ../test_result/coverage_report.html
