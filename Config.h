//
// Created by rafael on 09/08/22.
//

#ifndef CAPCLONE_CONFIG_H
#define CAPCLONE_CONFIG_H


#include <cmath>

class Config {

public:

    Config(int pSize, int gen, int nClonalSelection, double betaCoeff, double regRate, long seed) {
        this->pSize=pSize;
        this->gen=gen;
        this->nClonalSelection=nClonalSelection;
        this->betaCoeff=betaCoeff;
        this->regQty= this->pSize * regRate;
        this->seed=seed;

        countClonalSelection();
        this->arraySize= this->pSize + this->clonePop;

    };

    ~Config() {
        delete[] this->clonesPerI;
    }

    void countClonalSelection(){
        this->clonesPerI=new int [this->nClonalSelection];
        this->clonePop=0;
        for(int i=0;i<this->nClonalSelection; i++){
            int clones= this->nClonalSelection / (i+1);
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
        cout<<"seed: "<<seed<<endl;
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
    long seed;
    int* clonesPerI;


};


#endif //CAPCLONE_CONFIG_H
