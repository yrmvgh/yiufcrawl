# Dungeon Crawl Stone Soup: Circus Animals variation

This fork of DCSS is a playground for some ideas I wanted to try. 
The majority of my changes center around increasing the richness of the different species in DCSS. 
I would repeatedly take the species that I considered most boring to play, and alter that species until it was a compelling choice for me to play. 
I know quite well that I am not nearly as experienced in playing this game as many others are, and likely some of these changes may break important things that
I don't yet understand because of that lack of experience, but I figured since I am having so much fun with this fork, it
doesn't take much effort to share it with others, and maybe some of my crazy ideas may be merged into the main DCSS repo. And of
course you are welcome to submit changes to this fork or even make another fork if you don't like the direction I'm taking this one. 
This fork is in sync with the original DCSS repo as of Feb 2016. I intend to keep merging in non-conflicting changes from the main DCSS project.

## Major modifications from the original DCSS

* Summoning
    * no summon caps. Instead, summoned creatures continuously drain mana from the summoner, until there is no longer mana to support them, and then they
      disappear. More powerful summoned creatures drain a greater amount of mana. This opens up new strategies, enabling a player to summon one powerful
      creature for a longer period of time, or many of them for a very short time. For now, this only applies to the player summons. Monster summons work as
      before. 
    * summons occasionally disappear even if you have enough mana to support them. 
    * added a "release summons" ability, so you can free up your mana pool if you no longer need those summons. 

* Inventory expansion
    * The inventory has been divided into two groups: consumables (potions, scrolls, and food), and everything else. The 'i' command shows the weapons, armour,
      evokables, etc. The 'I' command shows the consumables. Each can have 52 items. 
      The drop command has been split into two also: 'd' to drop inventory items, 'D' to drop consumable items.
      The adjust command '=' can now also be applied to the (c)onsumables. 

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
    * They have the glow mutation, which increases with experience, which reduces their stealth. However, at level 3, it expands how far they can see. 
    * Cold and water damage does double the resistable amount of damage, in addition to the penalties of having one level of cold vulnerability. A ring of ice
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
    * added wild magic 3 mutation at the beginning, making it much more difficult for them to cast spells, but they are quite powerful when they do. 
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
    * has permanent powered by pain mutation at the beginning. This gives them occasional boosts as they are damaged. 
    * significantly increased health aptitude, making them have by far the greatest health pool of all species. 

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

* Deep Elf
    * increased spell casting aptitude
    * increased magic capacity

* Demigod
    * even higher increase of stats over time
    * lower exp aptitude

* Lava Orcs
    * brought back but not modified much yet. 
    * can't worship Dith.

* Vine Stalkers
    * added a lignify ability, allowing them to lignify or unlignify at will, costing some time and food, but
      possibly adding some interesting strategic choices. 

* Spells
    * because the new consumables inventory took the 'I' command, the spell list can now be seen with the 's' command.
    * I'm experimenting with a unified spell list, with all of the essential info at once (instead of the toggle). This should probably be an option, or auto
      detect when the screen is wide enough, but I'm not exactly sure how to do this yet. Suggestions welcome. 
    * All spells have the same spell cap... now even basic spells can continue to grow in power late in the game. 
    * the effect of spell casting skill on spell success and power has been increased.
    * Throw Flame has been removed from spell books. Monsters still cast it, but the player can't. Instead, Flame Tongue has been slightly enhanced to fill that
      void. Not much of a void, since I hardly used it anyway. Magma Bolt has been put in the Flames Book as a level 5 spell alongside Fireball. 

* Potions of heal wounds
    * now fully heal the target
    * less than half as common to find around the dungeon
    * wands of heal wounds work almost like before, except that their effectiveness scales with evocations skill, and the amount healed is proportional to
      health capacity. 

* Damage numbers are shown
    * yeah I know it adds some details that the original designers want to avoid. But really, how many crawl players aren't seriously hard core RPGers anyway? I
      personally find it a bit less tedious to try out different attack strategies when I can see the numbers, instead of trying to guess from vague
      descriptions whether or not a change is actually improving my attacking effectiveness. 

* Gozag
    * only half of corpses dropped change to gold, opening up gozag to many more species. There is a higher amount of gold dropped per corpse to make the total
      gold benefit the same as before. 

