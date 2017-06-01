# include "APCSimulatedAnnealing.h"

const float APCSimulatedAnnealing::K = 1.0;


APCSimulatedAnnealing::APCSimulatedAnnealing(const APCProblem *p)
    :APCAlgorithm(p,"SA")
{}

void APCSimulatedAnnealing::initialize(const APCPartition &p, targetFunction fitness, int max_neighbours, int max_success, int max_evals, float phi, float mu, float Tf, APCSolution *& initial_solution){
    if(initial_solution == NULL) initial_solution = new APCSolution(APCSolution::randomSolution(problem));
    best_solution = new APCSolution(*initial_solution);
    best_cost = fitness(p,*best_solution);
    T_0 = - (mu * best_cost/100.0) / log(phi);
    T_f = Tf;
    temp =  T_0;
    num_annealings = max_evals/max_neighbours;
    beta = (T_0 - T_f)/(num_annealings*T_0*T_f);
}

void APCSimulatedAnnealing::coolingScheme(){
    //temp = temp/(1+beta*temp);
    temp = 0.95*temp;
}

//Parámetros: initial_solution, phi, mu, Tf, max_evals, max_neighbours, max_success
APCSolution * APCSimulatedAnnealing::solve(const APCPartition & p, targetFunction fitness, int max_neighbours, int max_success, int max_evals, float phi, float mu, float Tf, APCSolution *s0){
    timer.start();

    initialize(p,fitness,max_neighbours,max_success,max_evals,phi,mu,Tf,s0);
    int num_neighbours;
    int num_success;

    APCSolution * s = s0;
    float fit = best_cost, diff_fit;
    const float a = 0.0, b = 1.0;

    int num_evals = 0;



    do{
        //cout << endl << endl << "NUM EV = " << num_evals << endl;
        //cout << "TEMP = " <<  temp << endl << endl << endl;
        int num_acc_temp = 0;
        int num_acc_mej = 0;



        num_neighbours = 0;
        num_success = 0;


        while(num_neighbours < max_neighbours && num_success < max_success){

            int r1 = SRandom::getInstance().rand(0,s->size()-1);
            float wi = (*s)[r1]; //Para deshacer mutación sin copias.
            //Generamos vecino
            s->move(r1,0.3);
            //cout << "TIEMPO MUT = " << timer.get_time() << endl;
            float newfit = fitness(p,*s);

            //cout << "TIEMPO FIT = " << timer.get_time() << endl;
            num_evals++;
            num_neighbours++;

            diff_fit = (fit - newfit)/100.0;
            //if(diff_fit > 0) cout << diff_fit << " " << temp << " " << exp(-diff_fit/(K*temp)) << endl;
            //else cout << "NO" << endl;


            //Criterio de aceptación.
            if(diff_fit != 0 && (diff_fit < 0 || SRandom::getInstance().getRealUniformDistributionElement(a,b) <= exp(-diff_fit/(K*temp)))){
                if(diff_fit >= 0) num_acc_temp++; // cout << "ACEPTADO PEOR" << endl;
                else num_acc_mej++; //cout << "ACEPTADO MEJOR" << endl;
                fit = newfit;
                if(fit > best_cost){
                    delete best_solution;
                    best_solution = new APCSolution(*s);
                    best_cost = fit;
                }
                num_success++;
               // cout << "TIEMPO UPDATE = " << timer.get_time() << endl;
                
            }
            //Si no, deshacemos e incrementamos no_improves para el criterio de parada.
            else{
                //cout << "NO ACEPTADO" << endl;
                (*s)[r1]=wi;
            }

            //cout << "TIEMPO IF = " << timer.get_time() << endl;
            //cout << num_evals << " " << num_success << " " << max_success << " " << num_neighbours << " " << max_neighbours << " " << temp << " " << best_cost << endl;   
        }
        //cout << "NUM NEIGH = " << num_neighbours << endl;
        //cout << "NUM SUCC = " << num_success << endl;

        coolingScheme();

        //cout << "ACEPTADOS MEJORES: " << num_acc_mej << " ACEPTADOS PEORES: " << num_acc_temp << " TOTAL: " << num_evals << endl;

        //- Preguntas:
        //- Muto una componente al azar?
        //- Constante K?
        //- Mismo sigma?
        //¡comprobando siempre que sea menor que la inicial! (?)
        //- Condicion de parada num_success > 0 necesaria? se queda muy lejos de las 15000 evaluaciones

    }while(num_evals < max_evals && num_success > 0);

    timer.stop();

    solutions.push_back(new APCSolution(*best_solution));

    times.push_back(timer.get_time());
    train_fits.push_back(best_cost);

    delete best_solution;
    best_solution = NULL;

    return solutions.back();
}

void APCSimulatedAnnealing::solve5Fold(const APC5FoldPartition & p, int max_neighbours, int max_success, int max_evals, float phi, float mu, float Tf, APCSolution *s0){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        APCSolution *s = solve(p[i][0],target1NNred,max_neighbours,max_success,max_evals,phi,mu,Tf,s0); //Resolvemos train
        vector<float> cr_fits = APCTargetCR::testFitness(p[i][1],p[i][0],*s); //Evaluamos test
        class_rates.push_back(cr_fits[0]);
        red_rates.push_back(cr_fits[1]);
        fitnesses.push_back(cr_fits[2]); 
    }
}

void APCSimulatedAnnealing::solve5x2(const APC5x2Partition & p, int max_neighbours, int max_success, int max_evals, float phi, float mu, float Tf, APCSolution *s0){
    clearSolutions();
    
    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 2; j++){
            //p_test = &p[i][(j+1)%2];  //!!!
            APCSolution *s = solve(p[i][j],target1NN,max_neighbours,max_success,max_evals,phi,mu,Tf,s0);
            this->fitnesses.push_back(APC_1NN::testFitness(p[i][(j+1)%2],p[i][j],*s));
        }
    }
}
