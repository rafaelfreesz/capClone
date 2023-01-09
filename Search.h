//
// Created by rafael on 03/08/22.
//

#ifndef CAPCLONE_SEARCH_H
#define CAPCLONE_SEARCH_H


#include "Antibody.h"
#include "Config.h"

#define FIRST_ANTIBODY 0

class Search {
public:
    Search(Config *config, Instance *instance, double litSol);
    ~Search();

    void evolve();
    void buildInitialPopulation();
    void operate();
    void maturate(int iClone, int cloneQty, int iMemSet);
    void reselect();
    void regenerate();


    void printPopulation();
    void printClones();
    void printAll();

    //Atributos;
    Config* config;
    Antibody** population;
    Instance* instance;
    double litSol;

private:
    void buildAntibody(int index);
    static bool antibodyCriterion(Antibody* a, Antibody* b);
    void sortPopulation();
    void sortClones();
    void swap(int i, int j);

};


#endif //CAPCLONE_SEARCH_H
