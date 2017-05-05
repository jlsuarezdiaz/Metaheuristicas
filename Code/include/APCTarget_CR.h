#ifndef APC_TARGET_CR_H
#define APC_TARGET_CR_H

#include "APCPartition.h"
#include "APCSolution.h"
#include "APC1NN.h"
#include <vector>

/**
 * @brief class APCTargetCR
 * @brief A target function for the APC Problem considering classification and reduction rates.
 */
class APCTargetCR{
private:
    /**
     * Reduction rate.
     */
    static float reduction_rate(const APCSolution & w, const float red_ceil);

    /**
     * Classification rate.
     */
    static float classification_rate(const APCPartition & p, const APCSolution & w, const float red_ceil = 0.1);

public:
    /**
     * Obtains the fitness for a solution in the given partition, using reduction and classification rates.
     * @param p Partition where fitness will be checked.
     * @param w Solution to check.
     * @param alpha Weight for classification rate (1-alpha will be the weight for reduction rate).
     * @pre 0 <= alpha <= 1
     * @param red_ceil Ceiling for reduction rate.
     */
    static vector<float> fitness(const APCPartition & p, const APCSolution & w, const float alpha = 0.5, const float red_ceil = 0.1);

};

#endif