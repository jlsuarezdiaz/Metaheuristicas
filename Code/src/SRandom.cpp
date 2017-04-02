# include "SRandom.h"

SRandom::SRandom(){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    setSeed(seed);
}

SRandom & SRandom::getInstance(){
    static SRandom instance;
    return instance;
}

void SRandom::setSeed(unsigned seed){
    generator = default_random_engine(seed);
    srand(seed);
}



