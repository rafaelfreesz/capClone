//
// Created by rafael on 19/07/22.
//

#ifndef CAPCLONE_INSTANCE_H
#define CAPCLONE_INSTANCE_H

#include <iostream>
#include <vector>

using namespace std;
class Instance {

public:

    Instance(int n);
    ~Instance();

    void print();

    void verify();


    string name;
    int n;
    int* length;
    int** demand;

};


#endif //CAPCLONE_INSTANCE_H
