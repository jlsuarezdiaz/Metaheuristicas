#ifndef APC_LS_H
#define APC_LS_H

#include "APCAlgorithm.h"

class APCLocalSearch: public APCAlgorithm{
private:
    /**
     *  Order of indexes where local search mutations will be applied.
     */
    int * permutation;

    


public:

    APCLocalSearch(const APCProblem *p);

    /**
     * Generates a weights vector for the specified partition.
     * @param p_train Partition where weights will be learnt.
     * @param p_test Partition where weights will be tested.
     * @param s_index Index of the solution in getSolutions()
     * @param sigma Standard deviation.
     * @param max_neighbours Stop criterion: if there is not improvement after max_neighboursxnum_attributes neighbours
     * @param max_evaluations Max number of evaluations to make stop the algorithm.
     * @pos The solution obtained will be returned as the last element of getSolutions()
     * @pos The elapsed time will be returned as the last element of getTimes()
     * @pos The fitness will be returned as the last element of getFitnesses()
     * @pos The train fitness will be returned as the last element of getTrainFits()
     * @return Solution obtained.
     */
    APCSolution * solve(const APCPartition & p_train, APCSolution *sol, int max_neighbours = 20, int max_evaluations = 15000, float sigma = 0.3);

    /**
     * Generates solutions for a 5x2 partition.
     * @param Partition 5x2 partition to evaluate.
     * @param solution Solutions of each partition to locally search. It must have 10 elements.
     * @param fitnesses Initial fitnesses
     * @param sigma Standard deviation.
     * @param max_neighbours Stop criterion: if there is not improvement after max_neighboursxnum_attributes neighbours
     * @param max_evaluations Max number of evaluations to make stop the algorithm.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, vector<APCSolution*> & solutions, int max_neighbours = 20, int max_evaluations = 15000, float sigma = 0.3);

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
    void solve5Fold(const APC5FoldPartition & partition, vector<APCSolution*> & solutions, int max_neighbours = 20, int max_evaluations = 15000, float sigma = 0.3);
    
    /**
     * Erases stored solutions.
     */
    void clearSolutions();
    
    /**
     * Destructor.
     */
    ~APCLocalSearch();


};

#endif