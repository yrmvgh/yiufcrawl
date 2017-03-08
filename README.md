# Yiuf's Home for Lost and Forgotten Species
## A trunk-based fork of Stone Soup with leftover soup
### Exterminated races, expired experiments, and extinct species
    download instructions:
    git clone https://github.com/yrmvgh/crawl.git yiufcrawl
    cd yiufcrawl/crawl-ref/source
    make TILES=y
    then just run the file named "yiufcrawl"

alternately "make TILES=y install" will try to install yiufcrawl to your system
for super- "git clone https://github.com/yrmvgh/crawl.git -b yiuf yiufcrawl"

for further help, check https://github.com/yrmvgh/crawl/blob/master/crawl-ref/INSTALL.txt

You can clone or checkout the "yiuf" branch if you want to see changes and new content *slightly* faster. "master" gets slightly more testing and is slightly behind "yiuf".

--------------------------------
> #### *Yiuf knows how it feels to be unloved. To be forgotten. Yiuf will make some space*
> #### * in his hut for other sad creatures and they will be a little family in the dark.*

### *They're Back:*

* **High Elf**, now with keen eyes, a Tolkien-friendly bonus to noticing traps.
* **Sludge Elf**, now with an evolving mutagenic body and orcish blood. Great at throwing.
* **Dorf**, your basic Dwarf, based loosely on Hill & Mountain dwarves. High MR and rN to keep your life simple.
* **Lava Orc**, as their heat increases, they get a fire/earth magic boost and a damage aura.
* **Imp**, a short-lived experimental race. Their undead-like resistances have been simplified to just rN+.
* **Plutonian**, an unstable, contaminated humanoid that occasionally explodes. !Curing and kills reduce contam.
*They're New:*
* **Skeleton**, murderous animated skeleton seeks revenge against all living creatures. ObsidianAxe-like.

Image album of the first five races above: http://imgur.com/a/J1XBP

All species are subject to having their aptitudes shuffled around, to reduce species overlap. Don't get too attached to apts being one particular number or another, aptitudes aren't very important in the long run.

