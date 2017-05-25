# include "APCMemetic.h"

APCMemetic::APCMemetic(const APCProblem *p)
:APCAlgorithm(p,"MEMETIC"), agg(p), ls(p)
{}



APCSolution * APCMemetic::solve(const APCPartition & p_train, targetFunction fitness, int ls_gens, float ls_prob , bool mej, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations, int ls_neighbour_evals_rate){
    algorithm_model = std::to_string(ls_gens)+"-"+std::to_string(ls_prob)+(mej?"-MEJ":"");
    int num_evaluations = 0;

    timer.start();

    agg.generatePopulation(p_train,fitness,population_size);
    agg.setParameters(cross_prob,mutation_prob);
    agg.resetEvaluations();

    //!!!
    //cout << population_size << endl;
    //vector<Individual*> expe;
    //expe.resize(6);
    //static int ixx = 0;
    //ixx++;
    //ofstream fout[6];
    //for(int i = 0; i < 6; i++){
    //    fout[i].open(("./sol/experiments/AM_evol3_"+std::to_string(ixx)+"_"+std::to_string(i)+".sol").c_str());
    //    expe[i] = agg.getPopulation()[i];
    //}
    //!!!

    int num_ls_inds = ls_prob * population_size; //Número de individuos a los que se aplicará la LS
    int neighbour_evals = ls_neighbour_evals_rate*problem->getNumNonClassAttributes();
    float best_val;
    Individual * best = NULL;

    while(num_evaluations < max_evaluations){

        for(int i = 0; i < ls_gens; i++){
            agg.nextGeneration(p_train,c,fitness);
        }

        vector<Individual*> solutions = agg.getPopulation();

        //cout << "GENETICO: " << timer.get_time() << endl;

        //for(unsigned i = 0; i < solutions.size(); i++){
        //    cout << solutions[i]->val << endl;
        //}

        if(num_ls_inds != population_size){ //Para no hacer el tonto si la prob es 1
            if(mej){
                sort(solutions.begin(),solutions.end(),IndividualComparator());
            }
            else{
                random_shuffle(solutions.begin(),solutions.end());
            }
        }
        //cout << "SELECCION PARA LS: " << timer.get_time() << endl;

        best_val = -1.0;
        best = NULL;

        for(int i = 0; i < num_ls_inds; i++){
            ls.solve(p_train,solutions[i]->s,fitness,2,neighbour_evals);
            solutions[i]->val = ls.getLastTrainFit();
            if(solutions[i]->val > best_val){
                best_val = solutions[i]->val;
                best = solutions[i];
            }
        }


        //cout << "LS: " << timer.get_time() << endl;

        ls.clearSolutions();
        agg.recalcBestSolution(best);
        
        num_evaluations+=(agg.getEvaluations() + neighbour_evals * num_ls_inds);
        agg.resetEvaluations();

        //cout << "RESTART: " << timer.get_time() << endl;
        //cout << "EVALS: " << num_evaluations << endl;
    }

    timer.stop();

    solutions.push_back(new APCSolution(*(best->s)));
    times.push_back(timer.get_time());
    train_fits.push_back(best->val);

    //cout << "NUM GENS = " << agg.getGeneration() << endl;
    //cout << "TIEMPO = " << timer.get_time() << endl;

    agg.clearPopulation();


    return solutions.back();
}

void APCMemetic::solve5x2(const APC5x2Partition & p, int ls_gens, float ls_prob , bool mej, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations, int ls_neighbour_evals_rate){
    clearSolutions();

    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p[i][j],target1NN,ls_gens,ls_prob,mej,c,population_size,cross_prob,mutation_prob,max_evaluations,ls_neighbour_evals_rate);
            this->fitnesses.push_back(APC_1NN::testFitness(p[i][(j+1)%2],p[i][j],*s));
            //cout << "FIN PARTICION " << i << " " << j << endl;
        }
    }
}

void APCMemetic::solve5Fold(const APC5FoldPartition & p, int ls_gens, float ls_prob , bool mej, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations, int ls_neighbour_evals_rate){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0],target1NNred,ls_gens,ls_prob,mej,c,population_size,cross_prob,mutation_prob,max_evaluations,ls_neighbour_evals_rate); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::testFitness(p[i][1],p[i][0],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}

APCMemetic::~APCMemetic(){
    clearSolutions();
}