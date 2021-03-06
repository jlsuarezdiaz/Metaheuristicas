# include "APCGenetic.h"


APCGenetic::APCGenetic(const APCProblem *p)
:APCAlgorithm(p,"GENETIC")
{
    this->population_size = 0;
    this->mutation_prob = 0.001;
    this->cross_prob = 0.7;
    best_solution = NULL;
    num_evaluations = 0;
    generation = 0;
    mutations = NULL;

}

void APCGenetic::clearSolutions(){
    for(unsigned i = 0; i < solutions.size(); i++){
        delete solutions[i];
    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
    parents_population.clear();
    children_population.clear();
}

void APCGenetic::clearPopulation(){
    for(unsigned i = 0; i < population.size(); i++){
        delete population[i];
    }
    population.clear();
    best_solution = NULL;
    population_size = 0;
    generation = 0;
    if(mutations != NULL) delete [] mutations;
    mutations = NULL;

}

void APCGenetic::generatePopulation(const APCPartition & p, targetFunction fitness, int population_size){
    clearPopulation();
    
    this->population_size = population_size;
    const float a = 0.0, b = 1.0;
    float best_val = -1.0;
    population.reserve(population_size);
    mutations = new bool[population_size];

    for(int i = 0; i < population_size; i++){
        mutations[i] = false;
        APCSolution * s = new APCSolution(problem);
        for(int j = 0; j < s->size(); j++){
            (*s)[j] = SRandom::getInstance().getRealUniformDistributionElement(a,b);
        }

        population.push_back(new Individual(s,fitness(p,*s)));

        //Recalcular mejor solución
        if(population[i]->val > best_val){
            best_solution = population[i];
            best_val = population[i]->val;
        }
    
    }


}

void APCGenetic::setInitialPopulation(const APCPartition &p, const vector<Individual*> & initial_population){
    clearPopulation();
    this->population_size = initial_population.size();
    mutations = new bool[population_size];

    float best_val = -1.0;

    for(int i = 0; i < population_size; i++){
        mutations[i] = false;

        population.push_back(initial_population[i]);
        //Recalcular mejor solución
        if(population[i]->val > best_val){
            best_solution = population[i];
            best_val = population[i]->val;
        }
    }
    

}


void APCGenetic::nextGeneration(const APCPartition &p, crossOperator c, targetFunction fitness){
       //cout << "\n\nOLD GEN:" << endl;
       //for(unsigned i = 0; i < population.size(); i++){
       //     cout << population[i]->val << endl;
       //}cout << endl << endl;

       selection();

       //cout << timer.get_time() << " s (Selección)" << endl;

       cross(c,p, fitness);

       //cout << timer.get_time() << " s (Cruce)" << endl;

       mutation(p, fitness);

       //cout << timer.get_time() << " s (Mutación)" << endl;

       replacement();

       //cout << timer.get_time() << " s (Reemplazo)" << endl;

       parents_population.clear();
       children_population.clear();

       generation++;


       //vector <float> allfit = APCTargetCR::fitness(p,*best_solution->s);
       //cout << generation << " " << allfit[0] << " " << allfit[1] << " " << allfit[2] << endl;

       

       //cout << "EVALS = " << num_evaluations << endl;
       //cout << "BEST = " << best_solution->val << endl;
}


APCSolution * APCGenetic::solve(const APCPartition &p, targetFunction fitness, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations){
    if(c == APCGenetic::BLXCross03) cross_algorithm = "BLX-0.3";
    if(c == APCGenetic::arithmeticCross) cross_algorithm = "ARITHMETIC";
    setParameters(cross_prob,mutation_prob);
    resetEvaluations();
    //ixx++; //!!!!
    this->population_size = population_size;

    timer.start();

    generatePopulation(p,fitness,population_size);

    //cout << timer.get_time() << " s (Generación Población inicial)" << endl;

    while(num_evaluations < max_evaluations){
        nextGeneration(p,c,fitness);

    }

    for(int i = 0; i < population_size; i++){
        if((*population[i]) > *best_solution){
            best_solution = population[i];
        }
    }

    timer.stop();
    solutions.push_back(new APCSolution(*(best_solution->s)));
    times.push_back(timer.get_time());
    train_fits.push_back(best_solution->val);

    best_solution = NULL;

    return solutions.back();
}

void APCGenetic::solve5x2(const APC5x2Partition & p, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations){
    clearSolutions();

    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            //cout << i << " " << j << " " << s_index << endl;
            APCSolution *s = solve(p[i][j],target1NN,c,population_size,cross_prob,mutation_prob,max_evaluations);
            this->fitnesses.push_back(APC_1NN::testFitness(p[i][(j+1)%2],p[i][j],*s));
        }
    }
}

