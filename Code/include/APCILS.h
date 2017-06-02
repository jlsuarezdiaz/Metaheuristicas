#ifndef APC_ILS_H
#define APC_ILS_H

#include "APCAlgorithm.h"
#include "APCLocalSearch.h"

class APCILS: public APCAlgorithm{
private:
    APCLocalSearch ls;

    int * permutation;

public:
    APCILS(const APCProblem * problem);

    /**
     * Generates a weights vector for the specified partition.
     * @param p Partition
     * @pos The solution obtained will be returned as the last element of getSolutions()
     * @pos The elapsed time will be returned as the last element of getTimes()
     * @pos No fitness is calculated. The stored fitness in getFitnesses() will be -1.0
     * @return Solution obtained.
     */
    APCSolution * solve(const APCPartition & p, targetFunction f, int num_mutations, float sigma_mutation = 0.4, int max_evals_ls = 1000, float sigma_ls = 0.3, int num_its = 15, APCSolution *s0 = NULL );

    /**
     * Generates solutions for a 5x2 partition.
     * @param partition 5x2 partition to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, int num_mutations, float sigma_mutation = 0.4, int max_evals_ls = 1000, float sigma_ls = 0.3, int num_its = 15, APCSolution *s0 = NULL );

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
    void solve5Fold(const APC5FoldPartition & partition, int num_mutations, float sigma_mutation = 0.4, int max_evals_ls = 1000, float sigma_ls = 0.3, int num_its = 15, APCSolution *s0 = NULL );

    inline ~APCILS(){
        delete [] permutation;
    }
};


#endif