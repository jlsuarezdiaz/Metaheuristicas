#ifndef APC_PROBLEM_H
#define APC_PROBLEM_H

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <limits>
#include "arff_parser.h"
#include "arff_data.h"

using namespace std;




/**
 * @brief Class APCProblem.
 * Class that represents the data for the APC Problem
 */
class APCProblem{
private:

    /**
     * File with the problem data.
     */
    string file_name;

    string relation;

    /** 
     * Number of attributes, including class attribute.
     */
    int num_attributes;

    int num_instances;

    /**
     * Matrix with the attributes for each instance (Size: num_instances x (num_attributes -1))
     */
    float ** attributes;

    bool * classes;

    /**
     * @brief Private method to read the data of the given file.
     */
    void readFileData();

    void normalize();

public:

    /**
     * @brief Constructor.
     * @param file_name Name of the file to be read to get problem data.
     */
    APCProblem(const string & file_name);

    /**
     * @brief Destructor.
     */
    ~APCProblem();

    /**
     * @brief Copy constructor.
     */
    APCProblem(const APCProblem & other);

    inline int getNumAttributes() const{
        return num_attributes;
    }

    inline int getNumNonClassAttributes() const{
        return num_attributes-1;
    }

    inline int getNumInstances() const{
        return num_instances;
    }

    inline float** const getAttributes() const{
        return attributes;
    }

    inline const bool * getClasses() const{
        return classes;
    }


};

#endif