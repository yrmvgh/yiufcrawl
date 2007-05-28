/*
 *  File:       mutation.cc
 *  Summary:    Functions for handling player mutations.
 *  Written by: Linley Henzell
 *
 *  Modified for Crawl Reference by $Author$ on $Date$
 *
 *  Change History (most recent first):
 *
 *               <1>     -/--/--        LRH             Created
 */


#ifndef MUTATION_H
#define MUTATION_H

// for formatted_string
#include "menu.h"

// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: acr - decks - effects - fight - food - it_use2 - items -
 *              mutation - religion - spell - spells
 * *********************************************************************** */
bool mutate(int which_mutation, bool failMsg = true,
            bool force_mutation = false, bool demonspawn = false);


// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: acr
 * *********************************************************************** */
void display_mutations();

formatted_string describe_mutations();


// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: decks - it_use2 - mutation - spells
 * *********************************************************************** */
bool delete_mutation(int which_mutation);


// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: chardump
 * *********************************************************************** */
// default of level == -1, means to use the player's current level
const char *mutation_name( int which_mutat, int level = -1 );


// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: items - religion
 * *********************************************************************** */
bool give_cosmetic_mutation( void );

// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: items - spells
 * *********************************************************************** */
bool give_bad_mutation( bool forceMutation = false, bool failMsg = true );

// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: player
 * *********************************************************************** */
void demonspawn(void);

bool perma_mutate(int which_mut, char how_much);

#endif
