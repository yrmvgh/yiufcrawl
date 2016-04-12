# Dungeon Crawl Stone Soup: Circus Animals variation

Checkout branch v1.2.2 if you want to try it yourself. The master branch will be where future, save game breaking features will be implemented, and then broken off into v1.2.
For those who don't know how to clone from a git repo: install git on your system, then type:

    git clone https://github.com/jeremygurr/dcssca.git -b v1.2.2

This fork of DCSS is a playground for some ideas I wanted to try. 
Many of my changes center around increasing the richness of the different species in DCSS. 
I would repeatedly take the species that I considered most boring to play, and alter that species until it was a compelling choice for me to play. 
I know quite well that I am not nearly as experienced in playing this game as many others are, and likely some of these changes may break important things that
I don't yet understand because of that lack of experience, but I figured since I am having so much fun with this fork, it
doesn't take much effort to share it with others, and maybe some of my crazy ideas may be merged into the main DCSS repo. And of
course you are welcome to submit changes to this fork or even make another fork if you don't like the direction I'm taking this one. 
This fork is in sync with the original DCSS repo as of Apr 2016. I intend to keep merging in non-conflicting changes from the main DCSS project.

Check out the FUTURE.md file in the same directory as the README.md for details about what features I'm considering on implementing.

There are two known servers running this fork at this time: 
- http://crawl.homedns.org/      (always has the latest code)
- https://crawl.project357.org/  (probably more stable)

Feel free to create issues on github for either problems with the crawl.homedns.org server, or bugs in the game itself, or ideas you have that you think are in line
with my goals here, or with concerns you have about my future plans. 

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

### v1.2.3

* Experience modes
    * You can now configure the experience settings through predefined modes. The old settings have been removed.
    * You select a mode by using "experience_mode = ..." in your rc file.
    * Current modes:
        * experience_mode = classic
            * this is the basic vanilla crawl way
            * experience is gained by killing monsters
        * experience_mode = simple_xl
            * experience is only gained by reaching new floors. 
            * every 3 floors gives a new experience level, assuming a species exp apt of 0, and difficulty mode is normal
        * experience_mode = simple_depth
            * experience is only gained by reaching new floors. 
            * depending on how difficult your current level is determines how much experience you get for reaching it. 
        * experience_mode = blended
            * 1 experience potion spawns on each floor
            * experience potions are dropped by each unique and player ghost
            * experience is evenly divided between reaching new floors, killing monsters, and drinking potions at the lowest possible level
        * experience_mode = intensity
            * 1 experience potion spawns on each floor
            * experience potions are dropped by each unique and player ghost
            * a little experience is gained for reaching a new floor
            * but most of the experience is gained from drinking potions at the lowest dungeon level possible
        * experience_mode = pacifist
            * 1 experience potion spawns on each floor
            * experience is gained by drinking potions and reaching new branch floors
            * experience is lost for each monster killed
        * experience_mode = destroyer
            * experience is gained for killing monsters
            * experience potions are dropped by each unique and player ghost
            * experience is lost for each new floor reached
            * experience potions give experience based on how deep they are quaffed

### v1.2.2

* Human 
    * exp apt +2
    * fighting and spellcasting apt +2
    * can crosstrain many more skills
        * fire, ice, earth, air
        * invo, evo, spellcasting
    * god wrath lasts half as long, since the gods understand how fickle humans can be.
    
* Draconian
    * Pale Draconian
        * inv apt 1 -> 0
        * evo apt 1 -> 2
    * Mottled Draconian
        * now has rF+ and rC+
        * sticky flame can go 2 spaces instead of 1
        * +1 apt for elemental magics and poison

