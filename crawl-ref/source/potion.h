/**
 * @file
 * @brief Potion and potion-like effects.
**/

#ifndef POTION_H
#define POTION_H

bool potion_effect(potion_type pot_eff,
                   item_def *potion, bool was_known = true);

class PotionEffect
{
private:
    PotionEffect() = delete;
    DISALLOW_COPY_AND_ASSIGN(PotionEffect);
protected:
    PotionEffect(potion_type);
public:
    bool can_quaff() const;

    // Elsewhere in the code there are things that can have the effect
    // effect of a potion without actually being potions, there
    // are even some legacy 'potions' around in this code that
    // nowdays only provide the potion-like effect

    // Returns whether or not the potion had an effect
    virtual bool effect(int pow = 40) const { return false; }

    // Quaff also handles god-conduct and potion-specific
    // uncancellability
    // Returns whether or not the potion was actually quaffed
    bool quaff(bool was_known = true) const;

    const string potion_name;
    const potion_type kind;
};

const PotionEffect* get_potion_effect(potion_type pot);

#endif
