//
// Created by rafael on 09/08/22.
//

#include "Stats.h"

Stats::Stats(int execs, int intancesQty) {
    this->execs=execs;
    this->instancesQty=intancesQty;

    this->bestTimes=new double [intancesQty];
    this->bestCosts=new double [intancesQty];
    this->avgTimes=new double [intancesQty];
    this->avgCosts=new double [intancesQty];
    this->times=new double * [intancesQty];
    this->costs=new double * [intancesQty];
    this->costs=new double * [intancesQty];

    for(int i=0;i<intancesQty;i++){
        this->times[i]=new double [execs];
        this->costs[i]=new double [execs];
        this->bestTimes[i]=0.0;
        this->bestCosts[i]=0.0;
        this->avgTimes[i]=0.0;
        this->avgCosts[i]=0.0;
    }

}

Stats::~Stats() {

    for(int i=0;i<this->instancesQty;i++){
        delete []this->times[i];
        delete []this->costs[i];
    }

    delete []this->times;
    delete []this->costs;
    delete []this->bestTimes;
    delete []this->bestCosts;
    delete []this->avgTimes;
    delete []this->avgCosts;

}

void Stats::setStat(int execI, int instanceI, double time, double cost) {
    this->times[instanceI][execI]=time;
    this->costs[instanceI][execI]=cost;

    this->avgTimes[instanceI]+=time;
    this->avgCosts[instanceI]+=cost;

    if(execI==0 || time<this->bestTimes[instanceI]){
        this->bestTimes[instanceI]=time;
    }

    if(execI==0 || cost<this->bestCosts[instanceI]){
        this->bestCosts[instanceI]=cost;
    }

    if(execI==(this->execs-1)){
        this->avgTimes[instanceI]/=this->execs;
        this->avgCosts[instanceI]/=this->execs;
    }

}

double Stats::getTime(int execI, int instanceI) {
    return this->times[instanceI][execI];
}

double Stats::getCost(int execI, int instanceI) {
    return this->costs[instanceI][execI];
}

void Stats::printStats(vector<Instance *> instances) {

    ofstream statsFile("stats");

    string linha="Instancia;avgTime;avgCost;bestTime;bestCost";

    statsFile<<linha<<endl;

    for(int i=0;i<this->instancesQty;i++){
        linha=instances.at(i)->name+";" + to_string(this->avgTimes[i]) +";"+ to_string(this->avgCosts[i]) +";"+ to_string(this->bestTimes[i]) +";"+ to_string(this->bestCosts[i]);
        statsFile<<linha<<endl;
    }

}

