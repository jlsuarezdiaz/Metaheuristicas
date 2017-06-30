#include "APCSCA.h"

APCSCA::APCSCA(const APCProblem *p)
    :APCAlgorithm(p,"SCA")
{}



APCSolution * APCSCA::solve(const APCPartition &p, targetFunction fitness, int max_evals){
    const int pop_size = 30;
    vector<APCSolution*> pop;
    vector<float> vals(pop_size);

    int d_i = 0;
    float destination_fit = -1.0;
    APCSolution * destination = NULL;

    timer.start();

    for(unsigned i = 0; i < pop_size; i++){
        pop.push_back(new APCSolution(APCSolution::randomSolution(problem)));
        vals[i] = fitness(p,*pop[i]);
        if(vals[i] > destination_fit){
            d_i = i;
            destination_fit = vals[i];
        }
    }


    destination = new APCSolution(*pop[d_i]);

    unsigned t = 1;
    unsigned max_its = max_evals / (pop.size());

    int num_evals = 0;

    const float a = 2.0;
    float r1, r2, r3, r4;

    const float min2 = 0.0, max2 = 8*atan(1.0),  //2*PI
                min3 = 0.0, max3 = 2.0,
                min4 = 0.0, max4 = 1.0;

    while(num_evals < max_evals){
        r1 = a-t*((a-1e-8)/max_its);

        for(unsigned i = 0; i < pop.size(); i++){
            for(int j = 0; j < problem->getNumNonClassAttributes(); j++){
                r2 = SRandom::getInstance().getRealUniformDistributionElement(min2,max2);
                r3 = SRandom::getInstance().getRealUniformDistributionElement(min3,max3);
                r4 = SRandom::getInstance().getRealUniformDistributionElement(min4,max4);

                if(r4 < 0.5){
                    (*pop[i])[j] += (r1*sin(r2)*fabs(r3*(*destination)[j]-(*pop[i])[j]));
                }
                else{
                    (*pop[i])[j] += (r1*cos(r2)*fabs(r3*(*destination)[j]-(*pop[i])[j]));
                }
                if((*pop[i])[j] > 1.0) (*pop[i])[j] = 1.0;
                if((*pop[i])[j] < 0.0) (*pop[i])[j] = 0.0;
            }
        }
        destination_fit = -1.0;

        for(unsigned i = 0; i < pop.size(); i++){
            vals[i] = fitness(p,*pop[i]);
            num_evals++;

            if(vals[i] > destination_fit){
                d_i = i;
                destination_fit = vals[i];
            }
        }

        delete destination;
        destination = new APCSolution(*pop[d_i]);

        t++;
    }

    timer.stop();

    solutions.push_back(new APCSolution(*destination));

    times.push_back(timer.get_time());
    train_fits.push_back(destination_fit);

    delete destination;
    destination = NULL;

    return solutions.back();

}

void APCSCA::solve5x2(const APC5x2Partition & p5x2, int max_evals){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p5x2[i][j], target1NN,max_evals);
            fitnesses.push_back(APC_1NN::testFitness(p5x2[i][(j+1)%2],p5x2[i][j],*s));
        }
    }
}

void APCSCA::solve5Fold(const APC5FoldPartition & p, int max_evals){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0],target1NNred,max_evals); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::testFitness(p[i][1],p[i][0],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}



APCSCA::~APCSCA(){
    clearSolutions();
}