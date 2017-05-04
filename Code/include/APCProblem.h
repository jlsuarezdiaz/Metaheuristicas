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

    /**
     * Relation name.
     */
    string relation;

    /** 
     * Number of attributes, including class attribute.
     */
    int num_attributes;

    /**
     * Number of instances.
     */
    int num_instances;

    /**
     * Matrix with the attributes for each instance (Size: num_instances x (num_attributes -1))
     */
    float ** attributes;

    /**
     * Array with the classes.
     */
    bool * classes;

    /**
     * @brief Private method to read the data of the given file.
     */
    void readFileData();

    /**
     * Normalizes the problem data to [0,1] (by attributes).
     */
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

    /**
     * @brief Obtains the number of attributes in the problem.
     */
    inline int getNumAttributes() const{
        return num_attributes;
    }

    /**
     * Obtains the nomber of attributes, excluding class attribute.
     */
    inline int getNumNonClassAttributes() const{
        return num_attributes-1;
    }

    /**
     * Obtains the number of instances in the problem.
     */
    inline int getNumInstances() const{
        return num_instances;
    }

    /**
     * Obtains the attributes matrix.
     */
    inline float** const getAttributes() const{
        return attributes;
    }

    /**
     * Obtains the classes array.
     */
    inline const bool * getClasses() const{
        return classes;
    }

    /**
     * Obtains the file_name.
     */
    inline string getFileName(){
        return file_name;
    }


};

#endif