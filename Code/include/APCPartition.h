#ifndef APC_PARTITION_H
#define APC_PARTITION_H

# include <math.h>

# include "APCProblem.h"

/**
 * Class partition.
 * It represents a partition of an APC Problem.
 */
class APCPartition{
private:
    /**
     * Problem.
     */
    const APCProblem *problem;

    /**
     * Indexes vector. Each element is an index in the problem matrix.
     */
    vector <int> partition;

    /**
     * Adds an index to the partition.
     */
    inline void addPartitionElement(int n){
        partition.push_back(n);
    }

public:
    /**
     * Default Constructor.
     */
    inline APCPartition(): problem(NULL){}

    /**
     * Constructor.
     */
    inline APCPartition(const APCProblem *problem)
        :problem(problem){}

    /**
     * Obtains an instance in the partition, given by index i.
     */
    inline const float * getInstance(int i) const{
        return problem->getAttributes()[partition[i]];
    }

    /**
     * Obtains the class for the instance in the partition given by index i.
     */
    inline const bool getClass(int i) const{
        return problem->getClasses()[partition[i]];
    }

    /**
     * Obtains an instance in the partition (a more simple access to getInstance)-
     */
    inline const float * operator[](int i) const{
        return getInstance(i);
    }

    /**
     * Obtains partition size.
     */
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
    friend class APC5FoldPartition;

    /**
     * Output operator.
     */
    friend ostream & operator<<(ostream & out, const APCPartition & p);

    //friend istream & operator>>(istream & in, APCPartition & p);

};


#endif