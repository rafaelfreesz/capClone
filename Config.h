//
// Created by rafael on 09/08/22.
//

#ifndef CAPCLONE_CONFIG_H
#define CAPCLONE_CONFIG_H


#include <cmath>

class Config {

public:

    Config(int pSize, int gen, int nClonalSelection ,double betaCoeff, double regRate) {
        this->pSize=pSize;
        this->gen=gen;
        this->nClonalSelection=nClonalSelection;
        this->betaCoeff=betaCoeff;
        this->regQty= this->pSize * regRate;

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

    int pSize;
    int gen;
    int nClonalSelection;
    double betaCoeff;
    int clonePop;
    int arraySize;
    int regQty;
    int* clonesPerI;


};


#endif //CAPCLONE_CONFIG_H
