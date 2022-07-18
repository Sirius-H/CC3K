#include <math.h>
#include "merchant.h"

int Merchant::hatred = 0;

Merchant::Merchant(Coordinate cdn): NPC{cdn} {
    HP = 30;
    Atk = 70;
    Def = 5;
}


void Merchant::resetHatred() {
    Merchant::hatred = 0;
}

void Merchant::setHatred() {
    if (Merchant::hatred == 0) {
        Merchant::hatred = 1;
    }
}

int Merchant::attack(int PCDefence) const { 
    if (Merchant::hatred == 0) {
        return 0;
    }
    return ceil(((double)100 / (100 + PCDefence)) * Atk); 
}

std::string Merchant::getName() const {
    return "Merchant";
}


int Merchant::attacked(int dmg) {
    Merchant::hatred = 1;
    int actualDmg = dmg;
    if (HP - actualDmg <= 0) {
        actualDmg = HP;
        HP = 0;
        return actualDmg;
    }
    HP -= actualDmg;
    return actualDmg;
}

