#!/bin/bash

PROGRAM=./bin/apc
SOLS_PATH=$1

PROBLEM_ADDRESS=./data/
PROBLEM_EXT=.arff

PROBLEMS=(sonar  wdbc spambase-460) # Todos los problemas
#PROBLEMS=(sonar) # Solo sonar

# Todos los algoritmos
#ALGORITHMS=(1NN RANDOM RELIEF RELIEF+LS RANDOM+LS AGG-BLX AGG-CA AGE-BLX AGE-CA AM-10-1.0 AM-10-0.1 AM-10-0.1mej SA ILS DE-RAND DE-CURRENTTOBEST)
# Todos los algoritmos P1
#ALGORITHMS=(1NN RANDOM RELIEF RELIEF+LS RANDOM+LS AGG-BLX AGG-CA AGE-BLX AGE-CA AM-10-1.0 AM-10-0.1 AM-10-0.1mej)
# Todos los algoritmos P2
ALGORITHMS=(1NN RELIEF RANDOM+LS AGG-BLX AM-10-0.1mej SA ILS DE-RAND DE-CURRENTTOBEST) 
# Algoritmos nuevos P2
#ALGORITHMS=(SA ILS DE-RAND DE-CURRENTTOBEST)

SEEDS=(3141592) 
#SEEDS=(3141592 77148642)

# Other program options (Independent of seed, problem and algorithm)
OTHER_OPTIONS="-t fcrti -p sp" # 5-FOLD
#OTHER_OPTIONS="-t fit -p sp"   #5x2

echo "COMIENZO DE LAS EJECUCIONES: $(date)"

for seed in ${SEEDS[@]}
do
    for alg in ${ALGORITHMS[@]}
    do
        for prob in ${PROBLEMS[@]}
        do
            PROBLEM_PATH=$PROBLEM_ADDRESS$prob$PROBLEM_EXT
            PROB_SOL_PATH=$SOLS_PATH/$prob\_$alg\_$seed
            echo -e "\n\nEJECUCIÓN DEL ALGORITMO $alg SOBRE EL PROBLEMA $prob (SEMILLA $seed):\n"
            CMD="$PROGRAM $PROBLEM_PATH -a $alg -s $seed -o $PROB_SOL_PATH $OTHER_OPTIONS"
            echo $CMD
            eval $CMD
            echo -e "\nFIN DE LA EJECUCIÓN DE $alg SOBRE EL PROBLEMA $prob:\n\n"
        done
    done
done


echo "FIN DE LAS EJECUCIONES: $(date)"