
#include <iostream>
#include <fstream>
#include <random>
#include <utility>
#include <vector>
#include <string>

#include "APCProblem.h"
#include "APCSolution.h"
#include "APCRelief.h"
#include "SRandom.h"
#include "APC5x2Partition.h"
#include "APC5FoldPartition.h"
#include "APCLocalSearch.h"
#include "APCRandom.h"
#include "APCGenetic.h"
#include "APCMemetic.h"
#include "APC1NNEval.h"
#include "APCSimulatedAnnealing.h"
#include "APCILS.h"
#include "APCDifferentialEvolution.h"
#include "APCSCA.h"


using namespace std;

void printHelp(){
    cout << "PROGRAM USE:" << endl
         << "apc [problem file] [options]" << endl
         << "OPTIONS:" << endl
         << "-a <algorithm>: Specifies the algorithm to use in the program (necessary). Allowed algorithms are:" << endl
         << "\t1NN: Evaluates 1NN classifier over a 1.0-weighted solution (unless -w is specified)" << endl
         << "\tRANDOM: Generates random solutions uniformely distributed in [0,1]." << endl
         << "\tRELIEF: Obtains a solution using the greedy algorithm RELIEF." << endl    
         << "\tRANDOM+LS: Improves a random solution with local search." << endl
         << "\tRELIEF+LS: Improves a relief solution with local search." << endl
         << "\tAGG-BLX: Obtains solutions using a genetic generational algorithm with the cross operator BLX-0.3." << endl
         << "\tAGG-CA: Obtains solutions using a genetic generational algorithm with the Arithmetic cross operator." << endl
         << "\tAGE-BLX: Obtains solutions using a genetic stationary algorithm with the cross operator BLX-0.3." << endl
         << "\tAGG-CA: Obtains solutions using a genetic stationary algorithm with the Arithmetic cross operator." << endl
         << "\tAM-10-1.0: Memetic algorithm that applies local search to every individual each 10 generations." << endl
         << "\tAM-10-0.1: Memetic algorithm that applies local search to 10% of population each 10 generations." << endl
         << "\tAM-10-0.1mej: Memetic algorithm that applies local search to the best 10% of the population each 10 generations." << endl
         << "\tSA: Obtains a solution using Simulated Annealing with Cauchy cooling scheme." << endl
         << "\tILS: Obtains a solution using Iterated Local Search." << endl
         << "\tDE-RAND: Obtains solutions using a Differential Evolution algoritm with the cross operator 'rand'." << endl
         << "\tDE-CURRENTTOBEST: Obtains solutions using a Differential Evolution algorithm with the cross operator 'current-to-best'." << endl
         << "\tSCA: Sine-Cosine Algorithm." << endl
         //<< "\tSCA+: Sine-Cosine Algorithm improved" << endl
         << "-m <mode>: Fitness evaluation mode. Default is 5-Fold. Allowed modes are:" << endl
         << "\t5x2: Evaluates 1NN classifier with a 5x2 Cross Validation." << endl
         << "\t5FOLD: Evaluates the mean of reduction rate and 1NN classification rate with a 5-Fold Cross Validation." << endl
         << "-o <output_name>: Specifies the name that output files will have. Extensions will be added by the program. Needed for -t and -p options." << endl
         << "-p <string>: Specifies which data will be printed in files. These kinds of data will be specified in a string, with these characters allowed:" << endl
         << "\tf: A file with fitnesses data will be created." << endl
         << "\tp: A file with partitions data (indexes for each partition) will be created." << endl
         << "\tt: A file with times data will be created." << endl
         << "\ti: A file with training fitnesses data will be created." << endl
         << "\ts: A file with solutions will be created." << endl
         << "-s <seed>: Specifies a seed for random numbers generation. Without this option, seed will obtained using current time." << endl
         << "-t <string>: With this option, a table will be written in a file, with the data specified in the given string. Characters allowed are the same in -p, except for 's' and 'p'" << endl
         << "-w <file>: It only will be considered when algorithm is 1NN. specifies a file where a solution is stored, and tests it with the classifier."
         << endl << "EXAMPLE OF USE: apc ./data/sonar.arff -a RELIEF -s 3 -o ./sol/relief_sonar -t fti -p ftisp";
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

void printOutput5x2(APCAlgorithm & a, const string & table_format, ostream & fout_table,
        const string & output_fits, ostream & fout_fits,
        const string & output_trains, ostream & fout_trains,
        const string & output_times, ostream & fout_times,
        const string & output_sols, ostream & fout_sols ){
    for(int i = 0; i < 10; i++){
        cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << a.getFitness(i) << "\tTRAIN FIT = " << a.getTrainFit(i) << ";\tTIME = " << a.getTime(i) << endl;
    }

    if(table_format != "") a.writeTable5x2(fout_table, table_format);
    if(output_fits != "") a.writeFitnesses(fout_fits);
    if(output_trains != "") a.writeTrainFits(fout_trains);
    if(output_times != "") a.writeTimes(fout_times);
    if(output_sols != "") a.writeSolutions(fout_sols);
}

void printOutput5Fold(APCAlgorithm & a, const string & table_format, ostream & fout_table,
        const string & output_fits, ostream & fout_fits,
        const string & output_trains, ostream & fout_trains,
        const string & output_times, ostream & fout_times,
        const string & output_sols, ostream & fout_sols){

    for(int i = 0; i < 5; i++){
        cout << "PARTITION " << i << ":\tFITNESS = " << a.getFitness(i) << ";\tCLASS_RATE = " << a.getClassRate(i) << ";\tRED_RATE = " << a.getRedRate(i) << ";\tTRAIN FIT = " << a.getTrainFit(i) << ";\tTIME = " << a.getTime(i) << endl;
    }

    if(table_format != "") a.writeTable5Fold(fout_table, table_format);
    if(output_fits != "") a.writeFitnesses(fout_fits);
    if(output_trains != "") a.writeTrainFits(fout_trains);
    if(output_times != "") a.writeTimes(fout_times);
    if(output_sols != "") a.writeSolutions(fout_sols);

}

int main(int argc, char const *argv[])
{


    vector <pair<char,string>> input = parseInput(argc,argv);

    //Data
    string problem_name(argv[1]);
    unsigned seed = time(NULL);
    string algorithm = "";
    string table_format = "";
    string output_name = "";
    string data_prints = "";
    string sol_file = "";
    string mode = "5FOLD";

    APCProblem problem(problem_name);

    APCRelief relief(&problem);
    APCLocalSearch LS(&problem);
    APCRandom apc_random(&problem);
    APCGeneticGenerational agg(&problem);
    APCGeneticStationary age(&problem);
    APCMemetic am(&problem);
    APC_1NN_Eval nn1(&problem);
    APCSimulatedAnnealing sa(&problem);
    APCILS ils(&problem);
    APCDifferentialEvolution de(&problem);
    APCSCA sca(&problem);

    //Parse args
    for(unsigned i = 0; i < input.size(); i++){
        switch(input[i].first){
            case 's':
                seed = stoul(input[i].second);
                break;
            case 'a':
                algorithm = input[i].second;
                break;
            case 't':
                table_format = input[i].second;
                break;
            case 'o':
                output_name = input[i].second;
                break;
            case 'p':
                data_prints = input[i].second;
                break;
            case 'w':
                sol_file = input[i].second;
                break;
            case 'm':
                mode = input[i].second;
                break;
            default:
                printHelp();
        }
    }

    if(mode != "5x2" && mode != "5FOLD"){
        cout << "INVALID MODE" << endl;
        printHelp();
    }

    //Output variables.
    if(output_name == "" && (table_format != "" || data_prints != "")){
        cout << "THERE IS NOT OUTPUT FILE" << endl;
        printHelp();
    }

    string output_table = output_name+".table";

    string output_sols = "";
    string output_fits = "";
    string output_trains = "";
    string output_times = "";
    string output_parts = "";

    for(unsigned i = 0; i < data_prints.size(); i++){
        switch(data_prints[i]){
            case 'f':
                output_fits = output_name+".fit";
                break;
            case 't':
                output_times = output_name+".time";
                break;
            case 'i':
                output_trains = output_name+".trfit";
                break;
            case 's':
                output_sols = output_name+".sol";
                break;
            case 'p':
                output_parts = output_name+".part";
                break;
            default:
                cout << "INCORRECT OPTION FOR -p" << endl;
                printHelp();
        }
    }

    //Output streams
    ofstream fout_table;
    ofstream fout_fits;
    ofstream fout_trains;
    ofstream fout_times;
    ofstream fout_sols;
    ofstream fout_parts;

    if(table_format != "") fout_table.open(output_table.c_str());
    if(output_fits != "") fout_fits.open(output_fits.c_str());
    if(output_trains != "") fout_trains.open(output_trains.c_str());
    if(output_times != "") fout_times.open(output_times.c_str());
    if(output_sols != "") fout_sols.open(output_sols.c_str());
    if(output_parts != "") fout_parts.open(output_parts.c_str());



    //Set random seed.
    SRandom::getInstance().setSeed(seed);


    //Create partitions
    APC5x2Partition myPartition5x2(&problem);
    APC5FoldPartition myPartition5F(&problem);

    //Output partition
    if(output_parts != "") fout_parts << myPartition5x2;


    cout << "PROBLEMA " << problem.getFileName() << endl;

    if(algorithm == "RELIEF" || algorithm == "RELIEF+LS"){

        if(mode == "5x2"){   
            relief.solve5x2(myPartition5x2);
        
            if(algorithm == "RELIEF"){
                cout << relief.getAlgorithmName() << endl;
                printOutput5x2(relief, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
                
            }

            if(algorithm == "RELIEF+LS"){
                vector<APCSolution *> relief_sols = relief.getSolutions();
                LS.solve5x2(myPartition5x2,relief_sols);
                cout << relief.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
                printOutput5x2(LS, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);

            }
        }

        else if(mode == "5FOLD"){
            relief.solve5Fold(myPartition5F);
        
            if(algorithm == "RELIEF"){
                cout << relief.getAlgorithmName() << endl;
                printOutput5Fold(relief, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
                
            }

            if(algorithm == "RELIEF+LS"){
                vector<APCSolution *> relief_sols = relief.getSolutions();
                LS.solve5Fold(myPartition5F,relief_sols);
                cout << relief.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
                printOutput5Fold(LS, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);

            }
        }

    } 

    else if(algorithm == "RANDOM" || algorithm == "RANDOM+LS"){

        if(mode == "5x2"){
            apc_random.solve5x2(myPartition5x2);

            if(algorithm == "RANDOM"){
                cout << apc_random.getAlgorithmName() << endl;
                printOutput5x2(apc_random, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
               
            }

            if(algorithm == "RANDOM+LS"){
                vector<APCSolution *> random_sols = apc_random.getSolutions();
                LS.solve5x2(myPartition5x2,random_sols);
                cout << apc_random.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
                printOutput5x2(LS, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
               
            }
        }

        else if(mode == "5FOLD"){
            apc_random.solve5Fold(myPartition5F);

            if(algorithm == "RANDOM"){
                cout << apc_random.getAlgorithmName() << endl;
                printOutput5Fold(apc_random, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
               
            }

            if(algorithm == "RANDOM+LS"){
                vector<APCSolution *> random_sols = apc_random.getSolutions();
                LS.solve5Fold(myPartition5F,random_sols);
                cout << apc_random.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
                printOutput5Fold(LS, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
               
            }
        }
    }

    else if(algorithm == "AGG-BLX"){

        if(mode == "5x2"){
            agg.solve5x2(myPartition5x2,APCGenetic::BLXCross03);
    
            cout << agg.getAlgorithmName() << endl;
            printOutput5x2(agg, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

        else if(mode == "5FOLD"){
            agg.solve5Fold(myPartition5F,APCGenetic::BLXCross03);
    
            cout << agg.getAlgorithmName() << endl;
            printOutput5Fold(agg, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

    }

    else if(algorithm == "AGG-CA"){

        if(mode == "5x2"){
            agg.solve5x2(myPartition5x2,APCGenetic::arithmeticCross);

            cout << agg.getAlgorithmName() << endl;
            //cout << "REACHED GENERATION " << agg.getGeneration() << endl;
            printOutput5x2(agg, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
       
        }

        else if(mode == "5FOLD"){
            agg.solve5Fold(myPartition5F,APCGenetic::arithmeticCross);

            cout << agg.getAlgorithmName() << endl;
            //cout << "REACHED GENERATION " << agg.getGeneration() << endl;
            printOutput5Fold(agg, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
       
        }
         
    }

    else if(algorithm == "AGE-BLX"){

        if(mode == "5x2"){
            age.solve5x2(myPartition5x2,APCGenetic::BLXCross03,30,1.0);
    
            cout << age.getAlgorithmName() << endl;
            //cout << "REACHED GENERATION " << age.getGeneration() << endl;
            printOutput5x2(age, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

        else if(mode == "5FOLD"){
            age.solve5Fold(myPartition5F,APCGenetic::BLXCross03,30,1.0);
    
            cout << age.getAlgorithmName() << endl;
            //cout << "REACHED GENERATION " << age.getGeneration() << endl;
            printOutput5Fold(age, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        
        }
    }

    else if(algorithm == "AGE-CA"){

        if(mode == "5x2"){
            age.solve5x2(myPartition5x2,APCGenetic::arithmeticCross,30,1.0);

            cout << age.getAlgorithmName() << endl;
            //cout << "REACHED GENERATION " << age.getGeneration() << endl;
            printOutput5x2(age, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

        else if(mode == "5FOLD"){
            age.solve5Fold(myPartition5F,APCGenetic::arithmeticCross,30,1.0);

            cout << age.getAlgorithmName() << endl;
            //cout << "REACHED GENERATION " << age.getGeneration() << endl;
            printOutput5Fold(age, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
       
        }
    }

    else if(algorithm == "AM-10-1.0"||algorithm == "AM-10-0.1"||algorithm == "AM-10-0.1mej"){
        
        if(mode == "5x2"){
            if(algorithm == "AM-10-1.0") am.solve5x2(myPartition5x2,10,1.0,false);
            else if(algorithm == "AM-10-0.1") am.solve5x2(myPartition5x2,10,0.1,false);
            else if(algorithm == "AM-10-0.1mej") am.solve5x2(myPartition5x2,10,0.1,true);

            cout << am.getAlgorithmName() << endl;
            printOutput5x2(am, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

        else if(mode == "5FOLD"){
            if(algorithm == "AM-10-1.0") am.solve5Fold(myPartition5F,10,1.0,false);
            else if(algorithm == "AM-10-0.1") am.solve5Fold(myPartition5F,10,0.1,false);
            else if(algorithm == "AM-10-0.1mej") am.solve5Fold(myPartition5F,10,0.1,true);

            cout << am.getAlgorithmName() << endl;
            printOutput5Fold(am, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        
        }
    }

    else if(algorithm == "SA"){
        int n_attr = problem.getNumNonClassAttributes();
        if(mode=="5x2"){
            sa.solve5x2(myPartition5x2,10*n_attr,n_attr);

            cout << sa.getAlgorithmName() << endl;
            printOutput5x2(sa, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);

        }

        else if(mode=="5FOLD"){
            sa.solve5Fold(myPartition5F,10*n_attr,n_attr);

            cout << sa.getAlgorithmName() << endl;
            printOutput5Fold(sa, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }
    }

    else if(algorithm == "ILS"){
        int n_attr = problem.getNumNonClassAttributes();
        if(mode=="5x2"){
            ils.solve5x2(myPartition5x2,0.1*n_attr);

            cout << ils.getAlgorithmName() << endl;
            printOutput5x2(ils, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

        else if(mode=="5FOLD"){
            ils.solve5Fold(myPartition5F,0.1*n_attr);

            cout << ils.getAlgorithmName() << endl;
            printOutput5Fold(ils, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }
    }

    else if(algorithm == "DE-RAND"||algorithm == "DE-CURRENTTOBEST"){
        if(mode == "5x2"){
            if(algorithm == "DE-RAND") de.solve5x2(myPartition5x2,APCDifferentialEvolution::DERand);
            else if(algorithm == "DE-CURRENTTOBEST") de.solve5x2(myPartition5x2,APCDifferentialEvolution::DECurrentToBest);

            cout << de.getAlgorithmName() << endl;
            printOutput5x2(de, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

        else if(mode == "5FOLD"){
            if(algorithm == "DE-RAND") de.solve5Fold(myPartition5F,APCDifferentialEvolution::DERand);
            else if(algorithm == "DE-CURRENTTOBEST") de.solve5Fold(myPartition5F,APCDifferentialEvolution::DECurrentToBest);

            cout << de.getAlgorithmName() << endl;
            printOutput5Fold(de, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        
        }
    }

    else if(algorithm == "SCA"){
        if(mode=="5x2"){
            sca.solve5x2(myPartition5x2);

            cout << sca.getAlgorithmName() << endl;
            printOutput5x2(sca, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }

        else if(mode == "5FOLD"){
            sca.solve5Fold(myPartition5F);

            cout << sca.getAlgorithmName() << endl;
            printOutput5Fold(sca, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        }
    }

    else if(algorithm == "1NN"){
        if(sol_file == ""){
            APCSolution s1 = APCSolution::weight1Solution(&problem);
            if(mode == "5x2") nn1.solve5x2(myPartition5x2,s1);
            else if(mode == "5FOLD") nn1.solve5Fold(myPartition5F,s1);
        }
        else{
            ifstream s_in(sol_file.c_str());
            APCSolution s_test(&problem);
            s_in >> s_test;
            if(mode == "5x2") nn1.solve5x2(myPartition5x2,s_test);
            else if(mode == "5FOLD") nn1.solve5Fold(myPartition5F,s_test);
        }

        cout << nn1.getAlgorithmName() << endl;
        if(mode == "5x2") printOutput5x2(nn1, table_format, fout_table, output_fits, fout_fits, output_trains, fout_trains, output_times, fout_times, output_sols, fout_sols);
        else if(mode == "5FOLD") printOutput5Fold(nn1,table_format,fout_table,output_fits,fout_fits,output_trains,fout_trains,output_times, fout_times, output_sols, fout_sols); 
    }

    else{
        cout << "INVALID ALGORITHM" << endl;
        printHelp();
    }

    return 0;
}