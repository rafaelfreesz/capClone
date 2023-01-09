//
// Created by rafael on 09/08/22.
//

#ifndef CAPCLONE_CONFIG_H
#define CAPCLONE_CONFIG_H


#include <cmath>

class Config {

public:

    Config(int pSize, int gen, double memorySetRate, double betaCoeff, double regenerationRate, int execs) {
        this->pSize=pSize;
        this->gen=gen;
        this->memorySetSize=(int)(memorySetRate * pSize);
        this->betaCoeff=betaCoeff;
        this->regenerationQty= this->pSize * regenerationRate;
        this->executions=execs;
        this->clonesPerI= nullptr;

        srand(clock());
        this->seeds=new long [execs];
        for(int i=0;i<execs;i++){
            this->seeds[i]=rand();
        }


        countClonalSelection();
        this->arraySize= this->pSize + this->clonePop;

    };

    ~Config() {
        delete[] this->clonesPerI;
        delete[] this->seeds;
    }

    void countClonalSelection(){

        this->clonesPerI=new int [this->memorySetSize];
        this->clonePop=0;

        int betaN=this->betaCoeff*this->pSize;

        for(int i=0;i<this->memorySetSize; i++){

            int clones= max(1,betaN / (i+1));
            this->clonesPerI[i]=clones;
            this->clonePop+=clones;
        }
    }
    void print(){
        cout<<"pSize: "<<pSize<<endl;
        cout<<"gen: "<<gen<<endl;
        cout << "nClonalSelection: " << memorySetSize << endl;
        cout<<"betaCoeff: "<<betaCoeff<<endl;
        cout<<"clonePop: "<<clonePop<<endl;
        cout<<"arraySize: "<<arraySize<<endl;
        cout << "regQty: " << regenerationQty << endl;
        cout<<"seeds: "<<endl<<"\t";
        for(int i=0; i < executions; i++){
            cout<<this->seeds[i]<<" ";
        }
        cout<<"clonesPerI: ";
        for(int i=0; i < memorySetSize; i++){
            cout<<this->clonesPerI[i]<<" ";
        }
        cout<<endl;

    }

    int pSize;
    int gen;
    int memorySetSize;
    double betaCoeff;
    int clonePop;
    int arraySize;
    int regenerationQty;
    int executions;
    long* seeds;
    int* clonesPerI;


};


#endif //CAPCLONE_CONFIG_H
