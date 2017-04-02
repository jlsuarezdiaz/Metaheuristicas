#ifndef SRANDOM_H
#define SRANDOM_H

# include <random>
# include <time.h>
# include <stdlib.h>
# include <chrono>
# include <iostream>

using namespace std;

/**
 * @brief Class SRandom (Singleton Random)
 * Singleton class to generate any kind of random numbers allowing to set a seed.
 */
class SRandom{
private:

    /* FORBIDDEN METHODS */

    /**
     * Constructor. Seeds will be initialized to current time until no setSeed() is used.
     */
     SRandom();

    /**
     * Copy constructor.
     */
     SRandom(const SRandom & s);

    /**
     * Assign operator.
     */
     void operator=(const SRandom & s);

    /* GENERATORS */

    /**
     * Random generator used for normal distribution.
     */
     default_random_engine generator;




public:
    /**
     * Gets class instance.
     */
    static SRandom & getInstance();

    /**
     * Sets random seed.
     */
    void setSeed(unsigned seed);

    /* RANDOM METHODS */
    /**
     * Obtains a normal distribution element.
     * @param mu Mean.
     * @param sigma Standard deviation.
     */
    template <class RealType=double>
    RealType getNormalDistributionElement(RealType mu, RealType sigma){
        return normal_distribution<RealType>(mu,sigma)(generator);
    }

    /**
     * Obtains a normal distribution element.
     * @param a Minimum of the interval.
     * @param b Maximum of the interval.
     */
    template <class RealType=double>
    RealType getRealUniformDistributionElement(RealType a, RealType b){
        return uniform_real_distribution<RealType>(a,b)(generator);
    }

    /**
     * Returns integer random number.
     */
    inline int rand(){
        return std::rand();
    }

    /**
     * Returns random number between min and max.
     */
    inline int rand(int min, int max){
        return std::rand() % (max - min + 1) + min;
    }

    /* GET METHODS */
    inline default_random_engine getGenerator(){
        return generator;
    }
};

#endif