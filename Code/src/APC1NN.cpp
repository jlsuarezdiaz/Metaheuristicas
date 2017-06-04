# include "APC1NN.h"

float ** APC_1NN::distances;

float APC_1NN::w_sqDistance(const APCPartition & p, const APCSolution & w,int i, int j){
    float sqDist = 0.0;
    for(int k = 0; k < w.size(); k++){
        sqDist += w[k]*(p[i][k]-p[j][k])*(p[i][k]-p[j][k]);
    }
    return sqDist;
    //return distances[max(i,j)][min(i,j)];
}

float APC_1NN::w_sqDistanceEff(const APCPartition & p, const APCSolution & w, int i, int j){
    return distances[max(i,j)][min(i,j)];
}

float APC_1NN::fitness(const APCPartition & p, const APCSolution & w, const float red_ceil){
    APCSolution ww = w.reduced(red_ceil);
    initializeDistances(p,ww,red_ceil);
    int success = 0;
    for(int i = 0; i < p.size(); i++){
        bool class_i = classify(p,ww,i);
        if(class_i == p.getClass(i)){
            success++;
        }
    }
    deleteDistances(p);
    return (100.0 * success) / p.size();
}

bool APC_1NN::classify(const APCPartition & p, const APCSolution & w,int i){
    bool c_min = (i==0)?p.getClass(1):p.getClass(0); //Supongo que al menos hay dos datos xD
    float d_min = (i==0)?w_sqDistanceEff(p,w,i,1):w_sqDistanceEff(p,w,i,0);
    float d = 0.0;

    //Recorremos los datos
    for(int k = 1; k < p.size(); k++){
        if(k != i){ //Si el dato no es mi dato
            d = w_sqDistanceEff(p,w,k,i);      //Calculo distancia
            if(d < d_min){              //Si es más cercano, me quedo con su clase.
                c_min = p.getClass(k);
                d_min = d;
            }
        }
    }
    return c_min;
}

void APC_1NN::initializeDistances(const APCPartition & p, const APCSolution & w, const float red_ceil){
    distances = new float*[p.size()];
    for(int i = 0; i < p.size(); i++){
        distances[i] = new float[i];
        for(int j = 0; j < i; j++){
            distances[i][j] = 0.0;
            for(int k = 0; k < w.size(); k++){
                distances[i][j] += w[k]*(p[i][k]-p[j][k])*(p[i][k]-p[j][k]);
            }
        }
    }
}

void APC_1NN::deleteDistances(const APCPartition & p){
    for(int i = 0; i < p.size(); i++){
        delete [] distances[i];
    }
    delete [] distances;
}

float APC_1NN::testFitness(const APCPartition & test, const APCPartition & train, const APCSolution & w, const float red_ceil){
    int success = 0;
    for(int i = 0; i < test.size(); i++){
        bool class_i = classifyTest(test,train,w,i,red_ceil);
        if(class_i == test.getClass(i)){
            success++;
        }
    }
    return (100.0 * success) / test.size();
}

bool APC_1NN::classifyTest(const APCPartition & test, const APCPartition & train, const APCSolution & w, int i, const float red_ceil){
    bool c_min = train.getClass(0); //Supongo que al menos hay dos datos xD
    float d_min = w_sqDistanceTrainTest(test,train,w,0,i,red_ceil);
    float d = 0.0;

    //Recorremos los datos
    for(int k = 1; k < train.size(); k++){
        d = w_sqDistanceTrainTest(test,train,w,k,i,red_ceil);      //Calculo distancia
        if(d < d_min){              //Si es más cercano, me quedo con su clase.
            c_min = train.getClass(k);
            d_min = d;
        }
    }
    return c_min;
}

float APC_1NN::w_sqDistanceTrainTest(const APCPartition & test, const APCPartition & train, const APCSolution & w, int i_train, int i_test, const float red_ceil){
    float sqDist = 0.0;
    for(int k = 0; k < w.size(); k++){
        if(w[k] >= red_ceil) sqDist += w[k]*(test[i_test][k]-train[i_train][k])*(test[i_test][k]-train[i_train][k]);
    }
    return sqDist;
}