[Tavern thread for this fork](https://crawl.develz.org/tavern/viewtopic.php?f=17&t=23209)

### *Future plans:*
Try to collect most of the historical crawl races, including both removed and experimental ones. Polish them up, give them a new home. Next on the horizon: change lava orcs to be based on damage, not tension, and add ~~plutonians~~ (done!) ~~and imps~~ (done!) to the game.

To further differentiate the elven subraces, deep elves will probably get a noise attenuation modifier that makes everything they do quieter.

Dorfs are just an apt race at this point. high MR and HP, like DD. I didn't have any bright ideas for a cool mut, yet. I'll leave that up to Tavern/Reddit. Job 1 was making dorfs/surface dwarves exist, not making them great or special or worthy of mainline crawl. If you miss Hill Dwarves or Mountain Dwarves, here's yer Dorf! Suggestions on how to make them special are welcome.

### *Minor "personal preference" changes:*
* Ogres have 1 more hp_mod, trolls have 1 less.
* Club, scythe, and GSC are a bit faster now. (stealth ogre buff?)
* Formicids can't fall in shafts by accident.
* Items don't block doorways.
* Berserkers start with no weapons, and no weapon skill.
* Scarves remain unenchantable, but have 1 base AC.
* Roles with ranged weapons start with more ammo.

#### *Below you will find the contents of the original DCSS readme.*
#### *Some of this information no longer applies, but most of it does!*
[![Build Status](https://travis-ci.org/yrmvgh/crawl.svg?branch=master)](https://travis-ci.org/yrmvgh/crawl)
--------------------------------
## Contents

1. [How to Play](#how-to-play)
3. [Community](#community)
5. [How you can help](#how-you-can-help)
4. [License and history information](#license-and-history-information)

## How to Play

If you'd like to dive in immediately, we suggest one of:

* Start a game and pick a tutorial (select tutorial in the game menu),
* Read [quickstart.txt](crawl-ref/docs/quickstart.txt) (in the [docs/](crawl-ref/docs/) directory), or
* For the studious, [read Crawl's full manual](crawl-ref/docs/crawl_manual.rst).

There is also an ingame list of frequently asked questions which you can access by typing
`?Q`.

#### Internet Play

You can play Crawl online, competing with other players or watching them. Click "Play Online Now!" on [the Crawl homepage](https://crawl.develz.org/) to find your closest server. You can play in your browser or over SSH.

#### Offline Play

Both classical ASCII and tiles (GUI) versions of Crawl are available to [download for Linux, Windows and OS X](https://crawl.develz.org/download.htm).

## Community

### Web
Our official homepage is [https://crawl.develz.org](https://crawl.develz.org/). You can find online play, offline downloads, a community forum and the development bug tracker/wiki.

### IRC
Chat with fellow crawlers in [##crawl on irc.freenode.net](https://webchat.freenode.net/?channels=##crawl), or talk development in [##crawl-dev](https://webchat.freenode.net/?channels=##crawl-dev).

## How you can help

If you like the game and you want to help make it better, there are a number
of ways to do so:

### Reporting bugs

At any time, there will be bugs -- finding and reporting them is a great help.
Many of the online servers host the regularly updated development version. Bugs
should be reported to [our bug tracker](https://crawl.develz.org/mantis/). Besides pointing out bugs, new ideas on how to improve interface or gameplay are welcome. These can be added to [the development wiki](https://crawl.develz.org/wiki/).

### Map making
Crawl creates levels by combining many hand-made (but often randomised) maps, known as *vaults*. Making them is fun and
easy. It's best to start with simple entry vaults: see [simple.des](crawl-ref/source/dat/des/arrival/simple.des) for examples. You can also read [the level-design manual](crawl-ref/docs/develop/levels/introduction.txt) for more help.

If you're ambitious, you can create new vaults for anywhere in the game. If you've
made some vaults, you can test them on your own system (no compiling needed) and
submit them to [our bug tracker](https://crawl.develz.org/mantis/).

### Monster Speech & Item Descriptions
Monster speech provides a lot of flavour. Just like vaults, varied speech depends
upon a large set of entries. Speech syntax is effective but unusual, so you may want to read [the formatting guide](crawl-ref/docs/develop/monster_speech.txt).

Current item descriptions can be read in-game with `?/` or out-of-game
them in [dat/descript/](crawl-ref/source/dat/descript/). The following conventions should be more or less obeyed:
* Descriptions ought to contain flavour text, ideally pointing out major weaknesses/strengths.
* No numbers, please.
* Citations are okay, but try to stay away from the most generic ones.

### Tiles
We're always open to improvements to existing tiles or variants of often-used tiles (eg floor tiles). If you want to give this a shot, please [contact us](#community) via forums or IRC. In case you drew some tiles of your own, you can submit them to [our bug tracker](https://crawl.develz.org/mantis/).

### Patches
For developers (both existing & aspiring!), you can download/fork the source code and write patches. Bug fixes as well as new features are very much welcome.

For large changes, it's always a good idea to [talk with the dev team](#community) first, to see if any plans already exist and if your suggestion is likely to be accepted.

Please be sure to read [docs/develop/coding_conventions.txt](crawl-ref/docs/develop/coding_conventions.txt) too.

## License and history information

Crawl is licensed as GPLv2+. See [licence.txt](crawl-ref/licence.txt) for the full text.

Crawl is a descendant of Linley's Dungeon Crawl. Development of the main branch stalled at version 4.0.0b26, with a final alpha of 4.1 being released by Brent Ross in 2005. Since 2006, the Dungeon Crawl Stone Soup team has continued development. [CREDITS.txt](crawl-ref/CREDITS.txt) contains a full list of contributors.

Crawl gladly uses the following open source packages; thanks to their developers:

* The Lua scripting language, for in-game functionality and user macros ([license](crawl-ref/docs/license/lualicense.txt)).
* The PCRE library, for regular expressions ([license](crawl-ref/docs/license/pcre_license.txt)).
* The SQLite library, as a database engine ([license](https://www.sqlite.org/copyright.html)).
* The SDL and SDL_image libraries, for tiles display ([license](crawl-ref/docs/license/lgpl.txt)).
* The libpng library, for tiles image loading ([license](crawl-ref/docs/license/libpng-LICENSE.txt)).

Thank you, and have fun crawling!
