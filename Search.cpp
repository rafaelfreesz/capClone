//
// Created by rafael on 03/08/22.
//

#include <algorithm>
#include "Search.h"

Search::Search(Config *config, Instance *instance) {
    this->config=config;
    this->population= new Antigen* [this->config->arraySize];
    this->instance=instance;

}

Search::~Search() {

    deletePopulation();

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

    for(int i=0;i<this->config->nClonalSelection;i++){

        for(int j=0;j<this->config->clonesPerI[i];j++){
            this->population[iClone]=this->population[i]->clone(); //Clonagem do antigeno
            maturate(iClone++,this->config->clonesPerI[i]); //Maturação (mutação) do indivíduo
        }
    }
    sortClones();
}

//Processo de maturação (mutação)
void Search::maturate(int iClone, int cloneQty) {

    double probSwap = (1.0-(double)cloneQty/this->config->clonePop);
    //TODO observar (MUITAS QUANTIDADES DE CLONE TORNA DIFICIL SAIR DO LAÇO) Solução: Limitar a quantidade de swaps
    int swaps=0;
    int swapsLimit=this->population[iClone]->instance->n/2;

    double randProb=(double)(rand()%100)/100.0;

    while(randProb<probSwap && swaps++<swapsLimit){
        this->population[iClone]->swap(rand()%this->instance->n,rand()%this->instance->n);
        randProb=(double)(rand()%100)/100.0;
    }

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
    for(int i=0;i<this->config->regQty;i++){
        this->population[iReg]->shake(this->instance->n);
        this->population[iReg--]->calculateSolution();
    }
    sortPopulation();
}

//Constroi população inicial de antigenos
void Search::buildInitialPopulation() {

    if(this->config->pSize>0){

        this->population = new Antigen*[this->config->arraySize];

        buildAntigen(0);
        for(int i=1;i<this->config->pSize;i++){
            buildAntigen(i);
        }

        sortPopulation();

    }else{
        cout<<"ERROR: config->pSize<=0"<<endl;
        exit(1);
    }
}

//Constroi um antigeno aleatoriamente com base no outro
void Search::buildAntigen(int index) {

    if(index!=FIRST_ANTIGEN){
        this->population[index]=this->population[index-1]->clone();

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

//Deleta a população inteira
void Search::deletePopulation() {
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

//Impressao da população
void Search::printPopulation() {

    cout<<"----POPULACAO---- "<<endl<<this->instance->name<<endl;
    for(int i=0;i<this->config->pSize;i++){
        cout<<"-- "<<i<<" ";
        this->population[i]->print();
        /*if(i>0 && this->population[i]->cost<this->population[i-1]->cost){
            cout<<"Erro de ordenação"<<endl;
            exit(1);
        }*/
    }

}

//Impressao dos Clones
void Search::printClones() {

    cout<<"----CLONES---- "<<endl<<this->instance->name<<endl;
    for(int i=this->config->pSize;i<this->config->arraySize; i++){
        this->population[i]->print();
        /*if(i>this->config->pSize && this->population[i]->cost<this->population[i-1]->cost){
            cout<<"Erro de ordenação"<<endl;
            exit(1);
        }*/
    }

}

//Critério de comparação de antigentos (decrescente)
bool Search::antigenCriterion(Antigen *a, Antigen *b) {
    return a->cost<b->cost;
}

//Ordenação da população
void Search::sortPopulation() {
    stable_sort(this->population,this->population+config->pSize, antigenCriterion);
}

//Ordenação dos clones
void Search::sortClones() {
    stable_sort(this->population+config->pSize,this->population+config->arraySize, antigenCriterion);
}

//Faz o swap de antigenos no vetor de população
void Search::swap(int i, int j) {
    Antigen* antigen=this->population[i];

    this->population[i]=this->population[j];
    this->population[j]=antigen;
}





