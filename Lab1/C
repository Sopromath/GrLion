#include <iostream>
#include <random>

class Base {
public:
    virtual unsigned roll() {
        return 0;
    }
    virtual ~Base() {}
};

class Dice: public Base {
public:
    Dice(unsigned max , unsigned seed):
        max(max), dstr(1, max), reng(seed) { }

    unsigned roll() {
        return dstr(reng);
    }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool: public Base {
public:
    ThreeDicePool(Dice d_1, Dice d_2, Dice d_3):
        dice_1(d_1), dice_2(d_2), dice_3(d_3){ }

        unsigned roll() {
            return dice_1.roll() + dice_2.roll() + dice_3.roll();
        }

private:
    Dice dice_1, dice_2, dice_3;
};

class PenaltyDice: public Base {
public:
    PenaltyDice(Base &b):
        base(b) { }

        unsigned roll() {
            return std::min(base.roll(), base.roll());
        }

private:
    Base &base;
};

class BonusDice: public Base {
public:
    BonusDice(Base &b):
        base(b) { }

        unsigned roll() {
            return std::max(base.roll(), base.roll());
        }

private:
    Base &base;
};

class DoubleDice : public BonusDice, public PenaltyDice {
public:
    DoubleDice(Base& b) :
        BonusDice(b), PenaltyDice(b) {}

    virtual unsigned roll() override {
        return (PenaltyDice::roll() + BonusDice::roll());
    }
};

double expected_value(Base &tdp, unsigned number_of_rolls = 1000) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += tdp.roll();
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

double value_probability(unsigned value, Dice &d, unsigned number_of_rolls = 1000){
        auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt){
        if(d.roll() == value){
            accum += 1;
        }
    }
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

int main() {
    Dice d_1(6,5), d_2(6,21), d_3(6,7);

    ThreeDicePool d(d_1, d_2, d_3);

    std::cout<<expected_value(d_1);

}
