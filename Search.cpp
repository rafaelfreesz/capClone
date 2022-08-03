//
// Created by rafael on 03/08/22.
//

#include <algorithm>
#include "Search.h"

Search::Search(int pSize, Instance *instance) {
    this->pSize=pSize;
    this->population= nullptr;
    this->instance=instance;

}

Search::~Search() {

    deletePopulation();

}

//Constroi população inicial de antigenos
void Search::buildInitialPopulation() {

    deletePopulation();

    if(this->pSize>0){

        this->population = new Antigen*[this->pSize];

        buildAntigen(nullptr,0);
        for(int i=1;i<this->pSize;i++){
            buildAntigen(this->population[i-1],i);
        }

        sortPopulation();

    }else{
        cout<<"ERROR: pSize<=0"<<endl;
        exit(1);
    }
}

//Constroi um antigeno aleatoriamente com base no outro
void Search::buildAntigen(Antigen *agReference, int index) {

    if(index!=FIRST_ANTIGEN){
        this->population[index]=agReference->clone();

    }else{
        //Instanciação do primeiro antigeno
        this->population[index] = new Antigen(this->instance);

        for(int i=0;i<this->instance->n;i++){
            this->population[index]->layout[i]=i;
        }
    }

    this->population[index]->shake(this->instance->n);
    this->population[index]->adjustP();
    this->population[index]->calculateSolution();
}

void Search::deletePopulation() {
    if(this->population!= nullptr){
        for(int i=0;i< this->pSize;i++){
            if(this->population[i]!= nullptr) {
                delete[] this->population[i];
            }else{
                break;
            }
        }
        delete[] this->population;

    }
}

void Search::printPopulation() {

    for(int i=0;i<this->pSize;i++){
        this->population[i]->print();
    }

}

bool Search::antigenCriterion(Antigen *a, Antigen *b) {
    return a->cost<b->cost;
}

void Search::sortPopulation() {
    stable_sort(this->population,this->population+pSize, antigenCriterion);
}





