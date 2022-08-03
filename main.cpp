#include <iostream>
#include "Instance.h"
#include "Utils.h"
#include "Antigen.h"
#include "Search.h"
#include <vector>

using namespace std;


int main() {
    long semente=3601;//clock();
    srand(semente);

    int pSize=10;

    vector<Instance*> instances = Utils::loadInstances();

    for(int i=0;i<instances.size();i++){
        Search* search = new Search(pSize,instances.at(i));
        search->buildInitialPopulation();
        search->printPopulation();

    }

    return 0;
}