* Options for experimenting with experience model changes (each option is shown with it's current default)
    * level_27_cap = false
        * when false, the level and skill cap is 99, and a smooth scale is used to determine how much exp is required for each level
        * when true, the old experience cap and scale is in place
    * exp_potion_on_each_floor = false
        * when true, an experience potion is placed on every floor
            * this is used in combination with other options, for example, getting rid of monster experience and making the potion give you experience based
              on the floor you drink it on. 
    * uniques_drop_exp_potions = false
        * when true, uniques and player ghosts will always drop an exp potion
        * when false, they don't, like before
    * exp_percent_from_monsters = 100 (-1000 - 1000)
        * when 0, killing a monster gives 0 experience
        * when 100, killing a monster gives the full experience it gave in standard crawl
    * exp_percent_from_potions = 100 (-1000 - 1000)
        * if exp_based_on_player_level = true
            * when 0, drinking a potion of experience gives 0 experience
            * when 100, drinking a potion of experience gives it's full amount 
        * if exp_based_on_player_level = false
            * when 20, drinking a potion of experience gives 20% of the experience needed for the player
              to advance to the next xp level
            * when 100, drinking a potion of experience gives 100% of the experience needed for the player
              to advance to the next xp level
    * exp_percent_from_new_branch_floor = 0 (-1000 - 1000)
        * if 0, no experience is gained by reaching a new floor
        * if 50 
            * and exp_based_on_player_level = false
                * each new floor the player reaches gives the player experience based on the depth of the floor
                * 50 here means 50% of the "normal" amount. 100% would give a balanced game if there were no other sources of experience.
            * and exp_based_on_player_level = true
                * each new floor the player reaches gives the player 50% of the experience needed to advanced to the next level.
        * only applies to floors that can have monsters on them. No experience is gained for safe floors (temple, trove, bazaar, or D:1)
    * exp_based_on_player_level = true
        * when false, potions of experience give experience relative to the floor the player is on
            * this encourages the player to take additional risks by plunging deeper into the dungeon before drinking the potion
            * for example, drinking the potion on D:1, would give 1 xp, but drinking it on D:4 would give 35 xp. 
        * when true, experience is given relative to the players xl
            * if exp_percent_from_potions_on_floor = 100, for example, then the player gets 100% of the 
              experience needed to advance to the next xl when drinking a potion.
            * this means that it doesn't matter where the potion is quaffed, they will get the same amount of experience. 
            * example: player is level 1, they drink a potion of exp, they are now level 2. If they are level 2, and drink it,
              they will be level 3. If exp_percent_from_potions_on_floor < 100, then it will take more than one potion to go
              up a full level. 
              
So, to recap the above, you can copy these options (which include their defaults) into your rc file and tweak them:
              
    level_27_cap = false
    exp_potion_on_each_floor = false
    uniques_drop_exp_potions = false
    exp_percent_from_monsters = 100
    exp_percent_from_potions = 100
    exp_percent_from_new_branch_floor = 0
    exp_based_on_player_level = true

### v1.1

* Better low health warnings, especially for new players
    * A new option, called danger_mode_threshold was made which defaults to 30. 
    * If damage is done to the player exceeding danger_mode_threadhold percent of the player's current hp, danger mode is switched to on, and a special
      warning message is given, the screen flashes, and the player is given a "more" message. This message appears only if danger mode is off. 
    * Danger mode stays on until no "danger" events have happened for 10 turns.
    * Example: A player has 20 health left, the danger_mode_threshold is set to 20, the player isn't currently in danger mode, and he gets hit for 5
      damage. 5 is less than 30% of 20, so no warning message is given. Then the player is hit for 5 points of damage again, and since 5 damage is more
      than 30% of 15, the danger message is given, and danger mode is turned on. No more warnings will be given until things quiet down. 
    * This is disabled by setting danger_mode_threshold to 0.
    
* Eliminate pillar dancing and extended kiting (don't knock it until you've given it a fair chance, it really works)
    * The first time a player moves, it takes 2.0 aut, independent of other factors.
    * If the player moves again in the same general direction (within 45 degress or less of the original movement), then they move 10% faster than their normal
      speed. As long as they don't stop (by doing something other than moving in a similar direction), they keep moving at the 10% faster than normal rate. 
    * If they stop to attack, then next time they move it will take exactly 2.0 aut. It might take longer if their normal movement speed would take longer. 
    * If they change directions by 90 degrees or more, they will pay again the 2.0 aut movement penalty. 
    * Monsters move just like before, no change there. 
    * Pillar dancing no longer will work because most monsters will easily be able to catch up.
    * Players can still escape from monsters like before, with speed boosts (haste, species speed, swiftness, etc) making it easier.
    * Extended kiting is impractical even for the fastest species with haste. Try it and you'll see what I mean. 
    * The 2.0 aut penalty mentioned is actually the greater of 2.0 or the normal speed of the player, so if you are playing Naga of Chei, your normal speed
      might exceed 2.0, in which case your change-movement penalty is your normal speed. In effect, that means there is no penalty for creatures that are 
      already slower than 2.0, since they can't kite anyway. 
    * This is tweakable with the rc file option: movement_penalty = 20 
        * 20 is the default, meaning 2.0 turns
        * set to 0 to have the old behavior
    
* Curse enhancement
    * Curses now have a curse level. 
    * The curse on equipped items decay as a player gains experience (unless the player worships Ash of course).
    * Typical early game curses are at curse level 100. 
    * Reading a remove curse scroll reduces the curse level by a minimum of 100, scaling up with invocations and piety (whichever is greater). 
    * Once the curse level drops to 0, the item is no longer cursed. 
    * Later game curses may be much higher than 100, and require multiple scrolls or a lot of waiting before they go away. 
    * Mummy death curses curse an equipment slot, not an item. Equipment slot curses pass on to an item as soon as it is equipped.
    * The same item can be cursed multiple times, increasing the curse level. 
    * Curse level > 1000 will cause the bonuses of that item to be neutralized. A +5 long sword of poison becomes a +0 long sword, while the curse is above 1000. 

### v1.0

* Inventory expansion
    * The inventory has been divided into two groups: consumables (potions, scrolls, and food), and everything else. The 'i' command shows the weapons, armour,
      evokables, etc. The 'I' command shows the consumables. Each can have 52 items. 
    * The drop command has been split into two also: 'd' to drop inventory items, control-D to drop consumable items.
    * The adjust command '=' can now also be applied to the (c)onsumables. 
    * The spell list command has been moved from 'I' to 's'. 
    * Wands automatically stack in inventory, possibly exceeding max capacity. 

* Stair changes
    * Monsters can't use stairs, but if they hurt you while going up or down stairs, you are interrupted.
        * Eliminates ability of player to pull apart groups of monsters into bite size pieces. 
        * Eliminates easy escapes on stairs when monsters can still inflict damage (even ranged ones).
    * If you take the stairs, but then immediately go back through the stairs again, you can't be interrupted.
        * Avoids the situation where a player goes onto a level surrounded by dangerous monsters, but can't escape
          back to the previous level. 

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
        * player has 50% more hp
        * player has 50% more mp
        * increases the amount of gold spawned by 33%
        * starts player with a healing potion
        * faster level advancement (exp apt + 2)
        * 1/4 normal score
        * healing potions heal 100% of health
        * ghosts don't spawn above level 10 of dungeon
    * Normal:
        * ghosts don't spawn above level 5 of dungeon
        * healing potions heal 50% of health, minimum of 20 points
    * Hard:
        * increases the chances of out of depth monsters spawning
        * slower level advancement (exp apt - 2)
        * 33% less gold spawned
        * 33% less health
        * 4x normal score
        * healing potions heal 25% of health, minimum of 20 points
    * This is just the beginning. I'm sure with more testing and experience, we can find ways to make these difficulty levels a lot more interesting.

* Unequipping shields
    * When trying to equip a weapon that is incompatible with a shield, the game will ask if you want to unequip the shield first, saving some hassel.

* Traps
    * Shafting doesn't happen in the first 2 floors of dungeon

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

* Mummy
    * initial attributes improved greatly, making early game much easier, but long term they are weaker, gaining levels more slowly than before
    * immunity to curses, so trying out cursed armour or weapons isn't a problem
    * can worship Ash, but their curse immunity leaves

* Felid
    * can wear 4 rings. Why not? They have four identical "hands"... I find that this helps to balance their severe limitations a bit better and make the early
      game a bit more doable. 
    * move faster as they advance in levels.
    * have exactly 9 lives right at the beginning, which never increases. How many lives can you end up with at the end of the game?
    * no loss of experience when death happens

* Ghouls
    * start with a permanent empowered by death mutation, making them regenerate much faster after going on a killing spree. 

* Halfling
    * added wild magic mutation at the level 6, 12, and 18, making it much more difficult for them to cast spells, but they are quite powerful when they do. 
    * added an extraordinary level of magic resistance
    * improved conjuration apt from -2 to +2

* Kobold
    * added permanent evolution mutation. Although this sounds similar to how demonspawns work, it actually plays quite differently, because the player will occasionally
      get random bad mutations, and sometimes have to adjust their play style to compensate. There is also a greater than normal chance that evolve will replace
      a bad mutation with a good, making drinking mutation potions a safer strategy for kobolds. Unlike demonspawn which get permanent mutations, the mutations
      a kobold gets through evolve are temporary, making cure mutation potions often a very bad thing. And of course they only get ordinary mutations, not the
      cool demonspawn specific ones.
    * as a result of their highly mutable nature, they cannot worship Zin.
      
* Sludge Elf
    * brought back because I needed a species to experiment with. 
    * added subdued magic 3 mutation at the beginning, making it very easy for them to cast spells, but they are quite weak as a result. 
      This enables them to cast high level, low power requirement spells early or even in heavy armour, that would be impossible for others. 
    * their starting attributes are unusually low.
    * they have a very low magic capacity.

* Naga
    * poison spitting advances to level 2 at xp level 6, and to level 3 at xp level 12.
    * made them even slower, but the slowness comes more steadily, gaining slow mutation at 1, 4, and 8.

* Ogre
    * has permanent powered by pain mutation at level 12. This gives them occasional boosts as they are damaged. 
    * powered by pain was modified so that it only works 2/3 of the time on normal, and 1/3 of the time on hard.
    * significantly increased health aptitude, making them have by far the greatest health pool of all species. 
    * added poor fitting armour mutation. That mutation was changed slightly, so that level 1 gives 2/3 of
      normal AC, level 2 gives 1/3 of normal AC. They get level 1 at exp level 10, and level 2 of this mutation at exp level 15.

* Octopode
    * has permanent augmentation mutation 1 after level 6. This boosts their fighting and magic usage when they have a high amount of health, making them
      significantly easier early on. Then their gelatinous body advances to level 3 by exp level 8.

* Tengu
    * while flying, their speed and dodging increases based on their experience level.
    * start with shock resistance
    * start with thin skeletal structure
    * can't wear rings at all
    * have a new mutation called enchantment absorbtion which gives a chance that enchantments used against the player will instead be absorbed into their magic pool
        * at xp 6 they get enchantment absorb 1, which means 10% of the time the enchantment is absorbed
        * at xp 12 they get ench absorb 2 = 40% of the time it is absorbed
        * at xp 18 they get ench absorb 3 = 90% of the time it is absorbed, making them *almost* immune to magic enchantments.

* Troll
    * split into two subspecies
    * Cave Troll
        * like old troll, except with a couple more shaggy fur mutations that come in later
    * Moon Troll
        * gain several different elemental resistances as they gain levels
        * start with higher hp than normal, but never gain hp with level increases. They can increase hp by training fighting or through items, but late game they
          will have much lower hp than normal.
        * better magic aptitudes, but spellcasting skill is still bad
        * apt 0 defensive skills (to help make up for their very low health later in the game).
          
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

* Spells
    * because the new consumables inventory took the 'I' command, the spell list can now be seen with the 's' command.
    * There is now a unified spell list which removes the need to toggle between two views.
    * All spells have the same spell cap... now even basic spells can continue to grow in power late in the game. 
    * the effect of spell casting skill on spell success and power has been increased.
    * Throw Flame has been removed from spell books. Monsters still cast it, but the player can't. Instead, Flame Tongue has been slightly enhanced to fill that
      void. Not much of a void, since I hardly used it anyway. Magma Bolt has been put in the Flames Book as a level 5 spell alongside Fireball. 

* Potions of heal wounds
    * easy mode: heals 100% of max hp
    * normal mode: heals 50% of max hp (minimum of 20 points)
    * hard mode: heals 25% of max hp (minimum of 20 points)

* Wands of heal wounds
    * heal based on evocations skill
    * always take 2 turns, uninterruptable

* Recharge scrolls
    * less common, but fully recharge the wand.
    * each time a wand is recharged, it's max charge capacity is cut in half.

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

* Minor item tweaks
    * ring of magical power now gives you 50% more mp, or 9 mp (like before), whichever is higher, making this ring still have some late game value.
