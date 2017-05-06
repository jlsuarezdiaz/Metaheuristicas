#ifndef APC_RANDOM_H
#define APC_RANDOM_H

#include "APCAlgorithm.h"

class APCRandom: public APCAlgorithm{
private:


public:

    APCRandom(const APCProblem *p);

    /**
     * Generates a weights vector for the specified partition.
     * @param p Partition
     * @pos The solution obtained will be returned as the last element of getSolutions()
     * @pos The elapsed time will be returned as the last element of getTimes()
     * @pos No fitness is calculated. The stored fitness in getFitnesses() will be -1.0
     * @return Solution obtained.
     */
    APCSolution * solve(const APCPartition & p, targetFunction f);

    /**
     * Generates solutions for a 5x2 partition.
     * @param partition 5x2 partition to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition);

    /**
     * Generates solutions for a 5 fold partition.
     * @param partition 5Fold partition to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector with the training fitnesses.
     * @pos getClassRates() will return a 10 elements vector with the classification rates.
     * @pos getRedRates() will return a 10 elements vector with the reduction rates.
     */
    void solve5Fold(const APC5FoldPartition & partition);


    ~APCRandom();

};

#endif