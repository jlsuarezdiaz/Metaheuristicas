# include "APC5x2Partition.h"

APC5x2Partition::APC5x2Partition(const APCProblem *p){
    this->problem = p;
    int num_instances = problem->getNumInstances();
    int *vn = new int[num_instances];
    vector<int> vpart[2];

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
}


