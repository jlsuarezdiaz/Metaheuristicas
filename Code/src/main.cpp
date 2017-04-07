
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
#include "APCLocalSearch.h"
#include "APCRandom.h"
#include "APCGenetic.h"
#include "APCMemetic.h"



using namespace std;

void printHelp(){
    cout << "PROGRAM USE:" << endl
         << "apc [problem file] [options]" << endl
         << "OPTIONS:" << endl
         << "-a <algorithm>: Specifies the algorithm to use in the program (necessary). Allowed algorithms are:" << endl
         << "\t1NN: Evaluates 1NN classifier over a 1.0-weighted solution." << endl
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
         << "-o <output_name>: Specifies the name that output files will have. Extensions will be added by the program. Needed for -t and -p options." << endl
         << "-p <string>: Specifies which data will be printed in files. These kinds of data will be specified in a string, with these characters allowed:" << endl
         << "\tf: A file with fitnesses data will be created." << endl
         << "\tt: A file with times data will be created." << endl
         << "\ti: A file with training fitnesses data will be created." << endl
         << "\ts: A file with solutions will be created." << endl
         << "-s <seed>: Specifies a seed for random numbers generation." << endl
         << "-t <string>: With this option, a table will be written in a file, with the data specified in the given string. Characters allowed are the same in -p, except for 's'" << endl
         << endl << "EXAMPLE OF USE: apc ./data/sonar.arff -a RELIEF -s 3 -o ./sol/relief_sonar -t fti -p ftis";
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

void printOutput5x2(APCAlgorithm & a){
    for(int i = 0; i < 10; i++){
        cout << "PARTITION (" << i/2 << "," << i%2 << "):\tFITNESS = " << a.getFitness(i) << "\tTRAIN FIT = " << a.getTrainFit(i) << ";\tTIME = " << a.getTime(i) << endl;
    }
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

    APCProblem problem(problem_name);

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
            case 't':
                table_format = input[i].second;
                break;
            case 'o':
                output_name = input[i].second;
                break;
            case 'p':
                data_prints = input[i].second;
                break;
            default:
                printHelp();
        }
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

    if(table_format != "") fout_table.open(output_table.c_str());
    if(output_fits != "") fout_fits.open(output_fits.c_str());
    if(output_trains != "") fout_trains.open(output_trains.c_str());
    if(output_times != "") fout_times.open(output_times.c_str());
    if(output_sols != "") fout_sols.open(output_sols.c_str());

    //Set random seed.
    SRandom::getInstance().setSeed(seed);
    APC5x2Partition myPartition(&problem);


    cout << "PROBLEMA " << problem.getFileName() << endl;

    if(algorithm == "RELIEF" || algorithm == "RELIEF+LS"){    
        relief.solve5x2(myPartition);
    
        if(algorithm == "RELIEF"){
            cout << relief.getAlgorithmName() << endl;
            printOutput5x2(relief);
            if(table_format != "") relief.writeTable5x2(fout_table, table_format);
            if(output_fits != "") relief.writeFitnesses(fout_fits);
            if(output_trains != "") relief.writeTrainFits(fout_trains);
            if(output_times != "") relief.writeTimes(fout_times);
            if(output_sols != "") relief.writeSolutions(fout_sols);
        }

        if(algorithm == "RELIEF+LS"){
            vector<APCSolution *> relief_sols = relief.getSolutions();
            LS.solve5x2(myPartition,relief_sols);
            cout << relief.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
            printOutput5x2(LS);
            if(table_format != "") LS.writeTable5x2(fout_table, table_format);
            if(output_fits != "") LS.writeFitnesses(fout_fits);
            if(output_trains != "") LS.writeTrainFits(fout_trains);
            if(output_times != "") LS.writeTimes(fout_times);
            if(output_sols != "") LS.writeSolutions(fout_sols);
        }

    } 

    else if(algorithm == "RANDOM" || algorithm == "RANDOM+LS"){
        apc_random.solve5x2(myPartition);

        if(algorithm == "RANDOM"){
            cout << apc_random.getAlgorithmName() << endl;
            printOutput5x2(apc_random);
            if(table_format != "") apc_random.writeTable5x2(fout_table, table_format);
            if(output_fits != "") apc_random.writeFitnesses(fout_fits);
            if(output_trains != "") apc_random.writeTrainFits(fout_trains);
            if(output_times != "") apc_random.writeTimes(fout_times);
            if(output_sols != "") apc_random.writeSolutions(fout_sols);
        }

        if(algorithm == "RANDOM+LS"){
            vector<APCSolution *> random_sols = apc_random.getSolutions();
            LS.solve5x2(myPartition,random_sols);
            cout << apc_random.getAlgorithmName() << "+" << LS.getAlgorithmName() << endl;
            printOutput5x2(LS);
            if(table_format != "") LS.writeTable5x2(fout_table, table_format);
            if(output_fits != "") LS.writeFitnesses(fout_fits);
            if(output_trains != "") LS.writeTrainFits(fout_trains);
            if(output_times != "") LS.writeTimes(fout_times);
            if(output_sols != "") LS.writeSolutions(fout_sols);
        }
    }

    else if(algorithm == "AGG-BLX"){
        //ofstream fout("./sol/sols_agg.sol");
        agg.solve5x2(myPartition,APCGenetic::BLXCross03);

        cout << agg.getAlgorithmName() << endl;
        printOutput5x2(agg);
        if(table_format != "") agg.writeTable5x2(fout_table, table_format);
        if(output_fits != "") agg.writeFitnesses(fout_fits);
        if(output_trains != "") agg.writeTrainFits(fout_trains);
        if(output_times != "") agg.writeTimes(fout_times);
        if(output_sols != "") agg.writeSolutions(fout_sols);
    }

    else if(algorithm == "AGG-CA"){
        agg.solve5x2(myPartition,APCGenetic::arithmeticCross);

        cout << agg.getAlgorithmName() << endl;
        printOutput5x2(agg);
        if(table_format != "") agg.writeTable5x2(fout_table, table_format);
        if(output_fits != "") agg.writeFitnesses(fout_fits);
        if(output_trains != "") agg.writeTrainFits(fout_trains);
        if(output_times != "") agg.writeTimes(fout_times);
        if(output_sols != "") agg.writeSolutions(fout_sols);
    }

    else if(algorithm == "AGE-BLX"){
        age.solve5x2(myPartition,APCGenetic::BLXCross03,30,1.0);

        cout << age.getAlgorithmName() << endl;
        printOutput5x2(age);
        if(table_format != "") age.writeTable5x2(fout_table, table_format);
        if(output_fits != "") age.writeFitnesses(fout_fits);
        if(output_trains != "") age.writeTrainFits(fout_trains);
        if(output_times != "") age.writeTimes(fout_times);
        if(output_sols != "") age.writeSolutions(fout_sols);
    }

    else if(algorithm == "AGE-CA"){
        age.solve5x2(myPartition,APCGenetic::arithmeticCross,30,1.0);

        cout << age.getAlgorithmName() << endl;
        printOutput5x2(age);
        if(table_format != "") age.writeTable5x2(fout_table, table_format);
        if(output_fits != "") agg.writeFitnesses(fout_fits);
        if(output_trains != "") agg.writeTrainFits(fout_trains);
        if(output_times != "") agg.writeTimes(fout_times);
        if(output_sols != "") agg.writeSolutions(fout_sols);
    }

    else if(algorithm == "AM-10-1.0"||algorithm == "AM-10-0.1"||algorithm == "AM-10-0.1mej"){
        if(algorithm == "AM-10-1.0") am.solve5x2(myPartition,10,1.0,false);
        else if(algorithm == "AM-10-0.1") am.solve5x2(myPartition,10,0.1,false);
        else if(algorithm == "AM-10-0.1mej") am.solve5x2(myPartition,10,0.1,true);

        cout << am.getAlgorithmName() << endl;
        printOutput5x2(am);
        if(table_format != "") am.writeTable5x2(fout_table, table_format);
        if(output_fits != "") am.writeFitnesses(fout_fits);
        if(output_trains != "") am.writeTrainFits(fout_trains);
        if(output_times != "") am.writeTimes(fout_times);
        if(output_sols != "") am.writeSolutions(fout_sols);
    }

    else if(algorithm == "1NN"){
        Timer timer;
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 2; j++){
                APCSolution s1 = APCSolution::weight1Solution(&problem);
                timer.start();
                float fit1 = APC_1NN::fitness(myPartition[i][j],s1);
                timer.stop();
                cout << "PARTITION (" << i << "," << j << "):\tFITNESS = " << fit1 << ";\tTIME = " << timer.get_time() << endl; 
            }
        }
    }

    else{
        cout << "ALGORITMO INVÁLIDO" << endl;
        printHelp();
    }

    return 0;
}