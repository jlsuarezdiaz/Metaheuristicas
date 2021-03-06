# include "APC5x2Partition.h"

APC5x2Partition::APC5x2Partition(const APCProblem *p){
    this->problem = p;
    int num_instances = problem->getNumInstances();
    int *vn = new int[num_instances];

    for(int i = 0; i < num_instances; i++){
        vn[i]=i;
    }
    for(int i = 0; i < 5; i++){
        random_shuffle(vn,vn+num_instances);
        partitions[i][0] = APCPartition(p);
        partitions[i][1] = APCPartition(p);

        int k = 0;
        for(int j = 0; j < num_instances; j++){
            partitions[i][k].addPartitionElement(vn[j]);
            k = (k+1)%2;
        }
    }
    delete [] vn;
//    vector<int> vn1;
//    vector<int> vn2;
//    for(int i = 0; i < num_instances; i++){
//        if(problem->getClasses()[i])
//            vn1.push_back(i);
//        else
//            vn2.push_back(i);
//    }
//
//    for(int i = 0; i < 5; i++){
//        random_shuffle(vn1.begin(), vn1.end());
//        random_shuffle(vn2.begin(), vn2.end());
//        partitions[i][0] = APCPartition(p);
//        partitions[i][1] = APCPartition(p);
//
//        int k = 0;
//        for(unsigned j = 0; j < vn1.size(); j++){
//            partitions[i][k].addPartitionElement(vn1[j]);
//            k = (k+1)%2;
//        }
//        for(unsigned j = 0; j < vn2.size(); j++){
//            partitions[i][k].addPartitionElement(vn2[j]);
//            k = (k+1)%2;
//        }
//    }
}

ostream & operator<<(ostream & out, const APC5x2Partition & p){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 2; j++){
            out << p.partitions[i][j] << "\n";
        }
    }
    return out;
}
