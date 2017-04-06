#!/bin/bash

PROGRAM=./bin/apc.exe

PROBLEM_ADDRESS=./data/
PROBLEM_EXT=.arff
PROBLEMS=(sonar wdbc spambase-460)

ALGORITHMS=(1NN RANDOM RELIEF RELIEF+LS RANDOM+LS AGG-BLX AGG-CA AGE-BLX AGE-CA AM-10-1.0 AM-10-0.1 AM-10-0.1mej)
SEEDS=(28)

echo "COMIENZO DE LAS EJECUCIONES: $(date)"

for seed in ${SEEDS[@]}
do
    for alg in ${ALGORITHMS[@]}
    do
        for prob in ${PROBLEMS[@]}
        do
            PROBLEM_PATH=$PROBLEM_ADDRESS$prob$PROBLEM_EXT
            echo -e "\n\nEJECUCIÓN DEL ALGORITMO $alg SOBRE EL PROBLEMA $prob (SEMILLA $seed):\n"
            CMD="$PROGRAM $PROBLEM_PATH -a $alg -s $seed"
            echo $CMD
            eval $CMD
            echo -e "\nFIN DE LA EJECUCIÓN DE $alg SOBRE EL PROBLEMA $prob:\n\n"
        done
    done
done


echo "FIN DE LAS EJECUCIONES: $(date)"