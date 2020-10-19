#include "UnitObject_class.h"

UnitObject_class::UnitObject_class()
{
    //ctor
}

UnitObject_class::UnitObject_class(int new_UnitType) {
    UnitType = new_UnitType;
}


UnitObject_class::UnitObject_class(int new_HP,int new_UnitType) {
    HP = new_HP;
    UnitType = new_UnitType;
}


UnitObject_class::~UnitObject_class()
{
    //dtor
}


//!-----------------------------!//
//!     GET TYPE FUNCTIONS      !//
//!-----------------------------!//


int UnitObject_class::get_HP() {
    return HP;
}
int UnitObject_class::get_ArmyID() {
    return ArmyID;
}
int UnitObject_class::get_UnitType() {
    return UnitType;
}

//!-----------------------------!//
//!     SET TYPE FUNCTIONS      !//
//!-----------------------------!//



int UnitObject_class::addHP(int added) {
    HP += added;
    return HP;
}

void UnitObject_class::changeType(int New_UnitType) {
    UnitType = New_UnitType;
}

void UnitObject_class::setNew_ArmyID(int new_ArmyID) {
    ArmyID = new_ArmyID;
}



