#ifndef RELIGION_ENUM_H
#define RELIGION_ENUM_H

enum piety_gain_t
{
    PIETY_NONE, PIETY_SOME, PIETY_LOTS,
    NUM_PIETY_GAIN
};

#if TAG_MAJOR_VERSION == 34
enum nemelex_gift_types
{
    NEM_GIFT_ESCAPE = 0,
    NEM_GIFT_DESTRUCTION,
    NEM_GIFT_SUMMONING,
    NEM_GIFT_WONDERS,
    NUM_NEMELEX_GIFT_TYPES,
};
#endif

#define ACQUIRE_KEY "acquired" // acquirement source prop on acquired items

/// ancestor gender
#define HEPLIAKLQANAL_ALLY_GENDER_KEY "hepliaklqanal_ally_gender"
/// chosen ancestor class (monster_type)
#define HEPLIAKLQANAL_ALLY_TYPE_KEY "hepliaklqanal_ally_type"
/// set of possible death effects the player can choose between
#define HEPLIAKLQANAL_DEATH_POSSIBILTIES_KEY "hepliaklqanal_death_possibilities"
 /// chosen on-death effect
#define HEPLIAKLQANAL_ALLY_DEATH_KEY "hepliaklqanal_ally_death"

/// custom monster gender
#define MON_GENDER_KEY "mon_gender"

#endif
