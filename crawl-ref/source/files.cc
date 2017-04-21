/**
 * @file
 * @brief Functions used to save and load levels/games.
**/

#include "AppHdr.h"

#include "files.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef HAVE_UTIMES
#include <sys/time.h>
#endif
#include <sys/types.h>
#ifdef UNIX
#include <unistd.h>
#endif

#include "abyss.h"
#include "act-iter.h"
#include "areas.h"
#include "branch.h"
#include "butcher.h" // for fedhas_rot_all_corpses
#include "chardump.h"
#include "cloud.h"
#include "coordit.h"
#include "dactions.h"
#include "dgn-overview.h"
#include "directn.h"
#include "dungeon.h"
#include "end.h"
#include "errors.h"
#include "fineff.h"
#include "ghost.h"
#include "god-abil.h"
#include "god-conduct.h" // for fedhas_rot_all_corpses
#include "god-companions.h"
#include "god-passive.h"
#include "hints.h"
#include "initfile.h"
#include "items.h"
#include "jobs.h"
#include "kills.h"
#include "level-state-type.h"
#include "libutil.h"
#include "macro.h"
#include "mapmark.h"
#include "message.h"
#include "mon-behv.h"
#include "mon-death.h"
#include "mon-place.h"
#include "notes.h"
#include "output.h"
#include "place.h"
#include "prompt.h"
#include "spl-summoning.h"
#include "stash.h"  // for fedhas_rot_all_corpses
#include "state.h"
#include "stringutil.h"
#include "syscalls.h"
#include "teleport.h"
#include "terrain.h"
#ifdef USE_TILE
 // TODO -- dolls
 #include "tiledef-player.h"
 #include "tilepick-p.h"
#endif
#include "tileview.h"
#include "timed-effects.h"
#include "unwind.h"
#include "version.h"
#include "view.h"
#include "xom.h"

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifndef F_OK // MSVC for example
#define F_OK 0
#endif

static void _save_level(const level_id& lid);

static bool _ghost_version_compatible(reader &ghost_reader);

static bool _restore_tagged_chunk(package *save, const string &name,
                                  tag_type tag, const char* complaint);
static bool _read_char_chunk(package *save);

const short GHOST_SIGNATURE = short(0xDC55);

const int GHOST_LIMIT = 27; // max number of ghost files per level

static void _redraw_all()
{
    you.redraw_hit_points    = true;
    you.redraw_magic_points  = true;
    you.redraw_stats.init(true);
    you.redraw_armour_class  = true;
    you.redraw_evasion       = true;
    you.redraw_experience    = true;
    you.redraw_status_lights = true;
}

static bool is_save_file_name(const string &name)
{
    int off = name.length() - strlen(SAVE_SUFFIX);
    if (off <= 0)
        return false;
    return !strcasecmp(name.c_str() + off, SAVE_SUFFIX);
}

// Returns the save_info from the save.
static player_save_info _read_character_info(package *save)
{
    player_save_info fromfile;

    // Backup before we clobber "you".
    const player backup(you);
    unwind_var<game_type> gtype(crawl_state.type);

    try // need a redundant try block just so we can restore the backup
    {   // (or risk an = operator on you getting misused)
        fromfile.save_loadable = _read_char_chunk(save);
        fromfile = you;
    }
    catch (ext_fail_exception &E) {}

    you = backup;

    return fromfile;
}

// Returns a vector of files (including directories if requested) in
// the given directory, recursively. All filenames returned are
// relative to the start directory. If an extension is supplied, all
// filenames (and directory names if include_directories is set)
// returned must be suffixed with the extension (the extension is not
// modified in any way, so if you want, say, ".des", you must include
// the "." as well).
//
// If recursion_depth is -1, the recursion is infinite, as far as the
// directory structure and filesystem allows. If recursion_depth is 0,
// only files in the start directory are returned.
vector<string> get_dir_files_recursive(const string &dirname, const string &ext,
                                       int recursion_depth,
                                       bool include_directories)
{
    vector<string> files;

    const int next_recur_depth =
        recursion_depth == -1? -1 : recursion_depth - 1;
    const bool recur = recursion_depth == -1 || recursion_depth > 0;

    for (const string &filename : get_dir_files(dirname))
    {
        if (dir_exists(catpath(dirname, filename)))
        {
            if (include_directories
                && (ext.empty() || ends_with(filename, ext)))
            {
                files.push_back(filename);
            }

            if (recur)
            {
                // Each filename in a subdirectory has to be prefixed
                // with the subdirectory name.
                for (const string &subdirfile
                        : get_dir_files_recursive(catpath(dirname, filename),
                                                  ext, next_recur_depth))
                {
                    files.push_back(catpath(filename, subdirfile));
                }
            }
        }
        else
        {
            if (ext.empty() || ends_with(filename, ext))
                files.push_back(filename);
        }
    }
    return files;
}

vector<string> get_dir_files_ext(const string &dir, const string &ext)
{
    return get_dir_files_recursive(dir, ext, 0);
}

string get_parent_directory(const string &filename)
{
    string::size_type pos = filename.rfind(FILE_SEPARATOR);
    if (pos != string::npos)
        return filename.substr(0, pos + 1);
#ifdef ALT_FILE_SEPARATOR
    pos = filename.rfind(ALT_FILE_SEPARATOR);
    if (pos != string::npos)
        return filename.substr(0, pos + 1);
#endif
    return "";
}

string get_base_filename(const string &filename)
{
    string::size_type pos = filename.rfind(FILE_SEPARATOR);
    if (pos != string::npos)
        return filename.substr(pos + 1);
#ifdef ALT_FILE_SEPARATOR
    pos = filename.rfind(ALT_FILE_SEPARATOR);
    if (pos != string::npos)
        return filename.substr(pos + 1);
#endif
    return filename;
}

string get_cache_name(const string &filename)
{
    string::size_type pos = filename.rfind(FILE_SEPARATOR);
    while (pos != string::npos && filename.find("/des", pos) != pos)
        pos = filename.rfind(FILE_SEPARATOR, pos - 1);
    if (pos != string::npos)
        return replace_all_of(filename.substr(pos + 5), " /\\:", "_");
#ifdef ALT_FILE_SEPARATOR
    pos = filename.rfind(ALT_FILE_SEPARATOR);
    while (pos != string::npos && filename.find("/des", pos) != pos)
        pos = filename.rfind(ALT_FILE_SEPARATOR, pos - 1);
    if (pos != string::npos)
        return replace_all_of(filename.substr(pos + 5), " /\\:", "_");
#endif
    return filename;
}

bool is_absolute_path(const string &path)
{
    return !path.empty()
           && (path[0] == FILE_SEPARATOR
#ifdef TARGET_OS_WINDOWS
               || path.find(':') != string::npos
#endif
             );
}

// Concatenates two paths, separating them with FILE_SEPARATOR if necessary.
// Assumes that the second path is not absolute.
//
// If the first path is empty, returns the second unchanged. The second path
// may be absolute in this case.
string catpath(const string &first, const string &second)
{
    if (first.empty())
        return second;

    string directory = first;
    if (directory[directory.length() - 1] != FILE_SEPARATOR
        && (second.empty() || second[0] != FILE_SEPARATOR))
    {
        directory += FILE_SEPARATOR;
    }
    directory += second;

    return directory;
}

// Given a relative path and a reference file name, returns the relative path
// suffixed to the directory containing the reference file name. Assumes that
// the second path is not absolute.
string get_path_relative_to(const string &referencefile,
                            const string &relativepath)
{
    return catpath(get_parent_directory(referencefile),
                   relativepath);
}

string change_file_extension(const string &filename, const string &ext)
{
    const string::size_type pos = filename.rfind('.');
    return (pos == string::npos? filename : filename.substr(0, pos)) + ext;
}

time_t file_modtime(const string &file)
{
    struct stat filestat;
    if (stat(file.c_str(), &filestat))
        return 0;

    return filestat.st_mtime;
}

time_t file_modtime(FILE *f)
{
    struct stat filestat;
    if (fstat(fileno(f), &filestat))
        return 0;

    return filestat.st_mtime;
}

static bool _create_directory(const char *dir)
{
    if (!mkdir_u(dir, 0755))
        return true;
    if (errno == EEXIST) // might be not a directory
        return dir_exists(dir);
    return false;
}

static bool _create_dirs(const string &dir)
{
    string sep = " ";
    sep[0] = FILE_SEPARATOR;
    vector<string> segments = split_string(sep, dir, false, false);

    string path;
    for (int i = 0, size = segments.size(); i < size; ++i)
    {
        path += segments[i];

        // Handle absolute paths correctly.
        if (i == 0 && dir.size() && dir[0] == FILE_SEPARATOR)
            path = FILE_SEPARATOR + path;

        if (!_create_directory(path.c_str()))
            return false;

        path += FILE_SEPARATOR;
    }
    return true;
}

