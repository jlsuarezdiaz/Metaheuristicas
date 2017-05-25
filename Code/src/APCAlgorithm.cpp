# include "APCAlgorithm.h"

APCAlgorithm::APCAlgorithm(const APCProblem *p, const string & algorithm_name)
    :problem(p), algorithm_name(algorithm_name)
{}

void APCAlgorithm::clearSolutions(){
    for(unsigned i = 0; i < solutions.size(); i++){
        delete solutions[i];
    }
    solutions.clear();
    times.clear();
    fitnesses.clear();
    train_fits.clear();
}

//APCAlgorithm::~APCAlgorithm(){
//    clearSolutions();
//}

void APCAlgorithm::writeSolutions(ostream & out){
    for(unsigned i = 0; i < solutions.size(); i++){
        out << *solutions[i] << "\n";
    }
}

void APCAlgorithm::writeFitnesses(ostream & out){
    for(unsigned i = 0; i < fitnesses.size(); i++){
        out << fitnesses[i] << "\n";
    }
}

void APCAlgorithm::writeTrainFits(ostream & out){
    for(unsigned i = 0; i < train_fits.size(); i++){
        out << train_fits[i] << "\n";
    }
}

void APCAlgorithm::writeTimes(ostream & out){
    for(unsigned i = 0; i < times.size(); i++){
        out << times[i] << "\n";
    }
}

void APCAlgorithm::writeClassRates(ostream & out){
    for(unsigned i = 0; i < class_rates.size(); i++){
        out << class_rates[i] << "\n";
    }
}

void APCAlgorithm::writeRedRates(ostream & out){
    for(unsigned i = 0; i < red_rates.size(); i++){
        out << red_rates[i] << "\n";
    }
}

