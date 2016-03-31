* To discuss these ideas, create an issue. Search closed issues to make sure your issue wasn't already discussed. If you have more to add to a closed issue,
  reopen it.

* Most of these ideas will probably be implemented in some form or another, unless an idea is so terrible it can't be saved, which there will likely be a few.

* They are basically in order of when I will implement them, although I often change that order.

*************************************
# Version 1.2
*************************************

Some of these changes will make the game much easier for the player, but that is intended to be solved by making the game harder by implementing some of the
other ideas presented here. 

## Remove eating and chopping

There will still be a food clock, but it will be automatically managed. The player will set a target satiation level (defaults to just above hungry). Each time
a player rests, if the satiation level would drop below the target, the player automatically eats any edible corpses that haven't rotted yet (no more carrying
around chunks), or permafood from their inventory if there aren't enough corpses. Vampires can still target various satiation levels depending on how alive they
want to be. Both the 'e' command and the 'c' command will be gone. 

Note: you will not be able to eat in the middle of a battle any more, so prepare accordingly. If you end up starving mid battle, you need to escape and try
again, or you will likely not survive, as your regeneration will stop and you will start fainting. This situation should be easy to avoid though by setting your
target satiation level high enough and having sufficient rest time between battles. 

## Add stamina

This is a huge change, but maybe when everything else is done this could be fun to play with. Stamina would be a new quantity like Health and Magic. Stamina of
course would be the primary resource used by fighters, like magic is used for ... magic... users... ;P ...  But this opens up a bunch of new interesting buffs
and maluses, spells, potions, etc. With this would be the option to "run", which burns through stamina quickly, but gives you slightly more movement speed. This
would replace the existing "Exhausted" status with a more interesting drain of stamina when berserk, etc, are used. Running of course could be used on top of
the "swiftness" spell for added speed. 

So the "walking" speed of all species would be the same, and it would be set to slightly slower than most monsters, probably at 1.1, where most monsters would
be 1.0. Walking won't consume any stamina. However, a player can hit a key to switch to running mode, which burns through stamina but maybe a normal species
would travel at 0.8 when running. A centaur could run at 0.7, while a spriggan maybe 0.6. A tengu flying could move even faster, maybe 0.4, but flying burns
through stamina twice as quickly, and a tengu running on the ground is no faster than normal (just 0.9). But all would have to stop running after stamina ran
out. Running with heavy armour would increase stamina burn. Also attacking costs some stamina, and attacking with a heavier weapon when the character doesn't
have much strength will cost even more. 

Sneaking is another speed that can be set. It lowers the players movement speed but increases stealth. 

Stamina would replace food costs in some areas. For example, spellcasting will require stamina based on spellcasting skill and level of spell, and intelligence
(just like for the hunger cost before). Trying to cast a high level spell when a player is not that high level yet will burn through a lot more stamina, even
though the magic cost will be the same. Lower level spells for a higher level, higher intelligence character would cost no stamina at all, but still have the
same magic cost. 

I think this helps with my mission to make things easier for new players. Even though we are adding complexity here, this enables us to remove much more
complexity than is added: spell hunger mechanics, vampiric weapon function, regeneration costing more hunger, exhaustion from berserking and spitting poison.
All of this collapses into a single simple system. 

## Regeneration comes from food

Instead of specifically making regen enchancers burn through more food on a case by case basis, all regeneration that happens will do so in exchange for food.
Different races can have different conversion rates. If health, magic, and stamina are full, no food is consumed. So naturally, anything that accelerates
regeneration, also accelerates food consumption, without special casing or code having to be inserted into each place it happens. Spells will no longer cost
hunger, higher level spells will just cost more magic, and the magic regen that follows will burn more food. 

Since mummies can't eat, they can't regenerate with food like other species. Their regeneration comes from death energy that is present in a level (which is
based on how many corpses there are when the mummy is resting). If there are no corpses on a level, the mummy stops regenerating, despite of spell of
regeneration, items of regeneration, etc. Regeneration *always* has a price, even for a mummy. This makes a mummy very powerful when in a battle with many
enemies that drop corpses. However, they are particularly weak when battling enemies that don't drop corpses. 

*************************************
# Version 1.3
*************************************

## Expansion of identification mini game

Identification becomes pointless after about halfway through the game, with identify scrolls nearly worthless. I think that can be changed in a fun way to carry
through all the way to the end of a 15 rune run. Here's a crude outline of some ideas I've had:

