# Dungeon Crawl Stone Soup: Circus Animals variation

This fork of DCSS is a playground for some ideas I wanted to try. 
The majority of my changes center around increasing the richness of the different species in DCSS. 
I would repeatedly take the species that I considered most boring to play, and alter that species until it was a compelling choice for me to play. 
I know quite well that I am not nearly as experienced in playing this game as many others are, and likely some of these changes may break important things that
I don't yet understand because of that lack of experience, but I figured since I am having so much fun with this fork, it
doesn't take much effort to share it with others, and maybe some of my crazy ideas may be merged into the main DCSS repo. And of
course you are welcome to submit changes to this fork or even make another fork if you don't like the direction I'm taking this one. 
This fork is in sync with the original DCSS repo as of Feb 2016. I intend to keep merging in non-conflicting changes from the main DCSS project.

Check out the github wiki for this project if you want to see what my future plans are: https://github.com/jeremygurr/dcssca/wiki/Future-Plans

There are two known servers running this fork at this time: 
https://crawl.project357.org/
http://crawl.homedns.org/

## Objectives

(Not necessarily all that different form mainstream crawl, just different in exactly how I interpret these...)

* No boring species, gods, monsters, items, branches, etc.
    * I never play humans in the mainstream crawl. It's just never interesting for me to do so, since they don't have any real benefit over any other species.
      So in this version, I amplified the differences between species, especially the "boring" ones I never play, to make each species more appealing in it's
      own way. 
    * My philosophy is to first try and make a problem area more interesting. If that seems utterly impossible, then it should be removed. 
    * For example, Djinni was removed from mainstream crawl because of serious problems with it's design. I brought it back, changed the design dramatically,
      and now I believe it is a fun and interesting species to play without the major issues it was removed for in the first place. 
* A more fun experience for new players and intermediate players.
    * Current mainstream crawl seems to heavily favor making the game fun for extreme veterans and experts. New players have a very hard time finding enough fun
      in the game to justify investing the time needed to really get good at it. Everything is balanced around understanding how to effectively use every item
      in the game, a situation most players don't achieve until they've played the game for a very long time. 
    * There should be a way for people who don't know how to properly use a blink scroll, or to effectively identify their inventory, for example, to still have fun.
    * A wiki shouldn't be required to have fun with the game. I'm hardly a newbie with many hundreds of hours of crawl gametime experience, yet I still feel
      like I have to lean on the wiki from time to time because of the way mainstream crawl is designed. 
    * Example: Mainstream crawl's refusal to show how much damage a given attack is doing makes it very difficult for new players to figure out which attacking
      strategies / weapons / buffs are most effective. Just adding a simple damage number makes it so much easier to see if the flame tongue spell is really
      accomplishing more than just trying to bash them with a morningstar. This is somewhat ironic because I think the reason to avoid numbers is to make it
      easier for new players... but in my experience it seems to be doing the opposite. 

## Major modifications from the original DCSS

* Inventory expansion
    * The inventory has been divided into two groups: consumables (potions, scrolls, and food), and everything else. The 'i' command shows the weapons, armour,
      evokables, etc. The 'I' command shows the consumables. Each can have 52 items. 
    * The drop command has been split into two also: 'd' to drop inventory items, control-D to drop consumable items.
    * The adjust command '=' can now also be applied to the (c)onsumables. 
    * The spell list command has been moved from 'I' to 's'. 
    * Wands automatically stack in inventory, possibly exceeding max capacity. 

* Monsters can't use stairs, but if they hurt you while going up or down stairs, you are interrupted
    * eliminates ability of player to pull apart groups of monsters into bite size pieces. 
    * eliminates easy escapes on stairs when monsters can still inflict damage (even ranged ones).

* Damage numbers are shown
    * yeah I know it adds some details that the original designers want to avoid. But really, how many crawl players aren't seriously hard core RPGers anyway? I
      personally find it a bit less tedious to try out different attack strategies when I can see the numbers, instead of trying to guess from vague
      descriptions whether or not a change is actually improving my attacking effectiveness. 

* Summoning
    * no summon caps. Instead, summoned creatures continuously drain mana from the summoner, until there is no longer mana to support them, and then they
      disappear. More powerful summoned creatures drain a greater amount of mana. This opens up new strategies, enabling a player to summon one powerful
      creature for a longer period of time, or many of them for a very short time. For now, this only applies to the player summons. Monster summons work as
      before. 
    * how much magic a summon drains is reduced by higher spell power
    * added a "release summons" ability, so you can free up your mana pool if you no longer need those summons. 
    * summoners start with a +1 dagger to make getting off the ground a little more possible. It's still tough though. Don't summon more than
      one at once, and focus on boosting your spell power until you can sustain more powerful creatures. 

