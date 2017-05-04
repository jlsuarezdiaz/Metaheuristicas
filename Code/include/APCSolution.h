#ifndef APC_SOLUTION_H
#define APC_SOLUTION_H

#include <vector>
#include <algorithm>

#include "APCProblem.h"
#include "SRandom.h"

using namespace std;


/**
 * Class APCSolution.
 * A class to store a solution of the APCProblem.
 */
class APCSolution{
private:
    /**
     * Weights vector.
     */
    float * weights;

    /**
     * Associated problem.
     */
    const APCProblem *problem;

public:

    /**
     * @brief Constructor.
     * Initializes the weights vector to 0
     * @param p APCProblem associated.
     */
    APCSolution(const APCProblem *p);

    /**
     * @brief Default constructor.
     */
    inline APCSolution()
        :weights(NULL), problem(NULL){}

    /**
     * @brief Sets the problem and resizes solution vector.
     */
    void setProblem(const APCProblem *p);

    /**
     * Obtains the i-th weight in the solution (l-value).
     */
    inline float & operator[](unsigned i){
        return weights[i];
    }

    /**
     * Obtains the i-th weight in the solution (r-value).
     */
    inline const float & operator[](unsigned i) const{
        return weights[i];
    }

    /**
     ^Obtains the weights array.
     */
    inline const float * getWeights() const{
        return weights;
    }

    /**
     * Obtains solutoin size.
     */
    inline const int size() const{
        return problem->getNumNonClassAttributes();
    }

    /**
     * Obtains the associated problem.
     */
    inline const APCProblem * getProblem(){
        return problem;
    }

    /**
     * @brief Normalizes the solution.
     * Converts any negative parameters in the solution vector to 0.
     * Divides all the elements by the maximum weight attribute.
     */
    void normalize();

    /**
     * @brief Destructor.
     */
    inline ~APCSolution(){
        delete [] weights;
    }

    /**
     * @brief Copy constructor.
     */
    APCSolution(const APCSolution & s);

    /**
     * @brief Assign operator.
     */
    APCSolution & operator=(const APCSolution & s);

    /* -- ALGEBRAIC OPERATORS -- */

    /**
     * Adds the solution the weights given by the iterator.
     * @param begin Iterator to float.
     * @pre begin should support access to as many positions as the size of weights.
     */
    template <class RandomAccessIterator>
    APCSolution & operator+= (RandomAccessIterator begin){
        for(int i = 0; i < problem->getNumNonClassAttributes(); i++){
            weights[i]+=*(begin+i);
        }
        return *this;
    }

    /**
     * Substracts to the solution the weights given by the iterator.
     * @param begin Iterator to float.
     * @pre begin should support access to as many positions as the size of weights.
     */
    template <class RandomAccessIterator>
    APCSolution & operator-= (RandomAccessIterator begin){
        for(int i = 0; i < problem->getNumNonClassAttributes(); i++){
            weights[i]-=*(begin+i);
        }
        return *this;
    }

    /**
     * Moves the solution in a neighbourhood by normal mutation.
     * @param i Index to move.
     * @param sigma Standard Deviation. The parameter will be moved an amount
     * z that follows a distribution N(0,sigma)
     */
    void move(int i, float sigma);

    /**
     * Output operator.
     */
    friend ostream & operator<<(ostream & out, const APCSolution &s);

    /**
     * Input operator.
     */
    friend istream & operator>>(istream & in, APCSolution &s);

    /**
     * Generates a solution with weights 1 everywhere.
     * Can be used to measure non-weighted classifiers.
     */
    static APCSolution weight1Solution(const APCProblem *problem);
    

};
#endif