Identity scrolls don't always work. For the main beginning usages, they will always work. But occasionally there will be a powerful unique item, that requires
more "identification power". Identification power is based purely on the players intelligence, enabling those with lower intelligence to quaff a brilliance
potion if necessary to identify some of the harder items. But I'm not talking about scrolls being lost when failing, like they were long ago. If you attempt to
identify something that's out of your league, the game will tell you that you aren't yet smart enough to do that, but the identify scroll will remain in your
inventory. 

There will be more rare and super rare items, both good and bad, that will be more difficult to identify than normal. Super rare potions and scrolls, as well as
unique weapons and armour. They would be infrequent enough not to cause a nuisance, but rewarding enough to be worth some effort to unlock their awesomeness.
Identify scrolls will also be less common, forcing the player to consider carefully how to best use them. Do I read-id all my scrolls, but use ID scrolls on my
potions? Or do I save the ID scrolls for more exotic items and quaff-id the potions too? Things should be balanced closely enough that there isn't a trivial
solution to those questions. It should vary based on character combo and the various random circumstances that happen. This also increases the value of
alternative forms of identification, such as Ash's powers, and the Djinni's native "insight" mutation, or even the rare chance that a non-djinni could get that
mutation. 

Brilliance potions will be more rare, but may be stacked to boost the effect further, so that even low intelligence characters could identify some advanced
items. Or an amplification scroll could be used (see below) in combination with the brilliance potion and / or identify scroll. 

3/4 of the unique items can still be identified by wearing them, although some will be cursed, and a more rare remove curse will be required (see below). But
1/4 of them can't be equipped at all until they are identified. They will tend to have an extra enchantment beyond what the other 3/4 have, and will never be
cursed. 

## Scroll of Amplification

This is a rare scroll that, when read, amplifies the next item used (scroll, potion, or wand). For example, reading an amplification scroll and then drinking a
brilliance potion will greatly enhance the effect, enabling a character to cast much more powerful spells, or to identify exceedingly rare and powerful
artifacts. Or they could amplify a teleport scroll, enabling them to specify exactly where in the level they want to teleport to. Or they could amplify a scroll
of acquirement, causing it to produce 3 items instead of one. Or you could amplify a remove curse scroll to eliminate all curses on any item in your inventory,
even the ones that are very difficult to get rid of. You could even amplify another amplify scroll, multiplying the power even further (if you happen to be
lucky enough to find 2 in a game). Double amplifying a teleport scroll could allow you to teleport to any branch, although you will be randomly placed in that
branch. 

Thinking about this further, it will work better to actually make them less rare, but work a little differently. All scrolls and potions will be grouped into
the following categories:

- mundane: potion of curing, scroll of teleportation, scroll of displacement, potion of ambrosia
- cool: scroll of inversion, blink scroll, potion of heal wounds
- awesome: scroll of acquirement, amp scroll, scroll of returning

it would require 1 amp scroll to work on a mundane item. It would require 2 to work on a cool item, and 4 to work on an awesome item. The categories do not
necessarily correspond with rarity. 

I'm open to better names :)

Of course it is so rare that you may only see a few of them in a game. Probably about as common as a scroll of acquirement. 

Some ideas for it:

    amp + recharge scroll: recharge all items in your inventory, or set one item to self-charge, or increase charging rate of a self-charging wand (replacements
for rods in this version).
    amp + cure mutation potion: cures all mutations, or cures only bad ones
    amp + beneficial mutation: 3 good mutations
    amp + cure potion: provides temporary poison immunity and clarity?
    amp + acquirement: get 3 items instead of 1
    amp + identify: identify items that would normally be much harder to identify, or identify whole inventory
    amp + amp: amp level 9 instead of 3. Can be stacked indefinitely. 
    amp + haste: longer speed boost (triple duration)
    amp + branding: will take a branded item and make it into an artifact with random properties, but the same brand

## Scroll of Inversion

kind of like the amp scroll, but reversing the effects of the next item used. More common than amp scrolls, but still fairly rare. 

    inv + cure mutation: gives you a bunch of mutations (like drinking a mutation potion, probably not a great idea). 
    inv + mutation: same as a cure mutation
    inv + silence: immunity to silence
    inv + noise: increases stealth
    inv + summoning: abjure all creatures in LOS
    amp + inv + mutation: cure all mutations
    inv + healing potion: instant death, at least on hard mode. Other modes would maybe reduce you to 1 HP.
    inv + teleport scroll: teleport all visible monsters 
    inv + blink scroll: blink a monster in LOS to anywhere else
    inv + poison: temporary regeneration boost
    inv + degeneration: temporary stat boost
    inv + acquirement: produces a amp scroll
    inv + identify: unidentify everything 
    inv + remove curse: curses all items
    inv + wand of random effect: changes the wand to always execute one effect
    inv + wand of polymorph: clones the target creature
    inv + invisibility: corona effect
    inv + berserk: clarity, stasis, slow movement?
    inv + amnesia: learn a random spell?
    inv + cure: confusion or poison

