#ifndef APC_1NN_H
#define APC_1NN_H

#include "APCPartition.h"
#include "APCSolution.h"

/**
 * @brief Class containing a set of functions to calculate fitness por a solution using
 * classifier 1NN.
 */
class APC_1NN{
private:
    //const APCPartition * p;
    //const APCSolution * w;

    /**
     * @brief Obtains the weight-pondered squared distance between two elements in the partition.
     * @param p Test partition.
     * @param w Solution to test.
     * @param i Index of first element.
     * @param j Index of second element.
     * @return weighted distance between partition[i] and partition[j]
     */
    //float w_sqDistance(int i, int j);
    static float w_sqDistance(const APCPartition & p, const APCSolution & w, int i, int j);

    /**
     * @brief Classifies an element of the partition using the rest of the partition.
     * @param p Test partition.
     * @param w Solution to test.
     * @param i Index of partition element.
     * @return class predicted (according 1-NN for partition[i])
     */
    //bool classify(int i);
     static bool classify(const APCPartition & p, const APCSolution & w, int i);

public:

    //inline APC_1NN(const APCPartition *p, const APCSolution *w)
    //    :p(p), w(w){}
    /**
     * @brief Obtains the fitness for a solution in the given test partition.
     * @param p Test partition.
     * @param w Solution to test.
     * @return Fitness.
     */
    static float fitness(const APCPartition & p, const APCSolution & w);

};

#endif