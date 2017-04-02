# include "APCLocalSearch.h"

APCLocalSearch::APCLocalSearch(const APCProblem *p)
    :problem(p), solutions(),  algorithm_name("LOCAL SEARCH")
{}

void APCLocalSearch::clearSolutions(){
    for(unsigned i = 0; i < solutions.size(); i++){
        delete solutions[i];
    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
}

APCLocalSearch::~APCLocalSearch(){
    clearSolutions();
}

APCSolution * APCLocalSearch::solve(const APCPartition & train, int s_index, int max_neighbours, int max_evaluations, float sigma){
    
    timer.start();

    float * fit = &train_fits[s_index];
    APCSolution * sol = solutions[s_index];

    int n_attr = problem->getNumNonClassAttributes();
    int neigh_stop = max_neighbours * n_attr;

    int num_evals = 0;
    int no_improves = 0;
    int improves = 0;    //Para depurar

    while(num_evals < max_evaluations && no_improves < neigh_stop){
        for(int i = 0; i < n_attr; i++){
            float wi = (*sol)[i]; //Para deshacer mutación sin copias.
            sol->move(i,sigma);
            float newfit = APC_1NN(&train, sol).fitness();
            num_evals++;

            //Si mejoramos, nos quedamos con la nueva.
            if(newfit > *fit){
                *fit = newfit;
                improves ++;
                //cout << *fit << endl;
            }
            //Si no, deshacemos e incrementamos no_improves para el criterio de parada.
            else{
                (*sol)[i]=wi;
                no_improves++;
            }
        }
    }

    //cout << num_evals << endl;
    //cout << no_improves << endl;
    //cout << improves << endl;

    timer.stop();
    times[s_index] = timer.get_time();
    //cout << "Completado en " << times[s_index] << endl;

    return sol;
}

void APCLocalSearch::solve5x2(const APC5x2Partition & p, const vector<APCSolution*> & solutions, const vector<float>& fitnesses, int max_neighbours, int max_evaluations, float sigma ){
    clearSolutions();
    for(int i = 0; i < 10; i++){
        this->solutions.push_back(new APCSolution(*solutions[i]));
        this->train_fits.push_back(fitnesses[i]);
        this->times.push_back(-1.0);
    }

    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            int s_index = 2*i + j;
            //cout << i << " " << j << " " << s_index << endl;
            APCSolution *s = solve(p[i][j],s_index,max_neighbours,max_evaluations,sigma);
            APC_1NN classifier(&p[i][(j+1)%2],s);
            this->fitnesses.push_back(classifier.fitness());
        }
    }
}