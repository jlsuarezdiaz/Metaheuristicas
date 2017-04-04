
# include <iostream>
# include <fstream>
# include <random>

#include "APCProblem.h"
#include "APCSolution.h"
#include "APCRelief.h"
#include "SRandom.h"
#include "APC5x2Partition.h"
#include "APCLocalSearch.h"
#include "APCRandom.h"
#include "APCGenetic.h"
#include "APCMemetic.h"

#include <iostream>
#include <utility>
#include <vector>
#include <string>

using namespace std;

void printHelp(){
    cout << "USO DEL PROGRAMA:" << endl;
    cout << "apc [fichero problema] [argumentos]" << endl;
    exit(0);
}

vector<pair<char,string>> parseInput(int argc, char const *argv[]){
    if(argc < 2){
        printHelp();
    }
    vector<pair<char,string>> args;
    for(int i = 2; i < argc; i+=2){
        if(argv[i][0] == '-' && i+1 < argc){
            args.push_back(pair<char,string>(argv[i][1],string(argv[i+1])));
        }
        else printHelp();
    }
    return args;
}

int main(int argc, char const *argv[])
{
    vector <pair<char,string>> input = parseInput(argc,argv);

    //Data
    string problem_name(argv[1]);
    unsigned seed = time(NULL);
    string algorithm = "";

    APCProblem problem(problem_name);
    APC5x2Partition myPartition(&problem);

    APCRelief relief(&problem);
    APCLocalSearch LS(&problem);
    APCRandom apc_random(&problem);
    APCGeneticGenerational agg(&problem);
    APCGeneticStationary age(&problem);
    APCMemetic am(&problem);

    //Parse args
    for(unsigned i = 0; i < input.size(); i++){
        switch(input[i].first){
            case 's':
                seed = stoul(input[i].second);
                break;
            case 'a':
                algorithm = input[i].second;
                break;
            default:
                printHelp();
        }
    }

    //Set random seed.
    SRandom::getInstance().setSeed(seed);

    if(algorithm == "RELIEF" || algorithm == "RELIEF+LS"){    
        relief.solve5x2(myPartition);
    
        if(algorithm == "RELIEF"){
            cout << relief.getAlgorithmName() << endl;
            for(int i = 0; i < 10; i++){
                cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << relief.getFitnesses()[i] << "\tTRAIN FIT = " << relief.getTrainFits()[i] << ";\tTIME = " << relief.getTimes()[i] << endl;
            }
        }

        if(algorithm == "RELIEF+LS"){
            vector<APCSolution *> relief_sols = relief.getSolutions();
            LS.solve5x2(myPartition,relief_sols);
            cout << relief.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
            for(int i = 0; i < 10; i++){
                cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << LS.getFitnesses()[i] << "\tTRAIN FIT = " << LS.getTrainFits()[i] << ";\tTIME = " << LS.getTimes()[i] << endl;
            }
        }

    } 

    else if(algorithm == "RANDOM" || algorithm == "RANDOM+LS"){
        apc_random.solve5x2(myPartition);

        if(algorithm == "RANDOM"){
            cout << apc_random.getAlgorithmName() << endl;
            for(int i = 0; i < 10; i++){
                cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << apc_random.getFitnesses()[i] << "\tTRAIN FIT = " << apc_random.getTrainFits()[i] << ";\tTIME = " << apc_random.getTimes()[i] << endl;
            }
        }

        if(algorithm == "RANDOM+LS"){
            vector<APCSolution *> random_sols = apc_random.getSolutions();
            LS.solve5x2(myPartition,random_sols);
            cout << apc_random.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
            for(int i = 0; i < 10; i++){
                cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << LS.getFitnesses()[i] << "\tTRAIN FIT = " << LS.getTrainFits()[i] << ";\tTIME = " << LS.getTimes()[i] << endl;
            }
        }
    }

    else if(algorithm == "AGG-BLX"){
        //ofstream fout("./sol/sols_agg.sol");
        agg.solve5x2(myPartition,APCGenetic::BLXCross03);

        cout << agg.getAlgorithmName() << endl;
        for(int i = 0; i < 10; i++){
            cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << agg.getFitnesses()[i] << "\tTRAIN FIT = " << agg.getTrainFits()[i] << ";\tTIME = " << agg.getTimes()[i] << endl;
            //fout << *agg.getSolutions()[i] << "\n";
        }

    }

    else if(algorithm == "AGG-CA"){
        agg.solve5x2(myPartition,APCGenetic::arithmeticCross);

        cout << agg.getAlgorithmName() << endl;
        for(int i = 0; i < 10; i++){
            cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << agg.getFitnesses()[i] << "\tTRAIN FIT = " << agg.getTrainFits()[i] << ";\tTIME = " << agg.getTimes()[i] << endl;
        }
    }

    else if(algorithm == "AGE-BLX"){
        age.solve5x2(myPartition,APCGenetic::BLXCross03,30,0.7);

        cout << age.getAlgorithmName() << endl;
        for(int i = 0; i < 10; i++){
            cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << age.getFitnesses()[i] << "\tTRAIN FIT = " << age.getTrainFits()[i] << ";\tTIME = " << age.getTimes()[i] << endl;
        }
    }

    else if(algorithm == "AGE-CA"){
        age.solve5x2(myPartition,APCGenetic::arithmeticCross,30,1.0);

        cout << age.getAlgorithmName() << endl;
        for(int i = 0; i < 10; i++){
            cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << age.getFitnesses()[i] << "\tTRAIN FIT = " << age.getTrainFits()[i] << ";\tTIME = " << age.getTimes()[i] << endl;
        }
    }

    else if(algorithm == "AM-10-1.0"||algorithm == "AM-10-0.1"||algorithm == "AM-10-0.1mej"){
        if(algorithm == "AM-10-1.0") am.solve5x2(myPartition,10,1.0,false);
        else if(algorithm == "AM-10-0.1") am.solve5x2(myPartition,10,0.1,false);
        else if(algorithm == "AM-10-0.1mej") am.solve5x2(myPartition,10,0.1,true);

        cout << am.getAlgorithmName() << endl;
        for(int i = 0; i < 10; i++){
            cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << age.getFitnesses()[i] << "\tTRAIN FIT = " << age.getTrainFits()[i] << ";\tTIME = " << age.getTimes()[i] << endl; 
        }
    }

    else{
        cout << "ALGORITMO INVÁLIDO" << endl;
        printHelp();
    }

    return 0;
}