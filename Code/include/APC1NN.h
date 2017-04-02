#ifndef APC_1NN_H
#define APC_1NN_H

#include "APCPartition.h"
#include "APCSolution.h"

class APC_1NN{
private:
    const APCPartition * p;
    const APCSolution * w;

    /**
     * @brief Obtains the weight-pondered squared distance between two elements in the partition.
     * @param i Index of first element.
     * @param j Index of second element.
     * @return weighted distance between partition[i] and partition[j]
     */
    float w_sqDistance(int i, int j);

    /**
     * @brief Classifies an element of the partition using the rest of the partition.
     * @param i Index of partition element.
     * @return class predicted (according 1-NN for partition[i])
     */
    bool classify(int i);

public:

    inline APC_1NN(const APCPartition *p, const APCSolution *w)
        :p(p), w(w){}

    float fitness();

};

#endif