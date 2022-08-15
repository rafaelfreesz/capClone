//
// Created by rafael on 09/08/22.
//

#ifndef CAPCLONE_CONFIG_H
#define CAPCLONE_CONFIG_H


#include <cmath>

class Config {

public:

    Config(int pSize, int gen, int nClonalSelection, double betaCoeff, double regRate, int execs) {
        this->pSize=pSize;
        this->gen=gen;
        this->nClonalSelection=nClonalSelection;
        this->betaCoeff=betaCoeff;
        this->regQty= this->pSize * regRate;
        this->execs=execs;

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



        this->clonesPerI=new int [this->nClonalSelection];
        this->clonePop=0;

        int betaN=this->betaCoeff*this->pSize;

        for(int i=0;i<this->nClonalSelection; i++){

            int clones= max(1,betaN / (i+1));
            this->clonesPerI[i]=clones;
            this->clonePop+=clones;
        }
    }

    void print(){
        cout<<"pSize: "<<pSize<<endl;
        cout<<"gen: "<<gen<<endl;
        cout<<"nClonalSelection: "<<nClonalSelection<<endl;
        cout<<"betaCoeff: "<<betaCoeff<<endl;
        cout<<"clonePop: "<<clonePop<<endl;
        cout<<"arraySize: "<<arraySize<<endl;
        cout<<"regQty: "<<regQty<<endl;
        cout<<"seeds: "<<endl<<"\t";
        for(int i=0;i<execs;i++){
            cout<<this->seeds[i]<<" ";
        }
        cout<<"clonesPerI: ";
        for(int i=0;i<nClonalSelection;i++){
            cout<<this->clonesPerI[i]<<" ";
        }
        cout<<endl;

    }

    int pSize;
    int gen;
    int nClonalSelection;
    double betaCoeff;
    int clonePop;
    int arraySize;
    int regQty;
    int execs;
    long* seeds;
    int* clonesPerI;


};


#endif //CAPCLONE_CONFIG_H
