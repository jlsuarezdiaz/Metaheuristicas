#ifndef APC_5x2_H
#define APC_5x2_H

# include "APCProblem.h"
# include "APCPartition.h"
# include <vector>

/**
 * @brief Class APC5x2 Partition
 * @brief Class that stores a 5x2 partition for an APC problem.
 */
class APC5x2Partition{
private:
    /**
     * Problem.
     */
    const APCProblem *problem;

    /**
     * Partitions array.
     */
    APCPartition partitions[5][2];

public:
    /**
     * @brief Constructor. Generates a random partition.
     * @param problem APCProblem to make partitions.
     */
    APC5x2Partition(const APCProblem * problem);

    /**
     * @brief Obtains a pair of train-test partitions in 5x2.
     * @param npart Index.
     * @pre npart must be between 1 and 5.
     */
    inline const APCPartition* operator[](int npart) const{
        return partitions[npart];
    }

    /**
     * Output operator.
     */
    friend ostream & operator<<(ostream & out, const APC5x2Partition & p);

};


#endif