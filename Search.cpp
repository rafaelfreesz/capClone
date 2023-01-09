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

    double probSwap = (1.0-(double)cloneQty/this->config->clonePop);
    //TODO observar (MUITAS QUANTIDADES DE CLONE TORNA DIFICIL SAIR DO LAÇO) Solução: Limitar a quantidade de swaps
    int swaps=0;

    double randProb=(double)(rand()%100)/100.0;
    do{
        this->population[iClone]->swapFacility(rand() % this->instance->n, rand() % this->instance->n);
        randProb=(double)(rand()%100)/100.0;
        swaps++;
    }while(randProb<probSwap);


    this->population[iClone]->adjustP();
    this->population[iClone]->calculateSolution();

}

//Processo de Re-seleção. Substitui na população original os clones maturados de melhor afinidade
void Search::reselect() {

    int iClone=this->config->pSize;
    int iPop=this->config->pSize-1;

    while(iClone<this->config->arraySize && this->population[iClone]->cost<this->population[iPop]->cost){
        swapAntibody(iClone, iPop--);
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

//Faz o swapFacility de anticorpos no vetor de população
void Search::swapAntibody(int i, int j) {
    Antibody* antibody=this->population[i];

    this->population[i]=this->population[j];
    this->population[j]=antibody;
}

void Search::localSearch(Antibody *antibody) {
    for(int i=0;i<this->instance->n-1;i++){

        for(int j=i+1;j<this->instance->n;j++){
            antibody->swapFacility(i, j);
            antibody->calculateSwap(i,j);
            //testCalculation();
        }
    }
}

void Search::rvnd(Antibody *antibody) {
    bool improved=true;

    int searchSequence[4]={0, 1, 2};
    int fase=0;
    while(fase < 3){

        if(fase==0){
            for(int i=0;i<4;i++){
                Utils::swapInt(rand()%3,rand()%3, searchSequence);
            }
        }

        switch (searchSequence[fase]) {
            case 0:
                if(neighborsSwap(antibody)){
                    fase=0;
                }else{
                    fase++;
                }
                break;
            case 1:
                if(nonNeiborhsSwap(antibody)){
                    fase=0;
                }else{
                    fase++;
                }
                break;
            case 2:
                if(opositeSideSwap(antibody)){
                    fase=0;
                }else{
                    fase++;
                }
                break;
        }
    }
}
//Fase de Trocas lado a lado //TODO Analisar melhor o movimento
bool Search::neighborsSwap(Antibody *antibody) {

    float bestCost=antibody->cost;

    int iE=1;
    int iD=antibody->p+1;

    while(iE<antibody->p && iD<antibody->instance->n){

        antibody->swapFacility(iE-1,iE);
        antibody->calculateSwap(iE-1,iE);

        if(antibody->cost<bestCost){
            return true;
        }else{
            antibody->swapFacility(iE-1,iE);
            antibody->calculateSwap(iE-1,iE);
            iE++;
        }

        antibody->swapFacility(iD-1,iD);
        antibody->calculateSwap(iD-1,iD);

        if(antibody->cost<bestCost){
            return true;
        }else{
            antibody->swapFacility(iD-1,iD);
            antibody->calculateSwap(iD-1,iD);
            iD++;
        }

    }

    while(iE<antibody->p){
        antibody->swapFacility(iE-1,iE);
        antibody->calculateSwap(iE-1,iE);

        if(antibody->cost<bestCost){
            return true;
        }else{
            antibody->swapFacility(iE-1,iE);
            antibody->calculateSwap(iE-1,iE);
            iE++;
        }
    }

    while(iD<antibody->instance->n){
        antibody->swapFacility(iD-1,iD);
        antibody->calculateSwap(iD-1,iD);

        if(antibody->cost<bestCost){
            return true;
        }else{
            antibody->swapFacility(iD-1,iD);
            antibody->calculateSwap(iD-1,iD);
            iD++;
        }
    }

    return false;

}

bool Search::nonNeiborhsSwap(Antibody *antibody) {

    float bestCost=antibody->cost;

    for(int i=0;i<(antibody->p-2);i++){

        for(int j=i+2;j<antibody->p;j++) {
            antibody->swapFacility(i,j);
            antibody->calculateSwap(i,j);

            if (antibody->cost < bestCost) {
                return true;
            } else {
                antibody->swapFacility(i,j);
                antibody->calculateSwap(i,j);
            }
        }
    }

    for(int i=antibody->p;i<(antibody->instance->n-2);i++){

        for(int j=i+2;j<antibody->instance->n;j++) {
            antibody->swapFacility(i,j);
            antibody->calculateSwap(i,j);

            if (antibody->cost < bestCost) {
                return true;
            } else {
                antibody->swapFacility(i,j);
                antibody->calculateSwap(i,j);
            }
        }
    }

    return false;
}
bool Search::opositeSideSwap(Antibody *antibody) {

    float bestCost=antibody->cost;
    for(int i=0;i<antibody->p;i++){
        for(int j=antibody->p;j<antibody->instance->n;j++){
            antibody->swapFacility(i,j);
            antibody->calculateSwap(i,j);

            if (antibody->cost < bestCost) {
                return true;
            } else {
                antibody->swapFacility(i,j);
                antibody->calculateSwap(i,j);
            }
        }
    }
    return false;
}

void Search::PathR(Antibody *antibody) {

    Antibody* bestSolution=antibody->clone();

    /*float foCorrente=this->funcaoObjetivo;

    bool* trocada=new bool[this->qtdSalasAlocadas];
    for(int i=0;i<this->qtdSalasAlocadas;i++) trocada[i]=false;

    int iE=0;
    int iD= this->particao - 1;
    int melhortroca[2]={-1,-1};
    float melhorParcial=-1;

    int nTrocas=this->qtdSalasAlocadas;
    if(this->particao%2==1){
        nTrocas--;
    }
    if((this->qtdSalasAlocadas-this->particao)%2==1){
        nTrocas--;
    }

    int trocas=0;
    while(trocas<nTrocas) {
        while (iE < iD) {
            if(!trocada[iE]) {
                trocarSalas(iE, iD);
                calcularTroca(iE, iD);

                if (this->funcaoObjetivo < melhorParcial || melhorParcial == -1) {
                    melhortroca[0] = iE;
                    melhortroca[1] = iD;
                    melhorParcial = this->funcaoObjetivo;
                }

                trocarSalas(iE, iD);
                //calcularTroca(iE++, iD--);
                arrumarAbcissas(iE++,iD--);
                this->funcaoObjetivo=foCorrente;
            }else{
                iE++;
                iD--;
            }
        }
        iE=this->particao;
        iD=this->qtdSalasAlocadas-1;

        while (iE < iD) {
            if(!trocada[iE]) {
                trocarSalas(iE, iD);
                calcularTroca(iE, iD);

                if (this->funcaoObjetivo < melhorParcial || melhorParcial == -1) {
                    melhortroca[0] = iE;
                    melhortroca[1] = iD;
                    melhorParcial = this->funcaoObjetivo;
                }

                trocarSalas(iE, iD);
                arrumarAbcissas(iE++,iD--);
                this->funcaoObjetivo=foCorrente;
            }else{
                iE++;
                iD--;
            }
        }

        trocas+=2;
        trocarSalas(melhortroca[0], melhortroca[1]);
        calcularTroca(melhortroca[0], melhortroca[1]);
        trocada[melhortroca[0]]=true;
        trocada[ melhortroca[1]]=true;
        foCorrente=this->funcaoObjetivo;

        melhortroca[0] = -1;
        melhortroca[1] = -1;
        melhorParcial = -1;

        iE = 0;
        iD = this->particao - 1;
        if (this->funcaoObjetivo < melhorSolucao->funcaoObjetivo) {
            melhorSolucao->armazenar(this->corredorSolucao, this->particao, this->funcaoObjetivo);
        }


    }
    melhorSolucao->restaurar(this->corredorSolucao, &this->particao, &this->funcaoObjetivo);
    montarAbcissas();

    delete melhorSolucao;
    delete[] trocada;*/

}




