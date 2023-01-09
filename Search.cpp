//
// Created by rafael on 03/08/22.
//

#include <algorithm>
#include "Search.h"

Search::Search(Config *config, Instance *instance, double litSol) {
    this->config=config;
    this->population= new Antibody* [this->config->arraySize];
    this->instance=instance;
    this->litSol=litSol;

}

Search::~Search() {

    if(this->population!= nullptr){
        for(int i=0;i< this->config->pSize;i++){
            if(this->population[i]!= nullptr) {
                delete this->population[i];
            }else{
                break;
            }
        }
        delete[] this->population;

    }

}

void Search::evolve() {
    buildInitialPopulation();

    for(int g=0;g<this->config->gen;g++){
        operate();
        reselect();
        regenerate();
    }
}
//Processo de clonagem
void Search::operate() {

    int iClone=this->config->pSize;

    for(int i=0;i<this->config->memorySetSize; i++){

        for(int j=0;j<this->config->clonesPerI[i];j++){
            this->population[iClone]=this->population[i]->clone(); //Clonagem do anticorpo
            maturate(iClone++, this->config->clonesPerI[i], i+1); //Maturação (mutação) do indivíduo
        }
    }
    sortClones();
}

//Processo de maturação (mutação)
void Search::maturate(int iClone, int cloneQty, int iMemSet) {

    for(int i=0;i<iMemSet;i++){
        this->population[iClone]->swap(rand()%this->instance->n,rand()%this->instance->n);
    }

    this->population[iClone]->adjustP();
    this->population[iClone]->calculateSolution();

}

//Processo de Re-seleção. Substitui na população original os clones maturados de melhor afinidade
void Search::reselect() {

    int iClone=this->config->pSize;
    int iPop=this->config->pSize-1;

    while(iClone<this->config->arraySize && this->population[iClone]->cost<this->population[iPop]->cost){
        swap(iClone,iPop--);
        delete this->population[iClone++];
    }
    while(iClone<this->config->arraySize){
        delete this->population[iClone++];
    }

    sortPopulation();
}

//Processo de troca dos d piores indivíduos por novos individuos gerados
void Search::regenerate() {

    int iReg=this->config->pSize-1;
    for(int i=0;i<this->config->regenerationQty; i++){
        this->population[iReg]->shake(this->instance->n);
        this->population[iReg--]->adjustP();
        this->population[iReg--]->calculateSolution();
    }
    sortPopulation();
}

//Constroi população inicial de anticorpos
void Search::buildInitialPopulation() {

    if(this->config->pSize>0){

        this->population = new Antibody*[this->config->arraySize];

        for(int i=0;i<this->config->pSize;i++){
            buildAntibody(i);
        }

        sortPopulation();

    }else{
        cout<<"ERROR: config->pSize<=0"<<endl;
        exit(1);
    }
}

//Constroi um anticorpo aleatoriamente com base no outro
void Search::buildAntibody(int index) {

    if(index != FIRST_ANTIBODY){
        this->population[index]=this->population[index-1]->clone();

    }else{
        //Instanciação do primeiro anticorpo
        this->population[index] = new Antibody(this->instance);

        for(int i=0;i<this->instance->n;i++){
            this->population[index]->layout[i]=i;
        }
    }

    this->population[index]->shake(this->instance->n);
    this->population[index]->adjustP();
    this->population[index]->calculateSolution();
}

//Impressao da população
void Search::printPopulation() {

    cout<<"----POPULACAO---- "<<endl<<this->instance->name<<endl;
    for(int i=0;i<this->config->pSize;i++){
        cout<<"-- "<<i<<" ";
        this->population[i]->print();
        if(i>0 && this->population[i]->cost<this->population[i-1]->cost){
            cout<<"Erro de ordenação"<<endl;
            exit(1);
        }
    }

}

void Search::printAll(){
    cout<<"--------Population:"<<endl;
    for(int i=0;i<this->config->pSize;i++){
        if(i>0 && this->population[i]->cost<this->population[i-1]->cost){
            cout<<"Erro de ordenação aqui-> ";
        }
        cout<<this->population[i]->cost<<endl;
    }
    cout<<"--------Clones:"<<endl;
    for(int i=this->config->pSize;i<this->config->arraySize;i++){
        if(i>this->config->pSize && this->population[i]->cost<this->population[i-1]->cost){
            cout<<"Erro de ordenação aqui-> ";
        }
        cout<<this->population[i]->cost<<endl;
    }
}
//Impressao dos Clones
void Search::printClones() {

    cout<<"----CLONES---- "<<endl<<this->instance->name<<endl;
    for(int i=this->config->pSize;i<this->config->arraySize; i++){
        this->population[i]->print();
        if(i>this->config->pSize && this->population[i]->cost<this->population[i-1]->cost){
            cout<<"Erro de ordenação"<<endl;
            exit(1);
        }
    }

}

//Critério de comparação de anticorpos (decrescente)
bool Search::antibodyCriterion(Antibody *a, Antibody *b) {
    return a->cost<b->cost;
}

//Ordenação da população
void Search::sortPopulation() {
    stable_sort(this->population, this->population + config->pSize, antibodyCriterion);
}

//Ordenação dos clones
void Search::sortClones() {
    stable_sort(this->population + config->pSize, this->population + config->arraySize, antibodyCriterion);
}

//Faz o swap de anticorpos no vetor de população
void Search::swap(int i, int j) {
    Antibody* antibody=this->population[i];

    this->population[i]=this->population[j];
    this->population[j]=antibody;
}





