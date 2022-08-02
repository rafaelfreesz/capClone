//
// Created by rafael on 26/07/22.
//

#include "Antigen.h"

Antigen::Antigen(Instance *instance) {
    this->instance=instance;
    this->layout=new int [instance->n];
    this->abcissa=new float[instance->n];
    this->p=0;
    this->cost=0;
}

Antigen::~Antigen() {
    delete []this->layout;
    delete []this->abcissa;
}

void Antigen::calculateAbcissa() {

    float tamAcum=0;

    for(int i=0;i<this->p;i++){
        float length=(float)this->instance->length[this->layout[i]];
        tamAcum+=length;
        this->abcissa[i]=tamAcum-length/2;
    }

    tamAcum=0;

    for(int i=this->instance->n-1;i>=this->p;i--){
        float length=(float)this->instance->length[this->layout[i]];
        tamAcum+=length;
        this->abcissa[i]=tamAcum-length/2;
    }



}

void Antigen::calculatCost() {

    this->cost=0.0;

    int limI=this->instance->n-1;
    int limJ=this->instance->n;
    for(int i=0;i<limI;i++){
        for(int j=i+1;j<limJ;j++){
            int fI=this->layout[i];
            int fJ=this->layout[j];
            this->cost+= abs(this->abcissa[i]-this->abcissa[j])*this->instance->demand[fI][fJ];
        }
    }

}

void Antigen::calculateSolution() {
    calculateAbcissa();
    calculatCost();
}

void Antigen::print() {

    cout<<"Layout:"<<endl;
    cout<<"E: ";
    for(int i=0;i<this->p;i++){
        cout<<to_string(this->layout[i])+" ";
    }
    cout<<endl;

    cout<<"D: ";
    for(int i=this->instance->n-1;i>=this->p;i--){
        cout<<to_string(this->layout[i])+" ";
    }
    cout<<endl;

    cout<<"Abcissa:"<<endl;
    for(int i=0;i<this->instance->n;i++){
        cout<<to_string(this->abcissa[i])+" ";
    }
    cout<<endl;

    cout<<"Cost: "+ to_string(this->cost)<<endl;
}



