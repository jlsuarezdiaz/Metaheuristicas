# include "APC1NNEval.h"

APC_1NN_Eval::APC_1NN_Eval(const APCProblem *p)
    :APCAlgorithm(p,"1NN")
{}

APC_1NN_Eval::~APC_1NN_Eval(){
    clearSolutions();
}

void APC_1NN_Eval::solve5x2(const APC5x2Partition & p5x2, const APCSolution & s){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            //APCSolution s = APCSolution::weight1Solution(problem);
            timer.start(); // Partición test (por seguir la estructura, en realidad esto es tontería)
            float fit_test = APC_1NN::testFitness(p5x2[i][(j+1)%2],p5x2[i][j],s);
            timer.stop();
            fitnesses.push_back(fit_test);

            float fit_train = APC_1NN::fitness(p5x2[i][j],s);
            train_fits.push_back(fit_train);

            solutions.push_back(new APCSolution(s));
            times.push_back(timer.get_time());
        }
    }
}

void APC_1NN_Eval::solve5Fold(const APC5FoldPartition & p, const APCSolution & s){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        timer.start();
        vector<float> fit_test = APCTargetCR::testFitness(p[i][1],p[i][0],s);
        timer.stop();
        class_rates.push_back(fit_test[0]);
        red_rates.push_back(fit_test[1]);
        fitnesses.push_back(fit_test[2]);

        vector <float> fit_train = APCTargetCR::fitness(p[i][0],s);
        train_fits.push_back(fit_train[2]);

        solutions.push_back(new APCSolution(s));
        times.push_back(timer.get_time());
    }
}