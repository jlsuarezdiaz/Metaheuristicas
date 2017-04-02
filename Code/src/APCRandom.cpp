# include "APCRandom.h"

APCRandom::APCRandom(const APCProblem *p)
    :problem(p), solutions(),  algorithm_name("RANDOM")
{}

APCSolution * APCRandom::solve(const APCPartition &p){
    APCSolution *solution = new APCSolution(problem);
    solutions.push_back(solution);

    timer.start();
    const float a = 0.0, b = 1.0;
    for(int i = 0; i < solution->size(); i++){
        (*solution)[i] = SRandom::getInstance().getRealUniformDistributionElement(a,b);
    }
    timer.stop();

    times.push_back(timer.get_time());

    train_fits.push_back(APC_1NN(&p,solution).fitness());
    fitnesses.push_back(-1.0);
    return solution;
}

void APCRandom::solve5x2(const APC5x2Partition & p5x2){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p5x2[i][j]);
            APC_1NN classifier(&p5x2[i][(j+1)%2],s);
            fitnesses[fitnesses.size()-1] = classifier.fitness();
        }
    }
}

void APCRandom::clearSolutions(){
    for(unsigned i = 0; i < solutions.size(); i++){
        delete solutions[i];
    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
}

APCRandom::~APCRandom(){
    clearSolutions();
}