// Checks whether the given path is safe to read from. A path is safe if:
// 1. If Unix: It contains no shell metacharacters.
// 2. If DATA_DIR_PATH is set: the path is not an absolute path.
// 3. If DATA_DIR_PATH is set: the path contains no ".." sequence.
void assert_read_safe_path(const string &path)
{
    // Check for rank tomfoolery first:
    if (path.empty())
        throw unsafe_path("Empty file name.");

#ifdef UNIX
    if (!shell_safe(path.c_str()))
        throw unsafe_path_f("\"%s\" contains bad characters.", path.c_str());
#endif

#ifdef DATA_DIR_PATH
    if (is_absolute_path(path))
        throw unsafe_path_f("\"%s\" is an absolute path.", path.c_str());

    if (path.find("..") != string::npos)
        throw unsafe_path_f("\"%s\" contains \"..\" sequences.", path.c_str());
#endif

    // Path is okay.
}

string canonicalise_file_separator(const string &path)
{
    const string sep(1, FILE_SEPARATOR);
    return replace_all_of(replace_all_of(path, "/", sep),
                          "\\", sep);
}

static vector<string> _get_base_dirs()
{
    const string rawbases[] =
    {
#ifdef DATA_DIR_PATH
        DATA_DIR_PATH,
#else
        !SysEnv.crawl_dir.empty()? SysEnv.crawl_dir : "",
        !SysEnv.crawl_base.empty()? SysEnv.crawl_base : "",
#endif
#ifdef TARGET_OS_MACOSX
        SysEnv.crawl_base + "../Resources/",
#endif
#ifdef __ANDROID__
        ANDROID_ASSETS,
        "/sdcard/Android/data/org.develz.crawl/files/",
#endif
    };

    const string prefixes[] =
    {
        string("dat") + FILE_SEPARATOR,
#ifdef USE_TILE_LOCAL
        string("dat/tiles") + FILE_SEPARATOR,
#endif
        string("docs") + FILE_SEPARATOR,
        string("settings") + FILE_SEPARATOR,
#ifndef DATA_DIR_PATH
        string("..") + FILE_SEPARATOR + "docs" + FILE_SEPARATOR,
        string("..") + FILE_SEPARATOR + "dat" + FILE_SEPARATOR,
#ifdef USE_TILE_LOCAL
        string("..") + FILE_SEPARATOR + "dat/tiles" + FILE_SEPARATOR,
#endif
        string("..") + FILE_SEPARATOR + "settings" + FILE_SEPARATOR,
        string("..") + FILE_SEPARATOR,
#endif
        "",
    };

    vector<string> bases;
    for (string base : rawbases)
    {
        if (base.empty())
            continue;

        base = canonicalise_file_separator(base);

        if (base[base.length() - 1] != FILE_SEPARATOR)
            base += FILE_SEPARATOR;

        for (unsigned p = 0; p < ARRAYSZ(prefixes); ++p)
            bases.push_back(base + prefixes[p]);
    }

    return bases;
}

string datafile_path(string basename, bool croak_on_fail, bool test_base_path,
                     bool (*thing_exists)(const string&))
{
    basename = canonicalise_file_separator(basename);

    if (test_base_path && thing_exists(basename))
        return basename;

    for (const string &basedir : _get_base_dirs())
    {
        string name = basedir + basename;
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_INFO,"Crawl","Looking for %s as '%s'",basename.c_str(),name.c_str());
#endif
        if (thing_exists(name))
            return name;
    }

    // Die horribly.
    if (croak_on_fail)
    {
        end(1, false, "Cannot find data file '%s' anywhere, aborting\n",
            basename.c_str());
    }

    return "";
}

// Checks if directory 'dir' exists and tries to create it if it
// doesn't exist, modifying 'dir' to its canonical form.
//
// If given an empty 'dir', returns true without modifying 'dir' or
// performing any other checks.
//
// Otherwise, returns true if the directory already exists or was just
// created. 'dir' will be modified to a canonical representation,
// guaranteed to have the file separator appended to it, and with any
// / and \ separators replaced with the one true FILE_SEPARATOR.
//
bool check_mkdir(const string &whatdir, string *dir, bool silent)
{
    if (dir->empty())
        return true;

    *dir = canonicalise_file_separator(*dir);

    // Suffix the separator if necessary
    if ((*dir)[dir->length() - 1] != FILE_SEPARATOR)
        *dir += FILE_SEPARATOR;

    if (!dir_exists(*dir) && !_create_dirs(*dir))
    {
        if (!silent)
        {
#ifdef __ANDROID__
            __android_log_print(ANDROID_LOG_INFO, "Crawl",
                                "%s \"%s\" does not exist and I can't create it.",
                                whatdir.c_str(), dir->c_str());
#endif
            fprintf(stderr, "%s \"%s\" does not exist "
                    "and I can't create it.\n",
                    whatdir.c_str(), dir->c_str());
        }
        return false;
    }

    return true;
}

// Get the directory that contains save files for the current game
// type. This will not be the same as get_base_savedir() for game
// types such as Sprint.
static string _get_savefile_directory()
{
    string dir = catpath(Options.save_dir, crawl_state.game_savedir_path());
    check_mkdir("Save directory", &dir, false);
    if (dir.empty())
        dir = ".";
    return dir;
}


/**
 * Location of legacy ghost files. (The save directory.)
 *
 * @return The path to the directory for old ghost files.
 */
static string _get_old_bonefile_directory()
{
    string dir = catpath(Options.shared_dir, crawl_state.game_savedir_path());
    check_mkdir("Bones directory", &dir, false);
    if (dir.empty())
        dir = ".";
    return dir;
}

/**
 * Location of ghost files.
 *
 * @return The path to the directory for ghost files.
 */
static string _get_bonefile_directory()
{
    string dir = catpath(Options.shared_dir, crawl_state.game_savedir_path());
    dir = catpath(dir, "bones");
    check_mkdir("Bones directory", &dir, false);
    if (dir.empty())
        dir = ".";
    return dir;
}

// Returns a subdirectory of the current savefile directory as returned by
// _get_savefile_directory.
static string _get_savedir_path(const string &shortpath)
{
    return canonicalise_file_separator(
        catpath(_get_savefile_directory(), shortpath));
}

// Returns a subdirectory of the base save directory that contains all saves
// and cache directories. Save files for game type != GAME_TYPE_NORMAL may
// be found in a subdirectory of this dir. Use _get_savefile_directory() if
// you want the directory that contains save games for the current game
// type.
static string _get_base_savedir_path(const string &subpath = "")
{
    return canonicalise_file_separator(catpath(Options.save_dir, subpath));
}

// Given a simple (relative) path, returns the path relative to the
// base save directory and a subdirectory named with the game version.
// This is useful when writing cache files and similar output that
// should not be shared between different game versions.
string savedir_versioned_path(const string &shortpath)
{
#ifdef DGL_VERSIONED_CACHE_DIR
    const string versioned_dir =
        _get_base_savedir_path(string("cache.") + Version::Long);
#else
    const string versioned_dir = _get_base_savedir_path();
#endif
    return catpath(versioned_dir, shortpath);
}

#ifdef USE_TILE
#define LINEMAX 1024
static bool _readln(chunk_reader &rd, char *buf)
{
    for (int space = LINEMAX - 1; space; space--)
    {
        if (!rd.read(buf, 1))
            return false;
        if (*buf == '\n')
            break;
        buf++;
    }
    *buf = 0;
    return true;
}

static void _fill_player_doll(player_save_info &p, package *save)
{
    dolls_data equip_doll;
    for (unsigned int j = 0; j < TILEP_PART_MAX; ++j)
        equip_doll.parts[j] = TILEP_SHOW_EQUIP;

    equip_doll.parts[TILEP_PART_BASE]
        = tilep_species_to_base_tile(p.species, p.experience_level);

    bool success = false;

    chunk_reader fdoll(save, "tdl");
    {
        char fbuf[LINEMAX];
        if (_readln(fdoll,fbuf))
        {
            tilep_scan_parts(fbuf, equip_doll, p.species, p.experience_level);
            tilep_race_default(p.species, p.experience_level, &equip_doll);
            success = true;
        }
    }

    if (!success) // Use default doll instead.
    {
        job_type job = get_job_by_name(p.class_name.c_str());
        if (job == JOB_UNKNOWN)
            job = JOB_FIGHTER;

        tilep_job_default(job, &equip_doll);
    }
    p.doll = equip_doll;
}
#endif

/*
 * Returns a list of the names of characters that are already saved for the
 * current user.
 */

static vector<player_save_info> _find_saved_characters()
{
    vector<player_save_info> chars;

    if (Options.no_save)
        return chars;

#ifndef DISABLE_SAVEGAME_LISTS
    string searchpath = _get_savefile_directory();

    if (searchpath.empty())
        searchpath = ".";

    for (const string &filename : get_dir_files(searchpath))
    {
        if (is_save_file_name(filename))
        {
            try
            {
                package save(_get_savedir_path(filename).c_str(), false);
                player_save_info p = _read_character_info(&save);
                if (!p.name.empty())
                {
                    p.filename = filename;
#ifdef USE_TILE
                    if (Options.tile_menu_icons && save.has_chunk("tdl"))
                        _fill_player_doll(p, &save);
#endif
                    chars.push_back(p);
                }
            }
            catch (ext_fail_exception &E)
            {
                dprf("%s: %s", filename.c_str(), E.what());
            }
        }

    }

    sort(chars.begin(), chars.end());
#endif // !DISABLE_SAVEGAME_LISTS
    return chars;
}