## Other new scrolls and potions

To support my philosophy of keeping the identification game interesting through late game, a few more late game tier scrolls and potions are needed. Most of
these are very rare, so they won't tend to clutter up your inventory.

- Scroll of Briars

    choose a smite targeted empty square, and a wall of briars will be formed in a line, perpendicular to the
    line between you and the targeted square, extending until they touch walls. The briars can be destroyed
    by monsters, but it will take a few turns. Ranged attackers can still shoot through them, just like with
    normal bushes.

- Scroll of Displacement

    swap places with a smite-targetted creature. Basically a weaker form of the blink scroll, but has some 
    interesting strategic side effects, since it can be used to get close to a summoner that is surrounded
    by summons (which blink can't). More common than blink scroll. Amplified version allows the player to
    swap with a wall, statue, or tree. Inverted form prevents both the target and player from moving or 
    teleporting for a period of time. 

- Scroll of Permanence

    choose one of your temporary statuses to be made permanent (mutations, briliance, MR+, Stealth+, rF+, 
    Fast, etc). Amplified version allows you to select 3. Inverted version allows you to choose a permanent
    status to be made temporary. Xom loves it when you use inversion on an unidentified scroll ... :)
    Requires multiple amp scrolls (according to the item class rules explained above) to use it on a higher
    level status (speed, health boost, for example). 

- Scroll of Karma

    applies an enchantment to all creatures in LOS, including the player. While this enchantment is in effect, 
    anything a creature does to another, will be reflected back on it. If a player heals a creature, the player
    also gets healed. If a monster shoots a needle of sleeping at the player, a needle of sleeping also has a
    chance of hitting the monster (although they can dodge it, just like the player). 

- Scroll of Replication

    target an item in inventory or a creature, and they will be duplicated. Of course, reading a scroll of 
    amplification before reading this scroll will cause 3 more of the target to be created instead of 1. The
    player can also target themselves, creating a friendly version that will fight alongside the player. 
    Inverted form of this will collapse all duplicates of the item down to one (handy for monsters that clone
    themselves, but will also apply to a pack of wolves, that can be converted into a single wolf). 
    Replication scrolls can't replicate Replication Scrolls or Amplification Scrolls, otherwise with 3 of
    each you'd have an unlimited supply.

- Scroll of Returning

    your current position is remembered, and a one time ability added that will instantly transport you back
    to that position, no matter where you are within the same branch. Of course if you are carrying the orb, 
    this won't work. Very powerful and very rare. Amplified version of this allows you to return even if
    you are in a different branch. Inverted form of this will teleport everything in LOS to the return point
    instead of the player. 

- Scroll of Time

    time stops for all creatures 2 spaces or closer to the player, including the player. Creatures outside of
    this 5x5 area may approach the player, but will also freeze when entering the time bubble. When the bubble
    expires, the player may have a much bigger challenge on their hands than before. Generally considered a
    bad scroll, however, the inverted form of this freezes all
    creatures outside of the 5x5 bubble. The player only has to deal with whatever is inside of the bubble, 
    then can rest / etc until the time runs out (which is pretty substantial, maybe 100 auts). If the player
    steps out of the bubble, it collapses. Amplified version creates a 3x3 bubble instead.

- Potion of Burning
    
    reduces your magic to 0, causing twice that much damage to your hp. The inverted version will also reduce
    magic to 0, but will heal twice that much. 

- Potion of Darkness

    lowers visibility to 3 squares in each direction. Amplified version lowers it to 1. Only affects player. 
    Player can still sense monsters within the normal LOS,
    even though they can't identify them until their are within 3 spaces. Monsters can see and attack the
    player as soon as they are within the standard 8 square LOS. This is very bad for the player. Inverted 
    version gives temporary see invisibility. Amplified inverted form gives temporary x-ray vision. 

- Potion of Patience

    Slows players movement and attack speed slightly, increases accuracy of all attacks significantly.

