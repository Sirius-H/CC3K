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
    std::cout << "Triggered applyEffect" << std::endl;
    if (effect == 0) { // Restore Health
        // Debugger
        std::cout << "Found a Restore Health potion" << std::endl;
        if (HP + 10 > maxHP) {
            HP = maxHP;
        } else {
            HP += 10;
        }
    } else if (effect == 1) { // Boost Atk
        // Debugger
        std::cout << "Found a Boost Atk potion" << std::endl;
        potionAtkEffect += 5;
    } else if (effect == 2) { // Boost Def
        // Debugger
        std::cout << "Found a Boost Def potion" << std::endl;
        pottionDefEffect += 5;
    } else if (effect == 3) { // Poison Health
        // Debugger
        std::cout << "Found a Poison Health potion" << std::endl;
        if (HP - 10 <= 0) {
            HP = 0;
        } else {
            HP -= 10;
        }
    } else if (effect == 4) { // Wound Atk
        // Debugger
        std::cout << "Found a Wound Atk potion" << std::endl;
        potionAtkEffect -= 5;
    } else if (effect == 5) { // Wound Def
        // Debugger
        std::cout << "Found a Wound Def potion" << std::endl;
        pottionDefEffect -= 5;
    } else if (effect == 6) { // Normal gold pile
        // Debugger
        std::cout << "Found a Normal gold pile" << std::endl;
        coin += 1;
        totalCoin += 1;
    } else if (effect == 7) { // Small Horde
        // Debugger
        std::cout << "Found a Small Horde" << std::endl;
        coin += 2;
        totalCoin += 2;
    } else if (effect == 8) { // Merchant Hoard
        // Debugger
        std::cout << "Found a merchant horde" << std::endl;
        coin += 4;
        totalCoin += 4;
    } else if (effect == 9) { // Dragon Hoard
        // Debugger
        std::cout << "Found a Dragon Hoard" << std::endl;
        coin += 6;
        totalCoin += 6;
    } else if (effect == 10) { // Barrier Suit acquired
        // Debugger
        std::cout << "Found a BarrierSuit" << std::endl;
        withBarrierSuit = true;
    } else if (effect == 11) { // Compass acquired
        // Debugger
        std::cout << "Found a Compass" << std::endl;
        withCompass = true;
    }
}

std::string PC::getType() const { return "PC"; }

int PC::state() { return moved; }

int PC::getHP() const { return HP; }
int PC::getAtk() const { return Atk + potionAtkEffect; }
int PC::getDef() const { return Def + pottionDefEffect; }
bool PC::getWithBarrierSuit() const { return withBarrierSuit; }

void PC::resetCoin() {
    PC::coin = 0;
    PC::totalCoin = 0;
}
