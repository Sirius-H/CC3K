#ifndef __NPC_H__
#define __NPC_H__
#include "cell.h"

class NPC : public Cell {
protected:
    int HP;
    int Atk;
    int Def;
    bool withCompass;
    int moved = 0;
public:
    NPC(Coordinate cdn);
    int getDefence() const override;
    int attack(int PCDefence) const override;
    int attacked(int dmg) override;
    std::string getType() const override;
    int state() override;
    void setMoved();
	void setCoin(double coin);
    int getHP() const;
    int getAtk() const;
    int getDef() const;
};

#endif
