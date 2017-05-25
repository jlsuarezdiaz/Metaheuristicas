#ifndef APC_SA_H
#define APC_SA_H

#include "APCAlgorithm.h"

class APCSimulatedAnnealing: public APCAlgorithm{
private:
    /**
     * Temperature.
     */
    float temp;

     /**
      * Initial temperature.
      */
    float T_0;

    /**
     * Final temperature.
     */
    float T_f;

    /**
     * Best solution.
     */
    APCSolution * best_solution;

    /**
     * Best cost.
     */
    float best_cost;

    /**
     * Number of annealings.
     */
    int num_annealings;

    /**
     * Cauchy Constant.
     */
    float beta;

    /**
     * Initializes algorithm.
     */
    void initialize(const APCPartition &p, targetFunction f, int max_neighbours, int max_success, int max_evals, float phi, float mu, float Tf, APCSolution *& initial);

    /**
     * Cooling function.
     */
    void coolingScheme();

    /**
     * Boltzmann constant.
     */
    static const float K;

public:
    APCSimulatedAnnealing(const APCProblem * problem);

    /**
     * Generates a weights vector for the specified partition.
     * @param p Partition
     * @pos The solution obtained will be returned as the last element of getSolutions()
     * @pos The elapsed time will be returned as the last element of getTimes()
     * @pos No fitness is calculated. The stored fitness in getFitnesses() will be -1.0
     * @return Solution obtained.
     */
    APCSolution * solve(const APCPartition & p, targetFunction f, int max_neighbours, int max_success, int max_evals = 15000, float phi = 0.3, float mu = 0.3, float Tf = 1e-3, APCSolution *s0 = NULL );

    /**
     * Generates solutions for a 5x2 partition.
     * @param partition 5x2 partition to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, int max_neighbours, int max_success, int max_evals = 15000, float phi = 0.3, float mu = 0.3, float Tf = 1e-3, APCSolution *s0 = NULL );

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
    void solve5Fold(const APC5FoldPartition & partition, int max_neighbours, int max_success, int max_evals = 15000, float phi = 0.3, float mu = 0.3, float Tf = 1e-3, APCSolution *s0 = NULL );

};


#endif