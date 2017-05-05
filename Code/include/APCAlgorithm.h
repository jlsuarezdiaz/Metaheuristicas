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
#include "APCTarget_CR.h"
#include "APC5x2Partition.h"
#include "APC5FoldPartition.h"
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

    /**
     * Vector with training fitnesses.
     */
    vector<float> train_fits;

    /**
     * Vector with obtained times.
     */
    vector<double> times;

    /**
     * Vector with reduction rates (for Target_CR)
     */
    vector <float> red_rates;

    /**
     * Vector with classification rates (for Target_CR)
     */
    vector <float> class_rates;

    /**
     * Class Timer.
     */
    Timer timer;

    /**
     * Algorithm name.
     */
    string algorithm_name;

    /**
     * Deletes the stored solutions.
     */
    void clearSolutions();

public:
    /**
     * Constructor.
     * @param problem Pointer to the APC problem.
     * @param algorithm name Algorithm name, to be initialized in subclasses.
     */
    APCAlgorithm(const APCProblem *problem,  const string & algorithm_name = "");

    /**
     * @brief Obtains a vector with the solutions for an evaluation strategy (like 5x2)-
     */
    inline vector <APCSolution *> getSolutions(){
        return solutions;
    }

    /**
     * Obtains a vector with the times associated to each solution in getSolutions().
     */ 
    inline vector <double> getTimes(){
        return times;
    }

    /**
     * Obtains a vector with the fitnesses associated to each solution in getSolutions().
     */ 
    inline vector <float> getFitnesses(){
        return fitnesses;
    }

    /**
     * Obtains a vector with the train fitnesses associated to each solution in getSolutions().
     */ 
    inline vector <float> getTrainFits(){
        return train_fits;
    }

    /**
     * Obtains a vector with the classification rates associated to each solution.
     */
    inline vector <float> getClassRates(){
        return class_rates;
    }

    /**
     * Obtains a vector with the reduction rates associated to each solution.
     */
    inline vector <float> getRedRates(){
        return red_rates;
    }

    /**
     * Gets the algorithm name.
     */
    inline string getAlgorithmName(){
        return algorithm_name;
    }

    /**
     * Obtains the solution in getSolutions() given by the index.
     */
    inline APCSolution * getSolution(int i){
        return solutions[i];
    }

    /**
     * Obtains the time in getTimes() given by the index.
     */
    inline double getTime(int i){
        return times[i];
    }

    /**
     * Obtains the fitness in getFitnesses() given by the index.
     */
    inline float getFitness(int i){
        return fitnesses[i];
    }

    /**
     * Obtains the train fitness in getTrainFits() given by the index.
     */
    inline float getTrainFit(int i){
        return train_fits[i];
    }

    /**
     * Obtains the classification rate in getClassRates() given by the index.
     */
    inline float getClassRate(int i){
        return class_rates[i];
    }

    /**
     * Obtains the reduction rate in getRedRates() given by the index.
     */
    inline float getRedRate(int i){
        return red_rates[i];
    }

    /**
     * Obtains the last solution in getSolutions().
     */
    inline APCSolution * getLastSolution(){
        return solutions.back();
    }

    /**
     * Obtains the last time in getTimes().
     */
    inline double getLastTime(){
        return times.back();
    }

    /**
     * Obtains the last fitness in getFitnesses().
     */
    inline float getLastFitness(){
        return fitnesses.back();
    }

    /**
     * Obtains the last train fitness in getTrainFits().
     */
    inline float getLastTrainFit(){
        return train_fits.back();
    }

    /**
     * Obtains the last classification rate fitness in getClassRates().
     */
    inline float getLastClassRate(){
        return class_rates.back();
    }

    /**
     * Obtains the last reduction rate fitness in getRedRates().
     */
    inline float getLastRedRate(){
        return red_rates.back();
    }

    /**
     * Writes the stored solutions in the output stream.
     */
    void writeSolutions(ostream & out);

    /**
     * Writes the stored fitnesses in the output stream.
     */
    void writeFitnesses(ostream & out);

    /**
     * Writes the stored training fitnesses in the output stream.
     */
    void writeTrainFits(ostream & out);

    /**
     * Writes the stored times in the output stream.
     */
    void writeTimes(ostream & out);

    /**
     * Writes a table for a 5x2 partition in the output stream.
     * @param out Output stream.
     * @param cols String with the columns in the table. Each char represents a kind of data.
     * Allowed chars are 'f' (fitness), 't' (times), 'i', (training fits)
     * @param stats Indicates if mean and stdev are calculated.
     * @pre There must be stored the 10 solutions of a 5x2 partition.
     */
    void writeTable5x2(ostream & out, string cols = "fti", bool stats =  true);

    //~APCAlgorithm();

};

#endif