- Potion of Poison Vulnerability

    replaces old potion of poison. Inverted form gives temporary poison resistance. Amplified inverted form
    gives temporary poison immunity. 

- Potion of Balance

    the player's health, magic, and stamina are averaged. If you start with 10% HP, 90% MP, 50% SP, you will
    end up with 50% HP, 50% MP, 50% SP. 10% HP, 10% MP, 10% SP will give you exactly the same after drinking
    this potion, since they are already balanced. Amped version briefly holds all stats in their averaged
    form, not allowing anything to change them, providing unlimited magic and invulnerability and stamina for 4 
    turns. Inverted version reverses the balancing effect. If balance would normally take 70% HP to 60% HP, 
    then it goes the other way instead (to 80% HP). Inverted example: HP%/MP%/SP% 20/20/60 -> 7/7/87.

*************************************
# Version 1.4
*************************************

## Improved acquirement

After reading a scroll of acquirement, a list of 4 random items is presented, with a good chance that each will be valuable and useful (but not guaranteed) and
the player chooses 1. Some of the items could be multiples, e.g. 4 cure mutation potions, 6 remove curse scrolls, a pile of 800 gold. Amplified version of this
will provide a list of 12 items instead of 4. Player still only chooses 1. Inverted form of this presents one random item to the player, which they get 4 of.
Amplified inverted form gives 12 copies of 1 random item. Xom loves this particular usage of inversion scrolls...

## Remove some stairs on orb run

For normal mode, one "up" stairs will be taken away once orb is picked up. For hard mode, two up stairs will be removed. One way upstairs are left in place,
still providing more than one way up.

## Each rune picked up makes the game harder

Each rune has a specific curse associated with it which makes the game more challenging. Ascending with the orb with 8 runes is dramatically more difficult than
ascending with 3, because of the additional 5 game altering curses. A 15 rune ascension would be nearly impossible on hard mode. Should be like a once a year
event that someone actually pulls off a 15 rune ascension under this change in hard mode. Even in easy mode, a 15 rune ascension should be impossible except for
the most elite of players. 

This increase of difficulty is necessary to compensate for some of these other changes which make the game easier, especially the removal of level caps.

### Rune curse ideas:

* serpentine: stairs randomly teleport nearer monsters ~every 50 turns
* decaying: player rots at a steady rate. 
* barnacled: player movement is slowed down by 0.2
* gossamer: traps more common and more deadly
* abyssal: monster spawning increased to be like abyss
* silver: ascending stairs sometimes moves player to a cluster of monsters on same floor instead
* slimy: player randomly mutates, good or bad 50/50
* dark: player vision is reduced to 3, but can still sense the position of monsters in normal LOS, just can't id them until they are closer. 
* glowing: contamination doesn't auto decay. Can be cured by drinking any potion. 
* fiery: monster damage increased by 25%
* magical: power of monster spells increased
* demonic: when a hit does a lot of damage to a monster, there is a chance that a demon will spawn. 
* golden: gold in los has a chance to distract player, causing their attack to miss, or their spell to fail. Higher chance of gold being dropped by monsters. 
* iron: walls are all indestructable, but paths to stairs are always guaranteed. 
* icy: ice walls or tunnels randomly appear. Can be melted with heat. Never close to player. 
* obsidian: walls and trees adjacent to the player occasionally come to life and attack the player, but don't move. When the player moves away from a living
* wall, it eventually fades back into a normal wall. 
* bone: when a monster dies, there is a 1/3 chance it comes back as a zombie or demon

*************************************
# Version 1.5
*************************************

## Spell casting time

Time it takes to cast a spell should vary by spellcasting skill from 1.5 (at 0) to 0.5 (at 20).

## Magic costs no longer are fixed for spells

Spell magic costs will be changed to be geometric instead of linear

    Level 1 spell: 10 
    Level 2 spell: 16
    Level 3 spell: 25
    Level 4 spell: 40
    ...
    Level 8 spell: 254
    Level 9 spell: 403

Of course players would have 10 times more magic capacity in this new system, so 403 points here would be the same as 40 points in the old system. Then they
would be adjusted according to spellcasting skill. 0 spellcasting skill would require the base cost of a spell. 

Cost of a level 9 spell at
 
    0 spellcasting: 403
    1 spellcasting: 376
    2 spellcasting: 351
    5 spellcasting: 285
    10 spellcasting: 202
    15 spellcasting: 143
    20 spellcasting: 101
    25 spellcasting: 71
    30 spellcasting: 50

