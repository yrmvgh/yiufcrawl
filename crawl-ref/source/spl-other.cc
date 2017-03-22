/**
 * @file
 * @brief Non-enchantment spells that didn't fit anywhere else.
 *           Mostly Transmutations.
**/

#include "AppHdr.h"

#include "spl-other.h"

#include "act-iter.h"
#include "coordit.h"
#include "delay.h"
#include "env.h"
#include "food.h"
#include "god-companions.h"
#include "libutil.h"
#include "message.h"
#include "misc.h"
#include "mon-place.h"
#include "mon-util.h"
#include "place.h"
#include "player-stats.h"
#include "potion.h"
#include "religion.h"
#include "spl-util.h"
#include "terrain.h"

spret_type cast_sublimation_of_blood(int pow, bool fail)
{
    bool success = false;

    if (you.duration[DUR_DEATHS_DOOR])
        mpr("You can't draw power from your own body while in Death's door.");
    else if (!you.can_bleed())
    {
        if (you.species == SP_VAMPIRE)
            mpr("You don't have enough blood to draw power from your own body.");
        else
            mpr("Your body is bloodless.");
    }
    else if (!enough_hp(2, true))
        mpr("Your attempt to draw power from your own body fails.");
    else
    {
        // Take at most 90% of currhp.
        const int minhp = max(div_rand_round(you.hp, 10), 1);

        while (you.magic_points < you.max_magic_points && you.hp > minhp)
        {
            fail_check();
            success = true;

            inc_mp(1);
            dec_hp(1, false);

            for (int i = 0; i < (you.hp > minhp ? 3 : 0); ++i)
                if (x_chance_in_y(6, pow))
                    dec_hp(1, false);

            if (x_chance_in_y(6, pow))
                break;
        }
        if (success)
            mpr("You draw magical energy from your own body!");
        else
            mpr("Your attempt to draw power from your own body fails.");
    }

    return success ? SPRET_SUCCESS : SPRET_ABORT;
}

spret_type cast_death_channel(int pow, god_type god, bool fail)
{
    if (you.duration[DUR_DEATH_CHANNEL] >= 60 * BASELINE_DELAY)
    {
        canned_msg(MSG_NOTHING_HAPPENS);
        return SPRET_ABORT;
    }

    fail_check();
    mpr("Malign forces permeate your being, awaiting release.");

    you.increase_duration(DUR_DEATH_CHANNEL, 30 + random2(1 + 2*pow/3), 200);

    if (god != GOD_NO_GOD)
        you.attribute[ATTR_DIVINE_DEATH_CHANNEL] = static_cast<int>(god);

    return SPRET_SUCCESS;
}

spret_type cast_recall(bool fail)
{
    fail_check();
    start_recall(RECALL_SPELL);
    return SPRET_SUCCESS;
}

void start_recall(recall_t type)
{
    // Assemble the recall list.
    typedef pair<mid_t, int> mid_hd;
    vector<mid_hd> rlist;

    you.recall_list.clear();
    for (monster_iterator mi; mi; ++mi)
    {
        if (!mons_is_recallable(&you, **mi))
            continue;

        if (type == RECALL_YRED)
        {
            if (!(mi->holiness() & MH_UNDEAD))
                continue;
        }
        else if (type == RECALL_BEOGH)
        {
            if (!is_orcish_follower(**mi))
                continue;
        }

        mid_hd m(mi->mid, mi->get_experience_level());
        rlist.push_back(m);
    }

    if (type != RECALL_SPELL && branch_allows_followers(you.where_are_you))
        populate_offlevel_recall_list(rlist);

    if (!rlist.empty())
    {
        // Sort the recall list roughly
        for (mid_hd &entry : rlist)
            entry.second += random2(10);
        sort(rlist.begin(), rlist.end(), greater_second<mid_hd>());

        you.recall_list.clear();
        for (mid_hd &entry : rlist)
            you.recall_list.push_back(entry.first);

        you.attribute[ATTR_NEXT_RECALL_INDEX] = 1;
        you.attribute[ATTR_NEXT_RECALL_TIME] = 0;
        mpr("You begin recalling your allies.");
    }
    else
        mpr("Nothing appears to have answered your call.");
}

