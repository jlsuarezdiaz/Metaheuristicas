# include "APCProblem.h"

void APCProblem::readFileData(){

    ArffParser parser(file_name);
    try{
        ArffData *data = parser.parse();

        relation = data->get_relation_name();
        num_attributes = data->num_attributes();
        num_instances = data->num_instances();

        classes = new bool[num_instances];
        attributes = new float*[num_instances];

        string true_class = "";

        for(int i = 0; i < num_instances; i++){
            ArffInstance * inst = data->get_instance(i);
            attributes[i] = new float[num_attributes-1];
            int k = 0;
            for(int j = 0; j < num_attributes; j++){
                ArffValue *val = inst->get(j);
                switch(val->type()){
                    case INTEGER:
                    case FLOAT:
                        attributes[i][k] = float(*val);
                        k++;
                        break;
                    break;
                    case STRING:
                        if(true_class=="") true_class = string(*val);
                        classes[i] = string(*val)==true_class;
                        break;
                    break;
                    default:
                        cerr << "Error: Formato inesperado." << endl;
                        exit(0);
                }
            }

        }

        //generatePartitions();
        //
        //for(int j=0; j < 2; j++){
        //    for(int i = 0; i < partitions[j].size(); i++){
        //        cout << partitions[j][i] << " ";
        //    }
        //    cout << endl;
        //}
        

        /*
        cout << "Despues de parse" << endl;
    
        cout << data->get_relation_name() << endl;
        cout << data->num_attributes() << endl;
        cout << data->num_instances() << endl;

        for(int i = 0; i < data->num_instances(); i++){
            ArffInstance * inst = data->get_instance(i);
            for(int j = 0; j < data->num_attributes(); j++){
                ArffValue * val = inst->get(j);
                switch(val->type()){
                    case INTEGER:
                        cout << int32(*val) << " ";
                        break;
                    case FLOAT:
                        cout << float(*val) << " ";
                        break;
                    case DATE:
                    case STRING:
                        cout << string(*val) << " ";
                        break;

                    case NUMERIC:

                    case NOMINAL:

                    default:
                    break;
                }
                //cout << *(inst->get(j)) << " ";
            }
            cout << endl;
        }*/
    }
    catch(runtime_error ex){
        cout << ex.what() << endl;
    }

    

}

APCProblem::APCProblem(const string & file_name){
    this->file_name = file_name;
    readFileData();
    normalize();
}

void APCProblem::normalize(){
    for(int j = 0; j < getNumNonClassAttributes(); j++){
        float vmin =  numeric_limits<float>::infinity();
        float vmax = -numeric_limits<float>::infinity();
        for(int i = 0; i < num_instances; i++){
            if(attributes[i][j] < vmin){
                vmin = attributes[i][j];
            }
            if(attributes[i][j] > vmax){
                vmax = attributes[i][j];
            }
        }
        bool constant = vmax==vmin;
        for(int i = 0; i < num_instances; i++){
            float x = attributes[i][j];
            attributes[i][j] = constant?0.5:(x - vmin)/(vmax - vmin);
        }
    }

    //for(int i = 0; i < num_instances; i++){
    //    for(int j = 0; j < getNumNonClassAttributes(); j++){
    //        cout << attributes[i][j] << " ";
    //    }
    //    cout << endl;
    //}

}


APCProblem::~APCProblem(){
    for(int i = 0; i < num_instances; i++){
        delete [] attributes[i];
    }
    delete [] classes;
    delete [] attributes;
}

APCProblem::APCProblem(const APCProblem & other){

}