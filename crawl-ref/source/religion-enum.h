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

/// the name of the ally helpal granted the player
#define HELPAL_ALLY_NAME_KEY "helpal_ally_name"
/// set of possible death effects the player can choose between
#define HELPAL_DEATH_POSSIBILTIES_KEY "helpal_death_possibilities"
 /// chosen on-death effect
#define HELPAL_ALLY_DEATH_KEY "helpal_ally_death"

#endif