// Remind a recalled ally (or one skipped due to proximity) not to run
// away or wander off.
void recall_orders(monster *mons)
{
    // FIXME: is this okay for berserk monsters? We still want them to
    // stick around...

    // Don't patrol
    mons->patrol_point = coord_def(0, 0);

    // Don't wander
    mons->behaviour = BEH_SEEK;

    // Don't pursue distant enemies
    const actor *foe = mons->get_foe();
    if (foe && !you.can_see(*foe))
        mons->foe = MHITYOU;
}

// Attempt to recall a single monster by mid, which might be either on or off
// our current level. Returns whether this monster was successfully recalled.
bool try_recall(mid_t mid)
{
    monster* mons = monster_by_mid(mid);
    // Either it's dead or off-level.
    if (!mons)
        return recall_offlevel_ally(mid);
    else if (mons->alive())
    {
        // Don't recall monsters that are already close to the player
        if (mons->pos().distance_from(you.pos()) < 3
            && mons->see_cell_no_trans(you.pos()))
        {
            recall_orders(mons);
            return false;
        }
        else
        {
            coord_def empty;
            if (find_habitable_spot_near(you.pos(), mons_base_type(*mons), 3, false, empty)
                && mons->move_to_pos(empty))
            {
                recall_orders(mons);
                simple_monster_message(*mons, " is recalled.");
                mons->apply_location_effects(mons->pos());
                // mons may have been killed, shafted, etc,
                // but they were still recalled!
                return true;
            }
        }
    }

    return false;
}

// Attempt to recall a number of allies proportional to how much time
// has passed. Once the list has been fully processed, terminate the
// status.
void do_recall(int time)
{
    while (time > you.attribute[ATTR_NEXT_RECALL_TIME])
    {
        // Try to recall an ally.
        mid_t mid = you.recall_list[you.attribute[ATTR_NEXT_RECALL_INDEX]-1];
        you.attribute[ATTR_NEXT_RECALL_INDEX]++;
        if (try_recall(mid))
        {
            time -= you.attribute[ATTR_NEXT_RECALL_TIME];
            you.attribute[ATTR_NEXT_RECALL_TIME] = 3 + random2(4);
        }
        if ((unsigned int)you.attribute[ATTR_NEXT_RECALL_INDEX] >
             you.recall_list.size())
        {
            end_recall();
            mpr("You finish recalling your allies.");
            return;
        }
    }

    you.attribute[ATTR_NEXT_RECALL_TIME] -= time;
    return;
}

void end_recall()
{
    you.attribute[ATTR_NEXT_RECALL_INDEX] = 0;
    you.attribute[ATTR_NEXT_RECALL_TIME] = 0;
    you.recall_list.clear();
}

static bool _feat_is_passwallable(dungeon_feature_type feat)
{
    // Worked stone walls are out, they're not diggable and
    // are used for impassable walls...
    switch (feat)
    {
    case DNGN_ROCK_WALL:
    case DNGN_SLIMY_WALL:
    case DNGN_CLEAR_ROCK_WALL:
        return true;
    default:
        return false;
    }
}

spret_type cast_passwall(const coord_def& delta, int pow, bool fail)
{
    coord_def dest;
    for (dest = you.pos() + delta;
         in_bounds(dest) && _feat_is_passwallable(grd(dest));
         dest += delta)
    {}

    int walls = (dest - you.pos()).rdist() - 1;
    if (walls == 0)
    {
        mpr("That's not a passable wall.");
        return SPRET_ABORT;
    }

    fail_check();

    // Below here, failing to cast yields information to the
    // player, so we don't make the spell abort (return SPRET_SUCCESS).
    const monster *mon = monster_at(dest);
    if (!in_bounds(dest))
        mpr("You sense an overwhelming volume of rock.");
    else if (cell_is_solid(dest) || (mon && mon->is_stationary()))
        mpr("Something is blocking your path through the rock.");
    else if (walls > spell_range(SPELL_PASSWALL, pow))
        mpr("This rock feels extremely deep.");
    else
    {
        string msg;
        if (grd(dest) == DNGN_DEEP_WATER)
            msg = "You sense a large body of water on the other side of the rock.";
        else if (grd(dest) == DNGN_LAVA)
            msg = "You sense an intense heat on the other side of the rock.";

        if (check_moveto(dest, "passwall", msg))
        {
            // Passwall delay is reduced, and the delay cannot be interrupted.
            start_delay<PasswallDelay>(1 + walls, dest);
        }
    }
    return SPRET_SUCCESS;
}