void APCAlgorithm::writeTable5x2(ostream & out, string cols, bool stats){
    out.setf(ios::fixed);
    out.precision(4);

    for(unsigned j = 0; j < cols.size(); j++){
        switch(cols[j]){
            case 'f':
                out << "\t\t\t\tFITNESS\t";
                break;
            case 't':
                out << "TIME\t";
                break;
            case 'i':
                out << "TRAIN FIT\t";
                break;
            default:
                out << "--ERR--\t";
                break;
        }
    }
    out << "\n";

    for(unsigned i = 0; i < times.size(); i++){
        int p = i/2, h = i%2; //Partition-Half
        out << "PARTITION (" << p << "," << h << ")\t";
        for(unsigned j = 0; j < cols.size(); j++){
            switch(cols[j]){
                case 'f':
                    out << fitnesses[i] << "\t";
                    break;
                case 't':
                    out << times[i] << "\t";
                    break;
                case 'i':
                    out << train_fits[i] << "\t";
                    break;
                default:
                    out << "--ERR--\t";
                    break;
            }
        }
        out << "\n";        
    } 

    if(stats){
        float sum, mean, sq_sum, stdev;

        out << "MEAN\t\t\t";
        for(unsigned j = 0; j < cols.size(); j++){
            switch(cols[j]){
                case 'f':
                    sum = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
                    mean = sum / fitnesses.size();
                    out << mean << "\t";
                    break;
                case 't':
                    sum = std::accumulate(times.begin(), times.end(), 0.0);
                    mean = sum / times.size();
                    out << mean << "\t";
                    break;
                case 'i':
                    sum = std::accumulate(train_fits.begin(), train_fits.end(), 0.0);
                    mean = sum / train_fits.size();
                    out << mean << "\t";
                    break;
                default:
                    out << "--ERR--\t";
                    break;
            }
        }
        out << "\n";

        out << "STDEV\t\t\t";
        for(unsigned j = 0; j < cols.size(); j++){
            switch(cols[j]){
                case 'f':
                    sum = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
                    mean = sum / fitnesses.size();
                    sq_sum = std::inner_product(fitnesses.begin(), fitnesses.end(), fitnesses.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / fitnesses.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                case 't':
                    sum = std::accumulate(times.begin(), times.end(), 0.0);
                    mean = sum / times.size();
                    sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / times.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                case 'i':
                    sum = std::accumulate(train_fits.begin(), train_fits.end(), 0.0);
                    mean = sum / train_fits.size();
                    sq_sum = std::inner_product(train_fits.begin(), train_fits.end(), train_fits.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / train_fits.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                default:
                    out << "--ERR--\t";
                    break;
            }
        }
        out << "\n";
    }
}


void APCAlgorithm::writeTable5Fold(ostream & out, string cols, bool stats){
    out.setf(ios::fixed);
    out.precision(4);

    out << getAlgorithmName() << "\t";
    for(unsigned j = 0; j < cols.size(); j++){
        switch(cols[j]){
            case 'f':
                out << "FITNESS\t";
                break;
            case 't':
                out << "TIME\t";
                break;
            case 'i':
                out << "TRAIN_FIT\t";
                break;
            case 'c':
                out << "CLASS_RATE\t";
                break;
            case 'r':
                out << "RED_RATE\t";
                break;
            default:
                out << "--ERR--\t";
                break;
        }
    }
    out << "\n";

    for(unsigned i = 0; i < times.size(); i++){
        out << "PARTITION-" << i << "\t";
        for(unsigned j = 0; j < cols.size(); j++){
            switch(cols[j]){
                case 'f':
                    out << fitnesses[i] << "\t";
                    break;
                case 't':
                    out << times[i] << "\t";
                    break;
                case 'i':
                    out << train_fits[i] << "\t";
                    break;
                case 'c':
                    out << class_rates[i] << "\t";
                    break;
                case 'r':
                    out << red_rates[i] << "\t";
                    break;
                default:
                    out << "--ERR--\t";
                    break;
            }
        }
        out << "\n";        
    } 

    if(stats){
        float sum, mean, sq_sum, stdev;

        out << "MEAN\t";
        for(unsigned j = 0; j < cols.size(); j++){
            switch(cols[j]){
                case 'f':
                    sum = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
                    mean = sum / fitnesses.size();
                    out << mean << "\t";
                    break;
                case 't':
                    sum = std::accumulate(times.begin(), times.end(), 0.0);
                    mean = sum / times.size();
                    out << mean << "\t";
                    break;
                case 'i':
                    sum = std::accumulate(train_fits.begin(), train_fits.end(), 0.0);
                    mean = sum / train_fits.size();
                    out << mean << "\t";
                    break;
                case 'c':
                    sum = std::accumulate(class_rates.begin(), class_rates.end(), 0.0);
                    mean = sum / class_rates.size();
                    out << mean << "\t";
                    break;
                case 'r':
                    sum = std::accumulate(red_rates.begin(), red_rates.end(), 0.0);
                    mean = sum / red_rates.size();
                    out << mean << "\t";
                    break;
                default:
                    out << "--ERR--\t";
                    break;
            }
        }
        out << "\n";

        out << "STDEV\t";
        for(unsigned j = 0; j < cols.size(); j++){
            switch(cols[j]){
                case 'f':
                    sum = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
                    mean = sum / fitnesses.size();
                    sq_sum = std::inner_product(fitnesses.begin(), fitnesses.end(), fitnesses.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / fitnesses.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                case 't':
                    sum = std::accumulate(times.begin(), times.end(), 0.0);
                    mean = sum / times.size();
                    sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / times.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                case 'i':
                    sum = std::accumulate(train_fits.begin(), train_fits.end(), 0.0);
                    mean = sum / train_fits.size();
                    sq_sum = std::inner_product(train_fits.begin(), train_fits.end(), train_fits.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / train_fits.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                case 'c':
                    sum = std::accumulate(class_rates.begin(), class_rates.end(), 0.0);
                    mean = sum / class_rates.size();
                    sq_sum = std::inner_product(class_rates.begin(), class_rates.end(), class_rates.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / class_rates.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                case 'r':
                    sum = std::accumulate(red_rates.begin(), red_rates.end(), 0.0);
                    mean = sum / red_rates.size();
                    sq_sum = std::inner_product(red_rates.begin(), red_rates.end(), red_rates.begin(), 0.0);
                    stdev = std::sqrt(sq_sum / red_rates.size() - mean * mean);
                    out << stdev << "\t";
                    break;
                default:
                    out << "--ERR--\t";
                    break;
            }
        }
        out << "\n";
    }
}