//
// Created by rafael on 03/08/22.
//

#ifndef CAPCLONE_SEARCH_H
#define CAPCLONE_SEARCH_H


#include "Antigen.h"
#define FIRST_ANTIGEN 0

class Search {
public:
    Search(int pSize, Instance *instance);
    ~Search();

    void buildInitialPopulation();

    void printPopulation();
    //Atributos;
    int pSize;
    Antigen** population;
    Instance* instance;

private:
    void deletePopulation();
    void buildAntigen(Antigen *agReference, int index);
    static bool antigenCriterion(Antigen* a, Antigen* b);
    void sortPopulation();

};


#endif //CAPCLONE_SEARCH_H