static int _intoxicate_monsters(coord_def where, int pow)
{
    monster* mons = monster_at(where);
    if (mons == nullptr
        || mons_intel(*mons) < I_HUMAN
        || !(mons->holiness() & MH_NATURAL)
        || mons->check_clarity(false)
        || monster_resists_this_poison(*mons))
    {
        return 0;
    }

    if (x_chance_in_y(40 + pow/3, 100))
    {
        mons->add_ench(mon_enchant(ENCH_CONFUSION, 0, &you));
        simple_monster_message(*mons, " looks rather confused.");
        return 1;
    }
    return 0;
}

spret_type cast_intoxicate(int pow, bool fail)
{
    fail_check();
    mpr("You attempt to intoxicate your foes!");
    int count = apply_area_visible([pow] (coord_def where) {
        return _intoxicate_monsters(where, pow);
    }, you.pos());
    if (count > 0)
    {
        if (x_chance_in_y(60 - pow/3, 100))
        {
            mprf(MSGCH_WARN, "The world spins around you!");
            you.increase_duration(DUR_VERTIGO, 4 + random2(20 + (100 - pow) / 10));
            you.redraw_evasion = true;
        }
    }

    return SPRET_SUCCESS;
}

// The intent of this spell isn't to produce helpful potions
// for drinking, but rather to provide ammo for the Evaporate
// spell out of corpses, thus potentially making it useful.
// Producing helpful potions would break game balance here...
// and producing more than one potion from a corpse, or not
// using up the corpse might also lead to game balance problems. - bwr
spret_type cast_fulsome_distillation(int pow, bool check_range, bool fail)
{
    int num_corpses = 0;
    item_def *corpse = corpse_at(you.pos(), &num_corpses);

    // If there is only one corpse, distill it; otherwise, ask the player
    // which corpse to use.
    switch (num_corpses)
    {
        case 0: 
            mpr("There aren't any corpses here.");
            return SPRET_ABORT;
        case 1:
            // Use the only corpse available without prompting.
            break;
        default:
            // Search items at the player's location for corpses.
            // The last corpse detected earlier is irrelevant.
            corpse = NULL;
            for (stack_iterator si(you.pos(), true); si; ++si)
            {
                if (item_is_corpse(*si))
                {
                    const std::string corpsedesc =
                        get_menu_colour_prefix_tags(*si, DESC_THE);
                    const std::string prompt =
                        make_stringf("Distill a potion from %s?",
                                     corpsedesc.c_str());

                    if (yesno(prompt.c_str(), true, 0, false))
                    {
                        corpse = &*si;
                        break;
                    }
                }
            }
    }

    if (!corpse)
    {
        canned_msg(MSG_OK);
        return SPRET_ABORT;
    }

    fail_check();

    potion_type pot_type = POT_DEGENERATION;

    switch (mons_corpse_effect(corpse->mon_type))
    {
    case CE_CLEAN:
        pot_type = one_chance_in(3) ? POT_AMBROSIA : POT_BLOOD;
        break;

    case CE_CONTAMINATED:
        pot_type = POT_DEGENERATION;
        break;

    case CE_POISONOUS:
    case CE_POISON_CONTAM:
        pot_type = POT_DEGENERATION;
        break;

    case CE_MUTAGEN:
        pot_type = POT_MUTATION;
        break;

    case CE_ROTTEN:         // actually this only occurs via mangling
    case CE_ROT:            // necrophage
        pot_type = POT_DEGENERATION;
        break;

    case CE_NOCORPSE:       // shouldn't occur
    default:
        break;
    }

    struct monsterentry* smc = get_monster_data(corpse->mon_type);

    for (int nattk = 0; nattk < 4; ++nattk)
    {
        if (smc->attack[nattk].flavour == AF_POISON_MEDIUM
            || smc->attack[nattk].flavour == AF_POISON_STRONG
            || smc->attack[nattk].flavour == AF_POISON_STR
            || smc->attack[nattk].flavour == AF_POISON_INT
            || smc->attack[nattk].flavour == AF_POISON_DEX
            || smc->attack[nattk].flavour == AF_POISON_STAT)
        {
            pot_type = POT_DEGENERATION;
        }
    }

    const bool was_orc = (mons_genus(corpse->mon_type) == MONS_ORC);
    const bool was_holy = (mons_class_holiness(corpse->mon_type) == MH_HOLY);

    // We borrow the corpse's object to make our potion.
    corpse->base_type = OBJ_POTIONS;
    corpse->sub_type  = pot_type;
    corpse->quantity  = 1;
    corpse->plus      = 0;
    corpse->plus2     = 0;
    corpse->flags     = 0;
    corpse->inscription.clear();
    item_colour(*corpse); // sets special as well

    // Always identify said potion.
    set_ident_type(*corpse, ID_KNOWN_TYPE);

    mprf("You extract %s from the corpse.",
         corpse->name(DESC_A).c_str());

    // Try to move the potion to the player (for convenience);
    // they probably won't autopickup bad potions.
    // Treats potion as though it was being picked up manually (0005916).
    std::map<int,int> tmp_l_p = you.last_pickup;
    you.last_pickup.clear();

    if (move_item_to_player(corpse->index(), 1) != 1)
        mpr("Unfortunately, you can't carry it right now!");

    if (you.last_pickup.empty())
        you.last_pickup = tmp_l_p;

    if (was_orc)
        did_god_conduct(DID_DESECRATE_ORCISH_REMAINS, 2);
    if (was_holy)
        did_god_conduct(DID_DESECRATE_HOLY_REMAINS, 2);

    return SPRET_SUCCESS;
}

