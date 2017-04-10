#pragma once

enum ability_type
{
    ABIL_NON_ABILITY = -1,
    // Innate abilities and (Demonspawn) mutations.
    ABIL_SPIT_POISON = 1,
    ABIL_BREATHE_FIRE,
    ABIL_BREATHE_FROST,
    ABIL_BREATHE_POISON,
    ABIL_BREATHE_LIGHTNING,
    ABIL_BREATHE_POWER,
#if TAG_MAJOR_VERSION == 34
    ABIL_BREATHE_STICKY_FLAME,
#endif
    ABIL_BREATHE_STEAM,
    ABIL_BREATHE_MEPHITIC,
    ABIL_BREATHE_ACID,
    ABIL_BLINK,
    // Others
#if TAG_MAJOR_VERSION == 34
    ABIL_DELAYED_FIREBALL,
#endif
    // Abort duration-based spells
    ABIL_END_TRANSFORMATION,
#if TAG_MAJOR_VERSION == 34
    ABIL_STOP_SINGING, // From song of slaying
#endif
    ABIL_CANCEL_PPROJ,

    // Species-specific abilities.
    // Demonspawn-only
    ABIL_DAMNATION,
    // Tengu, Draconians
    ABIL_FLY,
#if TAG_MAJOR_VERSION == 34
    ABIL_WISP_BLINK,
#endif
    ABIL_STOP_FLYING,
#if TAG_MAJOR_VERSION == 34
    ABIL_MUMMY_RESTORATION,
#endif
    // Vampires
    ABIL_TRAN_BAT,
#if TAG_MAJOR_VERSION == 34
    ABIL_BOTTLE_BLOOD,
#endif
    // Deep Dwarves
    ABIL_HEAL_WOUNDS,
    // Formicids
    ABIL_DIG,
    ABIL_SHAFT_SELF,
    // Barachim
    ABIL_HOP,
    // Plutonians
    ABIL_OVERLOAD,
    // Skeletons
    ABIL_REAP,
    // Bearkin
    ABIL_BEARSERK,
    ABIL_MAX_INTRINSIC = ABIL_BEARSERK,

    // Evoking items.
    ABIL_EVOKE_BERSERK = 40,
    ABIL_MIN_EVOKE = ABIL_EVOKE_BERSERK,
#if TAG_MAJOR_VERSION == 34
    ABIL_EVOKE_TELEPORTATION,
#endif
    ABIL_EVOKE_BLINK,
    ABIL_EVOKE_TURN_INVISIBLE,
    ABIL_EVOKE_TURN_VISIBLE,
    ABIL_EVOKE_FLIGHT,
#if TAG_MAJOR_VERSION == 34
    ABIL_EVOKE_STOP_LEVITATING,
#endif
    ABIL_EVOKE_FOG,
#if TAG_MAJOR_VERSION == 34
    ABIL_EVOKE_TELEPORT_CONTROL,
    ABIL_EVOKE_TWISTER,
#endif
    ABIL_EVOKE_RATSKIN,
    ABIL_MAX_EVOKE = ABIL_EVOKE_RATSKIN,