vector<player_save_info> find_all_saved_characters()
{
    set<string> dirs;
    vector<player_save_info> saved_characters;
    for (int i = 0; i < NUM_GAME_TYPE; ++i)
    {
        unwind_var<game_type> gt(
            crawl_state.type,
            static_cast<game_type>(i));

        const string savedir = _get_savefile_directory();
        if (dirs.count(savedir))
            continue;

        dirs.insert(savedir);

        vector<player_save_info> chars_in_dir = _find_saved_characters();
        saved_characters.insert(saved_characters.end(),
                                chars_in_dir.begin(),
                                chars_in_dir.end());
    }
    return saved_characters;
}

bool save_exists(const string& filename)
{
    return file_exists(_get_savefile_directory() + filename);
}

string get_savedir_filename(const string &name)
{
    return _get_savefile_directory() + get_save_filename(name);
}

#define MAX_FILENAME_LENGTH 250
string get_save_filename(const string &name)
{
    return chop_string(strip_filename_unsafe_chars(name), MAX_FILENAME_LENGTH,
                       false) + SAVE_SUFFIX;
}

string get_prefs_filename()
{
#ifdef DGL_STARTUP_PREFS_BY_NAME
    return _get_savefile_directory() + "start-"
           + strip_filename_unsafe_chars(Options.game.name) + "-ns.prf";
#else
    return _get_savefile_directory() + "start-ns.prf";
#endif
}

static void _write_ghost_version(writer &outf)
{
    marshallUByte(outf, TAG_MAJOR_VERSION);
    marshallUByte(outf, TAG_MINOR_VERSION);

    // extended_version just pads the version out to four 32-bit words.
    // This makes the bones file compatible with Hearse with no extra
    // munging needed.

    // Use a single signature 16-bit word to indicate that this is
    // Stone Soup and to disambiguate this (unmunged) bones file
    // from the munged bones files offered by the old Crawl-aware
    // hearse.pl. Crawl-aware hearse.pl will prefix the bones file
    // with the first 16-bits of the Crawl version, and the following
    // 7 16-bit words set to 0.
    marshallShort(outf, GHOST_SIGNATURE);

    // Write the three remaining 32-bit words of padding.
    for (int i = 0; i < 3; ++i)
        marshallInt(outf, 0);
}

static void _write_tagged_chunk(const string &chunkname, tag_type tag)
{
    writer outf(you.save, chunkname);

    // write version
    marshallUByte(outf, TAG_MAJOR_VERSION);
    marshallUByte(outf, TAG_MINOR_VERSION);

    tag_write(tag, outf);
}

static int _get_dest_stair_type(branch_type old_branch,
                                dungeon_feature_type stair_taken,
                                bool &find_first)
{
    // Order is important here.
    if (stair_taken == DNGN_EXIT_ABYSS)
    {
        find_first = false;
        return DNGN_EXIT_DUNGEON;
    }

    if (stair_taken == DNGN_EXIT_HELL)
        return DNGN_ENTER_HELL;

    if (stair_taken == DNGN_ENTER_HELL)
        return DNGN_EXIT_HELL;

    if (player_in_hell() && feat_is_stone_stair_down(stair_taken))
    {
        find_first = false;
        return DNGN_ENTER_HELL;
    }

    if (feat_is_stone_stair(stair_taken))
    {
        switch (stair_taken)
        {
        case DNGN_STONE_STAIRS_UP_I: return DNGN_STONE_STAIRS_DOWN_I;
        case DNGN_STONE_STAIRS_UP_II: return DNGN_STONE_STAIRS_DOWN_II;
        case DNGN_STONE_STAIRS_UP_III: return DNGN_STONE_STAIRS_DOWN_III;

        case DNGN_STONE_STAIRS_DOWN_I: return DNGN_STONE_STAIRS_UP_I;
        case DNGN_STONE_STAIRS_DOWN_II: return DNGN_STONE_STAIRS_UP_II;
        case DNGN_STONE_STAIRS_DOWN_III: return DNGN_STONE_STAIRS_UP_III;

        default: die("unknown stone stair %d", stair_taken);
        }
    }

    if (feat_is_escape_hatch(stair_taken) || stair_taken == DNGN_TRAP_SHAFT)
        return stair_taken;

    if (stair_taken == DNGN_ENTER_DIS
        || stair_taken == DNGN_ENTER_GEHENNA
        || stair_taken == DNGN_ENTER_COCYTUS
        || stair_taken == DNGN_ENTER_TARTARUS)
    {
        return player_in_hell() ? DNGN_ENTER_HELL : stair_taken;
    }

    if (feat_is_branch_exit(stair_taken))
    {
        for (branch_iterator it; it; ++it)
            if (it->exit_stairs == stair_taken)
                return it->entry_stairs;
        die("entrance corresponding to exit %d not found", stair_taken);
    }

    if (feat_is_branch_entrance(stair_taken))
    {
        for (branch_iterator it; it; ++it)
            if (it->entry_stairs == stair_taken)
                return it->exit_stairs;
        die("return corresponding to entry %d not found", stair_taken);
    }

    if (stair_taken == DNGN_ENTER_LABYRINTH)
    {
        // dgn_find_nearby_stair uses special logic for labyrinths.
        return DNGN_ENTER_LABYRINTH;
    }

    if (feat_is_portal_entrance(stair_taken))
        return DNGN_STONE_ARCH;

    // Note: stair_taken can equal things like DNGN_FLOOR
    // Just find a nice empty square.
    find_first = false;
    return DNGN_FLOOR;
}

static void _place_player_on_stair(branch_type old_branch,
                                   int stair_taken, const coord_def& dest_pos,
                                   const string &hatch_name)

{
    bool find_first = true;
    dungeon_feature_type stair_type = static_cast<dungeon_feature_type>(
            _get_dest_stair_type(old_branch,
                                 static_cast<dungeon_feature_type>(stair_taken),
                                 find_first));

    you.moveto(dgn_find_nearby_stair(stair_type, dest_pos, find_first,
                                     hatch_name));
}

static void _clear_env_map()
{
    env.map_knowledge.init(map_cell());
    env.map_forgotten.reset();
}

static bool _grab_follower_at(const coord_def &pos, bool can_follow)
{
    if (pos == you.pos())
        return false;

    monster* fol = monster_at(pos);
    if (!fol || !fol->alive() || fol->incapacitated())
        return false;

    // only H's ancestors can follow into portals & similar.
    if (!can_follow && !mons_is_hepliaklqana_ancestor(fol->type))
        return false;

    // The monster has to already be tagged in order to follow.
    if (!testbits(fol->flags, MF_TAKING_STAIRS))
        return false;

    // If a monster that can't use stairs was marked as a follower,
    // it's because it's an ally and there might be another ally
    // behind it that might want to push through.
    // This means we don't actually send it on transit, but we do
    // return true, so adjacent real followers are handled correctly. (jpeg)
    if (!mons_can_use_stairs(*fol))
        return true;

    level_id dest = level_id::current();

    dprf("%s is following to %s.", fol->name(DESC_THE, true).c_str(),
         dest.describe().c_str());
    bool could_see = you.can_see(*fol);
    fol->set_transit(dest);
    fol->destroy_inventory();
    monster_cleanup(fol);
    if (could_see)
        view_update_at(pos);
    return true;
}

