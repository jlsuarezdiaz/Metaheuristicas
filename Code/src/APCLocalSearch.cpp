# include "APCLocalSearch.h"

//const APCPartition * p_test; //!!!

APCLocalSearch::APCLocalSearch(const APCProblem *p)
    :APCAlgorithm(p,"LOCAL SEARCH")
{
    this->permutation = new int[p->getNumNonClassAttributes()];
    for(int i = 0; i < p->getNumNonClassAttributes(); i++){
        permutation[i]=i;
    }
}


APCLocalSearch::~APCLocalSearch(){
    clearSolutions();
    delete [] permutation;
}

void APCLocalSearch::clearSolutions(){
    //    for(unsigned i = 0; i < solutions.size(); i++){
    //        delete solutions[i];
    //    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
}


APCSolution * APCLocalSearch::solve(const APCPartition & train, APCSolution *s, targetFunction fitness, int max_neighbours, int max_evaluations, float sigma){
    timer.start();

    APCSolution *sol = s;//new APCSolution(*s); // De verdad quiero copiarla?

    float fit = fitness(train,*sol);

    int n_attr = problem->getNumNonClassAttributes();
    int neigh_stop = max_neighbours * n_attr;

    int num_evals = 0;
    int no_improves = 0;
    int improves = 0;    //Para depurar


    //cout << num_evals << " " << fit << " " << APC_1NN::fitness(*p_test,*sol) << endl; //!!!
    while(num_evals < max_evaluations && no_improves < neigh_stop){
        random_shuffle(permutation,permutation+problem->getNumNonClassAttributes());
        for(int i = 0; i < n_attr; i++){
            float wi = (*sol)[permutation[i]]; //Para deshacer mutación sin copias.
            sol->move(permutation[i],sigma);
            float newfit = fitness(train,*sol);
            num_evals++;

            //Si mejoramos, nos quedamos con la nueva.
            if(newfit > fit){
                fit = newfit;
                improves ++;
                no_improves = 0;
            }
            //Si no, deshacemos e incrementamos no_improves para el criterio de parada.
            else{
                (*sol)[permutation[i]]=wi;
                no_improves++;
            }
            //cout << num_evals << " " << fit << " " << APC_1NN::fitness(*p_test,*sol) << endl; //!!!
        }
    }

    //cout << "EVALS = " << num_evals << endl;
    //cout << "NO IMPROVES = " << no_improves << endl;
    //cout << "IMPROVES = " << improves << endl;

    timer.stop();
    
    solutions.push_back(sol);
    times.push_back(timer.get_time());
    train_fits.push_back(fit);

    return sol;
}

void APCLocalSearch::solve5x2(const APC5x2Partition & p, vector<APCSolution*> & solutions, int max_neighbours, int max_evaluations, float sigma ){
    clearSolutions();
    
    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            //p_test = &p[i][(j+1)%2];  //!!!
            APCSolution *s = solve(p[i][j],solutions[2*i+j],target1NN,max_neighbours,max_evaluations,sigma);
            this->fitnesses.push_back(APC_1NN::fitness(p[i][(j+1)%2],*s));
        }
    }
}

void APCLocalSearch::solve5Fold(const APC5FoldPartition & p, vector<APCSolution*> & solutions, int max_neighbours, int max_evaluations, float sigma ){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0],solutions[i],target1NNred,max_neighbours,max_evaluations,sigma); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::fitness(p[i][1],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}