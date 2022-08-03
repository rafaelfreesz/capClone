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

void Antigen::swap(int i, int j) {

    int fI=this->layout[i];

    this->layout[i]=this->layout[j];
    this->layout[j]=fI;
}

Antigen *Antigen::clone() {
    Antigen* agClone = new Antigen(this->instance);

    agClone->p=this->p;
    agClone->cost=this->cost;

    for(int i=0;i<this->instance->n;i++){
        agClone->abcissa[i]=this->abcissa[i];
        agClone->layout[i]=this->layout[i];
    }

    return agClone;
}

//Ajusta o valor de p balanceado
void Antigen::adjustP() {

    int factor=1;
    this->p=0;

    //Randomiza se vai começar a contar os comprimentos pelo lado direito ou esquerdo
    if(rand()%2==1) {
        this->p=this->instance->n-1;
        factor=-1;
    }

    float sizeLength=0;
    float halfLayoutLenght=this->instance->layoutLengh/2;
    while(sizeLength<halfLayoutLenght){
        sizeLength+=this->instance->lengths[this->layout[this->p]];
        this->p+=factor;
    }
}
//Bagunga o layout
void Antigen::shake(int size) {
    for(int i=0;i<this->instance->n;i++){
        swap(rand()%this->instance->n,rand()%this->instance->n);
    }
}


void Antigen::calculateAbcissa() {

    float tamAcum=0;

    for(int i=0;i<this->p;i++){
        float length=(float)this->instance->lengths[this->layout[i]];
        tamAcum+=length;
        this->abcissa[i]=tamAcum-length/2;
    }

    tamAcum=0;

    for(int i=this->instance->n-1;i>=this->p;i--){
        float length=(float)this->instance->lengths[this->layout[i]];
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
            this->cost+= abs(this->abcissa[i]-this->abcissa[j])*this->instance->demands[fI][fJ];
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