* Transmutation
    * It costs 3x more to cast a self-transforming spell
    * transformations don't time out
    * power of the transformation is based on spell power

* Game difficulty levels
    * At game start, or in the init file, you can specify that the game is easy, normal, or hard. 
    * Normal is the standard, unmodified parameters.
    * Easy:
        * reduces the chances of out of depth monsters spawning
        * starts the player with 10 more hp
        * increases the amount of gold spawned by 33%
        * starts player with a healing potion
        * faster level advancement (exp apt + 2)
        * 1/4 normal score
        * healing potions heal 100% of health
    * Hard:
        * increases the chances of out of depth monsters spawning
        * slower level advancement (exp apt - 2)
        * 33% less gold spawned
        * 25% less health
        * 4x normal score
        * healing potions heal 25% of health, minimum of 20 points
    * This is just the beginning. I'm sure with more testing and experience, we can find ways to make these difficulty levels a lot more interesting.

* Unequipping shields
    * When trying to equip a weapon that is incompatible with a shield, the game will ask if you want to unequip the shield first, saving some hassel.

* Movement
    * Changing direction 90 degrees slows movement by 50%. Changing direction 180 degrees slows movement by 100%. Other movement is 10% faster than normal to 
      balance things out. 
    * There is an option to disable this: old\_movement = true

* Traps
    * Shafting doesn't happen in the first 2 floors of dungeon
    * Shafting has a max depth of 1 for easy, 3 for normal, and 5 for hard.

* Flying
    * metabolism is multiplied by 4 while flying, unless you are a Djinni. So flying around all day as a tengu will now cost something. 

* Djinni brought back from the dead and greatly enhanced.
    * Unusual contamination mechanism that they originally had has been removed. They also consume food like normal species, so excessive spell casting has the
      normal consequence.
    * Djinni can no longer read scrolls of any kind, even though they can use spell books.
    * Since they can't read remove curse scrolls, cursed items are a much bigger deal for them. To mitigate this a little, they have a remove curse ability that
      costs one permanent MP.
    * Since they can't read identify scrolls, they can either identify items by using them (very dangerous for potentially cursed items), or wait and use the
      new insight mutation that they start with which, over time, randomly identifies attributes of items in their inventory. This mutation can also randomly be
      gained by other species as they drink mutation potions, etc.
    * Fire damage heals them. Standing in a flame cloud is a way to heal, but the flame cloud is absorbed by the Djinni and disappears more quickly
      than normal. Be careful though, increasing their fire resistance (through an item, ring, etc), will reduce their healing. Maybe you can find a source of
      lowering their fire resistance?
    * They have very low magic resistance.
    * They have the glow mutation, which increases with experience, which reduces their stealth.
    * Cold and water damage does triple the resistable amount of damage, in addition to the penalties of having one level of cold vulnerability. A ring of ice
      would be very valuable here, reducing the cold damage done, as well as increasing the healing from fire damage. Unfortunately, there is no water
      resistance buff... muahahaha....
    * Unarmed combat gets a fire brand. 
    * They start with ephemeral 3 mutation, and slowly lose it as they gain experience (as they become more corporeal). The ephemeral mutation give a chance
      that attacks (both melee and ranged) will completely pass through the Djinni. 
    * Dithmenos rejects Dj of course

* Mummies
    * initial attributes improved greatly, making early game much easier, but long term they are weaker, gaining levels more slowly than before
    * immunity to curses, so trying out cursed armour or weapons isn't a problem

* Felid
    * can wear 4 rings. Why not? They have four identical "hands"... I find that this helps to balance their severe limitations a bit better and make the early
      game a bit more doable. 
    * move faster as they advance in levels.
    * have exactly 9 lives right at the beginning, which never increases. How many lives can you end up with at the end of the game?
    * no loss of experience when death happens

* Ghouls
    * start with a permanent empowered by death mutation, making them regenerate much faster after going on a killing spree. 

* Halfling
    * added wild magic 1 mutation at the beginning, eventually growing into wild magic 3 as they gain experience, making it much more difficult for them to cast spells, 
      but they are quite powerful when they do. 
    * added an extraordinary level of magic resistance

* Hill Orc
    * added subdued magic 3 mutation at the beginning, making it very easy for them to cast spells, but they are quite weak as a result.

