//
// Created by rafael on 19/07/22.
//

#include <fstream>
#include "Instance.h"

Instance::Instance(int n){
    this->n=n;
    this->length = new int[n];
    this->demand = new int*[n];
    for(int i=0;i<n;i++){
        this->demand[i]=new int[n];
    }

}

Instance::~Instance() {
    delete[] this->length;
    for(int i=0;i<this->n;i++){
        delete []this->demand[i];
    }
    delete [] this->demand;
}


void Instance::print() {

    cout<<"Name:"<<this->name<<endl;
    cout<<"N:"<<this->n<<endl;
    cout<<"Lenths:{";
    for(int i=0;i<n;i++) {
        cout << to_string(this->length[i]) + " ";
    }
    cout<<"}"<<endl;
    cout<<"Demands:"<<endl;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
            cout << to_string(this->demand[i][j]) + " ";
        }
        cout<<endl;
    }
    cout<<"---------------------------"<<endl;


}

void Instance::verify() {
    if(this->demand!=nullptr){
        for(int i=0;i<this->n;i++){
            for(int j=0;j<this->n;j++){
                if(this->demand[i][j]!=this->demand[j][i]){
                    cout<<"Diferença de elementos da matriz, instancia "<<this->name<<endl;
                    exit(0);
                }
            }
        }
    }else{
        cout<<"Não instanciada"<<endl;
        exit(0);
    }

}

