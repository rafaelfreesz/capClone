//
// Created by rafael on 26/07/22.
//

#include "Antibody.h"

Antibody::Antibody(Instance *instance) {
    this->instance=instance;
    this->layout=new int [instance->n];
    this->abcissa=new float[instance->n];
    this->p=0;
    this->cost=0;
}

Antibody::~Antibody() {

    delete []this->layout;
    delete []this->abcissa;
}

void Antibody::swap(int i, int j) {

    int fI=this->layout[i];

    this->layout[i]=this->layout[j];
    this->layout[j]=fI;
}

Antibody *Antibody::clone() {
    Antibody* agClone = new Antibody(this->instance);

    agClone->p=this->p;
    agClone->cost=this->cost;

    for(int i=0;i<this->instance->n;i++){
        agClone->abcissa[i]=this->abcissa[i];
        agClone->layout[i]=this->layout[i];
    }

    return agClone;
}

//Ajusta o valor de p balanceado
void Antibody::adjustP() {

    int factor=1;
    this->p=0;

    //Randomiza se vai começar a contar os comprimentos pelo lado direito ou esquerdo
    if(rand()%2==1) {
        this->p=this->instance->n-1;
        factor=-1;
    }

    float sideLength=0;
    float halfLayoutLenght=this->instance->layoutLengh/2;
    while(sideLength < halfLayoutLenght){
        sideLength+=this->instance->lengths[this->layout[this->p]];
        this->p+=factor;
    }
}
//Bagunça o layout
void Antibody::shake(int size) {
    for(int i=0;i<this->instance->n;i++){
        swap(rand()%this->instance->n,rand()%this->instance->n);
    }
}


void Antibody::calculateAbcissa() {

    float tamAcum=0;

    for(int i=0;i<this->p;i++){
        float length=(float)this->instance->lengths[this->layout[i]];
        tamAcum+=length;
        this->abcissa[this->layout[i]]=tamAcum-length/2;
    }

    tamAcum=0;

    for(int i=this->instance->n-1;i>=this->p;i--){
        float length=(float)this->instance->lengths[this->layout[i]];
        tamAcum+=length;
        this->abcissa[this->layout[i]]=tamAcum-length/2;
    }



}

void Antibody::calculatCost() {

    this->cost=0.0;

    int limI=this->instance->n-1;
    int limJ=this->instance->n;
    for(int i=0;i<limI;i++){
        for(int j=i+1;j<limJ;j++){
            int fI=this->layout[i];
            int fJ=this->layout[j];
            this->cost+= abs(this->abcissa[fI]-this->abcissa[fJ])*this->instance->demands[fI][fJ]; //TODO Corrigir this->abcissa[i/j] para this->abicissa[fI/fJ]
        }
    }
}

void Antibody::calculateSolution() {
    calculateAbcissa();
    calculatCost();
}

void Antibody::print() {

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

    cout<<"Inline: ";
    for(int i=0;i<this->instance->n;i++){
        cout<<to_string(this->layout[i])+" ";
    }
    cout<<endl;

    cout<<"Abcissa:"<<endl;
    for(int i=0;i<this->instance->n;i++){
        cout<<to_string(this->abcissa[this->layout[i]])+" ";
    }
    cout<<endl;

    cout<<"Cost: "+ to_string(this->cost)<<endl;
}

void Antibody::calculateSwap(int i, int j) {
    int iMin,iMax;
    if(i<j){
        iMin=i;
        iMax=j;
    }else{
        iMin=j;
        iMax=i;
    }

    if(iMin<this->p && iMax>=this->p){
        oppositeSideCalc(iMin, iMax); //TODO ver isso
    }else {
        sameSideCalc(iMin, iMax);
    }
}

