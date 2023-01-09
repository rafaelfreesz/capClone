//
// Created by rafael on 26/07/22.
//

#ifndef CAPCLONE_ANTIBODY_H
#define CAPCLONE_ANTIBODY_H


#include "Instance.h"

class Antibody {
public:

    Antibody(Instance* instance);
    ~Antibody();

    void swap(int i, int j);
    void shake(int size);
    void adjustP();
    Antibody* clone();

    void calculateAbcissa();
    void calculatCost();
    void calculateSolution();

    //Swap calculating
    void calculateSwap(int i, int j);
    void sameSideCalc(int iMin, int iMax);
    void oppositeSideCalc(int iMin, int iMax);

    void print();

    int* layout;
    float* abcissa;
    int p;
    float cost;
    Instance* instance;
};


#endif //CAPCLONE_ANTIBODY_H
