#ifndef __NPC_H__
#define __NPC_H__
#include "cell.h"
#include "barriersuit.h"

class NPC : public Cell {
    protected:
    int HP;
    int Atk;
    int Def;
    double coin = 0;
    int moved = 0;
public:
    NPC(Coordinate cdn);
    NPC(Coordinate cdn, BarrierSuit* b);
    int getDefence() const;
    virtual int attack(int PCDefence) const;
    virtual int attacked(int dmg);
    std::string getType() const override;
    int state() override;
};

#endif
