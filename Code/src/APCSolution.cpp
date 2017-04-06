# include "APCSolution.h"

APCSolution::APCSolution(const APCProblem *p)
:problem(p)
{
    this->weights = new float[p->getNumNonClassAttributes()];
    for(int i = 0; i < size(); i++){
        weights[i]=0.0;
    }
}

void APCSolution::setProblem(const APCProblem * p){
    if(weights != NULL) delete [] weights;
    this->problem = p;
    this->weights = new float[p->getNumNonClassAttributes()];
}

void APCSolution::normalize(){
    float wmax = 0.0;
    for(int i = 0; i < problem->getNumNonClassAttributes(); i++){
        if(weights[i] < 0.0) weights[i]=0.0;
        if(weights[i] > wmax) wmax = weights[i];
    }
    if(wmax > 0.0){
        for(int i = 0; i < problem->getNumNonClassAttributes(); i++){
            weights[i] = weights[i]/wmax;
        }
    }
}

void APCSolution::move(int i, float sigma){
    float mean = 0.0;
    float z = SRandom::getInstance().getNormalDistributionElement(mean,sigma);
    weights[i]+=z;
    if(weights[i] > 1.0) weights[i] = 1.0;
    if(weights[i] < 0.0) weights[i] = 0.0;
}

APCSolution::APCSolution(const APCSolution & s){
    this->problem = s.problem;
    this->weights = new float[problem->getNumNonClassAttributes()];
    for(int i = 0; i < problem->getNumNonClassAttributes(); i++){
        this->weights[i] = s[i];
    }
}

APCSolution & APCSolution::operator=(const APCSolution & s){
    if(this != &s){
        this->problem = s.problem;
        if(s.size() != this->size()){
            delete [] weights;
            this->weights = new float[problem->getNumNonClassAttributes()];
        }
        for(int i = 0; i < problem->getNumNonClassAttributes(); i++){
            this->weights[i] = s[i];
        }
    }
    return *this;
}

ostream & operator<<(ostream & out, const APCSolution &s){
    for(int i = 0; i < s.size(); i++){
        out << s[i] << " ";
    }
    return out;
}

istream & operator>>(istream & in, APCSolution & s){
    for(int i = 0; i < s.size(); i++){
        in >> s[i];
    }
    return in;
}

APCSolution APCSolution::weight1Solution(const APCProblem *problem){
    APCSolution w(problem);
    for(int i = 0; i < w.size(); i++){
        w[i] = 1.0;
    }
    return w;
}