#ifndef APC_GENETIC_H
#define APC_GENETIC_H

#include "APCAlgorithm.h"

using namespace std;

/**
 * Struct individual.
 * It represens an individual of the population of a genetic algorithm.
 */
struct Individual{
    APCSolution *s;
    float val;

    inline Individual():s(NULL),val(-1.0){}

    inline Individual(APCSolution *s):s(s),val(-1.0){}

    inline Individual(APCSolution *s, const float &val):s(s),val(val){}

    /* ORDER OPERATORS. They only consider the individuals value. */

    inline bool operator<=(const Individual & i) const{
        return this->val <= i.val;
    }

    inline bool operator<(const Individual & i) const{
        return this->val < i.val;
    }

    inline bool operator>=(const Individual & i) const{
        return this->val >= i.val;
    }

    inline bool operator>(const Individual & i) const{
        return this->val > i.val;
    }

    inline bool operator==(const Individual & i) const{
        return this->val == i.val;
    }

    /**
     * Copy constructor.
     * COPY CONSTRUCTOR COPIES A NEW SOLUTION.
     * ASSIGN OPERATOR USES THE SAME SOLUTION.
     */
     inline Individual(const Individual & i){
        this->s = new APCSolution(*i.s);
        this->val = i.val;
     }

     inline ~Individual(){
        delete s;
     }

};

/**
 * Comparator used for sorting individuals from bigger to lower fitness.
 */
struct IndividualComparator{
    //typedef bool result_type;
    inline bool const operator()(Individual *i1, Individual *i2) const{
        return (*i1) > (*i2);
    }
};

/**
 * Cross operator type.
 */
typedef vector<Individual*> (*crossOperator) (const Individual & i1, const Individual & i2);

/**
 * Class APCGenetic.
 * Abstract class which includes functionalities for different genetic algorithms for APC.
 */
class APCGenetic: public APCAlgorithm{
protected:

    /**
     * Population.
     */
    vector<Individual*> population;

    /**
     * Population size.
     */
    int population_size;

    /**
     * Probabilites.
     */
    float mutation_prob;
    float cross_prob;

    /**
     * Binary tour function. It picks the best of the two individuals given in the parameters.
     */
    inline static Individual * binary_tour(Individual * i1, Individual * i2){
        return (*i1 > *i2)?i1:i2;
    }

    /**
      * Best solution for a generation (for elitism and returning)
      */
    Individual * best_solution;

    /**
      * Number of calls to evaluation function.
      */
    int num_evaluations;

    /* Population stages */

    vector<Individual*> parents_population;

    vector<Individual*> children_population;

    /**
     * Auxiliar vector to count mutations (for reducing evaluations)
     */
    bool * mutations;

    /**
     * Number of the generation.
     */
    int generation;

    /**
     * Algorithm metadata.
     */
    string cross_algorithm;
    string selection_algorithm;

    /**
     * Erases stored solutions.
     */
    void clearSolutions();



    /**
     * Selection operator. It will be implemented in APCGenetic subclasses.
     */
    virtual void selection() = 0;


    /**
     * Cross mechanics. It will be implemented in APCGenetic subclasses.
     */
    virtual void cross(crossOperator c, const APCPartition &p, targetFunction f) = 0;

    /**
     * Mutation mechanics. It will be implemented in APCGenetic subclasses.
     */
    virtual void mutation(const APCPartition &p, targetFunction f) = 0;

    
    /**
     * Replacement operator. It will be implemented in APCGenetic subclasses.
     */
    virtual void replacement() = 0;


    /**
     * Generates a weights vector for the specified partition.
     * @param p_train Partition where weights will be learnt.
     * @param max_evaluations Max number of evaluations to make stop the algorithm.
     * @param cross Cross operator.
     
     */
    APCSolution * solve(const APCPartition & p_train, targetFunction f, crossOperator c, int population_size = 30, float cross_prob = 0.7, float mutation_prob = 0.001, int max_evaluations = 15000);


public:

    /**
     * Constructor.
     */
    APCGenetic(const APCProblem *p);

    /**
     * Generates a new population of solutions with weights uniformely distributed.
     */
    void generatePopulation(const APCPartition &p, targetFunction f, int population_size = 30);

    /**
     * Sets the population with the given parameters.
     * @param p Training partition.
     * @param population Pointer to the population to set.
     * @param best Best individual of the population. If NULL, it will be checked.
     */
    void setInitialPopulation(const APCPartition &p, const vector<Individual*> & population);