static void _grab_followers()
{
    const bool can_follow = branch_allows_followers(you.where_are_you);

    int non_stair_using_allies = 0;
    int non_stair_using_summons = 0;

    monster* dowan = nullptr;
    monster* duvessa = nullptr;

    // Handle nearby ghosts.
    for (adjacent_iterator ai(you.pos()); ai; ++ai)
    {
        monster* fol = monster_at(*ai);
        if (fol == nullptr)
            continue;

        if (mons_is_mons_class(fol, MONS_DUVESSA) && fol->alive())
            duvessa = fol;

        if (mons_is_mons_class(fol, MONS_DOWAN) && fol->alive())
            dowan = fol;

        if (fol->wont_attack() && !mons_can_use_stairs(*fol))
        {
            non_stair_using_allies++;
            // If the class can normally use stairs it
            // must have been a summon
            if (mons_class_can_use_stairs(fol->type))
                non_stair_using_summons++;
        }

        if (fol->type == MONS_PLAYER_GHOST
            && fol->hit_points < fol->max_hit_points / 2)
        {
            if (fol->visible_to(&you))
                mpr("The ghost fades into the shadows.");
            monster_teleport(fol, true);
        }
    }

    // Deal with Dowan and Duvessa here.
    if (dowan && duvessa)
    {
        if (!testbits(dowan->flags, MF_TAKING_STAIRS)
            || !testbits(duvessa->flags, MF_TAKING_STAIRS))
        {
            dowan->flags &= ~MF_TAKING_STAIRS;
            duvessa->flags &= ~MF_TAKING_STAIRS;
        }
    }
    else if (dowan && !duvessa)
    {
        if (!dowan->props.exists("can_climb"))
            dowan->flags &= ~MF_TAKING_STAIRS;
    }
    else if (!dowan && duvessa)
    {
        if (!duvessa->props.exists("can_climb"))
            duvessa->flags &= ~MF_TAKING_STAIRS;
    }

    if (can_follow && non_stair_using_allies > 0)
    {
        // Summons won't follow and will time out.
        if (non_stair_using_summons > 0)
        {
            mprf("Your summoned %s left behind.",
                 non_stair_using_allies > 1 ? "allies are" : "ally is");
        }
        else
        {
            // Permanent undead are left behind but stay.
            mprf("Your mindless thrall%s behind.",
                 non_stair_using_allies > 1 ? "s stay" : " stays");
        }
    }

    memset(travel_point_distance, 0, sizeof(travel_distance_grid_t));
    vector<coord_def> places[2] = { { you.pos() }, {} };
    int place_set = 0;
    while (!places[place_set].empty())
    {
        for (const coord_def p : places[place_set])
        {
            for (adjacent_iterator ai(p); ai; ++ai)
            {
                if (travel_point_distance[ai->x][ai->y])
                    continue;

                travel_point_distance[ai->x][ai->y] = 1;
                if (_grab_follower_at(*ai, can_follow))
                    places[!place_set].push_back(*ai);
            }
        }
        places[place_set].clear();
        place_set = !place_set;
    }

    // Clear flags of monsters that didn't follow.
    for (auto &mons : menv_real)
    {
        if (!mons.alive())
            continue;
        if (mons.type == MONS_BATTLESPHERE)
            end_battlesphere(&mons, false);
        if (mons.type == MONS_SPECTRAL_WEAPON)
            end_spectral_weapon(&mons, false);
        mons.flags &= ~MF_TAKING_STAIRS;
    }
}

static void _do_lost_monsters()
{
    // Uniques can be considered wandering Pan just like you, so they're not
    // gone forever. The likes of Cerebov won't be generated elsewhere, but
    // there's no need to special-case that.
    if (player_in_branch(BRANCH_PANDEMONIUM))
        for (monster_iterator mi; mi; ++mi)
            if (mons_is_unique(mi->type) && !(mi->flags & MF_TAKING_STAIRS))
                you.unique_creatures.set(mi->type, false);
}

// Should be called after _grab_followers(), so that items carried by
// followers won't be considered lost.
static void _do_lost_items()
{
    for (const auto &item : mitm)
        if (item.defined() && item.pos != ITEM_IN_INVENTORY)
            item_was_lost(item);
}

/// Rot all corpses remaining on the level, giving Fedhas piety for doing so.
static void _fedhas_rot_all_corpses(const level_id& old_level)
{
    bool messaged = false;
    for (auto &item : mitm)
    {
        if (!item.defined()
            || !item.is_type(OBJ_CORPSES, CORPSE_BODY)
            || item.props.exists(CORPSE_NEVER_DECAYS))
        {
            continue;
        }

        turn_corpse_into_skeleton(item);

        if (!messaged)
        {
            simple_god_message("'s fungi set to work.");
            messaged = true;
        }

        const int piety = x_chance_in_y(2, 5) ? 2 : 1; // match fungal_bloom()
        // XXX: deduplicate above ^
        did_god_conduct(DID_ROT_CARRION, piety);
    }

    // assumption: CORPSE_NEVER_DECAYS is never set for seen corpses
    LevelStashes *ls = StashTrack.find_level(old_level);
    if (ls) // assert?
        ls->rot_all_corpses();
}

/**
 * Perform cleanup when leaving a level.
 *
 * If returning to the previous level on the level stack (e.g. when leaving the
 * abyss), pop it off the stack. Delete non-permanent levels. Also check to be
 * sure no loops have formed in the level stack, and, for Fedhasites, rots any
 * corpses left behind.
 *
 * @param stair_taken   The means used to leave the last level.
 * @param old_level     The ID of the previous level.
 * @param return_pos    Set to the level entrance, if popping a stack level.
 * @return Whether the level was popped onto the stack.
 */
static bool _leave_level(dungeon_feature_type stair_taken,
                         const level_id& old_level, coord_def *return_pos)
{
    bool popped = false;

    if (you.religion == GOD_FEDHAS)
        _fedhas_rot_all_corpses(old_level);

    if (!you.level_stack.empty()
        && you.level_stack.back().id == level_id::current())
    {
        *return_pos = you.level_stack.back().pos;
        you.level_stack.pop_back();
        env.level_state |= LSTATE_DELETED;
        popped = true;
    }
    else if (stair_taken == DNGN_TRANSIT_PANDEMONIUM
             || stair_taken == DNGN_EXIT_THROUGH_ABYSS
             || stair_taken == DNGN_STONE_STAIRS_DOWN_I
             && old_level.branch == BRANCH_ZIGGURAT
             || old_level.branch == BRANCH_ABYSS)
    {
        env.level_state |= LSTATE_DELETED;
    }

    if (is_level_on_stack(level_id::current())
        && !player_in_branch(BRANCH_ABYSS))
    {
        vector<string> stack;
        for (level_pos lvl : you.level_stack)
            stack.push_back(lvl.id.describe());
        if (you.wizard)
        {
            // warn about breakage so testers know it's an abnormal situation.
            mprf(MSGCH_ERROR, "Error: you smelly wizard, how dare you enter "
                 "the same level (%s) twice! It will be trampled upon return.\n"
                 "The stack has: %s.",
                 level_id::current().describe().c_str(),
                 comma_separated_line(stack.begin(), stack.end(),
                                      ", ", ", ").c_str());
        }
        else
        {
            die("Attempt to enter a portal (%s) twice; stack: %s",
                level_id::current().describe().c_str(),
                comma_separated_line(stack.begin(), stack.end(),
                                     ", ", ", ").c_str());
        }
    }

    return popped;
}


/**
 * Generate a new level.
 *
 * Cleanup the environment, build the level, and possibly place a ghost or
 * handle initial AK entrance.
 *
 * @param stair_taken   The means used to leave the last level.
 * @param old_level     The ID of the previous level.
 */
static void _make_level(dungeon_feature_type stair_taken,
                        const level_id& old_level)
{

    env.turns_on_level = -1;

    if (you.chapter == CHAPTER_POCKET_ABYSS
        && player_in_branch(BRANCH_DUNGEON))
    {
        // If we're leaving the Abyss for the first time as a Chaos
        // Knight of Lugonu (who start out there), enable normal monster
        // generation.
        you.chapter = CHAPTER_ORB_HUNTING;
    }

    tile_init_default_flavour();
    tile_clear_flavour();
    env.tile_names.clear();

    // XXX: This is ugly.
    bool dummy;
    dungeon_feature_type stair_type = static_cast<dungeon_feature_type>(
        _get_dest_stair_type(old_level.branch,
                             static_cast<dungeon_feature_type>(stair_taken),
                             dummy));

    _clear_env_map();
    builder(true, stair_type);

    if (!crawl_state.game_is_tutorial()
        && !Options.seed
        && !player_in_branch(BRANCH_ABYSS)
        && (!player_in_branch(BRANCH_DUNGEON) || you.depth > 2)
        && one_chance_in(3))
    {
        load_ghost(true);
    }
    env.turns_on_level = 0;
    // sanctuary
    env.sanctuary_pos  = coord_def(-1, -1);
    env.sanctuary_time = 0;
}

/**
 * Move the player to the appropriate entrance location in a level.
 *
 * @param stair_taken   The means used to leave the last level.
 * @param old_branch    The previous level's branch.
 * @param return_pos    The location of the entrance portal, if applicable.
 * @param dest_pos      The player's location on the last level.
 */
static void _place_player(dungeon_feature_type stair_taken,
                          branch_type old_branch, const coord_def &return_pos,
                          const coord_def &dest_pos, const string &hatch_name)
{
    if (player_in_branch(BRANCH_ABYSS))
        you.moveto(ABYSS_CENTRE);
    else if (!return_pos.origin())
        you.moveto(return_pos);
    else
        _place_player_on_stair(old_branch, stair_taken, dest_pos, hatch_name);

    // Don't return the player into walls, deep water, or a trap.
    for (distance_iterator di(you.pos(), true, false); di; ++di)
        if (you.is_habitable_feat(grd(*di))
            && !is_feat_dangerous(grd(*di), true)
            && !feat_is_trap(grd(*di), true))
        {
            if (you.pos() != *di)
                you.moveto(*di);
            break;
        }

    // This should fix the "monster occurring under the player" bug.
    monster *mon = monster_at(you.pos());
    if (mon && !fedhas_passthrough(mon))
    {
        for (distance_iterator di(you.pos()); di; ++di)
        {
            if (!monster_at(*di) && mon->is_habitable(*di))
            {
                mon->move_to_pos(*di);
                return;
            }
        }

        dprf("%s under player and can't be moved anywhere; killing",
             mon->name(DESC_PLAIN).c_str());
        monster_die(mon, KILL_DISMISSED, NON_MONSTER);
        // XXX: do we need special handling for uniques...?
    }
}