void APCGenetic::solve5Fold(const APC5FoldPartition & p, crossOperator c, int population_size, float cross_prob, float mutation_prob, int max_evaluations){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0],target1NNred,c,population_size,cross_prob,mutation_prob,max_evaluations); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::testFitness(p[i][1],p[i][0],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}

APCGenetic::~APCGenetic(){
    clearSolutions();
    clearPopulation();
}

void APCGenetic::setParameters(float cross_prob, float mutation_prob){
    this->cross_prob = cross_prob;
    this->mutation_prob = mutation_prob;
}

void APCGenetic::recalcBestSolution(){
    float best_val = -1.0;
    for(int i = 0; i < population_size; i++){
        //Recalcular mejor solución
        if(population[i]->val > best_val){
            best_solution = population[i];
            best_val = population[i]->val;
        }
    }
}

/* CROSS OPERATORS */
vector<Individual*> APCGenetic::arithmeticCross(const Individual & i1, const Individual & i2){
    APCSolution *s = new APCSolution;
    s->setProblem(i1.s->getProblem());

    for(int i = 0; i < s->size(); i++){
        (*s)[i] = 0.5*((*i1.s)[i] + (*i2.s)[i]);
    }
    vector<Individual*> v;
    v.push_back(new Individual(s));
    return v;
}

vector<Individual*> APCGenetic::BLXCross03(const Individual &i1, const Individual &i2){
    APCSolution *s1 = new APCSolution;
    APCSolution *s2 = new APCSolution;
    s1->setProblem(i1.s->getProblem());      
    s2->setProblem(i2.s->getProblem()); 


    const float lim_inf = 0.0, lim_sup = 1.0;
    const float alpha = 0.3;
    
    for(int i = 0; i < s1->size(); i++){
        float cmax = max((*i1.s)[i],(*i2.s)[i]);
        float cmin = min((*i1.s)[i],(*i2.s)[i]);
        float l  = cmax - cmin;
        float min_int = cmin - l*alpha, max_int = cmax + l*alpha;
        (*s1)[i] = min(lim_sup,max(lim_inf,SRandom::getInstance().getRealUniformDistributionElement(min_int,max_int)));
        (*s2)[i] = min(lim_sup,max(lim_inf,SRandom::getInstance().getRealUniformDistributionElement(min_int,max_int)));
    }

    vector<Individual*> v;
    v.push_back(new Individual(s1));
    v.push_back(new Individual(s2));
    return v;
}

void APCGeneticGenerational::selection(){
    for(int i = 0; i < population_size; i++){
        //cout << i << endl;
        int r1 = SRandom::getInstance().rand(0,population_size-1);
        int r2 = SRandom::getInstance().rand(0,population_size-1);
        if(r1==r2) r2 = (r2+1) % population_size;
        parents_population.push_back(new Individual(*binary_tour(population[r1],population[r2])));
    }
}

void APCGeneticGenerational::cross(crossOperator c, const APCPartition &p, targetFunction fitness){
    int n_cross = ceil(cross_prob * parents_population.size()/2);
    children_population.resize(population_size);
    int last_child = 2*n_cross-1;
    for(int i = 0; i < n_cross; i++){
        int child_index1 = 2*i, child_index2 = 2*i+1;

        vector<Individual*> children = c(*parents_population[child_index1],*parents_population[child_index2]);
        //Revisar borrado de soluciones
        
        // Si el cruce solo generara un hijo, hacemos dos cruces distintos (con el consecutivo y con el inverso de posición)
        if(children.size()==1){
            children_population[child_index1] = children[0];
            children_population[child_index2] = c(*parents_population[i],*parents_population[last_child-i])[0];
        }
        else{
            //Reemplazar hijos
            children_population[child_index1] = children[0];
            children_population[child_index2] = children[1];
        }
    }

    for(int i = last_child+1; i < population_size; i++){
        children_population[i] = parents_population[i];
    }

}

