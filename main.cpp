#include <iostream>
#include "Instance.h"
#include "Utils.h"
#include "Search.h"
#include "Config.h"
#include "Stats.h"
#include <vector>

using namespace std;


int main() {
    long semente=clock();
    srand(semente);

    int execs=30;


    vector<Instance*> instances = Utils::loadInstances();

    Stats* stats=new Stats(execs,instances.size());
    Config* config = new Config(100,500,5,0.15,0.05);

    for(int i=0;i<instances.size();i++){
        cout<<"Instância "+instances.at(i)->name<<endl;

        for(int j=0; j < execs; j++) {

            cout<<"\t"<<j<<" - ";

            Search* search = new Search(config,instances.at(i));

            clock_t time=clock();
            search->evolve();
            time=clock()-time;

            stats->setStat(j, i, ((double) time / CLOCKS_PER_SEC), search->population[0]->cost);

            cout<<"time: "<<to_string(stats->getTime(j,i))<<"s | cost: "<< to_string(stats->getCost(j,i))<<endl;


            delete search;
        }

        cout<<endl<<"RESUME: BEST TIME: "<<to_string(stats->bestTimes[i])<<"s | BEST COST: "<<to_string(stats->bestCosts[i])<<endl<<endl;


    }

    stats->printStats(instances);
    delete config;
    delete stats;

    return 0;
}


