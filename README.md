# Dungeon Crawl Stone Soup: Circus Animals variation

This fork of DCSS contains many wacky experiments I've toyed with over the years, which for me at least has made the game a lot more fun. 
The majority of my changes center around increasing the richness of the different species in DCSS. 
I also made a number of changes to make early game a little more enjoyable, especially for people new to DCSS. I find that many of my family and friends I want
to introduce to DCSS have a hard time getting over that initial learning curve.
I would repeatedly take the species that I considered most boring to play, and alter that species until it was a compelling choice for me to play. 
I know quite well that I am not nearly as experienced in playing this game as many others are, and likely some of these changes may break important things that
I don't yet understand because of that lack of experience, but I figured since I am having so much fun with this fork, it
doesn't take much effort to share it with others, and maybe some of my crazy ideas may be merged into the main DCSS repo. And of
course you are welcome to submit changes to this fork or even make another fork if you don't like the direction I'm taking this one. 
This fork is in sync with the original DCSS repo as of Feb 2016. I intend to keep merging in non-conflicting changes from the main DCSS project.

## Major modifications from the original DCSS

* Djinni brought back from the dead and greatly enhanced.
    * Unusual contamination mechanism that they originally had has been removed.
    * Djinni can no longer read scrolls of any kind, even though they can use spell books.
    * Since they can't read remove curse scrolls, cursed items are a much bigger deal for them. To mitigate this a little, they have a remove curse ability that
      costs one permanent MP.
    * Since they can't read identify scrolls, they can either identify items by using them (very dangerous for potentially cursed items), or wait and use the
      new insight mutation that they start with which, over time, randomly identifies attributes of items in their inventory. This mutation can also randomly be
      gained by other species as they drink mutation potions, etc.
    * They have an extraordinary spell casting aptitude to help balance their dramatic negatives
    * They have a poor species stealth modifier, because of the fact that they emit light, it's hard for them to hide in the shadows.
    * Fire damage heals them. Standing in a flame cloud is a way to heal, but the flame cloud is absorbed by the Djinni and disappears more quickly
      than normal.

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

* Troll
    * they now gain several different elemental resistances as they gain levels

* Deep Elf
    * increased spell casting aptitude
    * increased magic capacity

* Demigod
    * even higher increase of stats over time
    * lower exp aptitude

* Spell casting
    * no more spell caps... now even basic spells can continue to grow in power late in the game. 
    * the effect of spell casting skill on spell success and power has been increased.

* Summoning
    * no summon caps. Instead, summoned creatures continuously drain mana from the summoner, until there is no longer mana to support them, and then they
      disappear. More powerful summoned creatures drain a greater amount of mana. This opens up new strategies, enabling a player to summon one powerful
      creature for a longer period of time, or many of them for a very short time. For now, this only applies to the player summons. Monster summons work as
      before. 
    * summons occasionally disappear even if you have enough mana to support them. 
    * added a "release summons" ability, so you can free up your mana pool if you no longer need those summons. 


