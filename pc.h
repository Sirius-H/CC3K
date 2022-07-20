#ifndef __PC_H__
#define __PC_H__

#include "cell.h"

class PC: public Cell {
protected:
    int HP;
    int maxHP;
    int Atk;
    int Def;
    bool withCompass = false;
    bool withBarrierSuit = false;
    int potionAtkEffect = 0;
    int pottionDefEffect = 0;
    int moved = 0;
public:
    static double coin;
    static double totalCoin;
    PC(Coordinate cdn, bool withCompass = false, bool withBarrierSuit = false);
    int getDefence() const override;
    virtual int attack(int NPCDefence) const;
    virtual int attacked(int dmg);
    virtual void applyEffect(int effect);
    std::string getType() const override;
    int state() override;
    int getHP() const;
    int getAtk() const;
    int getDef() const;
    bool getWithBarrierSuit() const;
};

#endif
