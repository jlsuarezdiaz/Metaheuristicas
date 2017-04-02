#ifndef APC_SOLUTION_H
#define APC_SOLUTION_H

#include <vector>
#include <algorithm>

#include "APCProblem.h"
#include "SRandom.h"

using namespace std;



class APCSolution{
private:
    float * weights;

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

    inline float & operator[](unsigned i){
        return weights[i];
    }

    inline const float & operator[](unsigned i) const{
        return weights[i];
    }

    inline const float * getWeights() const{
        return weights;
    }

    inline const int size() const{
        return problem->getNumNonClassAttributes();
    }

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
     * Moves the solution in a neighbourhood by normal mutation.
     * @param i Index to move.
     * @param sigma Standard Deviation. The parameter will be moved an amount
     * z that follows a distribution N(0,sigma)
     */
    void move(int i, float sigma);

    friend ostream & operator<<(ostream & out, const APCSolution &s);

    friend istream & operator>>(istream & in, APCSolution &s);
    

};
#endif