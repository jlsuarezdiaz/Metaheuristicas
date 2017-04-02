#include "APCRelief.h"

APCRelief::APCRelief(const APCProblem *p)
    :problem(p), solutions(),  algorithm_name("RELIEF")
{}

int APCRelief::findNearestEnemy(const APCPartition & p, int index) const{
    float near_dist = numeric_limits<float>::infinity();
    int enemy_index = -1;
    //Para cada elemento de la partición
    for(int i = 0; i < p.size(); i++){
        //Si es de distinta clase
        if(p.getClass(i) != p.getClass(index)){
            //Actualizamos distancias si es necesario
            float new_dist = p.euclideanSqDistance(i,index); 
            if(new_dist < near_dist){
                enemy_index = i;
                near_dist = new_dist;;
            }
        }
    }
    return enemy_index;
}

int APCRelief::findNearestFriend(const APCPartition & p, int index) const{
    float near_dist = numeric_limits<float>::infinity();
    int friend_index = -1;
    //Para cada elemento de la partición
    for(int i = 0; i < p.size(); i++){
        //Si es de la misma clase (y no es el propio elemento)
        if(i != index && p.getClass(i) == p.getClass(index)){
            //Actualizamos distancias si es necesario
            float new_dist = p.euclideanSqDistance(i,index); 
            if(new_dist < near_dist){
                friend_index = i;
                near_dist = new_dist;
            }
        }
    }
    return friend_index;
}

APCSolution * APCRelief::solve(const APCPartition &p){
    APCSolution *solution = new APCSolution(problem);
    solutions.push_back(solution);

    timer.start();
    for(int i = 0; i < p.size(); i++){
        //Pbtenemos amigo y enemigo más cercano
        int en_i = findNearestEnemy(p,i);
        int fr_i = findNearestFriend(p,i);
        //cout << i << " " << en_i << " " << fr_i << endl;
        //Actualizamos pesos
        //cout << endl << i << endl;
        //for(int j = 0; j < solution->size(); j++){
        //    cout << (*solution)[j] << " ";
        //}

        (*solution) += p.componentDistance(i,en_i).begin();
        (*solution) += p.componentDistance(i,fr_i).begin();

        //cout << endl << endl;
        //for(int j = 0; j < solution->size(); j++){
        //    cout << p.componentDistance(i,en_i)[j] << " ";
        //}
        //cout << endl << endl;
        //for(int j = 0; j < solution->size(); j++){
        //    cout << p.componentDistance(i,fr_i)[j] << " ";
        //}
        //cout << endl << endl;
        //for(int j = 0; j < solution->size(); j++){
        //    cout << (*solution)[j] << " ";
        //}
    }

    //Normalizamos
    solution->normalize();

    timer.stop();
    times.push_back(timer.get_time());
    fitnesses.push_back(-1.0);
    train_fits.push_back(APC_1NN(&p,solution).fitness());
    return solution;
}

void APCRelief::solve5x2(const APC5x2Partition & p5x2){
    clearSolutions();

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            APCSolution *s = solve(p5x2[i][j]);
            APC_1NN classifier(&p5x2[i][(j+1)%2],s);
            fitnesses[fitnesses.size()-1] = classifier.fitness();
        }
    }
}

void APCRelief::clearSolutions(){
    for(unsigned i = 0; i < solutions.size(); i++){
        delete solutions[i];
    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
}

APCRelief::~APCRelief(){
    clearSolutions();
}