void APCGeneticGenerational::mutation(const APCPartition &p, targetFunction fitness){
    float exp_mut  = mutation_prob*children_population.size()*problem->getNumNonClassAttributes();
    int gen_muts = ceil(1.0/exp_mut);
    int n_muts = (gen_muts!=0&&generation%gen_muts==0)?ceil(exp_mut):0;
    int r1, r2;


    for(int i = 0; i < n_muts; i++){
        r1 = SRandom::getInstance().rand(0,children_population.size()-1);
        r2 = SRandom::getInstance().rand(0,problem->getNumNonClassAttributes()-1);

        children_population[r1]->s->move(r2,0.3);
        children_population[r1]->val = fitness(p,*children_population[r1]->s);
        mutations[r1] = true;

    }

    num_evaluations+= n_muts;
    //Cómputo de evaluaciones de la generación completa (hacerlo mejor si hubiera tiempo)
    int child_size = 2*ceil(cross_prob * parents_population.size()/2);
    for(int i = 0; i < child_size; i++){
        if(!mutations[i]){
            children_population[i]->val = fitness(p,*children_population[i]->s);
            num_evaluations++;
        }
    }
}

void APCGeneticGenerational::replacement(){
    float worst_val = 101.0;
    float best_val = -1.0;
    int worst_ind = -1;
    int child_size = 2*ceil(cross_prob * parents_population.size()/2);
    int best_ind = -1;

    for(int i = 0; i < population_size; i++){
        mutations[i] = false;
        //Eliminamos población para reemplazar
        if(population[i] != best_solution) delete population[i];

        //Reemplazamos con hijo (o el seleccionado si no ha habido cruce)
        if(i < child_size){
            //El hijo es distinto del padre (borramos el padre)
            delete parents_population[i];
        }
        population[i] = children_population[i];
        
        //Nos quedamos con la peor solución para el elitismo
        if(population[i]->val < worst_val){
            worst_val = population[i]->val;
            worst_ind = i;
        }
        if(population[i]->val > best_val){
            best_val = population[i]->val;
            best_ind = i;
        }
    }

    delete population[worst_ind];
    population[worst_ind] = best_solution;

    if(*population[best_ind] > *best_solution) best_solution = population[best_ind];
    //cout << best_solution->val << endl;
    ////!!!!
    //string str = "./sol/experiments/AGGCA_prueba2.sol";
    //static ofstream fout(str);
    //fout << generation << " " << *(best_solution->s) << " " << best_solution->val << "\n";
    //}
}

void APCGeneticStationary::selection(){
    for(int i = 0; i < 4; i++){
        int r1 = SRandom::getInstance().rand(0,population_size-1);
        int r2 = SRandom::getInstance().rand(0,population_size-1);
        if(r1==r2) r2 = (r2 + 1) % population_size;
        //parents_population.push_back(new Individual(*binary_tour(population[r1],population[r2])));
        parents_population.push_back(binary_tour(population[r1],population[r2]));
    }
}

void APCGeneticStationary::cross(crossOperator c, const APCPartition &p, targetFunction fitness){
    children_population.resize(2);

    vector<Individual*> children = c(*parents_population[0],*parents_population[1]);
   
        
    // Si el cruce solo generara un hijo, hacemos dos cruces usando cuatro padres
    if(children.size()==1){
        children_population[0] = children[0];
        //children_population[0]->val = APC_1NN::fitness(p,*children_population[0]->s);
        children_population[1] = c(*parents_population[2],*parents_population[3])[0];
        //children_population[1]->val = APC_1NN::fitness(p,*children_population[1]->s);
        //!! Necesario computar vals, no puede hacerlo el operador de cruce (no tiene partición)
    }
    else{ //Si el cruce genera dos hijos usamos solo dos padres
        //Reemplazar hijos
        children_population[0] = children[0];
        //children_population[0]->val = APC_1NN::fitness(p,*children[0]->s);
        children_population[1] = children[1];
        //children_population[1]->val = APC_1NN::fitness(p,*children[1]->s);
    }

    //num_evaluations += 2;
}

