# include "APCMemetic.h"

APCMemetic::APCMemetic(const APCProblem *p)
:agg(p), ls(p), algorithm_name("MEMETIC")
{
    this->problem = p;

}

void APCMemetic::clearSolutions(){
    for(unsigned i = 0; i < solutions.size(); i++){
        delete solutions[i];
    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
}

APCSolution * APCMemetic::solve(const APCPartition & p_train, int ls_gens, float ls_prob , bool mej, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations, int ls_neighbour_evals_rate){
    algorithm_model = std::to_string(ls_gens)+"-"+std::to_string(ls_prob)+(mej?"-MEJ":"");
    int num_evaluations = 0;

    timer.start();

    agg.generatePopulation(p_train,population_size);
    agg.setParameters(cross_prob,mutation_prob);
    agg.resetEvaluations();

    int num_ls_inds = ls_prob * population_size; //Número de individuos a los que se aplicará la LS
    int neighbour_evals = ls_neighbour_evals_rate*problem->getNumNonClassAttributes();
    float best_val;
    Individual * best;

    while(num_evaluations < max_evaluations){
        for(int i = 0; i < ls_gens; i++){
            agg.nextGeneration(p_train,c);
        }

        vector<Individual*> solutions = agg.getPopulation();

        cout << "GENETICO: " << timer.get_time() << endl;

        if(num_ls_inds != population_size){ //Para no hacer el tonto si la prob es 1
            if(mej){
                sort(solutions.begin(),solutions.end(),IndividualComparator());
                for(int i = 0; i < solutions.size(); i++){
                    cout << solutions[i]->val << endl;
                }
                cout << endl;
            }
            else{
                random_shuffle(solutions.begin(),solutions.end());
            }
        }

        cout << "SELECCION PARA LS: " << timer.get_time() << endl;

        best_val = -1.0;
        best = NULL;

        for(int i = 0; i < num_ls_inds; i++){
            ls.solve(p_train,solutions[i]->s,2,neighbour_evals);
            solutions[i]->val = ls.getTrainFits().back();
            if(solutions[i]->val > best_val){
                best_val = solutions[i]->val;
                best = solutions[i];
            }
        }

        cout << "LS: " << timer.get_time() << endl;

        ls.clearSolutions();
        agg.recalcBestSolution(best);
        
        num_evaluations+=(agg.getEvaluations() + neighbour_evals * num_ls_inds);
        agg.resetEvaluations();

        cout << "RESTART: " << timer.get_time() << endl;
        cout << "EVALS: " << num_evaluations << endl;
    }

    timer.stop();

    solutions.push_back(new APCSolution(*(best->s)));
    times.push_back(timer.get_time());
    train_fits.push_back(best->val);
    cout << "TIEMPO = " << timer.get_time() << endl;

    agg.clearPopulation();

    return solutions.back();
}

void APCMemetic::solve5x2(const APC5x2Partition & p, int ls_gens, float ls_prob , bool mej, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations, int ls_neighbour_evals_rate){
    clearSolutions();

    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p[i][j],ls_gens,ls_prob,mej,c,population_size,cross_prob,mutation_prob,max_evaluations,ls_neighbour_evals_rate);
            this->fitnesses.push_back(APC_1NN::fitness(p[i][(j+1)%2],*s));
            cout << "FIN PARTICION " << i << " " << j << endl;
        }
    }
}

APCMemetic::~APCMemetic(){
    clearSolutions();
}