spret_type cast_darkness(int pow, bool fail)
{
    fail_check();
    if (you.duration[DUR_DARKNESS])
        mprf(MSGCH_DURATION, "It gets a bit darker.");
    else
        mprf(MSGCH_DURATION, "It gets dark.");
    you.increase_duration(DUR_DARKNESS, 15 + random2(1 + pow/3), 100);
    update_vision_range();

    return SPRET_SUCCESS;
}

spret_type cast_reap(bool fail)
{
    if (you.attribute[ATTR_REAPING]) {
        mprf("You are already reaping!");
        return SPRET_ABORT;
    }

    bool living_beings_nearby = false;
    for (monster_near_iterator mi(you.pos(), LOS_NO_TRANS); mi; ++mi)
        if (mi->holiness() != MH_NONLIVING && mi->holiness() != MH_UNDEAD)
            living_beings_nearby = true;

    if (!living_beings_nearby) {
        mprf("There are no living beings to reap nearby!");
        return SPRET_ABORT;
    }

    fail_check();
    you.attribute[ATTR_REAPING] = 1;
    if (renew_reap()) {
        you.redraw_status_lights = true;
        you.redraw_evasion = true;
        you.redraw_armour_class = true;
        return SPRET_SUCCESS;
    } else {
        mprf("There are no suitable enemies nearby!");
        return SPRET_ABORT;
    }
}

bool renew_reap()
{
    if (!you.attribute[ATTR_REAPING])
       return false;

    monster* mon = nullptr;
    for (distance_iterator di(you.pos(), true, true, LOS_RADIUS); di; ++di) {
        mon = monster_at(*di);
        if (mon && you.can_see(*mon)
            && you.possible_beholder(mon)
            && mons_is_threatening(*mon))
        {
            break;
        }
    }

    if (!mon) {
        you.attribute[ATTR_REAPING] = 0;
        return false;
    }

    if (!you.beheld_by(*mon))
    {
        const char* message[11] =
        {
           "You lay your malignant gaze on %s.",
           "%s is next on your list.",
           "You stare at %s eerily.",
           "You swivel your skull at %s.",
           "All your focus is on %s now.",
           "Target: %s.",
           "Murderous intent drives you towards %s.",
           "Your bones shudder as you turn to face %s.",
           "%s has your attention.",
           "%s would look better as a corpse.",
           "Better off dead? %s, most definitely.",
        };

        mprf(message[random2(4)], mon->name(DESC_THE).c_str());
        you.clear_beholders(true);
    }

    you.duration[DUR_MESMERISE_IMMUNE] = 0;
    you.add_beholder(*mon, false, true);
    you.duration[DUR_MESMERISED] = 30;
    return true;
}
