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

class PenaltyDice: public virtual Base {
public:
    PenaltyDice(Base &b):
        base(b) { }

        unsigned roll() override{
            return std::min(base.roll(), base.roll());
        }

private:
    Base &base;
};

class BonusDice: public virtual Base {
public:
    BonusDice(Base &b):
        base(b) { }

        unsigned roll() override{
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

double value_probability(unsigned value, Base &d, unsigned number_of_rolls = 100000){
        auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt){
        if(d.roll() == value){
            accum += 1;
        }
    }
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

void value_for_histogram(Base &b, unsigned number_of_rolls = 100000) {
    unsigned int i = 1;
    while (i <= 100) {
        std::cout << value_probability(i, b) << ", ";
        i += 1;
    }
    std::cout << "\n" << "\n";
}

int main() {
    Dice dice(100, 1), dice_1(6, 171), dice_2(6, 19), dice_3(6, 11);
    ThreeDicePool three_dice(dice_1, dice_2, dice_3);
    PenaltyDice penalty_dice_1(dice), penalty_dice_3(three_dice);
    BonusDice bonus_dice_1(dice), bonus_dice_3(three_dice);
    DoubleDice double_dice_1(dice), double_dice_3(three_dice);

    std::cout << "Expected value for one roll: " << expected_value(dice) << "\n";
    std::cout << "Expected value for three rolls: " << expected_value(three_dice) << "\n";
    std::cout << "The rools of Dice: ";
    value_for_histogram(dice);
    std::cout << "The rools of PenaltyDice: ";
    value_for_histogram(penalty_dice_1);
    std::cout << "The rools of BonusDice: ";
    value_for_histogram(bonus_dice_1);
    std::cout << "The rools of DoubleDice: ";
    value_for_histogram(double_dice_1);
    std::cout << "The rools of ThreeDicePool: ";
    value_for_histogram(three_dice);
    std::cout << "The rools of PenaltyDice_3: ";
    value_for_histogram(penalty_dice_3);
    std::cout << "The rools of BonusDice_3: ";
    value_for_histogram(bonus_dice_3);
    std::cout << "The rools of DoubleDice_3: ";
    value_for_histogram(double_dice_3);

    return 0;
}