    // Divine abilities
    // Zin
    ABIL_ZIN_SUSTENANCE = 1000,
    ABIL_ZIN_RECITE,
    ABIL_ZIN_VITALISATION,
    ABIL_ZIN_IMPRISON,
    ABIL_ZIN_SANCTUARY,
    ABIL_ZIN_CURE_ALL_MUTATIONS,
    ABIL_ZIN_DONATE_GOLD,
    // TSO
    ABIL_TSO_DIVINE_SHIELD = 1010,
    ABIL_TSO_CLEANSING_FLAME,
    ABIL_TSO_SUMMON_DIVINE_WARRIOR,
    ABIL_TSO_BLESS_WEAPON,
    // Kiku
    ABIL_KIKU_RECEIVE_CORPSES = 1020,
    ABIL_KIKU_TORMENT,
    ABIL_KIKU_BLESS_WEAPON,
    ABIL_KIKU_GIFT_NECRONOMICON,
    // Yredelemnul
    ABIL_YRED_INJURY_MIRROR = 1030,
    ABIL_YRED_ANIMATE_REMAINS,
    ABIL_YRED_RECALL_UNDEAD_SLAVES,
    ABIL_YRED_ANIMATE_DEAD,
    ABIL_YRED_DRAIN_LIFE,
    ABIL_YRED_ENSLAVE_SOUL,
    // Vehumet
    // = 1040
    // Okawaru
    ABIL_OKAWARU_HEROISM = 1050,
    ABIL_OKAWARU_FINESSE,
    // Makhleb
    ABIL_MAKHLEB_MINOR_DESTRUCTION = 1060,
    ABIL_MAKHLEB_LESSER_SERVANT_OF_MAKHLEB,
    ABIL_MAKHLEB_MAJOR_DESTRUCTION,
    ABIL_MAKHLEB_GREATER_SERVANT_OF_MAKHLEB,
    // Sif Muna
    ABIL_SIF_MUNA_CHANNEL_ENERGY = 1070,
    ABIL_SIF_MUNA_FORGET_SPELL,
    ABIL_SIF_MUNA_DIVINE_ENERGY,
    ABIL_SIF_MUNA_STOP_DIVINE_ENERGY,
    // Trog
    ABIL_TROG_BERSERK = 1080,
    ABIL_TROG_REGEN_MR,
    ABIL_TROG_BROTHERS_IN_ARMS,
    // Elyvilon
    ABIL_ELYVILON_LIFESAVING = 1090,
    ABIL_ELYVILON_LESSER_HEALING,
#if TAG_MAJOR_VERSION == 34
    ABIL_ELYVILON_LESSER_HEALING_OTHERS,
#endif
    ABIL_ELYVILON_PURIFICATION,
    ABIL_ELYVILON_GREATER_HEALING,
    ABIL_ELYVILON_HEAL_OTHER,
    ABIL_ELYVILON_DIVINE_VIGOUR,
    // Lugonu
    ABIL_LUGONU_ABYSS_EXIT = 1100,
    ABIL_LUGONU_BEND_SPACE,
    ABIL_LUGONU_BANISH,
    ABIL_LUGONU_CORRUPT,
    ABIL_LUGONU_ABYSS_ENTER,
    ABIL_LUGONU_BLESS_WEAPON,
    // Nemelex
#if TAG_MAJOR_VERSION == 34
    ABIL_NEMELEX_DRAW_ONE = 1110,
    ABIL_NEMELEX_PEEK_TWO,
#endif
    ABIL_NEMELEX_TRIPLE_DRAW = 1112,
    ABIL_NEMELEX_DEAL_FOUR,
    ABIL_NEMELEX_STACK_FIVE,
    // Beogh
    ABIL_BEOGH_SMITING = 1120,
    ABIL_BEOGH_RECALL_ORCISH_FOLLOWERS,
    ABIL_BEOGH_GIFT_ITEM,
    ABIL_BEOGH_RESURRECTION,
    // Jiyva
    ABIL_JIYVA_CALL_JELLY = 1130,
    ABIL_JIYVA_JELLY_PARALYSE,
    ABIL_JIYVA_SLIMIFY,
    ABIL_JIYVA_CURE_BAD_MUTATION,
    // Fedhas
    ABIL_FEDHAS_SUNLIGHT = 1140,
    ABIL_FEDHAS_RAIN,
    ABIL_FEDHAS_PLANT_RING,
    ABIL_FEDHAS_SPAWN_SPORES,
    ABIL_FEDHAS_EVOLUTION,
    ABIL_FEDHAS_FUNGAL_BLOOM,
    // Cheibriados
    ABIL_CHEIBRIADOS_TIME_STEP = 1151,
    ABIL_CHEIBRIADOS_TIME_BEND,
    ABIL_CHEIBRIADOS_SLOUCH,
    ABIL_CHEIBRIADOS_DISTORTION,
    // Ashenzari
    ABIL_ASHENZARI_SCRYING = 1160,
    ABIL_ASHENZARI_TRANSFER_KNOWLEDGE,
    ABIL_ASHENZARI_END_TRANSFER,
    ABIL_ASHENZARI_CURSE,
    // Dithmenos
    ABIL_DITHMENOS_SHADOW_STEP = 1170,
    ABIL_DITHMENOS_SHADOW_FORM,
    // Gozag
    ABIL_GOZAG_POTION_PETITION = 1180,
    ABIL_GOZAG_CALL_MERCHANT,
    ABIL_GOZAG_BRIBE_BRANCH,
    // Qazlal
    ABIL_QAZLAL_UPHEAVAL = 1190,
    ABIL_QAZLAL_ELEMENTAL_FORCE,
    ABIL_QAZLAL_DISASTER_AREA,
    // Ru
    ABIL_RU_DRAW_OUT_POWER = 1200,
    ABIL_RU_POWER_LEAP,
    ABIL_RU_APOCALYPSE,

