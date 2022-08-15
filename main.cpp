#include <iostream>
#include "Instance.h"
#include "Utils.h"
#include "Search.h"
#include "Config.h"
#include "Stats.h"
#include <vector>

using namespace std;


int main() {

    int execs=30;


    vector<Instance*> instances = Utils::loadInstances();

    Config* config = new Config(100, 500, 10, 0.15, 0.05,execs);
    Stats* stats= new Stats(execs, instances.size(), config);

    for(int i=0;i<instances.size();i++){
        cout<<"Instância "+instances.at(i)->name<<endl;

        for(int j=0; j < execs; j++) {
            srand(config->seeds[j]);
            cout<<"\t"<<j<<" - ";

            Search* search = new Search(config,instances.at(i));

            clock_t time=clock();
            search->evolve();
            time=clock()-time;

            stats->setStat(j, i, ((double) time / CLOCKS_PER_SEC), search->population[0]->cost);

            cout<<"time: "<<to_string(stats->getTime(j,i))<<"s | cost: "<< to_string(stats->getCost(j,i))<<endl;


            delete search;
        }

        stats->printStats(instances.at(i)->name,i);
        cout<<endl<<"RESUME: BEST TIME: "<<to_string(stats->bestTimes[i])<<"s | BEST COST: "<<to_string(stats->bestCosts[i])<<" | LitSol: "<<to_string(stats->litSol[i])<<" | GAP: "<<to_string(stats->gapsSol[i])<<endl<<endl;

    }


    delete config;
    delete stats;

    return 0;
}


