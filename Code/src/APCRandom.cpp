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
    return solution;
}

void APCRandom::solve5x2(const APC5x2Partition & p5x2){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p5x2[i][j]);
            fitnesses.push_back(APC_1NN::fitness(p5x2[i][(j+1)%2],*s));
        }
    }
}

void APCRandom::solve5Fold(const APC5FoldPartition & p){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0]); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::fitness(p[i][1],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}


APCRandom::~APCRandom(){
    clearSolutions();
}