// Update the trackers after the player changed level.
void trackers_init_new_level(bool transit)
{
    travel_init_new_level();
}

static string _get_hatch_name()
{
    vector <map_marker *> markers;
    markers = find_markers_by_prop(HATCH_NAME_PROP);
    for (auto m : markers)
    {
        if (m->pos == you.pos())
        {
            string name = m->property(HATCH_NAME_PROP);
            ASSERT(!name.empty());
            return name;
        }
    }
    return "";
}

/**
 * Load the current level.
 *
 * @param stair_taken   The means used to enter the level.
 * @param load_mode     Whether the level is being entered, examined, etc.
 * @return Whether a new level was created.
 */
bool load_level(dungeon_feature_type stair_taken, load_mode_type load_mode,
                const level_id& old_level)
{

    const string level_name = level_id::current().describe();
    const bool make_changes =
    (load_mode == LOAD_START_GAME || load_mode == LOAD_ENTER_LEVEL);

    // Did we get here by popping the level stack?
    bool popped = false;

    coord_def return_pos; //TODO: initialize to null

    string hatch_name = "";
    if (feat_is_escape_hatch(stair_taken))
        hatch_name = _get_hatch_name();

    if (load_mode != LOAD_VISITOR)
        popped = _leave_level(stair_taken, old_level, &return_pos);

    unwind_var<dungeon_feature_type> stair(
        you.transit_stair, stair_taken, DNGN_UNSEEN);
    unwind_bool ylev(you.entering_level, load_mode != LOAD_VISITOR, false);

#ifdef DEBUG_LEVEL_LOAD
    mprf(MSGCH_DIAGNOSTICS, "Loading... branch: %d, level: %d",
                            you.where_are_you, you.depth);
#endif

    // Save position for hatches to place a marker on the destination level.
    coord_def dest_pos = you.pos();

    you.prev_targ     = MHITNOT;
    you.prev_grd_targ.reset();

    // We clear twice - on save and on load.
    // Once would be enough...
    if (make_changes)
        delete_all_clouds();

    // Lose all listeners.
    dungeon_events.clear();

    // This block is to grab followers and save the old level to disk.
    if (load_mode == LOAD_ENTER_LEVEL)
    {
        dprf("stair_taken = %s", dungeon_feature_name(stair_taken));
        // Not the case normally, but can happen during recovery of damaged
        // games.
        if (old_level.depth != -1)
        {
            _grab_followers();

            if (env.level_state & LSTATE_DELETED)
                delete_level(old_level), dprf("<lightmagenta>Deleting level.</lightmagenta>");
            else
                _save_level(old_level);
        }

        // The player is now between levels.
        you.position.reset();

        update_companions();
    }

    clear_travel_trail();

#ifdef USE_TILE
    if (load_mode != LOAD_VISITOR)
    {
        tiles.clear_minimap();
        crawl_view_buffer empty_vbuf;
        tiles.load_dungeon(empty_vbuf, crawl_view.vgrdc);
    }
#endif

    bool just_created_level = false;

    // GENERATE new level when the file can't be opened:
    if (!you.save->has_chunk(level_name))
    {
        ASSERT(load_mode != LOAD_VISITOR);
        dprf("Generating new level for '%s'.", level_name.c_str());
        _make_level(stair_taken, old_level);
        just_created_level = true;
    }
    else
    {
        dprf("Loading old level '%s'.", level_name.c_str());
        _restore_tagged_chunk(you.save, level_name, TAG_LEVEL, "Level file is invalid.");

        _redraw_all();
    }

    // Clear map knowledge stair emphasis.
    show_update_emphasis();

    // Shouldn't happen, but this is too unimportant to assert.
    deleteAll(env.final_effects);

    los_changed();

    // Markers must be activated early, since they may rely on
    // events issued later, e.g. DET_ENTERING_LEVEL or
    // the DET_TURN_ELAPSED from update_level.
    if (make_changes || load_mode == LOAD_RESTART_GAME)
        env.markers.activate_all();

    if (make_changes && env.elapsed_time && !just_created_level)
        update_level(you.elapsed_time - env.elapsed_time);

    // Apply all delayed actions, if any.
    if (just_created_level)
        env.dactions_done = you.dactions.size();
    else
        catchup_dactions();

    // Here's the second cloud clearing, on load (see above).
    if (make_changes)
    {
        delete_all_clouds();

        _place_player(stair_taken, old_level.branch, return_pos, dest_pos,
                      hatch_name);
    }

    crawl_view.set_player_at(you.pos(), load_mode != LOAD_VISITOR);

    // Actually "move" the followers if applicable.
    if (load_mode == LOAD_ENTER_LEVEL)
        place_followers();

    // Load monsters in transit.
    if (load_mode == LOAD_ENTER_LEVEL)
        place_transiting_monsters();

    if (make_changes)
    {
        // Tell stash-tracker and travel that we've changed levels.
        trackers_init_new_level(true);
        tile_new_level(just_created_level);
    }
    else if (load_mode == LOAD_RESTART_GAME)
    {
        // Travel needs initialize some things on reload, too.
        travel_init_load_level();
    }

    _redraw_all();

    if (load_mode != LOAD_VISITOR)
        dungeon_events.fire_event(DET_ENTERING_LEVEL);

    // Things to update for player entering level
    if (load_mode == LOAD_ENTER_LEVEL)
    {
        // new levels have less wary monsters, and we don't
        // want them to attack players quite as soon:
        you.time_taken *= (just_created_level ? 1 : 2);

        you.time_taken = div_rand_round(you.time_taken * 3, 4);

        dprf("arrival time: %d", you.time_taken);

        if (just_created_level)
            run_map_epilogues();
    }

    // Save the created/updated level out to disk:
    if (make_changes)
        _save_level(level_id::current());

    setup_environment_effects();

    setup_vault_mon_list();

    // Inform user of level's annotation.
    if (load_mode != LOAD_VISITOR
        && !get_level_annotation().empty()
        && !crawl_state.level_annotation_shown)
    {
        mprf(MSGCH_PLAIN, YELLOW, "Level annotation: %s",
             get_level_annotation().c_str());
    }

    if (load_mode != LOAD_VISITOR)
        crawl_state.level_annotation_shown = false;

    if (make_changes)
    {
        // Update PlaceInfo entries
        PlaceInfo& curr_PlaceInfo = you.get_place_info();
        PlaceInfo  delta;

        if (load_mode == LOAD_START_GAME
            || (load_mode == LOAD_ENTER_LEVEL
                && old_level.branch != you.where_are_you
                && !popped))
        {
            delta.num_visits++;
        }

        if (just_created_level)
            delta.levels_seen++;

        you.global_info += delta;
#ifdef DEBUG_LEVEL_LOAD
        mprf(MSGCH_DIAGNOSTICS,
             "global_info:: num_visits: %d, levels_seen: %d",
             you.global_info.num_visits, you.global_info.levels_seen);
#endif
        you.global_info.assert_validity();

        curr_PlaceInfo += delta;
#ifdef DEBUG_LEVEL_LOAD
        mprf(MSGCH_DIAGNOSTICS,
             "curr_PlaceInfo:: num_visits: %d, levels_seen: %d",
             curr_PlaceInfo.num_visits, curr_PlaceInfo.levels_seen);
#endif
        curr_PlaceInfo.assert_validity();
    }

    if (just_created_level)
        you.attribute[ATTR_ABYSS_ENTOURAGE] = 0;

    if (load_mode != LOAD_VISITOR)
        dungeon_events.fire_event(DET_ENTERED_LEVEL);

    if (load_mode == LOAD_ENTER_LEVEL)
    {
        // 50% chance of repelling the stair you just came through.
        if (you.duration[DUR_REPEL_STAIRS_MOVE]
            || you.duration[DUR_REPEL_STAIRS_CLIMB])
        {
            dungeon_feature_type feat = grd(you.pos());
            if (feat != DNGN_ENTER_SHOP
                && feat_stair_direction(feat) != CMD_NO_CMD
                && feat_stair_direction(stair_taken) != CMD_NO_CMD)
            {
                string stair_str = feature_description_at(you.pos(), false,
                                                          DESC_THE, false);
                string verb = stair_climb_verb(feat);

                if (coinflip()
                    && slide_feature_over(you.pos()))
                {
                    mprf("%s slides away from you right after you %s it!",
                         stair_str.c_str(), verb.c_str());
                }

                if (coinflip())
                {
                    // Stairs stop fleeing from you now you actually caught one.
                    mprf("%s settles down.", stair_str.c_str());
                    you.duration[DUR_REPEL_STAIRS_MOVE]  = 0;
                    you.duration[DUR_REPEL_STAIRS_CLIMB] = 0;
                }
            }
        }

        ash_detect_portals(is_map_persistent());

        if (just_created_level)
            xom_new_level_noise_or_stealth();
    }
    // Initialize halos, etc.
    invalidate_agrid(true);

    // Maybe make a note if we reached a new level.
    // Don't do so if we are just moving around inside Pan, though.
    if (just_created_level && stair_taken != DNGN_TRANSIT_PANDEMONIUM)
        take_note(Note(NOTE_DUNGEON_LEVEL_CHANGE));

    // If the player entered the level from a different location than they last
    // exited it, have monsters lose track of where they are
    if (you.position != env.old_player_pos)
       shake_off_monsters(you.as_player());

#if TAG_MAJOR_VERSION == 34
    if (make_changes && you.props.exists("zig-fixup")
        && you.where_are_you == BRANCH_TOMB
        && you.depth == brdepth[BRANCH_TOMB])
    {
        if (!just_created_level)
        {
            int obj = items(false, OBJ_MISCELLANY, MISC_ZIGGURAT, 0);
            ASSERT(obj != NON_ITEM);
            bool success = move_item_to_grid(&obj, you.pos(), true);
            ASSERT(success);
        }
        you.props.erase("zig-fixup");
    }
#endif

    return just_created_level;
}

