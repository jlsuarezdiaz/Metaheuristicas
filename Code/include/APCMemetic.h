#ifndef APC_MEMETIC_H
#define APC_MEMETIC_H

#include "APCLocalSearch.h"
#include "APCGenetic.h"


using namespace std;

class APCMemetic: public APCAlgorithm{
private:

    /**
     * Internal algorithms.
     */
    APCGeneticGenerational agg;
    APCLocalSearch ls;

    /**
     * Algorithm model (10-1.0, 10-0.1, 10-0.1mej)
     */
    string algorithm_model;

    

    /**
     * Generates a weights vector for the specified partition.
     * @param p_train Partition where weights will be learnt.
     * @param ls_gens Period (in generations) of local search application.
     * @param ls_prob Rate of local search application for the population.
     * @param mej If TRUE, ls_prob will be used to select the 100*ls_prob % best individuals to perform LS.
     * Else, ls_prob will be used to select randomly a 100*ñs_prob % of the population.
     * @param cross Cross operator.
     * @param population_size.
     * @param cross_prob
     * @param mutation_prob
     * @param max_evaluations Max number of evaluations to make stop the algorithm.
     * @param ls_neighbour_evals_rate LS evaluation rate. 2*ls_neighbours_eval_rate neighbours will be generated in each LS perform.
     */
    APCSolution * solve(const APCPartition & p_train, int ls_gens, float ls_prob , bool mej,crossOperator c = APCGenetic::BLXCross03, int population_size = 10, float cross_prob = 0.7, float mutation_prob = 0.001, int max_evaluations = 15000, int ls_neighbour_evals_rate = 2);


public:

    APCMemetic(const APCProblem * problem);

    /**
     * Generates solutions for a 5x2 partition.
     * @param Partition 5x2 partition to evaluate.
     * @param cross Cross operator.
     * @param max_evaluations Max number of evaluations to make stop the algorithm.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, int ls_gens, float ls_prob , bool mej, crossOperator c = APCGenetic::BLXCross03, int population_size = 10, float cross_prob = 0.7, float mutation_prob = 0.001, int max_evaluations = 15000, int ls_neighbour_evals_rate = 2);

    /**
     * @brief Gets algorithm name.
     */
    inline string getAlgorithmName(){
        return algorithm_name+"-"+algorithm_model;
    }

    /**
     * Destructor.
     */
    ~APCMemetic();

};

#endif