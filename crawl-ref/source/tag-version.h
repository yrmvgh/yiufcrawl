#ifndef TAG_VERSION_H
#define TAG_VERSION_H

// Character info has its own top-level tag, mismatching majors don't break
// compatibility there.
// DO NOT BUMP THIS UNLESS YOU KNOW WHAT YOU'RE DOING. This would break
// the save browser across versions, possibly leading to overwritten games.
// It's only there in case there's no way out.
#define TAG_CHR_FORMAT 0

// Let CDO updaters know if the syntax changes.
// Really, really, REALLY _never_ ever bump this and clean up old #ifdefs
// in a single commit, please.  Making clean-up and actual code changes,
// especially of this size, separated is vital for sanity.
#ifndef TAG_MAJOR_VERSION
#define TAG_MAJOR_VERSION 35
#endif

// Minor version will be reset to zero when major version changes.
enum tag_minor_version
{
    TAG_MINOR_INVALID         = -1,
    TAG_MINOR_RESET           = 0, // Minor tags were reset
#if TAG_MAJOR_VERSION == 35
    TAG_MINOR_GOLD_TRACKING   = 1, // Track gold generated
    TAG_MINOR_BRIBE_BRANCH    = 2, // Bribe branch tracking
#endif
    NUM_TAG_MINORS,
    TAG_MINOR_VERSION = NUM_TAG_MINORS - 1
};

#endif