static void _save_level(const level_id& lid)
{
    travel_cache.get_level_info(lid).update();

    // Nail all items to the ground.
    fix_item_coordinates();

    _write_tagged_chunk(lid.describe(), TAG_LEVEL);
}

#if TAG_MAJOR_VERSION == 34
# define CHUNK(short, long) short
#else
# define CHUNK(short, long) long
#endif

#define SAVEFILE(short, long, savefn)           \
    do                                          \
    {                                           \
        writer w(you.save, CHUNK(short, long)); \
        savefn(w);                              \
    } while (false)

// Stack allocated string's go in separate function, so Valgrind doesn't
// complain.
static void _save_game_base()
{
    /* Stashes */
    SAVEFILE("st", "stashes", StashTrack.save);

#ifdef CLUA_BINDINGS
    /* lua */
    SAVEFILE("lua", "lua", clua.save);
#endif

    /* kills */
    SAVEFILE("kil", "kills", you.kills.save);

    /* travel cache */
    SAVEFILE("tc", "travel_cache", travel_cache.save);

    /* notes */
    SAVEFILE("nts", "notes", save_notes);

    /* tutorial/hints mode */
    if (crawl_state.game_is_hints_tutorial())
        SAVEFILE("tut", "tutorial", save_hints);

    /* messages */
    SAVEFILE("msg", "messages", save_messages);

    /* tile dolls (empty for ASCII)*/
#ifdef USE_TILE
    // Save the current equipment into a file.
    SAVEFILE("tdl", "tiles_doll", save_doll_file);
#endif

    _write_tagged_chunk("you", TAG_YOU);
    _write_tagged_chunk("chr", TAG_CHR);
}

// Stack allocated string's go in separate function, so Valgrind doesn't
// complain.
static void _save_game_exit()
{
    clua.save_persist();

    // Prompt for saving macros.
    if (crawl_state.unsaved_macros
        && !crawl_state.seen_hups
        && yesno("Save macros?", true, 'n'))
    {
        macro_save();
    }

    // Must be exiting -- save level & goodbye!
    if (!you.entering_level)
        _save_level(level_id::current());

    clrscr();

#ifdef DGL_WHEREIS
    whereis_record("saved");
#endif
#ifdef USE_TILE_WEB
    tiles.send_exit_reason("saved");
#endif

    delete you.save;
    you.save = 0;
}

void save_game(bool leave_game, const char *farewellmsg)
{
    unwind_bool saving_game(crawl_state.saving_game, true);


    if (leave_game && Options.dump_on_save)
    {
        if (!dump_char(you.your_name, true))
        {
            mpr("Char dump unsuccessful! Sorry about that.");
            if (!crawl_state.seen_hups)
                more();
        }
#ifdef USE_TILE_WEB
        else
            tiles.send_dump_info("save", you.your_name);
#endif
    }

    // Stack allocated string's go in separate function,
    // so Valgrind doesn't complain.
    _save_game_base();

    // If just save, early out.
    if (!leave_game)
    {
        if (!crawl_state.disables[DIS_SAVE_CHECKPOINTS])
            you.save->commit();
        return;
    }

    // Stack allocated string's go in separate function,
    // so Valgrind doesn't complain.
    _save_game_exit();

    if (Options.restart_after_game && Options.restart_after_save
        && !crawl_state.seen_hups)
    {
        throw game_ended_condition(true);
    }

    end(0, false, farewellmsg? "%s" : "See you soon, %s!",
        farewellmsg? farewellmsg : you.your_name.c_str());
}

// Saves the game without exiting.
void save_game_state()
{
    save_game(false);
    if (crawl_state.seen_hups)
        save_game(true);
}

static string _make_ghost_filename()
{
    return "bones."
           + replace_all(level_id::current().describe(), ":", "-");
}

#define BONES_DIAGNOSTICS (defined(WIZARD) || defined(DEBUG_BONES) | defined(DEBUG_DIAGNOSTICS))

/**
 * Lists all bonefiles for the current level.
 *
 * @return A vector containing absolute paths to 0+ bonefiles.
 */
static vector<string> _list_bones()
{
    string bonefile_dir = _get_bonefile_directory();
    string base_filename = _make_ghost_filename();
    string underscored_filename = base_filename + "_";

    vector<string> filenames = get_dir_files(bonefile_dir);
    vector<string> bonefiles;
    for (const auto &filename : filenames)
        if (starts_with(filename, underscored_filename))
            bonefiles.push_back(bonefile_dir + filename);

    string old_bonefile = _get_old_bonefile_directory() + base_filename;
    if (access(old_bonefile.c_str(), F_OK) == 0)
    {
        dprf("Found old bonefile %s", old_bonefile.c_str());
        bonefiles.push_back(old_bonefile);
    }

    return bonefiles;
}

/**
 * Attempts to find a file containing ghost(s) appropriate for the player.
 *
 * @return The filename of an appropriate bones file; may be "".
 */
static string _find_ghost_file()
{
    vector<string> bonefiles = _list_bones();
    if (bonefiles.empty())
        return "";
    return bonefiles[ui_random(bonefiles.size())];
}

/**
 * Attempt to load one or more ghosts into the level.
 *
 * @param creating_level    Whether a level is currently being generated.
 * @return                  Whether ghosts were actually generated.
 */
bool load_ghost(bool creating_level)
{
    const bool wiz_cmd = (crawl_state.prev_cmd == CMD_WIZARD);

    ASSERT(you.transit_stair == DNGN_UNSEEN || creating_level);
    ASSERT(!you.entering_level || creating_level);
    ASSERT(!creating_level
           || (you.entering_level && you.transit_stair != DNGN_UNSEEN));
    // Only way to load a ghost without creating a level is via a wizard
    // command.
    ASSERT(creating_level || wiz_cmd);

#if !defined(DEBUG) && !defined(WIZARD)
    UNUSED(creating_level);
#endif

#ifdef BONES_DIAGNOSTICS
    const bool do_diagnostics =
#  if defined(DEBUG_BONES) || defined(DEBUG_DIAGNOSTICS)
        true
#  elif defined(WIZARD)
        !creating_level
#  else // Can't happen currently
        false
#  endif
        ;
#endif // BONES_DIAGNOSTICS

    const string ghost_filename = _find_ghost_file();
    if (ghost_filename.empty())
    {
        if (wiz_cmd && !creating_level)
            mprf(MSGCH_PROMPT, "No ghost files for this level.");
        return false; // no such ghost.
    }

    reader inf(ghost_filename);
    if (!inf.valid())
    {
        if (wiz_cmd && !creating_level)
            mprf(MSGCH_PROMPT, "Ghost file invalidated before read.");
        return false;
    }

    inf.set_safe_read(true); // don't die on 0-byte bones
    if (_ghost_version_compatible(inf))
    {
        try
        {
            ghosts.clear();
            tag_read(inf, TAG_GHOST);
            inf.fail_if_not_eof(ghost_filename);
        }
        catch (short_read_exception &short_read)
        {
            mprf(MSGCH_ERROR, "Broken bones file: %s",
                 ghost_filename.c_str());
        }
    }
    inf.close();

    // Remove bones file - ghosts are hardly permanent.
    unlink(ghost_filename.c_str());

    if (!debug_check_ghosts())
    {
        mprf(MSGCH_DIAGNOSTICS,
             "Refusing to load buggy ghost from file \"%s\"!",
             ghost_filename.c_str());

        return false;
    }

#ifdef BONES_DIAGNOSTICS
    if (do_diagnostics)
    {
        mprf(MSGCH_DIAGNOSTICS, "Loaded ghost file with %u ghost(s)",
             (unsigned int)ghosts.size());
    }
#endif

#ifdef BONES_DIAGNOSTICS
    unsigned int  unplaced_ghosts = ghosts.size();
    bool          ghost_errors    = false;
#endif

    // Translate ghost to monster and place.
    while (!ghosts.empty())
    {
        monster * const mons = get_free_monster();
        if (!mons)
            break;

        mons->set_new_monster_id();
        mons->set_ghost(ghosts[0]);
        mons->type = MONS_PLAYER_GHOST;
        mons->ghost_init();
        mons->bind_melee_flags();
        if (mons->has_spells())
            mons->bind_spell_flags();

        ghosts.erase(ghosts.begin());
#ifdef BONES_DIAGNOSTICS
        if (do_diagnostics)
        {
            unplaced_ghosts--;
            if (!mons->alive())
            {
                mprf(MSGCH_DIAGNOSTICS, "Placed ghost is not alive.");
                ghost_errors = true;
            }
            else if (mons->type != MONS_PLAYER_GHOST)
            {
                mprf(MSGCH_DIAGNOSTICS,
                     "Placed ghost is not MONS_PLAYER_GHOST, but %s",
                     mons->name(DESC_PLAIN, true).c_str());
                ghost_errors = true;
            }
        }
#endif
    }

#ifdef BONES_DIAGNOSTICS
    if (do_diagnostics && unplaced_ghosts > 0)
    {
        mprf(MSGCH_DIAGNOSTICS, "Unable to place %u ghost(s)",
             (unsigned int)ghosts.size());
        ghost_errors = true;
    }
    if (ghost_errors)
        more();
#endif

    return true;
}

