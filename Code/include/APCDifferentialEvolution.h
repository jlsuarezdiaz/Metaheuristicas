#ifndef APC_DE_H
#define APC_DE_H

# include "APCGenetic.h"

/**
 * Cross operator type.
 */
typedef Individual* (*differentialCross) (const Individual * i1, const Individual * i2, const Individual *i3, const Individual *x, const Individual *best, float cr, float f);

class APCDifferentialEvolution: public APCGenetic{
private:

    APCSolution * solve(const APCPartition &p, targetFunction fitness, differentialCross c, int population_size = 50, float cr = 0.5, float f = 0.5, int max_evaluations = 15000);

    void nextGeneration(const APCPartition & p, targetFunction fitness, differentialCross c, float cr, float f);
    inline void nextGeneration(const APCPartition & p_train, crossOperator c, targetFunction f){}


    /**
     * Selection operator.
     */
    virtual void selection();

    /**
     * Cross mechanics.
     */
    inline virtual void cross(crossOperator c, const APCPartition &p, targetFunction f){}
    void cross(differentialCross c, const APCPartition & p, targetFunction fitness,float cr, float f);

    /**
     * Mutation mechanics.
     */
    inline virtual void mutation(const APCPartition &p, targetFunction f){}

    /**
     * Replacement operator.
     */
    virtual void replacement();

public:

    inline APCDifferentialEvolution(const APCProblem *p)
        :APCGenetic(p){
            this->selection_algorithm = "DiffferentialEvolution";
    }

    static Individual * DERand(const Individual * i1, const Individual *i2, const Individual *i3, const Individual *x, const Individual *best, float cr, float f);

    static Individual * DECurrentToBest(const Individual * i1, const Individual *i2, const Individual *i3, const Individual *x, const Individual *best, float cr, float f);

    /**
     * Generates solutions for a 5x2 partition.
     * @param partition 5x2 partition to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, differentialCross c, int population_size = 50, float cr = 0.5, float f = 0.5, int max_evaluations = 15000);

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
    void solve5Fold(const APC5FoldPartition & partition, differentialCross c, int population_size = 50, float cr = 0.5, float f = 0.5, int max_evaluations = 15000);
    

};


#endif