This also allows us to more clearly vary costs for different kinds of spells. Currently transmutation spells cost 3 times as much to cast, because they last
indefinitely, so after this change is made, the different spell costs will show up in the spell list menu. 

## No more level 27 caps

Level max is 99 instead of 27, which will be impossible to ever attain. But you might, in some extended game cases get a few levels past 27. I suspect that the
reasons for the level 27 cap can be fixed in a better way. This change will allow me to test this theory. I think part of it is to discourage long term grinding
to get higher levels or points. If other changes mentioned in this doc are made (especially removing experience from kills), and points are based on how many
runes you have on not on kills or XL, and especially if you are rewarded for completing it more quickly (which I think is the way crawl is already), there will
be no incentive for grinding.  

This level 99 cap would apply to XL and skill levels. It is not expected that people would get past 30 in most cases, but no longer do you lose all value in
progression. A great deal of the fun in these games comes from improving your character through extending their attributes in meaningful ways, and the level 27
cap just cuts out that fun in late game. 

## Double sided branches

* if it is a rune branch, the rune is in the middle floor of that branch.
* there is an entrance / exit at the bottom of the branch in addition to the one at the top. Always two ways in
  and out of a branch.
* bottom half of the branch is an optional challenge area for those who are having too easy of a time. 
* branches may be the same length (which would significantly shorten the game, since you are only required 
  to travel half the distance to get the rune) or extended a little. We'll experiment and find what's most fun.

*************************************
# Version 1.6
*************************************

## Altar destruction

Destroying an altar will bring that god's wrath down upon you, but in exchange, you gain an immediate boost of experience. For those of you that find hard mode
too easy... 

## Ghost drops

Upon killing a ghost, it drops some of the items it was carrying when it died, randomly chosen. Ghosts would be more difficult to kill. I think this would pose
an interesting challenge for those brave enough to go ghost hunting. Ghosts would appear much more rarely, most commonly around "haunted" areas, such as special
vaults or the tomb and crypt. But it would be more common to come across grave stones which can be destroyed to summon a ghost. 

## Deck improvements

Picking up another deck of the same type just adds cards to an existing deck if you already have one. 

## Eliminate door dancing

If a monster is adjacent to a door, the player can't close it. Problem solved. No more opening and closing doors until the player heals.

## Haste potion, wand of hasting

Feels like a debuff here would be nice, and add a little more flavor. These effectively replace the hasting spell, but I'd rather distinguish them more. Haste
potion will improve all movement speeds, but will reduce accuracy and spell potency a little. It's inverted form slows you down a bit, but in exchange you have
higher accuracy and spell potency. 

The wand will only affect movement speed. Attack speed will be normal. 

## Make abyss more of a challenge for late game players

Possibly add more depth to the abyss (is it infinite anyway?), with harder challenges the deeper you go, so that being cast into the abyss late game is actually
pretty scary still. 

*************************************
# Version 1.7
*************************************

## Merge rods into wands

There may be an important reason for rods that I'm not getting, but at least from my limited experience with them, they seem to be just like wands, basically
giving the player the ability to cast spells without spell skills or magic required, except that, unlike wands, they charge back up. 