// returns false if a new game should start instead
static bool _restore_game(const string& filename)
{
    if (Options.no_save)
        return false;

    you.save = new package((_get_savefile_directory() + filename).c_str(), true);

    if (!_read_char_chunk(you.save))
    {
        // Note: if we are here, the save info was properly read, it would
        // raise an exception otherwise.
        if (yesno(("This game comes from an incompatible version of Crawl ("
                   + you.prev_save_version + ").\n"
                   "Unless you reinstall that version, you can't load it.\n"
                   "Do you want to DELETE that game and start a new one?"
                  ).c_str(),
                  true, 'n'))
        {
            you.save->unlink();
            you.save = 0;
            return false;
        }
        fail("Cannot load an incompatible save from version %s",
             you.prev_save_version.c_str());
    }

    if (numcmp(you.prev_save_version.c_str(), Version::Long, 2) == -1
        && version_is_stable(you.prev_save_version.c_str()))
    {
        if (!yesno("This game comes from a previous release of Crawl. If you "
                   "load it now, you won't be able to go back. Continue?",
                   true, 'n'))
        {
            you.save->abort(); // don't even rewrite the header
            delete you.save;
            you.save = 0;
            end(0, false, "Please reinstall the stable version then.\n");
        }
    }

    _restore_tagged_chunk(you.save, "you", TAG_YOU, "Save data is invalid.");

    const int minorVersion = crawl_state.minor_version;

    if (you.save->has_chunk(CHUNK("st", "stashes")))
    {
        reader inf(you.save, CHUNK("st", "stashes"), minorVersion);
        StashTrack.load(inf);
    }

#ifdef CLUA_BINDINGS
    if (you.save->has_chunk("lua"))
    {
        vector<char> buf;
        chunk_reader inf(you.save, "lua");
        inf.read_all(buf);
        buf.push_back(0);
        clua.execstring(&buf[0]);
    }
#endif

    if (you.save->has_chunk(CHUNK("kil", "kills")))
    {
        reader inf(you.save, CHUNK("kil", "kills"),minorVersion);
        you.kills.load(inf);
    }

    if (you.save->has_chunk(CHUNK("tc", "travel_cache")))
    {
        reader inf(you.save, CHUNK("tc", "travel_cache"), minorVersion);
        travel_cache.load(inf, minorVersion);
    }

    if (you.save->has_chunk(CHUNK("nts", "notes")))
    {
        reader inf(you.save, CHUNK("nts", "notes"), minorVersion);
        load_notes(inf);
    }

    /* hints mode */
    if (you.save->has_chunk(CHUNK("tut", "tutorial")))
    {
        reader inf(you.save, CHUNK("tut", "tutorial"), minorVersion);
        load_hints(inf);
    }

    /* messages */
    if (you.save->has_chunk(CHUNK("msg", "messages")))
    {
        reader inf(you.save, CHUNK("msg", "messages"), minorVersion);
        load_messages(inf);
    }

    return true;
}

// returns false if a new game should start instead
bool restore_game(const string& filename)
{
    try
    {
        return _restore_game(filename);
    }
    catch (corrupted_save &err)
    {
        if (yesno(make_stringf(
                   "There exists a save by that name but it appears to be invalid.\n"
                   "(Error: %s). Do you want to delete it?", err.what()).c_str(),
                  true, 'n'))
        {
            if (you.save)
                you.save->unlink();
            you.save = 0;
            return false;
        }
        // Shouldn't crash probably...
        fail("Aborting; you may try to recover it somehow.");
    }
}

static void _load_level(const level_id &level)
{
    // Load the given level.
    you.where_are_you = level.branch;
    you.depth =         level.depth;

    load_level(DNGN_STONE_STAIRS_DOWN_I, LOAD_VISITOR, level_id());
}

// Given a level returns true if the level has been created already
// in this game.
bool is_existing_level(const level_id &level)
{
    return you.save && you.save->has_chunk(level.describe());
}

void delete_level(const level_id &level)
{
    travel_cache.erase_level_info(level);
    StashTrack.remove_level(level);
    shopping_list.del_things_from(level);

    clear_level_exclusion_annotation(level);
    clear_level_annotations(level);

    if (you.save)
        you.save->delete_chunk(level.describe());
    if (level.branch == BRANCH_ABYSS)
    {
        save_abyss_uniques();
        destroy_abyss();
    }
    _do_lost_monsters();
    _do_lost_items();
}

// This class provides a way to walk the dungeon with a bit more flexibility
// than you used to get with apply_to_all_dungeons.
level_excursion::level_excursion()
    : original(level_id::current()), ever_changed_levels(false)
{
}

void level_excursion::go_to(const level_id& next)
{
    if (level_id::current() != next)
    {
        ever_changed_levels = true;

        _save_level(level_id::current());
        _load_level(next);

        LevelInfo &li = travel_cache.get_level_info(next);
        li.set_level_excludes();
    }

    you.on_current_level = (level_id::current() == original);
}

level_excursion::~level_excursion()
{
    // Go back to original level and reactivate markers if we ever
    // left the level.
    if (ever_changed_levels)
    {
        // This may be a no-op if the level-excursion subsequently
        // returned to the original level. However, at this point
        // markers will still not be activated.
        go_to(original);

        // Quietly reactivate markers.
        env.markers.activate_all(false);
    }
}

bool get_save_version(reader &file, int &major, int &minor)
{
    // Read first two bytes.
    uint8_t buf[2];
    try
    {
        file.read(buf, 2);
    }
    catch (short_read_exception& E)
    {
        // Empty file?
        major = minor = -1;
        return false;
    }

    major = buf[0];
    minor = buf[1];

    return true;
}

static bool _read_char_chunk(package *save)
{
    reader inf(save, "chr");

    try
    {
        uint8_t format, major, minor;
        inf.read(&major, 1);
        inf.read(&minor, 1);

        unsigned int len = unmarshallInt(inf);
        if (len > 1024) // something is fishy
            fail("Save file corrupted (info > 1KB)");
        vector<unsigned char> buf;
        buf.resize(len);
        inf.read(&buf[0], len);
        inf.fail_if_not_eof("chr");
        reader th(buf);

        // 0.8 trunks (30.0 .. 32.12) were format 0 but without the marker.
        if (major > 32 || major == 32 && minor >= 13)
            th.read(&format, 1);
        else
            format = 0;

        if (format > TAG_CHR_FORMAT)
            fail("Incompatible character data");

        tag_read_char(th, format, major, minor);

        // Check if we read everything only on the exact same version,
        // but that's the common case.
        if (major == TAG_MAJOR_VERSION && minor == TAG_MINOR_VERSION)
            inf.fail_if_not_eof("chr");

#if TAG_MAJOR_VERSION == 34
        if (major == 33 && minor == TAG_MINOR_0_11)
            return true;
#endif
        return major == TAG_MAJOR_VERSION && minor <= TAG_MINOR_VERSION;
    }
    catch (short_read_exception &E)
    {
        fail("Save file corrupted");
    };
}

