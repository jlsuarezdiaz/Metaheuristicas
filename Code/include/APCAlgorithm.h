#ifndef APC_ALGORITHM_H
#define APC_ALGORITHM_H

# include <string>
# include <vector>
# include <fstream>
# include <iostream>
# include <algorithm>
# include <limits>
# include <numeric>

#include "APCSolution.h"
#include "APC1NN.h"
#include "APC5x2Partition.h"
#include "APCPartition.h"
#include "Timer.h"
#include "SRandom.h"

using namespace std;


/**
 * @brief Abstract class with attributes to be used with any 
 * APC Algorithm.
 */
class APCAlgorithm{
protected:
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

    string algorithm_name;

    void clearSolutions();

public:

    APCAlgorithm(const APCProblem *problem,  const string & algorithm_name = "");

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

    inline string getAlgorithmName(){
        return algorithm_name;
    }

    inline APCSolution * getSolution(int i){
        return solutions[i];
    }

    inline double getTime(int i){
        return times[i];
    }

    inline float getFitness(int i){
        return fitnesses[i];
    }

    inline float getTrainFit(int i){
        return train_fits[i];
    }

    inline APCSolution * getLastSolution(){
        return solutions.back();
    }

    inline double getLastTime(){
        return times.back();
    }

    inline float getLastFitness(){
        return fitnesses.back();
    }

    inline float getLastTrainFit(){
        return train_fits.back();
    }

    void writeSolutions(ostream & out);

    void writeFitnesses(ostream & out);

    void writeTrainFits(ostream & out);

    void writeTimes(ostream & out);

    void writeTable5x2(ostream & out, string cols = "fti", bool stats =  true);

    //~APCAlgorithm();

};

#endif