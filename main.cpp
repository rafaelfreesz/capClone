#include <iostream>
#include "Instance.h"
#include "Utils.h"
#include "Search.h"
#include "Config.h"
#include <vector>

using namespace std;


int main() {
    long semente=clock();
    srand(semente);

    int execs=30;

    vector<Instance*> instances = Utils::loadInstances();
    Config* config = new Config(100,500,5,0.15,0.05);

    for(int i=0;i<instances.size();i++){
        cout<<"Instância "+instances.at(i)->name<<endl;

        double avgTime=0.0;
        double bestCost=0.0;
        for(int j=0; j < execs; j++) {

            cout<<"\t"<<j<<" - ";

            Search* search = new Search(config,instances.at(i));

            clock_t time=clock();
            search->evolve();
            time=clock()-time;

            avgTime+=((double) time / CLOCKS_PER_SEC);

            if(j==0 || search->population[0]->cost<bestCost){
                bestCost=search->population[0]->cost;
            }

            cout<<"time: "<<to_string(((double) time / CLOCKS_PER_SEC))<<"s | cost: "<< to_string(search->population[0]->cost)<<endl;

            delete search;
        }

        avgTime/=(double)execs;

        cout<<endl<<"RESUME: BEST TIME: "<<to_string(avgTime)<<"s | BEST COST: "<<bestCost<<endl<<endl;


    }

    delete config;

    return 0;
}

/*TODO verificar o balanceamento desta solução
Abcissa:
0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
Cost: 2829.500000
Layout:
E: 2 5
D: 3 7 6 4 0 1 8 */


