#ifndef APC_LS_H
#define APC_LS_H

#include "SRandom.h"
#include "APCProblem.h"
#include "APCSolution.h"
#include "APCPartition.h"
#include "APC5x2Partition.h"
#include "APC1NN.h"
#include "Timer.h"

class APCLocalSearch{
    const APCProblem * problem;

    /**
     * Vector with obtained solutions.
     */
    vector<APCSolution *> solutions;

    /**
     * Vector with obtained fitnesses.
     */
    vector<float> fitnesses;

    /**
     * Vector with obtained fitnesses.
     */
    vector<float> train_fits;

    /**
     * Vector with obtained times.
     */
    vector<double> times;

    /**
     * Class Timer.
     */
    Timer timer;

    const string algorithm_name;




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
     * @pre There must exist solutions[s_index], fitnesses[s_index] and times[s_index]
     * @pos The solution obtained will be returned as the element s_index of getSolutions()
     * @pos The elapsed time will be returned as the element s_index of getTimes()
     * @pos The fitness will be calculated over p_test and will be returned as the element s_index of getFitnessed()
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
     */
    void solve5x2(const APC5x2Partition & partition, vector<APCSolution*> & solutions, int max_neighbours = 20, int max_evaluations = 15000, float sigma = 0.3);

    inline vector <APCSolution *> getSolutions(){
        return solutions;
    }

    inline vector <double> getTimes(){
        return times;
    }
    inline vector <float> getFitnesses(){
        return fitnesses;
    }

    inline const vector <float> & getTrainFits(){
        return train_fits;
    }

    inline const string & getAlgorithmName(){
        return algorithm_name;
    }

    void clearSolutions();
    
    ~APCLocalSearch();


};

#endif