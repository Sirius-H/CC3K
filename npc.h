#ifndef __NPC_H__
#define __NPC_H__
#include "cell.h"

class NPC : public Cell {
protected:
    int HP;
    int Atk;
    int Def;
    bool withCompass;
    int moved = 0; // 0 is not moved; 1 is moved
public:
    NPC(Coordinate cdn);
    int getDefence() const;
    int attack(int PCDefence) const override;
    int attacked(int dmg) override;
    std::string getType() const override;
    int state() override;
    int setState() override;
};

#endif
