#!/bin/bash

SOLS_PATH=$1

PROBLEM_ADDRESS=./data/
PROBLEM_EXT=.arff
PROBLEMS=(sonar  wdbc spambase-460)

ALGORITHMS=(1NN RANDOM RELIEF RELIEF+LS RANDOM+LS AGG-BLX AGG-CA AGE-BLX AGE-CA AM-10-1.0 AM-10-0.1 AM-10-0.1mej)
SEEDS=(3141592 77148642)

echo "CALCULO DE PROMEDIOS: $(date)"

for seed in ${SEEDS[@]}
do
    AVG_FILE=$SOLS_PATH/means_$seed.table
    LINE="SEED-$seed\t"

    echo "" > $AVG_FILE

    for prob in ${PROBLEMS[@]}
    do
        LINE="$LINE$prob\t$prob\t$prob\t"
    done
    echo -e $LINE  >> $AVG_FILE

    LINE="ALGORITHMS"
    for prob in ${PROBLEMS[@]}
    do
        LINE="$LINE\tFITNESS\tTRAIN\t\tTIME"
    done
    echo -e $LINE >> $AVG_FILE

    for alg in ${ALGORITHMS[@]}
    do
        LINE="$alg\t\t\t\t"
        for prob in ${PROBLEMS[@]}
        do
            MEANS=$(tail -2 $SOLS_PATH/$prob\_$alg\_$seed.table | head -1 | cut -f 4-)
            LINE=$LINE$MEANS
            
        done
        echo -e $LINE >> $AVG_FILE
    done

    column -t $AVG_FILE | tee $AVG_FILE
done


echo "FIN DE CALCULO DE PROMEDIOS: $(date)"
