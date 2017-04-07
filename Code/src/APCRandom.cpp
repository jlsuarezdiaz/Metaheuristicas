# include "APCRandom.h"

APCRandom::APCRandom(const APCProblem *p)
    :APCAlgorithm(p,"RANDOM")
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

    train_fits.push_back(APC_1NN::fitness(p,*solution));
    fitnesses.push_back(-1.0);
    return solution;
}

void APCRandom::solve5x2(const APC5x2Partition & p5x2){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p5x2[i][j]);
            fitnesses[fitnesses.size()-1] = APC_1NN::fitness(p5x2[i][(j+1)%2],*s);
        }
    }
}



APCRandom::~APCRandom(){
    clearSolutions();
}