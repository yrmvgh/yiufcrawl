/**
 * @file
 * @brief Functions used to print messages.
**/

#pragma once

#include <iostream>
#include <streambuf>
#include <string>

#include "canned-message-type.h"
#include "enum.h"
#include "player.h"

// Write the message window contents out.
void display_message_window();
void clear_message_window();

void scroll_message_window(int n);

void clear_messages(bool force = false);

void flush_prev_message();

void more(bool user_forced = false);

void canned_msg(canned_message_type which_message);

bool simple_monster_message(const monster& mons, const char *event,
                            msg_channel_type channel = MSGCH_PLAIN,
                            int param = 0,
                            description_level_type descrip = DESC_THE);
void simple_god_message(const char *event, god_type which_deity = you.religion);
void wu_jian_sifu_message(const char *event);

class formatted_string;

void formatted_mpr(const formatted_string& fs,
                   msg_channel_type channel = MSGCH_PLAIN, int param = 0);

// mpr() an arbitrarily long list of strings
void mpr_comma_separated_list(const string &prefix,
                              const vector<string> &list,
                              const string &andc = ", and ",
                              const string &comma = ", ",
                              const msg_channel_type channel = MSGCH_PLAIN,
                              const int param = 0);

#include "cio.h"

// Sets whether messages that are printed through mpr are
// considered temporary.
void msgwin_set_temporary(bool temp);
// Clear the last set of temporary messages from both
// message window and history.
void msgwin_clear_temporary();

void msgwin_prompt(string prompt);
void msgwin_reply(string reply);

unsigned int msgwin_lines();
unsigned int msgwin_line_length();

// Tell the message window that previous messages may be considered
// read, e.g. after reading input from the player.
void msgwin_got_input();

int msgwin_get_line(string prompt,
                    char *buf, int len,
                    input_history *mh = nullptr,
                    const string &fill = "");

// Do not use this templated function directly. Use the macro below instead.
template<int> static int msgwin_get_line_autohist_temp(string prompt,
                                                       char *buf, int len,
                                                       const string &fill = "")
{
    static input_history hist(10);
    return msgwin_get_line(prompt, buf, len, &hist, fill);
}

// This version of mswgin_get_line will automatically retain its own
// input history, independent of other calls to msgwin_get_line.
#define msgwin_get_line_autohist(...) \
    msgwin_get_line_autohist_temp<__LINE__>(__VA_ARGS__)

// Tell the message window that the game is about to read a new
// command from the player.
void msgwin_new_cmd();
// Tell the message window that a new turn has started.
void msgwin_new_turn();

class no_messages
{
public:
    no_messages();
    no_messages(bool really_suppress);
    ~no_messages();
private:
    bool msuppressed;
};

#ifdef USE_TILE_WEB
void webtiles_send_messages();
void webtiles_send_last_messages(int n = 20);
#endif

void save_messages(writer& outf);
void load_messages(reader& inf);
void clear_message_store();

// Have any messages been printed since the last clear?
bool any_messages();

void replay_messages();

void set_more_autoclear(bool on);

string get_last_messages(int mcount, bool full = false);
void get_recent_messages(vector<string> &messages,
                         vector<msg_channel_type> &channels);

int channel_to_colour(msg_channel_type channel, int param = 0);
bool strip_channel_prefix(string &text, msg_channel_type &channel,
                          bool silence = false);

namespace msg
{
    extern ostream stream;
    ostream& streams(msg_channel_type chan = MSGCH_PLAIN);

    struct setparam
    {
        setparam(int param);
        int m_param;
    };

    struct capitalisation
    {
        capitalisation(bool cap);
        bool m_cap;
    };
    extern capitalisation cap, nocap;

    struct mute
    {
        mute(bool value = true);
        bool m_value;
    };

    class mpr_stream_buf : public streambuf
    {
    public:
        mpr_stream_buf(msg_channel_type chan);
        virtual ~mpr_stream_buf() {}
        void set_param(int p);
        void set_muted(bool m);
        void set_capitalise(bool m);
    protected:
        int overflow(int c);
    private:
        static const int INTERNAL_LENGTH = 500;
        char internal_buf[500]; // if your terminal is wider than this, too bad
        int internal_count;
        int param;
        bool muted;
        bool capitalise;
        msg_channel_type channel;
    };

    void initialise_mpr_streams();
    void deinitialise_mpr_streams();
}

ostream& operator<<(ostream& os, const msg::setparam& sp);
ostream& operator<<(ostream& os, const msg::capitalisation& cap);

void set_msg_dump_file(FILE* file);
