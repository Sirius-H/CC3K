#include <math.h>
#include "pc.h"

PC::PC(Coordinate cdn): Cell{cdn} { }

int PC::getDefence() const { return Def; }

int PC::attack(int NPCDefence) const { return ceil(((double)100 / (100 + NPCDefence)) * (Atk + potionAtkEffect)); }

int PC::attacked(int dmg) { // returns the actual damage to PC
    int actualDmg = dmg;
    if (withBarrierSuit) {
        actualDmg = std::ceil((double)dmg / 2);
    }
    if (HP - actualDmg <= 0) {
        actualDmg = HP;
        HP = 0;
        return actualDmg;
    }
    HP -= actualDmg;
    return actualDmg;
}

void PC::applyEffect(int effect) {
    if (effect == 0) { // Restore Health
        if (HP + 10 > maxHP) {
            HP = maxHP;
        } else {
            HP += 10;
        }
    } else if (effect == 1) { // Boost Atk
        potionAtkEffect += 5;
    } else if (effect == 2) { // Boost Def
        pottionDefEffect += 5;
    } else if (effect == 3) { // Poison Health
        if (HP - 10 <= 0) {
            HP = 0;
        } else {
            HP -= 10;
        }
    } else if (effect == 4) { // Wound Atk
        potionAtkEffect -= 5;
    } else if (effect == 5) { // Wound Def
        pottionDefEffect -= 5;
    } else if (effect == 6) { // Normal gold pile
        coin += 1;
    } else if (effect == 7) { // Small Horde
        coin += 2;
    } else if (effect == 8) { // Merchant Hoard
        coin += 4;
    } else if (effect == 9) { // Dragon Hoard
        coin += 6;
    } else if (effect == 10) { // Barrier Suit acquired
        withBarrierSuit = true;
    } else if (effect == 11) { // Compass acquired
        withCompass = true;
    }
}

std::string PC::getType() const { return "PC"; }

int PC::state() { return moved; }
