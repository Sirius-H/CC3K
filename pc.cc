#include <math.h>
#include "pc.h"

double PC::coin = 0;
double PC::totalCoin = 0;

PC::PC(Coordinate cdn, bool withCompass, bool withBarrierSuit): Cell{cdn}, withCompass{withCompass}, withBarrierSuit{withBarrierSuit} {}

int PC::getDefence() const { return Def; }

int PC::attack(int NPCDefence) const { return ceil(((double)100 / (100 + NPCDefence)) * (Atk + potionAtkEffect)); }

int PC::attacked(int dmg) { // returns the actual damage to PC
    int actualDmg = dmg;
    if (withBarrierSuit) {
        actualDmg = ceil((double)dmg / 2);
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
        totalCoin += 1;
    } else if (effect == 7) { // Small Horde
        coin += 2;
        totalCoin += 2;
    } else if (effect == 8) { // Merchant Hoard
        coin += 4;
        totalCoin += 4;
    } else if (effect == 9) { // Dragon Hoard
        coin += 6;
        totalCoin += 6;
    } else if (effect == 10) { // Barrier Suit acquired
        withBarrierSuit = true;
    } else if (effect == 11) { // Compass acquired
        withCompass = true;
    }
}

std::string PC::getType() const { return "PC"; }

int PC::state() { return moved; }

int PC::getHP() const { return HP; }
int PC::getAtk() const { return Atk; }
int PC::getDef() const { return Def; }
bool PC::getWithBarrierSuit() const {return withBarrierSuit; }
static void Grid::resetCoin() {
    coin = 0;
    totalCoin = 0;
}
