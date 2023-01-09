#include <iostream>
#include "Instance.h"
#include "Utils.h"
#include "Search.h"
#include "Config.h"
#include "Stats.h"
#include <vector>

using namespace std;


int main() {

    vector<Instance*> instances = Utils::loadInstances();
    //500, 5000, 0.2, 0.50, 0.1,30
    Config* config = new Config(100, 500, 0.1, 0.2, 0.4,30);
    Stats* stats= new Stats(instances.size(), config);


    for(int i=0;i<instances.size();i++){
        cout<<"Instância "+instances.at(i)->name<<endl;

        for(int j=0; j < config->executions; j++) {
            srand(config->seeds[j]);
            cout<<"\t"<<j<<" - ";

            Search* search = new Search(config, instances.at(i), stats->litSol[i]);

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


