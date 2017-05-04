#ifndef APC_1NN_EVAL_H
#define APC_1NN_EVAL_H

#include "APCAlgorithm.h"

/**
 * An algorithm class to evaluate solutions.
 */
class APC_1NN_Eval: public APCAlgorithm{
public:
    /**
     * Constructor.
     */
    APC_1NN_Eval(const APCProblem * p);

    
    /**
     * Generates solutions for a 5x2 partition.
     * @param partition 5x2 partition to evaluate.
     * @param s Solution to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     * @pos getTrainFits() will return a 10 elements vector eith the training fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition, const APCSolution & s);

    /**
     * Destructor.
     */
    ~APC_1NN_Eval();

};

#endif