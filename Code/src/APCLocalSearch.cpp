# include "APCLocalSearch.h"

APCLocalSearch::APCLocalSearch(const APCProblem *p)
    :problem(p), solutions(),  algorithm_name("LOCAL SEARCH")
{}

void APCLocalSearch::clearSolutions(){
//    for(unsigned i = 0; i < solutions.size(); i++){
//        delete solutions[i];
//    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
}

APCLocalSearch::~APCLocalSearch(){
    clearSolutions();
}

APCSolution * APCLocalSearch::solve(const APCPartition & train, APCSolution *s, int max_neighbours, int max_evaluations, float sigma){
    timer.start();

    APCSolution *sol = s;//new APCSolution(*s); // De verdad quiero copiarla?

    float fit = APC_1NN::fitness(train,*sol);

    int n_attr = problem->getNumNonClassAttributes();
    int neigh_stop = max_neighbours * n_attr;

    int num_evals = 0;
    int no_improves = 0;
    int improves = 0;    //Para depurar

    while(num_evals < max_evaluations && no_improves < neigh_stop){
        for(int i = 0; i < n_attr; i++){
            float wi = (*sol)[i]; //Para deshacer mutación sin copias.
            sol->move(i,sigma);
            float newfit = APC_1NN::fitness(train,*sol);
            num_evals++;

            //Si mejoramos, nos quedamos con la nueva.
            if(newfit > fit){
                fit = newfit;
                improves ++;
                no_improves = 0;
            }
            //Si no, deshacemos e incrementamos no_improves para el criterio de parada.
            else{
                (*sol)[i]=wi;
                no_improves++;
            }
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
            APCSolution *s = solve(p[i][j],solutions[2*i+j],max_neighbours,max_evaluations,sigma);
            this->fitnesses.push_back(APC_1NN::fitness(p[i][(j+1)%2],*s));
        }
    }
}