Why not just have all wands have a charging rate, which is usually 0 (meaning they don't charge up on their own at all), but in some rare cases you find a wand
that has a charge rate of +1 or even +2 (maybe one out of 20 or even 50 chance)? A recharge scroll used on a self-charging wand would increase it's charging
rate, instead of actually recharging it. 

But on normal wands the recharge scroll behaves normally. Of course, they should charge up as a player gets experience, so players aren't encouraged to just sit
around between battles until they charge up. You would find self-charging wands much more often among the more common wands, like the flame, etc, but only in
extremely rare cases (god gifts, troves, boss drops) would you find a self charging wand of teleportation or hasting, and then probably only one in 10 games.
Some wands maybe would never appear as a self charging wand, such as wands of healing. 

## Merge evocables into wands or scrolls (not sure about this one yet)

Is there really a point in having a stone of tremors instead of just a wand or scroll of tremors? Ok so it add a tiny bit of flavor, but I'm not sure it's worth
the complexity cost of multiple oddities that a new player must figure out how to use. If they were just scrolls, the new player would know that they were a one
time use item that does what it says. When I pick up a sack of spiders, I have no idea if it is a one-time-use item, or if it recharges and I can use it
multiple times. Even decks of cards are functionally the same as random effect wands. You could still use Nemelex's abilities to identify what the next few wand
zaps will do, and there can still be different sets of random functions just like different decks. 

## Xom changes

Change him to have piety like other gods. Piety starts half filled. No selectable abilities, just like normal, but there are higher chances the random things he
does will benefit you at higher piety (like normal). Gaining piety is based on doing actions that are interesting (exploring new territory, taking on powerful
monsters, getting unintentionally banished). Scumming type activities will reduce piety quickly. Playing it safe too much will also end up with lower piety
levels, which will cause Xom to make things more interesting for you. He's nice like that. 

*************************************
# Version 1.8
*************************************

## Experience rework

Because a player primarily gains experience through killing monsters, there is always the fear of grinding to get to higher experience levels, and a lot of
limitations have been added to crawl in an attempt to mitigate this. So no longer will players get experience from kills. Experience will come from reaching new
levels and branches in the dungeon, and from quaffing potions and fountains of experience (which will be more common than they are now). Potions of experience
will give experience based on the difficulty of the branch level they drink it on, encouraging delaying consuming the potion for as long as possible, creating a
natural challenge as a player rushes to get as deep as possible with the lowest amount of experience. Fountains of experience will be fixed in place, however,
so a player should just drink those at the first opportunity. No longer do we have to worry about players hanging around in a level for too long, trying to get
more experience. Of course potions of experience are generated once when the level is generated, and are never dropped by monsters, so there is no chance of
grinding monsters for experience. 

Piety for some gods will still come from killing monsters, but for others killing monsters will be completely unnecessary. Conceivably, there could be
competitions to see who can beat the game with the least number of kills. Some gods could even get more interesting by reducing piety for killing monsters (Ely
maybe). The longer you go between killing monsters for some, the higher your piety gain. 

## Geometric scaling for most attributes and skills

I'm thinking of something like this: 6 strength should have twice the impact on how much damage you do with a weapon as 3 strength. 9 strength = twice what 6
strength would do. 12 strength would then be 8 times the effect of having 3 strength. 24 strength would have 128 times the damage of 3 strength. This eliminates
any diminishing returns for boosting those attributes. Gaining strength / dex / int, will always matter in a significant way. The same applies to skill levels.
Now gaining skills will still have a diminishing return as it will cost exponentially more experience for each new level. 

So to clarify further: 

for simplicity, say damage caused by a particular spell = Fire Magic power

    Fire Magic -> Fire Magic Power = Damage caused by spell
    0 -> 1.0
    1 -> 1.2
    2 -> 1.6
    3 -> 2.0
    4 -> 2.5
    5 -> 3.2
    6 -> 4.0
    7 -> 5.0
    8 -> 6.3
    9 -> 8
    10 -> 10
    ...
    15 -> 32
    ...
    20 -> 102
    21 -> 128
    22 -> 161
    23 -> 203
    24 -> 256
    25 -> 323
    26 -> 406
    27 -> 512

Every level up kicks up the damage 26%, with no diminishing return. Even late in the game, each level up is meaningful.  

Most attributes and skills I think would follow this 2^(x/3) pattern. Spell levels however, should translate into a steeper scale, maybe a 4^x kind.

    Spell level -> Spell difficulty
    1 -> 4
    2 -> 16
    3 -> 64
    ...
    9 -> 262144

Chance of casting it successfully would be (spell power)/(spell power + spell difficulty). Say spell power was skill power * int power

    Skill, Intelligence -> success rate for level 4 spell
    0, 0 -> 1.2%
    0, 2 -> 1.9%
    0, 5 -> 3.7%
    0, 10 -> 11%
    2, 10 -> 16%
    5, 10 -> 28%
    10, 10 -> 55%
    15, 10 -> 79%
    20, 10 -> 92%
    25, 10 -> 97%

    Skill, Int -> success rate for level 9 spell
    10, 10 -> 0.5%
    15, 10 -> 1.6%
    15, 15 -> 4.9%
    20, 20 -> 34%
    21, 20 -> 39%
    21, 21 -> 45%
    25, 25 -> 84%
    25, 26 -> 86%
    27, 27 -> 93%

Notice how at any position along the scale, incrementing the factors by one or two still made a noticeable difference in the outcome. 

## Traps enhancement

For me I pretty much ignore traps, almost all of the time. They just don't impact my game very much. There's got to be something that can be done to either make
traps more interesting and meaningful, or remove them entirely. 

Teleportation traps are useful and interesting, as well as shaft traps. So they can stay as is. I'm not sure about the rest though...

## Creature changes

Dragons ought to be harder than they are. A single dragon, even one of the weaker variety, ought to be a couple times more deadly than a hydra. 

## New Species

A slime species and a fairy species would be nice. I would like an even 30 species. Any species needed beyond that should replace the most boring species.

*************************************
# Version 1.9
*************************************

## Score changes

Scoring should be based purely on runes obtained and difficulty level, not on speed or kills or anything else. Since each additional rune picked up makes the
orb ascension more difficult, it means much more for a player to beat the game with 5 runes instead of 3. Each player will have a total score, based on the
highest score of each species played. So to have the highest possible score, a player must win 15 runes with each of the species on hard mode. The game will be
calibrated to the point where a 15 rune win with even the easiest species on hard mode would be a legendary feat that few would accomplish. And to be able to do
it with all species should be something people spend years trying to achieve. 

Winning with 12 runes on easy mode would give the same score (and should be roughly as difficult) as winning with 3 runes on normal mode. Winning with 12 runes
on normal mode should be as difficult as winning with 3 runes on hard mode, and the score will reflect this. 

So a 3 rune win on easy mode would produce 3 points for the player. Then if with a different species they do a 3 rune win on normal mode, that win produces 3 x
4 = 12 points, which is added to the 3 points of their easy mode win, giving that player a total of 15 points. A 3 rune hard mode win would give 48 points for
each species. A 15 rune hard mode win would give 15 x 4 x 4 = 240 points. Maximum possible score then would be 30 species x 15 runes x 4 x 4 = 7200 points. 

Games that are too old (played on too different of a game version, where the rules were significantly different, and so can't fairly be compared to modern
games), would expire, and the win would have to be achieved again to count. Maybe somewhere between 1 and 3 years?

Streaks and speed runs would have their own scoreboards, and be only scored and compared to others in the same category. Maybe to facilitate this, the game
itself can award multiple scores, one for getting runes, and one for speed runs. Streaks can just be pulled from a database. 

## Variable spell power

Spells no longer have levels associated with them. A player can cast a spell at whatever level they want. This would definitely add to the complexity
of the game. I'm ok with adding complexity if it really adds to the depth and replayability, with minimal added annoyance.

A player would type z, the spell letter, and then the power level, 1-9. One extra keystroke is required per cast, which can be made into a macro of course for the
more common spells. As some of these other proposals get freed up, more keys will be freed up for macros, which will be essential to minimize the tedium
of this particular proposal. 

Both spell power and cost of the spell will change based on the power selected. The advantage of this is that several spells that are really just different
power levels of the same idea can be collapsed into a single spell. But it could add incredible depth to existing spells, enriching the game in interesting
ways. For example, a level 1 shatter could just shake up any monsters adjacent to the caster, causing a small amount of damage to each. A level 9 version of
force lance becomes tornado. Level 5 version of shock is a lightning bolt. Level 1 version of fireball is flaming tongue. Level 9 version of fireball is
firestorm. 

This does take away from the aspect of trying to find high level spells. There would be less spells overall, and spellbooks would probably have 1 fewer spell
than before, but the spells inside would be more versatile. Maybe when you start with a spell, say flame tongue, you can only cast up to level 3 of it, without
learning more about it from a spellbook. Spellbooks can contain beginner, intermediate, or advanced versions of any given spell. Learning an advanced version of
a spell automatically gives lower level knowledge. Intermediate spells can cast levels 4-6 of the spell, while advanced can cast 7-9. 

Maybe not all spells would have all 3 classes, but it would be simpler if they did. Some spells would only have levels 4-9, for example, and some may only have
1-5. This adds more complexity, and it would be nice to avoid this if possible, and just allow 9 levels for every spell.

Example spells:

    Level
    1-3     Blink                   Conjure Flame   Force Lance     Freeze          Swiftness       
    4-6     Semicontrolled Blink    Fire Ball       Air Strike      Freezing Cloud  Haste           
    7-9     Controlled Blink        Fire Storm      Tornado         Glaciate        Phase Shift     

    Level
    1-3     Call Imp                Spider Form     Sting           Inner Flame
    4-6     Summon Demon            Hydra Form      Venom Bolt      Ignite Poison
    7-9     Summon Greater Demon    Dragon Form     Poison Arrow    

    Level
    1-3     Summon Butterflies          Sticks to Snakes        Stoneskin       Beastly Appendage   
    4-6     Monstrous Menagerie         Sticks to Hydra         Statue Form     Blade Hands         
    7-9     Summon Horrible Things      Sticks to Dragons       Passwall        Song of Slaying     

    Level
    1-3     Infusion                Confuse             Animate Skeleton    Zombie Form         Sneak
    4-6     Spectral Weapon         Mass Confusion      Animate Dead        Wraith Form         Hide
    7-9     Tukima's Dance          Discord             Resurrection        Necromutation       Invisibility

    Level
    1-3     Mephitic Cloud          Metabolic Englaciation      Summon Jackal
    4-6     Poison Cloud            Ensorcelled Hibernation     Summon Warg
    7-9                             Freezing Paralysis          Summon Hellhound

Notes: 

Semicontrolled blink allows the player to choose the direction of the blink, but not the specific location.

Phase Shift is a much more powerful version. Swiftness increases movement speed, Haste adds to that attack speed, Phase Shift adds to that change
to avoid all attacks, even normally undodgable ones (like lightning).

Passwall includes the benefits of stone form, plus the ability to merge with walls, walking right through them. Movement is very slow through walls
of course, but can provide ridiculously powerful escape options. 

Zombie form is basically a weaker version of the player, with half health, and vulnerability to dispel undead, holy word, etc., but with: rC+ rN+ rP+ rRot

Wraith form is full health of the player, with the attributes of Zombie form, adding: rC+ rN+ rMut rConstr

Necromutation is everything good about wraith plus: rN+ rC+ rTorm MR+50 AC+6 draining branded attacks, no hunger or quaffing, necromancy power boost

Sneak increases stealth significantly

Hide makes a player undetectable (if they haven't been seen yet) if they are next to a wall, until they move again. This lets them rest for free safely.

Invisibility is more powerful than normal invisibility, since it auto hides when stopped next to a wall. Higher power levels makes it harder even for monsters
with see invisible to detect the player.

## Elemental distinction

(Some of this crawl already has, it's just all laid out in this form for completeness)

I'd like to see more distinct differences between the elemental magics. 

Electric attacks should not be dodgeable, and can penetrate multiple enemies and bounce off walls. Although it is logical that electric attacks would also
bypass armour, that is too much. Rarely do monsters resist this.

Ice attacks should bypass armour and have a chance of slowing the attacker (any attacker not cold resistant, not just reptiles). Monsters are often resistant to
this.

Poison attacks are all about lingering damage, or confusing effects. Monsters are often resistant to this.

Acid attacks are about degrading armour or weapon effectiveness. Rarely do monsters resist this.

Earth attacks should be good against heavy armour, with slow, powerful blows, although they are more easily dodged, have a chance to stun the target, and are
not resistible. 

Fire damage should be distinct from these others. It can be resisted by armour, can be dodged, and elemental resistance to it isn't hard to come by. Maybe it
just does more immediate damage than the other elements. It should have a lingering effect on others. Maybe it's just better at area effects. If an enemy is 
damaged with fire, there's a chance it could also damage a nearby enemy (but not going any further, unlike chain lightning). Even simple fire bolts should have
a little explosion when they hit, possibly damaging nearby enemies. 

## More intuitive auto-pickup mechanism

The current auto-pickup menu and options in the rc file are both not powerful enough to avoid some awkwardness and inconvenience, and also somewhat hard to
understand and use for new players. The problem is that what I want to autopickup varies based on my character combo and the stage of the game I'm in at the
time. So I pull up the autopickup menu a few times to set things up. That part isn't too bad, but what if I want to pick up any long swords, but leave behind
other weapons? What if I'm okay with picking up cursed weapons, or what if I'm not? There is no way to set that now, so I have to either pick up all weapons, or
manually move over to any weapons and pick them up, which causes me to often miss items of value in some cases. Or what if you only want to pick up artifact
armour, ignoring the rest?

Maybe instead of the configuration menu, the game can just intelligently respond to what the player does. At the beginning, the autopickup system picks
everything up. The player decides that he doesn't have any use for maces, so he drops the common mace that he has. That is automatically removed from the
auto-pickup list. But branded and unique maces will still be picked up. If he then drops a mace of flaming, all maces of flaming are no longer on the
auto-pickup list, but artifacts will still be picked up. Since all artifacts are unique, dropping an artifact will only take that exact one off of the list,
other artifacts will still be picked up. Now if you drop an unidentified rare mace, that means you don't want any maces, so none will be picked up in the
future. Then if you go to manually pick up a mace again, that will change your auto-pickup options too.

This would probably just be an option: adaptive-autopickup = true  or something like that.