void Antibody::sameSideCalc(int iMin, int iMax) {
    int iMinFacility=this->layout[iMin];
    int iMaxFacility=this->layout[iMax];

    float iMinNewAbcissa=this->abcissa[iMaxFacility]-this->instance->halfLengths[iMinFacility]+this->instance->halfLengths[iMaxFacility];
    float iMaxNewAbcissa=this->abcissa[iMinFacility]-this->instance->halfLengths[iMinFacility]+this->instance->halfLengths[iMaxFacility];

    int zoneLimits[8]; //{zone1,zone2,zone3,zone4} *Zone 2 may not exist
    //Setting Zone Limits
    if(iMinFacility<this->p){
        zoneLimits[0]=0;
        zoneLimits[1]=iMin;           //Zone 1 =[0,..,iMin)
        zoneLimits[2]=iMin+1;
        zoneLimits[3]=iMax;             //Zone 2 =[iMin+1,...,iMax)
        zoneLimits[4]=iMax+1;
        zoneLimits[5]=this->p;          //Zone 3= [iMax+1,...,p)
        zoneLimits[6]=this->p;
        zoneLimits[7]=this->instance->n;   //Zone 4= [p,...,n)
    }else{
        zoneLimits[0]=this->p;
        zoneLimits[1]=iMin;           //Zone 1 =[p,..,iMin)
        zoneLimits[2]=iMin+1;
        zoneLimits[3]=iMax;             //Zone 2 =[iMin+1,...,iMax)
        zoneLimits[4]=iMax+1;
        zoneLimits[5]=this->instance->n;   //Zone 3= [iMax+1,...,assigned)
        zoneLimits[6]=0;
        zoneLimits[7]=this->p;          //Zone 3= [0,...,p)
    }

    float deltaDist, deltaCost;
    int iFacility, jFacility;
    //Delta calculating by iMin & iMax with zones 1, 3, and 4
    int z=0;
    while(z<8){
        for(int i=zoneLimits[z];i<zoneLimits[z+1];i++){ //TODO maybe one for is necessary
            //iMin x zone z
            iFacility=this->layout[i];
            deltaDist=abs(iMinNewAbcissa-this->abcissa[iFacility])-abs(this->abcissa[iMinFacility]-this->abcissa[iFacility]);
            deltaCost=deltaDist*this->instance->demands[iMinFacility][iFacility];
            this->cost+=deltaCost;
            //iMax x zone z
            iFacility=this->layout[i];
            deltaDist=abs(iMaxNewAbcissa-this->abcissa[iFacility])-abs(this->abcissa[iMaxFacility]-this->abcissa[iFacility]);
            deltaCost=deltaDist*this->instance->demands[iMaxFacility][iFacility];
            this->cost+=deltaCost;
        }

        if(z==0){
            z+=4;
        }else{
            z+=2;
        }
    }

    //Delta calculating by zone 2 with iMin & iMax *Zone 2 may not exist
    float zone2Delta=(float)(this->instance->lengths[iMaxFacility]-this->instance->lengths[iMinFacility]);
    for(int i=zoneLimits[2];i<zoneLimits[3];i++){
        //zone 2 -> iMin
        iFacility=this->layout[i]; //TODO maybe the deltaDistance is the same for all zone 2 facilities
        deltaDist=abs(iMinNewAbcissa-(this->abcissa[iFacility]+zone2Delta))-abs(this->abcissa[iMinFacility]-this->abcissa[iFacility]);
        deltaCost=deltaDist*this->instance->demands[iMinFacility][iFacility];
        this->cost+=deltaCost;

        //zone 2 -> iMax
        deltaDist=abs(iMaxNewAbcissa-(this->abcissa[iFacility]+zone2Delta))-abs(this->abcissa[iMaxFacility]-this->abcissa[iFacility]);
        deltaCost=deltaDist*this->instance->demands[iMaxFacility][iFacility];
        this->cost+=deltaCost;

        //Delta calculating by zone 2 with zones 1, 3 and 4 TODO needed only if l(iMinFacility)!= l(iMaxFacility)
        if(zone2Delta!=0.0) {
            z = 0;
            while (z < 8) {

                for (int j = zoneLimits[z]; j < zoneLimits[z + 1]; j++) {
                    jFacility = this->layout[j];
                    deltaDist = abs(this->abcissa[jFacility] - (this->abcissa[iFacility] + zone2Delta)) -
                                abs(this->abcissa[jFacility] - this->abcissa[iFacility]);
                    deltaCost = deltaDist * this->instance->demands[iFacility][jFacility];
                    this->cost += deltaCost;
                }

                if (z == 0) {
                    z += 4;
                } else {
                    z += 2;
                }
            }
        }
    }

    //UpdatingAbcissa //TODO PAREI AQUI
    this->abcissa[iMaxFacility]=iMaxNewAbcissa;
    this->abcissa[iMinFacility]=iMinNewAbcissa;

    float deltaAbcissa = this->instance->lengths[iMaxFacility]-this->instance->lengths[iMinFacility];
    for(int i=iMin+1;i<iMax;i++){
        this->abcissa[this->layout[i]]+=deltaAbcissa;
    }
}