    ABIL_RU_SACRIFICE_PURITY,
        ABIL_FIRST_SACRIFICE = ABIL_RU_SACRIFICE_PURITY,
    ABIL_RU_SACRIFICE_WORDS,
    ABIL_RU_SACRIFICE_DRINK,
    ABIL_RU_SACRIFICE_ESSENCE,
    ABIL_RU_SACRIFICE_HEALTH,
    ABIL_RU_SACRIFICE_STEALTH,
    ABIL_RU_SACRIFICE_ARTIFICE,
    ABIL_RU_SACRIFICE_LOVE,
    ABIL_RU_SACRIFICE_COURAGE,
    ABIL_RU_SACRIFICE_ARCANA,
    ABIL_RU_SACRIFICE_NIMBLENESS,
    ABIL_RU_SACRIFICE_DURABILITY,
    ABIL_RU_SACRIFICE_HAND,
    ABIL_RU_SACRIFICE_EXPERIENCE,
    ABIL_RU_SACRIFICE_SKILL,
    ABIL_RU_SACRIFICE_EYE,
    ABIL_RU_SACRIFICE_RESISTANCE,
        ABIL_FINAL_SACRIFICE = ABIL_RU_SACRIFICE_RESISTANCE,
    ABIL_RU_REJECT_SACRIFICES,

    // Pakellas
    ABIL_PAKELLAS_DEVICE_SURGE = 1230,
    ABIL_PAKELLAS_QUICK_CHARGE,
    ABIL_PAKELLAS_SUPERCHARGE,

    // Uskayaw
    ABIL_USKAYAW_STOMP = 1240,
    ABIL_USKAYAW_LINE_PASS,
    ABIL_USKAYAW_GRAND_FINALE,

    // Hepliaklqana
    ABIL_HEPLIAKLQANA_RECALL = 1250,
    ABIL_HEPLIAKLQANA_IDEALISE,
    ABIL_HEPLIAKLQANA_TRANSFERENCE,

    ABIL_HEPLIAKLQANA_TYPE_KNIGHT,
        ABIL_HEPLIAKLQANA_FIRST_TYPE = ABIL_HEPLIAKLQANA_TYPE_KNIGHT,
    ABIL_HEPLIAKLQANA_TYPE_BATTLEMAGE,
    ABIL_HEPLIAKLQANA_TYPE_HEXER,
        ABIL_HEPLIAKLQANA_LAST_TYPE = ABIL_HEPLIAKLQANA_TYPE_HEXER,

    ABIL_HEPLIAKLQANA_IDENTITY,

#if TAG_MAJOR_VERSION == 34
    ABIL_HEPLIAKLQANA_KNIGHT_REACHING,
    ABIL_HEPLIAKLQANA_KNIGHT_CLEAVING,
    ABIL_HEPLIAKLQANA_BATTLEMAGE_FORCE_LANCE,
    ABIL_HEPLIAKLQANA_BATTLEMAGE_MAGMA,
    ABIL_HEPLIAKLQANA_HEXER_MASS_CONFUSION,
    ABIL_HEPLIAKLQANA_HEXER_ENGLACIATION,
#endif

    // Wu Jian
    ABIL_WU_JIAN_SERPENTS_LASH = 1280,
    ABIL_WU_JIAN_HEAVENLY_STORM,

    // For both Yred and Beogh
    ABIL_STOP_RECALL = 1500,

    // General divine (pseudo) abilities.
    ABIL_RENOUNCE_RELIGION,
    ABIL_CONVERT_TO_BEOGH,
    NUM_ABILITIES
};
