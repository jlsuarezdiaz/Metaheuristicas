# include "APCPartition.h"

float APCPartition::euclideanSqDistance(int i, int j) const{
    float sqDist = 0.0;
    for(int k = 0; k < problem->getNumNonClassAttributes(); k++){
        sqDist += ((*this)[i][k]-(*this)[j][k])*((*this)[i][k]-(*this)[j][k]);
    }
    return sqDist;
}

vector <float> APCPartition::componentDistance(int i, int j) const{
    vector<float> dist(problem->getNumNonClassAttributes());
    for(unsigned k = 0; k < dist.size(); k++){
        dist[k] = fabs((*this)[i][k]-(*this)[j][k]);
    }
    return dist;
}