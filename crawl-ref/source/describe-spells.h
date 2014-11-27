/**
 * @file
 * @brief Functions used to print information about spells, spellbooks, rods,
 *        etc.
 **/

#ifndef DESCRIBE_SPELLS_H
#define DESCRIBE_SPELLS_H

#include <map>

#include "enum.h"
#include "format.h"

/// What type of 'book' do a given set of spellbook_contents represent?
/*enum spellbook_type
{
    /// A spellbook in the player's inventory.
    SPELLBOOK_BOOK_IN_INV,
    /// A spellbook not in the player's inventory (on the ground, ?/S, etc)
    SPELLBOOK_BOOK_OUTSIDE_INV,
    /// An evocable rod.
    SPELLBOOK_ROD,
    /// [one of] a monster [type]'s spellbook[s].
    SPELLBOOK_MONSTER,
};*/

/// What's in a given spellbook?
struct spellbook_contents
{
    /// A label for the book.
    const string label;
    /// The spells contained in the book (or 'book').
    const vector<spell_type> spells;
};

typedef vector<spellbook_contents> spellset;

spellset item_spellset(const item_def &item);
void describe_spellset(const spellset &spells,
                       const item_def* const source_item,
                       formatted_string &description);
string describe_item_spells(const item_def &item);
void list_spellset(const spellset &spells, const item_def *source_item,
                   formatted_string &initial_desc);

#endif
