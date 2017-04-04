#!/bin/bash

PROGRAM=./bin/apc.exe

PROBLEM_ADDRESS=./data/
PROBLEM_EXT=.arff
PROBLEMS=(sonar wdbc spambase-460)

ALGORITHMS=(RANDOM RELIEF RELIEF+LS RANDOM+LS AGG-BLX AGG-CA AGE-BLX AGE-CA AM-10-1.0 AM-10-0.1 AM-10-0.1mej)


echo "COMIENZO DE LAS EJECUCIONES: $(date)"

for alg in ${ALGORITHMS[@]}
do
    for prob in ${PROBLEMS[@]}
    do
        PROBLEM_PATH=$PROBLEM_ADDRESS$prob$PROBLEM_EXT
        echo $prob
        echo -e "\n\nEJECUCIÓN DEL ALGORITMO $alg SOBRE EL PROBLEMA $prob:\n"
        $PROGRAM $PROBLEM_PATH -a $alg -s 28
        echo -e "\nFIN DE LA EJECUCIÓN DE $alg SOBRE EL PROBLEMA $prob:\n\n"
    done
done


echo "FIN DE LAS EJECUCIONES: $(date)"