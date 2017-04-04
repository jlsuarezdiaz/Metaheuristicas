#ifndef APC_RELIEF
#define APC_RELIEF

#include <string>
#include <limits>
#include <iostream>

#include "APC1NN.h"
#include "APC5x2Partition.h"
#include "Timer.h"

using namespace std;

class APCRelief{
private:
    const APCProblem *problem;

    /**
     * Vector with obtained solutions.
     */
    vector<APCSolution *> solutions;

    /**
     * Vector with obtained fitnesses.
     */
    vector<float> fitnesses;

    vector<float> train_fits;

    /**
     * Vector with obtained times.
     */
    vector<double> times;

    /**
     * Class Timer.
     */
    Timer timer;

    const string algorithm_name;

    int findNearestEnemy(const APCPartition & p, int index) const;

    int findNearestFriend(const APCPartition & p, int index) const;

    void clearSolutions();



public:

    APCRelief(const APCProblem *p);

    /**
     * Generates a weights vector for the specified partition.
     * @param p Partition
     * @pos The solution obtained will be returned as the last element of getSolutions()
     * @pos The elapsed time will be returned as the last element of getTimes()
     * @pos No fitness is calculated. The stored fitness in getFitnesses() will be -1.0
     * @return Solution obtained.
     */
    APCSolution * solve(const APCPartition & p);

    /**
     * Generates solutions for a 5x2 partition.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     */
    inline void solve5x2(){
        solve5x2(APC5x2Partition(problem));
    }

    /**
     * Generates solutions for a 5x2 partition.
     * @param Partition 5x2 partition to evaluate.
     * @pos getSolutions() will return a 10 elements vector with the solutions.
     * @pos getTimes() will return a 10 elements vector with the partition times.
     * @pos getFitnesses() will return a 10 elements vector with the partition fitnesses.
     */
    void solve5x2(const APC5x2Partition & partition);

    inline vector <APCSolution *> getSolutions(){
        return solutions;
    }

    inline vector <double> getTimes(){
        return times;
    }

    inline vector <float> getFitnesses(){
        return fitnesses;
    }

    inline vector <float> getTrainFits(){
        return train_fits;
    }

    inline const string & getAlgorithmName(){
        return algorithm_name;
    }

    ~APCRelief();


};

#endif