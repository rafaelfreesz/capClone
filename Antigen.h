//
// Created by rafael on 26/07/22.
//

#ifndef CAPCLONE_ANTIGEN_H
#define CAPCLONE_ANTIGEN_H


#include "Instance.h"

class Antigen {
public:

    Antigen(Instance* instance);
    ~Antigen();

    void calculateAbcissa();
    void calculatCost();
    void calculateSolution();

    void print();

    int* layout;
    float* abcissa;
    int p;
    float cost;
    Instance* instance;
};


#endif //CAPCLONE_ANTIGEN_H
