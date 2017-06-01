#include "APCDifferentialEvolution.h"


APCSolution * APCDifferentialEvolution::solve(const APCPartition & p, targetFunction fitness, differentialCross c, int population_size, float cr, float f, int max_evaluations){
    if(c == APCDifferentialEvolution::DERand) cross_algorithm = "RAND";
    if(c == APCDifferentialEvolution::DECurrentToBest) cross_algorithm = "CurrentToBest";
    this->population_size = population_size;
    resetEvaluations();

    timer.start();

    generatePopulation(p,fitness,population_size);

    while(num_evaluations < max_evaluations){
        nextGeneration(p,fitness,c,cr,f);
    }

    timer.stop();

    solutions.push_back(new APCSolution(*(best_solution->s)));
    times.push_back(timer.get_time());
    train_fits.push_back(best_solution->val);

    best_solution = NULL;

    return solutions.back();
}

void APCDifferentialEvolution::nextGeneration(const APCPartition & p, targetFunction fitness, differentialCross c,float cr, float f){

    selection();

    cross(c,p,fitness,cr,f);

    replacement();


    parents_population.clear();
    children_population.clear();
    generation++;

    //cout << num_evaluations << " " << best_solution->val << endl;
}

void APCDifferentialEvolution::selection(){
    for(int i = 0; i < population_size; i++){
        int r1 = SRandom::getInstance().rand(0,population_size-1);
        int r2 = SRandom::getInstance().rand(0,population_size-1);
        int r3 = SRandom::getInstance().rand(0,population_size-1);
        if(r1==r2) r2 = (r2+1)%population_size;
        if(r1==r3) r3 = (r3+1)%population_size;
        if(r2==r3) r3 = (r3+1)%population_size;

        parents_population.push_back(population[r1]);
        parents_population.push_back(population[r2]);
        parents_population.push_back(population[r3]);
    }
}

void APCDifferentialEvolution::cross(differentialCross c, const APCPartition & p, targetFunction fitness, float cr, float f){
    children_population.resize(population_size);
    for(int i = 0; i < population_size; i++){
        children_population[i] = c(parents_population[3*i],parents_population[3*i+1],parents_population[3*i+2],population[i],best_solution,cr,f);
        //cout << best_solution->s << " 5 " << children_population[i]->s << endl;
        //cout << best_solution << endl;
        //cout << best_solution->val << endl;
        children_population[i]->val = fitness(p,*children_population[i]->s);
        //cout << best_solution->s << " 6 " << children_population[i]->s << endl;
        //cout << best_solution->val << endl;
        //cout << best_solution << endl;
        num_evaluations++;
    }
}

void APCDifferentialEvolution::replacement(){
    for(int i = 0; i < population_size; i++){
        if(*population[i] > *children_population[i]){
            delete children_population[i];
        }
        else{
            if(*children_population[i] > *best_solution){
                //Deberia funcionar sin copiar, pero da un error, averiguar por qué si hay tiempo.
                //delete best_solution;
                best_solution = new Individual(new APCSolution(*children_population[i]->s),children_population[i]->val);
            }
            delete population[i];
            population[i] = children_population[i];

        }
    }

}

Individual * APCDifferentialEvolution::DERand(const Individual * i1, const Individual *i2, const Individual *i3, const Individual *x, const Individual *best, float cr, float f){
    APCSolution *s = new APCSolution;
    s->setProblem(i1->s->getProblem());      


    const float lim_inf = 0.0, lim_sup = 1.0;
    
    for(int k = 0; k < s->size(); k++){
        if(SRandom::getInstance().getRealUniformDistributionElement(lim_inf,lim_sup) < cr){
            (*s)[k] = (*i1->s)[k] + f*((*i2->s)[k]-(*i3->s)[k]);
            if((*s)[k] < 0.0) (*s)[k]=0.0;
            if((*s)[k] > 1.0) (*s)[k]=1.0;
        }
        else{
            (*s)[k] = (*x->s)[k];
        }
    }

    return new Individual(s);
}


Individual * APCDifferentialEvolution::DECurrentToBest(const Individual * i1, const Individual *i2, const Individual *i3, const Individual *x, const Individual *best, float cr, float f){
    APCSolution *s = new APCSolution;
    s->setProblem(i1->s->getProblem());      

    const float lim_inf = 0.0, lim_sup = 1.0;
    
    for(int k = 0; k < s->size(); k++){
        if(SRandom::getInstance().getRealUniformDistributionElement(lim_inf,lim_sup) < cr){
            //cout << x->s << " " << best->s << " " << i1->s << " " << i2->s << " " << endl;
            (*s)[k] = (*x->s)[k] + f*((*best->s)[k]-(*x->s)[k]) + f*((*i1->s)[k]-(*i2->s)[k]);
            if((*s)[k] < 0.0) (*s)[k]=0.0;
            if((*s)[k] > 1.0) (*s)[k]=1.0;
        }
        else{
            (*s)[k] = (*x->s)[k];
        }
    }

    return new Individual(s);
}

void APCDifferentialEvolution::solve5x2(const APC5x2Partition & p, differentialCross c, int population_size, float cr, float f, int max_evaluations){
    clearSolutions();

    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p[i][j],target1NN,c,population_size,cr,f,max_evaluations);
            this->fitnesses.push_back(APC_1NN::testFitness(p[i][(j+1)%2],p[i][j],*s));
        }
    }
}

void APCDifferentialEvolution::solve5Fold(const APC5FoldPartition & p, differentialCross c, int population_size, float cr, float f, int max_evaluations){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0],target1NNred,c,population_size,cr,f,max_evaluations); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::testFitness(p[i][1],p[i][0],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}