    /**
     * Developes a new generation for the given population.
     */
    void nextGeneration(const APCPartition & p_train, crossOperator c, targetFunction f);

    /**
     * Generates solutions for a 5x2 partition.
     * @param partition 5x2 partition to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, crossOperator c, int population_size = 30, float cross_prob = 0.7, float mutation_prob = 0.001, int max_evaluations = 15000);

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
    void solve5Fold(const APC5FoldPartition & partition, crossOperator c, int population_size = 30, float cross_prob = 0.7, float mutation_prob = 0.001, int max_evaluations = 15000);
    
    /**
     * Obtains the algorithm name.
     */
    inline string getAlgorithmName(){
        return algorithm_name+"-"+selection_algorithm+"-"+cross_algorithm;
    }

    /**
     * Obtains the population. Any modification od the elements in the returned vector
     * will affect the population if the algorithm is run again.
     */
    inline vector<Individual*> & getPopulation(){
        return population;
    }

    /**
     * Resets target function evaluations count.
     */
    inline void resetEvaluations(){
        num_evaluations = 0;
    }

    /**
     * Obtains target function evaluations count.
     */
    inline int getEvaluations(){
        return num_evaluations;
    }

    /**
     * Tells the algorithm to recalc the best solution (for extern modifications in memetic algorithms).
     */
    void recalcBestSolution();

    /**
     * @brief Indicates to the algorithm which is the best solution in the population.
     * @param best Pointer to the best solution.
     * @pre The pointer must be pointing to the correct best solution.
     */
    inline void recalcBestSolution(Individual * best){
        if(*best > *best_solution) best_solution = best;
    }

    /**
     * Destructor.
     */
    ~APCGenetic();

    /**
     * Stablish algorithm probabilities.
     */
    void setParameters(float cross_prob = 0.7, float mutation_prob = 0.001);
    
    /**
     * Obtains the number of the reached generation.
     */
    inline int getGeneration(){
        return generation;
    }
    /**
     * Removes all the elements in the population.
     */
    void clearPopulation();

    /* CROSS OPERATORS */

    /**
     * @brief Arithmetic cross operator.
     * @param i1 First parent
     * @param i2 Second parent
     * @return A vector with a child. Its solution components will be the arithmetical mean of the parents' components.
     */
    static vector<Individual*> arithmeticCross(const Individual &i1, const Individual &i2);

    /**
     * @brief BLX-0.3 cross operator.
     * @param i1 First parent.
     * @param i2 Second parent.
     * @return A vector with the children.
     * Children are generated randomly in the interval [Cmin - l*0.3, Cmax + l*0.3]
     * Cmin = min{parent1[i],parent2[i]}, Cmax = max{parent1[i],parent2[i]}, l = Cmax-Cmin
     */
    //Generalizar en un futuro si hay tiempo
    static vector<Individual*> BLXCross03(const Individual &i1, const Individual &i2);
};

/**
 * Class APCGeneticGenerational.
 * A class to manage Genetic Generational Algorithms.
 */
class APCGeneticGenerational : public APCGenetic{
public:
    /**
     * Constructor.
     */
    inline APCGeneticGenerational(const APCProblem *p)
        :APCGenetic(p){
            this->selection_algorithm = "GENERATIONAL";
    }

    /**
     * Selection operator.
     */
    virtual void selection();

    /**
     * Cross mechanics.
     */
    virtual void cross(crossOperator c, const APCPartition &p, targetFunction f);

    /**
     * Mutation mechanics.
     */
    virtual void mutation(const APCPartition &p, targetFunction f);

    /**
     * Replacement operator.
     */
    virtual void replacement();   


};

/**
 * Class APCGeneticGenerational.
 * A class to manage Genetic Stationary Algorithms.
 */
class APCGeneticStationary : public APCGenetic{
public:
    /**
     * Constructor.
     */
    inline APCGeneticStationary(const APCProblem *p)
        :APCGenetic(p){
            this->selection_algorithm = "STATIONARY";
    }

    /**
     * Selection operator.
     */
    virtual void selection();

    /**
     * Cross mechanics.
     */
    virtual void cross(crossOperator c, const APCPartition &p, targetFunction f);

    /**
     * Mutation mechanics.
     */
    virtual void mutation(const APCPartition &p, targetFunction f);

    /**
     * Replacement operator.
     */
    virtual void replacement();
};

#endif