* Kobold
    * added permanent evolution mutation. Although this sounds similar to how demonspawns work, it actually plays quite differently, because the player will occasionally
      get random bad mutations, and sometimes have to adjust their play style to compensate. There is also a greater than normal chance that evolve will replace
      a bad mutation with a good, making drinking mutation potions a safer strategy for kobolds. Unlike demonspawn which get permanent mutations, the mutations
      a kobold gets through evolve are temporary, making cure mutation potions often a very bad thing. And of course they only get ordinary mutations, not the
      cool demonspawn specific ones.
    * as a result of their highly mutable nature, they cannot worship Zin.
      
* Naga
    * increased effectiveness of their poison spitting ability, making them a bit easier in the beginning
    * removed the misshapen armour restriction. Body armour gives full benefit, unlike with centaurs. This better balances the fact that not only are they slow,
      but they have poor shield, armour, and dodging aptitudes. 

* Ogre
    * has permanent powered by pain mutation at level 12. This gives them occasional boosts as they are damaged. 
    * powered by pain was modified so that it only works 2/3 of the time on normal, and 1/3 of the time on hard.
    * significantly increased health aptitude, making them have by far the greatest health pool of all species. 
    * added poor fitting armour mutation. That mutation was changed slightly, so that level 1 gives 2/3 of
      normal AC, level 2 gives 1/3 of normal AC. They get level 1 at exp level 10, and level 2 of this mutation at exp level 15.

* Octopode
    * has permanent augmentation mutation at the beginning. This boosts their fighting and magic usage when they have a high amount of health, making them
      significantly easier early on.

* Tengu
    * start with clarity
    * while flying, their speed and dodging increases based on their experience level.
    * start with shock resistance
    * start with thin skeletal structure
    * can't wear rings at all

* Troll
    * they now gain several different elemental resistances as they gain levels
    * start with higher hp than normal, but never gain hp with level increases. They can increase hp by training fighting or through items, but late game they
      will have much lower hp than normal.

* Deep Dwarf
    * recharge ability has been split in two: lesser recharge (costs 1 PMP), and greater recharge (costs 3 PMP). Greater recharge is required to recharge
      wands of healing, haste, and teleport

* Deep Elf
    * increased spell casting aptitude
    * increased magic capacity

* Demigod
    * even higher increase of stats over time
    * lower exp aptitude

* Lava Orcs
    * brought back but not modified much yet. 
    * can't worship Dith.
    * probably still quite broken until I have the time to attempt to smooth out their problematic areas (if it is even possible).

* Vine Stalkers
    * added a lignify ability, allowing them to lignify or unlignify at will, costing some time and food, but
      possibly adding some interesting strategic choices. 
    * can't wear body armour at all

* Spells
    * because the new consumables inventory took the 'I' command, the spell list can now be seen with the 's' command.
    * There is now a unified spell list which removes the need to toggle between two views.
    * All spells have the same spell cap... now even basic spells can continue to grow in power late in the game. 
    * the effect of spell casting skill on spell success and power has been increased.
    * Throw Flame has been removed from spell books. Monsters still cast it, but the player can't. Instead, Flame Tongue has been slightly enhanced to fill that
      void. Not much of a void, since I hardly used it anyway. Magma Bolt has been put in the Flames Book as a level 5 spell alongside Fireball. 

* Potions and wands of heal wounds
    * easy mode: heals 100% of max hp
    * normal mode: heals 50% of max hp (minimum of 20 points)
    * hard mode: heals 25% of max hp (minimum of 20 points)
    * wands of heal wounds take from 2 to 10 turns to execute successfully, depending on evocations, and may be interrupted by monsters (making them almost useless during combat).

* Recharge scrolls
    * less common, but fully recharge the wand.

* Potions of magic
    * easy mode: gives 100% magic
    * normal mode: gives 50% magic
    * hard mode: give 25% magic
    * restores a minimum of 10 points

* Potions of lignification (and lignify ability of vinestalkers)
    * lower initial AC (20 -> 15)
    * higher AC scaling with experience (0.5 -> 1.5 AC per level)
    * higher health boost (1.5x -> 2x)
    * no hp regen while lignified
    * intention is to make it useful even in late game. It's a rare consumable, it won't break the game by being a little more powerful later in the game. 

* Mutation tweaks
    * powered by pain works as often as before on easy, only 2/3 of the time on normal, and 1/3 of the time on hard. 

* Gozag
    * only half of corpses dropped change to gold, opening up gozag to many more species. There is a higher amount of gold dropped per corpse to make the total
      gold benefit the same as before. 

