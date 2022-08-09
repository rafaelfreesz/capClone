//
// Created by rafael on 03/08/22.
//

#ifndef CAPCLONE_SEARCH_H
#define CAPCLONE_SEARCH_H


#include "Antigen.h"
#include "Config.h"

#define FIRST_ANTIGEN 0

class Search {
public:
    Search(Config *config, Instance *instance);
    ~Search();

    void evolve();
    void buildInitialPopulation();
    void operate();
    void maturate(int iClone, int cloneQty);
    void reselect();
    void regenerate();


    void printPopulation();
    void printClones();

    //Atributos;
    Config* config;
    Antigen** population;
    Instance* instance;

private:
    void deletePopulation();
    void buildAntigen(int index);
    static bool antigenCriterion(Antigen* a, Antigen* b);
    void sortPopulation();
    void sortClones();
    void swap(int i, int j);

};


#endif //CAPCLONE_SEARCH_H
