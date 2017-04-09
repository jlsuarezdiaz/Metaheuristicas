#ifndef APC_GENETIC_H
#define APC_GENETIC_H

#include "APCAlgorithm.h"

using namespace std;


struct Individual{
    APCSolution *s;
    float val;

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

typedef vector<Individual*> (*crossOperator) (const Individual & i1, const Individual & i2);

class APCGenetic: public APCAlgorithm{
protected:

    vector<Individual*> population;

    int population_size;

    float mutation_prob;

    float cross_prob;

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
     * Number of the generation.
     */
    int generation;


 
    string cross_algorithm;
    string selection_algorithm;

    void clearSolutions();



    /**
     * Selection operator. It will be implemented in APCGenetic subclasses.
     */
    virtual void selection() = 0;


    /**
     * Cross mechanics. It will be implemented in APCGenetic subclasses.
     */
    virtual void cross(crossOperator c, const APCPartition &p) = 0;

    /**
     * Mutation mechanics. It will be implemented in APCGenetic subclasses.
     */
    virtual void mutation(const APCPartition &p) = 0;

    
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
    APCSolution * solve(const APCPartition & p_train, crossOperator c, int population_size = 30, float cross_prob = 0.7, float mutation_prob = 0.001, int max_evaluations = 15000);


public:

    APCGenetic(const APCProblem *p);

    /**
     * Generates a new population of solutions with weights uniformely distributed.
     */
    void generatePopulation(const APCPartition &p, int population_size = 30);

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
    void nextGeneration(const APCPartition & p_train, crossOperator c);

    /**
     * Generates solutions for a 5x2 partition.
     * @param Partition 5x2 partition to evaluate.
     * @param cross Cross operator.
     * @param max_evaluations Max number of evaluations to make stop the algorithm.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, crossOperator c, int population_size = 30, float cross_prob = 0.7, float mutation_prob = 0.001, int max_evaluations = 15000);


    inline string getAlgorithmName(){
        return algorithm_name+"-"+selection_algorithm+"-"+cross_algorithm;
    }

    inline vector<Individual*> & getPopulation(){
        return population;
    }

    inline void resetEvaluations(){
        num_evaluations = 0;
    }

    inline int getEvaluations(){
        return num_evaluations;
    }

    void recalcBestSolution();

    /**
     * @brief Indicates to the algorithm which is the best solution in the population.
     * @param best Pointer to the best solution.
     * @pre The pointer must be pointing to the correct best solution.
     */
    inline void recalcBestSolution(Individual * best){
        if(*best > *best_solution) best_solution = best;
    }

    ~APCGenetic();

    void setParameters(float cross_prob = 0.7, float mutation_prob = 0.001);
    
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
     * @return A vector with a child.
     * Children are generated randomly in the interval [Cmin - l*0.3, Cmax + l*0.3]
     * Cmin = min{parent1[i],parent2[i]}, Cmax = max{parent1[i],parent2[i]}, l = Cmax-Cmin
     */
    //Generalizar en un futuro si hay tiempo
    static vector<Individual*> BLXCross03(const Individual &i1, const Individual &i2);
};


class APCGeneticGenerational : public APCGenetic{
public:

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
    virtual void cross(crossOperator c, const APCPartition &p);

    /**
     * Mutation mechanics.
     */
    virtual void mutation(const APCPartition &p);

    /**
     * Replacement operator.
     */
    virtual void replacement();   


};

class APCGeneticStationary : public APCGenetic{
public:
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
    virtual void cross(crossOperator c, const APCPartition &p);

    /**
     * Mutation mechanics.
     */
    virtual void mutation(const APCPartition &p);

    /**
     * Replacement operator.
     */
    virtual void replacement();
};

#endif