#ifndef APC_1NN_EVAL_H
#define APC_1NN_EVAL_H

#include "APCAlgorithm.h"

class APC_1NN_Eval: public APCAlgorithm{
public:

    APC_1NN_Eval(const APCProblem * p);

    

    void solve5x2(const APC5x2Partition & partition);

    ~APC_1NN_Eval();

};

#endif