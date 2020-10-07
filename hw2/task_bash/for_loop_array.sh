#!/bin/bash
my_array=(el0 el1 el2 el3 el4 el5 el6 el7 el8 el9)
for el in ${my_array[@]}
do
    echo $el
done
