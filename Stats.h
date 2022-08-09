//
// Created by rafael on 09/08/22.
//

#ifndef CAPCLONE_STATS_H
#define CAPCLONE_STATS_H
#include <fstream>
#include <vector>
#include "Instance.h"

using namespace std;
class Stats {
public:
    Stats(int execs, int intancesQty);

    ~Stats();

    void setStat(int execI, int instanceI, double time, double cost);
    double getTime(int execI, int instanceI);
    double getCost(int execI, int instanceI);
    void printStats(vector<Instance*> instances);

    int instancesQty;
    int execs;

    double** times;
    double** costs;

    double* bestTimes;
    double* bestCosts;

    double* avgTimes;
    double* avgCosts;
};


#endif //CAPCLONE_STATS_H
