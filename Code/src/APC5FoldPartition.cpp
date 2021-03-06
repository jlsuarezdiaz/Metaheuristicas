#include "APC5FoldPartition.h"


APC5FoldPartition::APC5FoldPartition(const APCProblem *p){
    this->problem = p;
    int num_instances = problem->getNumInstances();

    //Indices de la clase TRUE
    vector<int> vn1;
    //Indices de la clase FALSE
    vector<int> vn2;
    for(int i = 0; i < num_instances; i++){
        if(problem->getClasses()[i])
            vn1.push_back(i);
        else
            vn2.push_back(i);
    }

    //Las 5 particiones con las que trabajaremos
    vector <int> pre_partitions[5];

    random_shuffle(vn1.begin(), vn1.end());
    random_shuffle(vn2.begin(), vn2.end());

    int part = 0;
    //Repartimos los elementos de clase TRUE
    for(unsigned j = 0; j < vn1.size(); j++){
        pre_partitions[part].push_back(vn1[j]);
        part = (part+1)%5;
    }
    //Repartimos los elementos de la clase FALSE
    for(unsigned j = 0; j < vn2.size(); j++){
        pre_partitions[part].push_back(vn2[j]);
        part = (part+1)%5;
    }
    

    //Creamos las particiones train y test que se utilizarán
    for(int i = 0; i < 5; i++){
        partitions[i][0] = APCPartition(p);
        partitions[i][1] = APCPartition(p);
        for(int j = 0; j < 5; j++){
            //Si i==j a la partición test, else al train
            int r = (i==j)?1:0;
            for(unsigned k = 0; k < pre_partitions[j].size(); k++){
                partitions[i][r].addPartitionElement(pre_partitions[j][k]);
            }            
        }
    }
}

ostream & operator<<(ostream & out, const APC5FoldPartition & p){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            out << p.partitions[i][j] << "\n";
        }
    }
    return out;
}
