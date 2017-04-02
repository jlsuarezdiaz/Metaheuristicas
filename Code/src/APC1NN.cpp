# include "APC1NN.h"

float APC_1NN::w_sqDistance(int i, int j){
    float sqDist = 0.0;
    for(int k = 0; k < w->size(); k++){
        sqDist += (*w)[k]*((*p)[i][k]-(*p)[j][k])*((*p)[i][k]-(*p)[j][k]);
    }
    return sqDist;
}

float APC_1NN::fitness(){
    int success = 0;
    for(int i = 0; i < p->size(); i++){
        bool class_i = classify(i);
        if(class_i == p->getClass(i)){
            success++;
        }
    }
    return (100.0 * success) / p->size();
}

bool APC_1NN::classify(int i){
    bool c_min = (i==1)?p->getClass(2):p->getClass(1); //Supongo que al menos hay dos datos xD
    float d_min = (i==1)?w_sqDistance(i,2):w_sqDistance(i,1);
    float d = 0.0;

    //Recorremos los datos
    for(int k = 2; k < p->size(); k++){
        if(k != i){ //Si el dato no es mi dato
            d = w_sqDistance(k,i);      //Calculo distancia
            if(d < d_min){              //Si es más cercano, me quedo con su clase.
                c_min = p->getClass(k);
                d_min = d;
            }
        }
    }
    return c_min;
}