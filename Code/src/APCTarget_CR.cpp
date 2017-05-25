# include "APCTarget_CR.h"

float APCTargetCR::reduction_rate(const APCSolution & w, const float red_ceil){
    int num_reds = 0; //Número de reducciones.
    for(int i = 0; i < w.size(); i++){
        if(w[i] < red_ceil) num_reds++;
    }
    return (float) 100.0 * num_reds / w.size();
}

float APCTargetCR::classification_rate(const APCPartition & p, const APCSolution & w, const float red_ceil){
    return APC_1NN::fitness(p,w,red_ceil);
}

float APCTargetCR::test_classification_rate(const APCPartition & test, const APCPartition & train, const APCSolution & w, const float red_ceil){
    return APC_1NN::testFitness(test,train,w,red_ceil);
}

vector <float> APCTargetCR::fitness(const APCPartition & p, const APCSolution & w, const float alpha, const float red_ceil){
    vector <float> fits;
    fits.push_back(classification_rate(p,w,red_ceil));
    fits.push_back(reduction_rate(w,red_ceil));
    fits.push_back((alpha)*fits[0]+(1-alpha)*fits[1]);
    return fits;
}

vector <float> APCTargetCR::testFitness(const APCPartition & test, const APCPartition & train, const APCSolution & w, const float alpha, const float red_ceil){
    vector <float> fits;
    fits.push_back(test_classification_rate(test,train,w,red_ceil));
    fits.push_back(reduction_rate(w,red_ceil));
    fits.push_back((alpha)*fits[0]+(1-alpha)*fits[1]);
    return fits;
}