void APCGeneticStationary::mutation(const APCPartition &p, targetFunction fitness){
    float exp_mut  = mutation_prob*children_population.size()*problem->getNumNonClassAttributes();
    int gen_muts = ceil(1.0/exp_mut);
    int n_muts = (gen_muts!=0&&generation%gen_muts==0)?ceil(exp_mut):0;
    int r1, r2;
    for(int i = 0; i < n_muts; i++){
        //LOS PADRES TAMBIEN MUTAN (?)
        r1 = SRandom::getInstance().rand(0,children_population.size()-1);
        r2 = SRandom::getInstance().rand(0,problem->getNumNonClassAttributes()-1);

        children_population[r1]->s->move(r2,0.3);
        //children_population[r1]->val = APC_1NN::fitness(p,*children_population[r1]->s);
    }
    //num_evaluations+= n_muts;

    //Cómputo de los nuevos fitness (son solo 2 siempre) (hacer mejor si hubiera tiempo)
    children_population[0]->val = fitness(p,*children_population[0]->s);
    children_population[1]->val = fitness(p,*children_population[1]->s);
    num_evaluations+=2;
}

void APCGeneticStationary::replacement(){
    int best_child_index = (*children_population[0] > *children_population[1])?0:1;
    int worst_child_index = (best_child_index+1)%2;
    int worst_ind_1 = -1, worst_ind_2 = -1;
    float worst_val_1 = 101.0, worst_val_2 = 102.0;
    for(int i = 0; i < population_size; i++){
        //Encontramos nuevo peor valor, el actual pasa a ser el segundo peor
        if(population[i]->val < worst_val_1){
            worst_val_2 = worst_val_1;
            worst_ind_2 = worst_ind_1;
            worst_val_1 = population[i]->val;
            worst_ind_1 = i;
        }
        //Nuevo segundo peor valor
        else if(population[i]->val < worst_val_2){
            worst_val_2 = population[i]->val;
            worst_ind_2 = i; 
        }

    }
        
    //Reemplazamos hijos según sea mejores o no que los dos peores de la población
    //Los dos son peores (no hay reemplazo)
    if(*children_population[best_child_index] < *population[worst_ind_1]){
        delete children_population[0];
        delete children_population[1];
    }

    //El mejor hijo es mejor que el peor, pero no que el segundo peor
    //o el mejor hijo es mejor que los dos peores, pero el peor hijo no es mejor que ninguno
    //(1 reemplazo)
    else if(*children_population[best_child_index] < *population[worst_ind_2] ||
      *children_population[worst_child_index] < *population[worst_ind_1]){

        if(best_solution==population[worst_ind_1]) best_solution = children_population[best_child_index];
        delete children_population[worst_child_index];
        delete population[worst_ind_1];
        population[worst_ind_1] = children_population[best_child_index];
        if(*children_population[best_child_index] > *best_solution) best_solution = children_population[best_child_index];
    }
    //Los dos hijos son mejores (dos reemplazos)
    else{
        if(best_solution==population[worst_ind_1]||best_solution==population[worst_ind_2]) best_solution = children_population[best_child_index];
        delete population[worst_ind_1];
        delete population[worst_ind_2];
        population[worst_ind_1] = children_population[best_child_index];
        population[worst_ind_2] = children_population[worst_child_index];
        if(*children_population[best_child_index] > *best_solution) best_solution = children_population[best_child_index];
    }


    //static ofstream fout("./sol/experiments/pruebas_AGE.sol");
    //fout << generation << " ";
    //for(int i = 0; i < 6; i++){
    //    fout << population[i]->val << " ";
    //}
    //fout << "\n";
}