static bool _tagged_chunk_version_compatible(reader &inf, string* reason)
{
    int major = 0, minor = TAG_MINOR_INVALID;
    ASSERT(reason);

    if (!get_save_version(inf, major, minor))
    {
        *reason = "File is corrupt.";
        return false;
    }

    if (major != TAG_MAJOR_VERSION
#if TAG_MAJOR_VERSION == 34
        && (major != 33 || minor != 17)
#endif
       )
    {
        if (Version::ReleaseType)
        {
            *reason = (CRAWL " " + string(Version::Short) + " is not compatible with "
                       "save files from older versions. You can continue your "
                       "game with the appropriate older version, or you can "
                       "delete it and start a new game.");
        }
        else
        {
            *reason = make_stringf("Major version mismatch: %d (want %d).",
                                   major, TAG_MAJOR_VERSION);
        }
        return false;
    }

    if (minor < 0)
    {
        *reason = make_stringf("Minor version %d is negative!",
                               minor);
        return false;
    }

    if (minor > TAG_MINOR_VERSION)
    {
        *reason = make_stringf("Minor version mismatch: %d (want <= %d). "
                               "The save is from a newer version.",
                               minor, TAG_MINOR_VERSION);
        return false;
    }

    inf.setMinorVersion(minor);
    return true;
}

static bool _restore_tagged_chunk(package *save, const string &name,
                                  tag_type tag, const char* complaint)
{
    reader inf(save, name);
    string reason;
    if (!_tagged_chunk_version_compatible(inf, &reason))
    {
        if (!complaint)
        {
            dprf("chunk %s: %s", name.c_str(), reason.c_str());
            return false;
        }
        else
            end(-1, false, "\n%s %s\n", complaint, reason.c_str());
    }

    crawl_state.minor_version = inf.getMinorVersion();
    try
    {
        tag_read(inf, tag);
    }
    catch (short_read_exception &E)
    {
        fail("truncated save chunk (%s)", name.c_str());
    };

    inf.fail_if_not_eof(name);
    return true;
}

static bool _ghost_version_compatible(reader &inf)
{
    try
    {
        const int majorVersion = unmarshallUByte(inf);
        const int minorVersion = unmarshallUByte(inf);

        if (majorVersion != TAG_MAJOR_VERSION
            || minorVersion > TAG_MINOR_VERSION)
        {
            dprf("Ghost version mismatch: ghost was %d.%d; wanted %d.%d",
                 majorVersion, minorVersion,
                 TAG_MAJOR_VERSION, TAG_MINOR_VERSION);
            return false;
        }

        inf.setMinorVersion(minorVersion);

        // Check for the DCSS ghost signature.
        if (unmarshallShort(inf) != GHOST_SIGNATURE)
            return false;

        // Discard three more 32-bit words of padding.
        inf.read(nullptr, 3*4);
    }
    catch (short_read_exception &E)
    {
        mprf(MSGCH_ERROR,
             "Ghost file \"%s\" seems to be invalid (short read); deleting it.",
             inf.filename().c_str());
        return false;
    }
    return true;
}

/**
 * Attempt to open a new bones file for saving ghosts.
 *
 * @param[out] return_gfilename     The name of the file created, if any.
 * @return                          A FILE object, or nullptr.
 **/
static FILE* _make_bones_file(string * return_gfilename)
{

    const string bone_dir = _get_bonefile_directory();
    const string base_filename = _make_ghost_filename();
    for (int i = 0; i < GHOST_LIMIT; i++)
    {
        const string g_file_name = make_stringf("%s%s_%d", bone_dir.c_str(),
                                                base_filename.c_str(), i);
        FILE *gfil = lk_open_exclusive(g_file_name);
        // need to check file size, so can't open 'wb' - would truncate!

        if (!gfil)
        {
            dprf("Could not open %s", g_file_name.c_str());
            continue;
        }

        dprf("found %s", g_file_name.c_str());

        *return_gfilename = g_file_name;
        return gfil;
    }

    return nullptr;
}

/**
 * Attempt to save all ghosts from the current level.
 *
 * Including the player, if they're not undead. Doesn't save ghosts from D:1-2
 * or Temple.
 *
 * @param force   Forces ghost generation even in otherwise-disallowed levels.
 **/

void save_ghost(bool force)
{
#ifdef BONES_DIAGNOSTICS
    const bool do_diagnostics =
#  if defined(DEBUG_BONES) || defined(DEBUG_DIAGNOSTICS)
        true
#  elif defined(WIZARD)
        you.wizard
#  else // Can't happen currently
        false
#  endif
        ;
#endif // BONES_DIAGNOSTICS

    ghosts = ghost_demon::find_ghosts();

    if (ghosts.empty())
    {
#ifdef BONES_DIAGNOSTICS
        if (do_diagnostics)
            mprf(MSGCH_DIAGNOSTICS, "Could not find any ghosts for this level.");
#endif
        return;
    }

    // No ghosts on D:1, D:2, the Temple, or the Abyss.
    if (!force && (you.depth < 3 && player_in_branch(BRANCH_DUNGEON)
                   || player_in_branch(BRANCH_TEMPLE)
                   || player_in_branch(BRANCH_ABYSS)))
    {
        return;
    }

    if (_list_bones().size() >= static_cast<size_t>(GHOST_LIMIT))
    {
#ifdef BONES_DIAGNOSTICS
        if (do_diagnostics)
            mprf(MSGCH_DIAGNOSTICS, "Too many ghosts for this level already!");
#endif
        return;
    }

    string g_file_name = "";
    FILE* ghost_file = _make_bones_file(&g_file_name);

    if (!ghost_file)
    {
#ifdef BONES_DIAGNOSTICS
        if (do_diagnostics)
            mprf(MSGCH_DIAGNOSTICS, "Could not open file to save ghosts.");
#endif
        return;
    }

    writer outw(g_file_name, ghost_file);

    _write_ghost_version(outw);
    tag_write(TAG_GHOST, outw);

    lk_close(ghost_file, g_file_name);

#ifdef BONES_DIAGNOSTICS
    if (do_diagnostics)
        mprf(MSGCH_DIAGNOSTICS, "Saved ghosts (%s).", g_file_name.c_str());
#endif
}

////////////////////////////////////////////////////////////////////////////
// Locking

bool lock_file_handle(FILE *handle, bool write)
{
    return lock_file(fileno(handle), write, true);
}

bool unlock_file_handle(FILE *handle)
{
    return unlock_file(fileno(handle));
}

/**
 * Attempts to open & lock a file.
 *
 * @param mode      The file access mode. ('r', 'ab+', etc)
 * @param file      The path to the file to be opened.
 * @return          A handle for the specified file, if successful; else nullptr.
 */
FILE *lk_open(const char *mode, const string &file)
{
    ASSERT(mode);

    FILE *handle = fopen_u(file.c_str(), mode);
    if (!handle)
        return nullptr;

    const bool write_lock = mode[0] != 'r' || strchr(mode, '+');
    if (!lock_file_handle(handle, write_lock))
    {
        mprf(MSGCH_ERROR, "ERROR: Could not lock file %s", file.c_str());
        fclose(handle);
        handle = nullptr;
    }

    return handle;
}

/**
 * Attempts to open and lock a file for exclusive write access; fails if
 * the file already exists.
 *
 * @param file The path to the file to be opened.
 * @return     A locked file handle for the specified file, if
 *             successful; else nullptr.
 */
FILE *lk_open_exclusive(const string &file)
{
    int fd = open_u(file.c_str(), O_WRONLY|O_BINARY|O_EXCL|O_CREAT, 0666);
    if (fd < 0)
        return nullptr;

    if (!lock_file(fd, true))
    {
        mprf(MSGCH_ERROR, "ERROR: Could not lock file %s", file.c_str());
        close(fd);
        return nullptr;
    }

    return fdopen(fd, "wb");
}

void lk_close(FILE *handle, const string &file)
{
    if (handle == nullptr || handle == stdin)
        return;

    unlock_file_handle(handle);

    // actually close
    fclose(handle);
}

/////////////////////////////////////////////////////////////////////////////
// file_lock
//
// Locks a named file (usually an empty lock file), creating it if necessary.

file_lock::file_lock(const string &s, const char *_mode, bool die_on_fail)
    : handle(nullptr), mode(_mode), filename(s)
{
    if (!(handle = lk_open(mode, filename)) && die_on_fail)
        end(1, true, "Unable to open lock file \"%s\"", filename.c_str());
}

file_lock::~file_lock()
{
    if (handle)
        lk_close(handle, filename);
}

/////////////////////////////////////////////////////////////////////////////

FILE *fopen_replace(const char *name)
{
    int fd;

    // Stave off symlink attacks. Races will be handled with O_EXCL.
    unlink_u(name);
    fd = open_u(name, O_CREAT|O_EXCL|O_WRONLY, 0666);
    if (fd == -1)
        return 0;
    return fdopen(fd, "w");
}

// Returns the size of the opened file with the give FILE* handle.
off_t file_size(FILE *handle)
{
#ifdef __ANDROID__
    off_t pos = ftello(handle);
    if (fseeko(handle, 0, SEEK_END) < 0)
        return 0;
    off_t ret = ftello(handle);
    fseeko(handle, pos, SEEK_SET);
    return ret;
#else
    struct stat fs;
    const int err = fstat(fileno(handle), &fs);
    return err? 0 : fs.st_size;
#endif
}

vector<string> get_title_files()
{
    vector<string> titles;
    for (const string &dir : _get_base_dirs())
        for (const string &file : get_dir_files(dir))
            if (file.substr(0, 6) == "title_")
                titles.push_back(file);
    return titles;
}
