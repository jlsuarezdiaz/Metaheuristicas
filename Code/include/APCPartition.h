#ifndef APC_PARTITION_H
#define APC_PARTITION_H

# include <math.h>

# include "APCProblem.h"


class APCPartition{
private:

    const APCProblem *problem;

    vector <int> partition;

    inline void addPartitionElement(int n){
        partition.push_back(n);
    }

public:
    inline APCPartition(): problem(NULL){}

    inline APCPartition(const APCProblem *problem)
        :problem(problem){}

    inline const float * getInstance(int i) const{
        return problem->getAttributes()[partition[i]];
    }

    inline const bool getClass(int i) const{
        return problem->getClasses()[partition[i]];
    }

    inline const float * operator[](int i) const{
        return getInstance(i);
    }

    inline const int size() const{
        return partition.size();
    }

    /**
     * @brief return euclidean (squared) distance between two elements in the partition.
     * @param i Index of the first element.
     * @param j Index of the second element.
     * @return Distance from partition[i] to partition[j]
     */
    float euclideanSqDistance(int i, int j) const;

    /**
     * @brief Given two partitions, obtains a vector with the distances between each
     *  component of the vectors.
     * @param i Index of the first element in the partition.
     * @param j Index of the second element in the partition.
     * @return Component distances between partition[i] and partition[j]
     */
    vector<float> componentDistance(int i,int j) const;

    friend class APC5x2Partition;

};


#endif