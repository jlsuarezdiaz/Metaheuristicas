#ifndef APC_5x2_H
#define APC_5x2_H

# include "APCProblem.h"
# include "APCPartition.h"
# include <vector>

class APC5x2Partition{
private:
    const APCProblem *problem;

    APCPartition partitions[5][2];

public:
    /**
     * @brief Constructor. Generates a random partition.
     * @param problem APCProblem to make partitions.
     */
    APC5x2Partition(const APCProblem * problem);

    inline const APCPartition* operator[](int npart) const{
        return partitions[npart];
    }

};


#endif