void Antibody::oppositeSideCalc(int iMin, int iMax) {
    int iMinFacility=this->layout[iMin];
    int iMaxFacility=this->layout[iMax];

    float iMinNewAbcissa=this->abcissa[iMaxFacility]-this->instance->halfLengths[iMinFacility]+this->instance->halfLengths[iMaxFacility];
    float iMaxNewAbcissa=this->abcissa[iMinFacility]-this->instance->halfLengths[iMinFacility]+this->instance->halfLengths[iMaxFacility];

    int zoneLimits[8]; //{zone1,zone2,zone3,zone4} *Zone 2 and zone 4 may not exist
    //Setting Zone Limits
    if(iMinFacility<this->p){
        zoneLimits[0]=0;
        zoneLimits[1]=iMin;           //Zone 1 =[0,..,iMin)
        zoneLimits[2]=iMin+1;
        zoneLimits[3]=this->p;             //Zone 2 =[iMin+1,...,p)
        zoneLimits[4]=this->p;
        zoneLimits[5]=iMax;          //Zone 3= [p,...,iMax)
        zoneLimits[6]=iMax+1;
        zoneLimits[7]=this->instance->n;   //Zone 4= [iMax+1,...,assigned)
    }

    float deltaDist, deltaCost;
    int iFacility, jFacility;

    //Delta calculating between iMin & iMax
    deltaDist=abs(iMinNewAbcissa-iMaxNewAbcissa)-abs(this->abcissa[iMinFacility]-this->abcissa[iMaxFacility]);
    deltaCost=deltaDist*this->instance->demands[iMinFacility][iMaxFacility];
    this->cost+=deltaCost;

    //Delta calculating by iMin & iMax with zones 1, 2, 3, and 4
    if(deltaDist!=0.0) {
        int z = 0;
        while (z < 8) {
            for (int i = zoneLimits[z]; i < zoneLimits[z + 1]; i++) {
                //iMin x zone z
                iFacility = this->layout[i];
                deltaDist = abs(iMinNewAbcissa - this->abcissa[iFacility]) -
                            abs(this->abcissa[iMinFacility] - this->abcissa[iFacility]);
                deltaCost = deltaDist * this->instance->demands[iMinFacility][iFacility];
                this->cost += deltaCost;
                //iMax x zone z
                iFacility = this->layout[i];
                deltaDist = abs(iMaxNewAbcissa - this->abcissa[iFacility]) -
                            abs(this->abcissa[iMaxFacility] - this->abcissa[iFacility]);
                deltaCost = deltaDist * this->instance->demands[iMaxFacility][iFacility];
                this->cost += deltaCost;
            }

            z += 2;
        }
    }
    float zoneDelta=(float)(this->instance->lengths[iMaxFacility] - this->instance->lengths[iMinFacility]);
    if(zoneDelta != 0.0){
        //Delta calculating by zone 2 with zones 1,3 and 4 *zones 2 and/or 4 may not exist
        for(int i=zoneLimits[2];i<zoneLimits[3];i++){
            int z=0;

            while(z<8){

                for(int j=zoneLimits[z];j<zoneLimits[z+1];j++){
                    jFacility = this->layout[j];
                    deltaDist = abs(this->abcissa[jFacility] - (this->abcissa[iFacility] + zoneDelta)) -
                                abs(this->abcissa[jFacility] - this->abcissa[iFacility]);
                    deltaCost = deltaDist * this->instance->demands[iFacility][jFacility];
                    this->cost += deltaCost;
                }


                if(z==0){
                    z+=4;
                }else{
                    z+=2;
                }
            }
        }
        //Delta calculating by zone 4 with zones 1 and 3 *zones 4 may not exist
        zoneDelta=-zoneDelta;
        for(int i=zoneLimits[6];i<zoneLimits[7];i++){
            int z=0;

            while(z<8){

                for(int j=zoneLimits[z];j<zoneLimits[z+1];j++){
                    jFacility = this->layout[j];
                    deltaDist = abs(this->abcissa[jFacility] - (this->abcissa[iFacility] + zoneDelta)) -
                                abs(this->abcissa[jFacility] - this->abcissa[iFacility]);
                    deltaCost = deltaDist * this->instance->demands[iFacility][jFacility];
                    this->cost += deltaCost;
                }

                z+=4;
            }
        }
    }
    //UpdatingAbcissa //TODO TO TEST
    float deltaAbcissa = iMaxNewAbcissa-this->abcissa[iMaxFacility];

    for(int i=iMin;i<this->p;i++){
        this->abcissa[this->layout[i]]+=deltaAbcissa;
    }

    for(int i=iMax;i<this->instance->n;i++){
        this->abcissa[this->layout[i]]-=deltaAbcissa;
    }
}













