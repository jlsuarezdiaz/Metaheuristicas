#include "APCILS.h"

APCILS::APCILS(const APCProblem *p)
    :APCAlgorithm(p,"ILS"), ls(p)
{
    this->permutation = new int[p->getNumNonClassAttributes()];
    for(int i = 0; i < p->getNumNonClassAttributes(); i++){
        permutation[i]=i;
    }
}


APCSolution * APCILS::solve(const APCPartition & p, targetFunction fitness, int num_mutations, float sigma_mutation, int max_evals_ls, float sigma_ls, int num_its, APCSolution *s0){
    timer.start();

    float fit;//, best_cost;

    APCSolution *s;
    if(s0 == NULL) s = new APCSolution(APCSolution::randomSolution(problem));
    else s = new APCSolution(*s0);
    ls.solve(p,s,fitness,max_evals_ls,max_evals_ls,sigma_ls); //Debería modificar s (comprobar)

    //APCSolution *best_solution = new APCSolution(*s);
    fit = fitness(p,*s);

    int rnd_i = 0;
    int p_size = problem->getNumNonClassAttributes();

    for(int i = 1; i < num_its; i++){
        APCSolution *s_ = new APCSolution(*s);
        for(int i = 0; i < num_mutations; i++){
            if(rnd_i==0) random_shuffle(permutation,permutation+p_size);
            int rnd = permutation[rnd_i];//SRandom::getInstance().rand(0,s->size()-1);
            rnd_i = (rnd_i+1)%p_size;
            //int rnd = SRandom::getInstance().rand(0,s_->size()-1);
            s_->move(rnd,sigma_mutation);
        }

        ls.solve(p,s_,fitness,max_evals_ls,max_evals_ls,sigma_ls);
        float newfit = fitness(p,*s_);


        if(newfit > fit){
            delete s;
            s = s_;
            fit = newfit;
            //if(fit > best_cost){
            //    delete best_solution;
            //    best_solution = new APCSolution(*s);
            //    best_cost = fit;
            //}
        }
        else{
            delete s_;
        }

        //cout << i << " " << best_cost << endl;
        
    }
       
    timer.stop();

    solutions.push_back(new APCSolution(*s));

    times.push_back(timer.get_time());
    train_fits.push_back(fit);

    //delete best_solution;
    //best_solution = NULL;

    return solutions.back();
}


void APCILS::solve5Fold(const APC5FoldPartition & p, int num_mutations, float sigma_mutation, int max_evals_ls, float sigma_ls, int num_its, APCSolution *s0){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0],target1NNred,num_mutations,sigma_mutation,max_evals_ls,sigma_ls,num_its,s0); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::testFitness(p[i][1],p[i][0],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}

void APCILS::solve5x2(const APC5x2Partition & p, int num_mutations, float sigma_mutation, int max_evals_ls, float sigma_ls, int num_its, APCSolution *s0){
    clearSolutions();
    
    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            //p_test = &p[i][(j+1)%2];  //!!!
            APCSolution *s = solve(p[i][j],target1NN,num_mutations,sigma_mutation,max_evals_ls,sigma_ls,num_its,s0);
            this->fitnesses.push_back(APC_1NN::testFitness(p[i][(j+1)%2],p[i][j],*s));
        }
    }
}