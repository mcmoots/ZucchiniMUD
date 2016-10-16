/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*    ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
*    ROM has been brought to you by the ROM consortium                      *
*        Russ Taylor (rtaylor@hypercube.org)                                *
*        Gabrielle Taylor (gtaylor@hypercube.org)                           *
*        Brian Moore (zump@rom.org)                                         *
*    By using this code, you have agreed to follow the terms of the         *
*    ROM license, in the file Rom24/doc/rom.license                         *
****************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

char	last_command[MSL];

bool check_social args ((CHAR_DATA * ch, char *command, char *argument));

/*
 * Command logging types.
 */
#define LOG_NORMAL  0
#define LOG_ALWAYS  1
#define LOG_NEVER   2



/*
 * Log-all switch.
 */
bool fLogAll = FALSE;



/*
 * Command table.
 */
const struct cmd_type cmd_table[] = {
    /*
     * Common movement commands.
     */
    {"north", do_north, POS_STANDING, 0, LOG_NEVER, 0, CAT_MOVE},
    {"east",  do_east,  POS_STANDING, 0, LOG_NEVER, 0, CAT_MOVE},
    {"south", do_south, POS_STANDING, 0, LOG_NEVER, 0, CAT_MOVE},
    {"west",  do_west,  POS_STANDING, 0, LOG_NEVER, 0, CAT_MOVE},
    {"up",    do_up,    POS_STANDING, 0, LOG_NEVER, 0, CAT_MOVE},
    {"down",  do_down,  POS_STANDING, 0, LOG_NEVER, 0, CAT_MOVE},

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    {"at",        do_at,        POS_DEAD,     L6, LOG_NORMAL, 1, CAT_IMM},
    {"cast",      do_cast,      POS_FIGHTING, 0,  LOG_NORMAL, 1, CAT_MISC},
    {"auction",   do_auction,   POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_COMM},
    {"buy",       do_buy,       POS_RESTING,  0,  LOG_NORMAL, 1, CAT_MISC},
    {"channels",  do_channels,  POS_DEAD,     0,  LOG_NORMAL, 1, CAT_COMM},
    {"exits",     do_exits,     POS_RESTING,  0,  LOG_NORMAL, 1, CAT_INFO},
    {"get",       do_get,       POS_RESTING,  0,  LOG_NORMAL, 1, CAT_OBJ},
    {"goto",      do_goto,      POS_DEAD,     L8, LOG_NORMAL, 1, CAT_MOVE},
    {"group",     do_group,     POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_MISC},
    {"guild",     do_guild,     POS_DEAD,     L4, LOG_ALWAYS, 1, CAT_IMM},
    {"hit",       do_kill,      POS_FIGHTING, 0,  LOG_NORMAL, 0, CAT_FIGHT},
    {"inventory", do_inventory, POS_DEAD,     0,  LOG_NORMAL, 1, CAT_INFO},
    {"kill",      do_kill,      POS_FIGHTING, 0,  LOG_NORMAL, 1, CAT_FIGHT},
    {"look",      do_look,      POS_RESTING,  0,  LOG_NORMAL, 1, CAT_INFO},
    {"clan",      do_clantalk,  POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_COMM},
    {"music",     do_music,     POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_COMM},
    {"order",     do_order,     POS_RESTING,  0,  LOG_NORMAL, 1, CAT_COMM},
    {"practice",  do_practice,  POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_MISC},
    {"quest",	  do_quest,	POS_STANDING, 0,  LOG_NORMAL, 1, CAT_MISC},
    {"rest",      do_rest,      POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_MOVE},
    {"sit",       do_sit,       POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_MOVE},
    {"sockets",   do_sockets,   POS_DEAD,     L4, LOG_NORMAL, 1, CAT_INFO},
    {"stand",     do_stand,     POS_SLEEPING, 0,  LOG_NORMAL, 1, CAT_MOVE},
    {"tell",      do_tell,      POS_RESTING,  0,  LOG_NORMAL, 1, CAT_COMM},
    {"unlock",    do_unlock,    POS_RESTING,  0,  LOG_NORMAL, 1, CAT_OBJ},
    {"wield",     do_wear,      POS_RESTING,  0,  LOG_NORMAL, 1, CAT_OBJ},
    {"wizhelp",   do_wizhelp,   POS_DEAD,     LH, LOG_NORMAL, 1, CAT_INFO},

    /*
     * Informational commands.
     */
    {"affects",   do_affects,   POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"areas",     do_areas,     POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"bug",       do_bug,       POS_DEAD,     0, LOG_NORMAL, 1, CAT_COMM},
    {"board",     do_board,     POS_SLEEPING, 0, LOG_NORMAL, 1, CAT_COMM},
    {"commands",  do_commands,  POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"compare",   do_compare,   POS_RESTING,  0, LOG_NORMAL, 1, CAT_INFO},
    {"consider",  do_consider,  POS_RESTING,  0, LOG_NORMAL, 1, CAT_INFO},
    {"count",     do_count,     POS_SLEEPING, 0, LOG_NORMAL, 1, CAT_INFO},
    {"credits",   do_credits,   POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"equipment", do_equipment, POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"examine",   do_examine,   POS_RESTING,  0, LOG_NORMAL, 1, CAT_INFO},
/*  {"groups",    do_groups,    POS_SLEEPING, 0, LOG_NORMAL, 1, CAT_INFO}, */
    {"help",      do_help,      POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"info",      do_groups,    POS_SLEEPING, 0, LOG_NORMAL, 1, CAT_INFO},
    {"motd",      do_motd,      POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"read",      do_read,      POS_RESTING,  0, LOG_NORMAL, 1, CAT_INFO},
    {"report",    do_report,    POS_RESTING,  0, LOG_NORMAL, 1, CAT_COMM},
    {"rules",     do_rules,     POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"scan",	  do_scan,	POS_STANDING, 0, LOG_NORMAL, 1, CAT_INFO},
    {"score",     do_score,     POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"skills",    do_skills,    POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"socials",   do_socials,   POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"show",      do_show,      POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"spells",    do_spells,    POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"story",     do_story,     POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"time",      do_time,      POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"typo",      do_typo,      POS_DEAD,     0, LOG_NORMAL, 1, CAT_COMM},
    {"weather",   do_weather,   POS_RESTING,  0, LOG_NORMAL, 1, CAT_INFO},
    {"who",       do_who,       POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"whois",     do_whois,     POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"wizlist",   do_wizlist,   POS_DEAD,     0, LOG_NORMAL, 1, CAT_INFO},
    {"worth",     do_worth,     POS_SLEEPING, 0, LOG_NORMAL, 1, CAT_INFO},

    /*
     * Configuration commands.
     */
    {"alia", 		do_alia, 	POS_DEAD, 	0, LOG_NORMAL, 0, CAT_NULL},
    {"alias", 		do_alias, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"autolist", 	do_autolist, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_INFO},
    {"autoall", 	do_autoall, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"autoassist",	do_autoassist, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"autoexit", 	do_autoexit, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"autogold", 	do_autogold, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"autoloot", 	do_autoloot, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"autosac", 	do_autosac, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"autosplit", 	do_autosplit, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"brief", 		do_brief, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_CONFIG},
/*  { "channels",       do_channels,    POS_DEAD,     	0, LOG_NORMAL, 1, CAT_INFO}, */
    {"colour",		do_colour,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"color",		do_colour,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"combine",		do_combine,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"compact",		do_compact,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"description",	do_description,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"delet",		do_delet,	POS_DEAD,	0, LOG_ALWAYS, 0, CAT_NULL},
    {"delete",		do_delete,	POS_STANDING,	0, LOG_ALWAYS, 1, CAT_MISC},
    {"nofollow",	do_nofollow,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"noloot",		do_noloot,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"nosummon",	do_nosummon,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"outfit",		do_outfit,	POS_RESTING,	0, LOG_NORMAL, 1, CAT_MISC},
    {"password",	do_password,	POS_DEAD,	0, LOG_NEVER,  1, CAT_CONFIG},
    {"prompt",		do_prompt,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"scroll",		do_scroll,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"telnetga",	do_telnetga,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"title",		do_title,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"unalias",		do_unalias,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},
    {"wimpy",		do_wimpy,	POS_DEAD,	0, LOG_NORMAL, 1, CAT_CONFIG},

    /*
     * Communication commands.
     */
    {"afk",		do_afk,		POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"answer", 		do_answer, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
/*  { "auction",    	do_auction,    	POS_SLEEPING,   0, LOG_NORMAL,	1, CAT_COMM}, */
    {"deaf", 		do_deaf, 	POS_DEAD, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"emote", 		do_emote, 	POS_RESTING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"pmote", 		do_pmote, 	POS_RESTING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {".", 		do_gossip, 	POS_SLEEPING, 	0, LOG_NORMAL,	0, CAT_COMM},
    {"gossip", 		do_gossip, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {",", 		do_emote, 	POS_RESTING, 	0, LOG_NORMAL, 	0, CAT_COMM},
    {"grats", 		do_grats, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"gtell", 		do_gtell, 	POS_DEAD, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {";", 		do_gtell, 	POS_DEAD, 	0, LOG_NORMAL, 	0, CAT_COMM},
/*  { "music",        	do_music,    	POS_SLEEPING,   0, LOG_NORMAL, 	1, CAT_COMM}, */
    {"note", 		do_note, 	POS_DEAD, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"pose", 		do_pose, 	POS_RESTING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"question", 	do_question, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"quote", 		do_quote, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"quiet", 		do_quiet, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"reply", 		do_reply, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"replay", 		do_replay, 	POS_SLEEPING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"rp",		do_rp,		POS_SLEEPING,	0, LOG_NORMAL,	1, CAT_CONFIG},
    {"say", 		do_say, 	POS_RESTING, 	0, LOG_NORMAL, 	1, CAT_COMM},
    {"'", 		do_say, 	POS_RESTING, 	0, LOG_NORMAL, 	0, CAT_COMM},
    {"shout", 		do_shout, 	POS_RESTING, 	3, LOG_NORMAL, 	1, CAT_COMM},
    {"whisper",		do_whisper,	POS_RESTING,	0, LOG_NORMAL,  1, CAT_COMM},
    {"yell", 		do_yell, 	POS_RESTING, 	0, LOG_NORMAL, 	1, CAT_COMM},

    /*
     * Object manipulation commands.
     */
    {"balance",		do_balance,	POS_STANDING,	0, LOG_NORMAL, 1, CAT_INFO},
    {"brandish", 	do_brandish, POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"butcher", 	do_butcher,	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"close", 		do_close, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"deposit",		do_deposit,	POS_STANDING,	0, LOG_NORMAL, 1, CAT_OBJ},
    {"donate",		do_donate,	POS_RESTING,	0, LOG_NORMAL, 1, CAT_OBJ},
    {"drink", 		do_drink, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"drop", 		do_drop, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"eat", 		do_eat, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"envenom", 	do_envenom, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"fill", 		do_fill, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"give", 		do_give, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"heal", 		do_heal, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_MISC},
    {"hold", 		do_wear, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"list", 		do_list, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_INFO},
    {"lock", 		do_lock, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"open", 		do_open, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"pick", 		do_pick, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"pour", 		do_pour, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"put", 		do_put, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"quaff", 		do_quaff, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"recite", 		do_recite, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"remove", 		do_remove, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"sell", 		do_sell, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"take", 		do_get, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"sacrifice", 	do_sacrifice, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"junk", 		do_sacrifice, 	POS_RESTING, 	0, LOG_NORMAL, 0, CAT_OBJ},
    {"tap", 		do_sacrifice, 	POS_RESTING, 	0, LOG_NORMAL, 0, CAT_OBJ},
/*  {"unlock",        	do_unlock,    	POS_RESTING,    0, LOG_NORMAL, 1, CAT_OBJ}, */
    {"value", 		do_value, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_INFO},
    {"vomit",		do_vomit,	POS_RESTING,	0, LOG_NORMAL, 1, CAT_OBJ},
    {"wear", 		do_wear, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"withdraw", 	do_withdraw,	POS_STANDING,	0, LOG_NORMAL, 1, CAT_OBJ},
    {"zap", 		do_zap, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},

    /*
     * Combat commands.
     */
    {"backstab", 	do_backstab, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"bash", 		do_bash, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"bs", 		do_backstab, 	POS_FIGHTING, 0, LOG_NORMAL, 0, CAT_FIGHT},
    {"berserk", 	do_berserk, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"dirt", 		do_dirt, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"disarm", 		do_disarm, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"flee", 		do_flee, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"kick", 		do_kick, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"murde", 		do_murde, 	POS_FIGHTING, 0, LOG_NORMAL, 0, CAT_NULL},
    {"murder", 		do_murder, 	POS_FIGHTING, 5, LOG_ALWAYS, 1, CAT_FIGHT},
    {"rescue", 		do_rescue, 	POS_FIGHTING, 0, LOG_NORMAL, 0, CAT_FIGHT},
    {"surrender", 	do_surrender, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"tackle",		do_tackle,	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},
    {"trip", 		do_trip, 	POS_FIGHTING, 0, LOG_NORMAL, 1, CAT_FIGHT},

    /*
     * Mob command interpreter (placed here for faster scan...)
     */
    {"mob", do_mob, POS_DEAD, 0, LOG_NEVER, 0, CAT_NULL},

    /*
     * Miscellaneous commands.
     */
    {"enter", 		do_enter, 	POS_STANDING, 	0, LOG_NORMAL, 1, CAT_MOVE},
    {"follow", 		do_follow, 	POS_RESTING,  	0, LOG_NORMAL, 1, CAT_MOVE},
    {"gain", 		do_gain, 	POS_STANDING, 	0, LOG_NORMAL, 1, CAT_MISC},
    {"go", 		do_enter, 	POS_STANDING, 	0, LOG_NORMAL, 0, CAT_MOVE},
/*  {"group",        	do_group,    	POS_SLEEPING, 	0, LOG_NORMAL, 1, CAT_INFO}, */
    {"groups", 		do_groups, 	POS_SLEEPING, 	0, LOG_NORMAL, 1, CAT_INFO},
    {"heel",		do_heel,	POS_SITTING,	0, LOG_NORMAL, 1, CAT_MISC},
    {"hide", 		do_hide, 	POS_RESTING,  	0, LOG_NORMAL, 1, CAT_MOVE},
    {"play", 		do_play, 	POS_RESTING,  	0, LOG_NORMAL, 1, CAT_MISC},
/*  {"practice",    	do_practice,    POS_SLEEPING, 	0, LOG_NORMAL, 1, CAT_MISC}, */
    {"qui", 		do_qui, 	POS_DEAD, 	0, LOG_NORMAL, 0, CAT_NULL},
    {"quit", 		do_quit, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_MISC},
    {"recall", 		do_recall, 	POS_FIGHTING, 	0, LOG_NORMAL, 1, CAT_MOVE},
    {"/", 		do_recall, 	POS_FIGHTING, 	0, LOG_NORMAL, 0, CAT_MOVE},
    {"rent", 		do_rent, 	POS_DEAD, 	0, LOG_NORMAL, 0, CAT_NULL},
    {"save", 		do_save, 	POS_DEAD, 	0, LOG_NORMAL, 1, CAT_MISC},
    {"sleep", 		do_sleep, 	POS_SLEEPING, 	0, LOG_NORMAL, 1, CAT_MOVE},
    {"sneak", 		do_sneak, 	POS_STANDING, 	0, LOG_NORMAL, 1, CAT_MOVE},
    {"split", 		do_split, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"steal", 		do_steal, 	POS_STANDING, 	0, LOG_NORMAL, 1, CAT_OBJ},
    {"study",		do_study,	POS_SITTING,	0, LOG_NORMAL, 1, CAT_OBJ},
    {"train", 		do_train, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_MISC},
    {"visible", 	do_visible, 	POS_SLEEPING, 	0, LOG_NORMAL, 1, CAT_MISC},
    {"wake", 		do_wake, 	POS_SLEEPING, 	0, LOG_NORMAL, 1, CAT_MOVE},
    {"where", 		do_where, 	POS_RESTING, 	0, LOG_NORMAL, 1, CAT_INFO},



    /*
     * Immortal commands.
     */
    {"advance",		do_advance,	POS_DEAD, ML, LOG_ALWAYS, 1, CAT_IMM},
    {"arealinks",	do_arealinks,	POS_DEAD, ML, LOG_NORMAL, 1, CAT_IMM},
    {"brainlist", 	do_brainlist,	POS_DEAD, ML, LOG_NORMAL, 1, CAT_IMM},
    {"copyover",	do_copyover,	POS_DEAD, ML, LOG_ALWAYS, 1, CAT_IMM},
    {"dump",		do_dump,	POS_DEAD, ML, LOG_ALWAYS, 0, CAT_IMM},
    {"helpcheck",	do_helpcheck,	POS_DEAD, L5, LOG_NORMAL, 1, CAT_IMM},
    {"trust",		do_trust,	POS_DEAD, ML, LOG_ALWAYS, 1, CAT_IMM},
    {"violate",		do_violate,	POS_DEAD, ML, LOG_ALWAYS, 1, CAT_IMM},

    {"allow",		do_allow,	POS_DEAD, L2, LOG_ALWAYS, 1, CAT_IMM},
    {"ban",		do_ban,		POS_DEAD, L2, LOG_ALWAYS, 1, CAT_IMM},
    {"deny",		do_deny,	POS_DEAD, L1, LOG_ALWAYS, 1, CAT_IMM},
    {"disconnect",	do_disconnect,	POS_DEAD, L3, LOG_ALWAYS, 1, CAT_IMM},
    {"flag",		do_flag,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_IMM},
    {"freeze",		do_freeze,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_IMM},
    {"permban",		do_permban,	POS_DEAD, L1, LOG_ALWAYS, 1, CAT_IMM},
    {"protect", 	do_protect,	POS_DEAD, L7, LOG_NORMAL, 1, CAT_IMM},
    {"reboo",		do_reboo,	POS_DEAD, L1, LOG_NORMAL, 0, CAT_NULL},
    {"reboot",		do_reboot,	POS_DEAD, L1, LOG_ALWAYS, 1, CAT_IMM},
    {"set",		do_set,		POS_DEAD, L2, LOG_ALWAYS, 1, CAT_IMM},
    {"shutdow",		do_shutdow,	POS_DEAD, L1, LOG_NORMAL, 0, CAT_NULL},
    {"shutdown",	do_shutdown,	POS_DEAD, L1, LOG_ALWAYS, 1, CAT_IMM},
    {"snoopproof",	do_snoopproof,	POS_DEAD, L1, LOG_ALWAYS, 1, CAT_IMM},
/*  {"sockets",    	do_sockets,    POS_DEAD,  L4, LOG_NORMAL, 1, CAT_INFO}, */
    {"wizlock",		do_wizlock,	POS_DEAD, L2, LOG_ALWAYS, 1, CAT_IMM},

    {"force",		do_force,	POS_DEAD, L7, LOG_ALWAYS, 1, CAT_IMM},
    {"load",		do_load,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_IMM},
    {"newlock",		do_newlock,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_IMM},
    {"nochannels",	do_nochannels,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},
    {"noemote",		do_noemote,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},
    {"noshout",		do_noshout,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},
    {"notell",		do_notell,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},
    {"pecho",		do_pecho,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_COMM},
    {"pardon",		do_pardon,	POS_DEAD, L3, LOG_ALWAYS, 1, CAT_IMM},
    {"purge",		do_purge,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_IMM},
    {"qmconfig",	do_qmconfig,	POS_DEAD, ML, LOG_ALWAYS, 1, CAT_IMM},
    {"restore",		do_restore,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_IMM},
    {"sla",		do_sla,		POS_DEAD, L3, LOG_NORMAL, 0, CAT_NULL},
    {"slay",		do_slay,	POS_DEAD, L3, LOG_ALWAYS, 1, CAT_IMM},
    {"teleport",	do_transfer,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},
    {"transfer",	do_transfer,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},

/*  {"at",        	do_at,        	POS_DEAD, L6, LOG_NORMAL, 1, CAT_IMM}, */
    {"award",		do_award,	POS_DEAD, L7, LOG_NORMAL, 1, CAT_IMM},
    {"poofin",		do_bamfin,	POS_DEAD, L8, LOG_NORMAL, 1, CAT_CONFIG},
    {"poofout",		do_bamfout,	POS_DEAD, L8, LOG_NORMAL, 1, CAT_CONFIG},
    {"gecho",		do_echo,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_COMM},
/*  {"goto",        	do_goto,    	POS_DEAD, L8, LOG_NORMAL, 1, CAT_MOVE}, */
    {"holylight",	do_holylight,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_CONFIG},
    {"immtitle",	do_immtitle,	POS_DEAD, LH, LOG_NORMAL, 1, CAT_CONFIG},
    {"incognito",	do_incognito,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_CONFIG},
    {"invis",		do_invis,	POS_DEAD, IM, LOG_NORMAL, 0, CAT_CONFIG},
    {"log",		do_log,		POS_DEAD, L1, LOG_ALWAYS, 1, CAT_IMM},
    {"memory",		do_memory,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_INFO},
    {"mlevel",		do_mlevel,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_INFO},
    {"mwhere",		do_mwhere,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_INFO},
    {"olevel",		do_olevel,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_INFO},
    {"owhere",		do_owhere,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_INFO},
    {"peace",		do_peace,	POS_DEAD, L5, LOG_NORMAL, 1, CAT_IMM},
    {"echo",		do_recho,	POS_DEAD, L6, LOG_ALWAYS, 1, CAT_COMM},
    {"return",		do_return,	POS_DEAD, L6, LOG_NORMAL, 1, CAT_IMM},
    {"snoop",		do_snoop,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},
    {"stat",		do_stat,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_IMM},
    {"string",		do_string,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},
    {"switch",		do_switch,	POS_DEAD, L6, LOG_ALWAYS, 1, CAT_IMM},
    {"wizinvis",	do_invis,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_CONFIG},
    {"vnum",		do_vnum,	POS_DEAD, L4, LOG_NORMAL, 1, CAT_INFO},
    {"zecho",		do_zecho,	POS_DEAD, L4, LOG_ALWAYS, 1, CAT_COMM},

    {"clone",		do_clone,	POS_DEAD, L5, LOG_ALWAYS, 1, CAT_IMM},

    {"wiznet",		do_wiznet,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_IMM},
    {"immtalk",		do_immtalk,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_COMM},
    {"imotd",		do_imotd,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_INFO},
    {":",		do_immtalk,	POS_DEAD, IM, LOG_NORMAL, 0, CAT_COMM},
    {"smote",		do_smote,	POS_DEAD, LH, LOG_NORMAL, 1, CAT_IMM},
    {"prefi",		do_prefi,	POS_DEAD, IM, LOG_NORMAL, 0, CAT_NULL},
    {"prefix",		do_prefix,	POS_DEAD, IM, LOG_NORMAL, 1, CAT_IMM},
    {"mpdump",		do_mpdump,	POS_DEAD, IM, LOG_NEVER,  1, CAT_IMM},
    {"mpstat",		do_mpstat,	POS_DEAD, IM, LOG_NEVER,  1, CAT_IMM},

    /*
     * OLC
     */
    {"edit",		do_olc, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"asave", 		do_asave, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"alist", 		do_alist, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"resets", 		do_resets, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"redit", 		do_redit, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"medit", 		do_medit, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"aedit", 		do_aedit, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"oedit", 		do_oedit, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"mpedit", 		do_mpedit, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"hedit", 		do_hedit, 	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},
    {"stocksave",	do_stocksave,	POS_DEAD, 0, LOG_NORMAL, 1, CAT_IMM},

    /*
     * End of list.
     */
    {"", 0, POS_DEAD, 0, LOG_NORMAL, 0, CAT_NULL}
};




/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret (CHAR_DATA * ch, char *argument)
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    int cmd;
    int trust;
    bool found;

    /*
     * Strip leading spaces.
     */
    while (isspace (*argument))
        argument++;
    if (argument[0] == '\0')
        return;

    /*
     * No hiding.
     */
    REMOVE_BIT (ch->affected_by, AFF_HIDE);

    /*
     * Implement freeze command.
     */
    if (!IS_NPC (ch) && IS_SET (ch->act, PLR_FREEZE))
    {
        send_to_char ("You're totally frozen!\n\r", ch);
        return;
    }

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     * also no spaces needed after punctuation.
     */
    strcpy (logline, argument);
    
    /* Let's see who is doing what */
    sprintf(last_command, "%s in room [%d]: %s", 
    		ch->name, ch->in_room->vnum, logline);
		
    if (!isalpha (argument[0]) && !isdigit (argument[0]))
    {
        command[0] = argument[0];
        command[1] = '\0';
        argument++;
        while (isspace (*argument))
            argument++;
    }
    else
    {
        argument = one_argument (argument, command);
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust (ch);
    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
    {
        if (command[0] == cmd_table[cmd].name[0]
            && !str_prefix (command, cmd_table[cmd].name)
            && cmd_table[cmd].level <= trust)
        {
            found = TRUE;
            break;
        }
    }

    /*
     * Log and snoop.
     */
    smash_dollar(logline);
    
    if (cmd_table[cmd].log == LOG_NEVER)
        strcpy (logline, "");

	/* Replaced original block of code with fix from Edwin
	 * to prevent crashes due to dollar signs in logstrings.
	 * I threw in the above call to smash_dollar() just for
	 * the sake of overkill :) JR -- 10/15/00
	 */
    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
		||   fLogAll
		||   cmd_table[cmd].log == LOG_ALWAYS )
	{
    	char    s[2*MAX_INPUT_LENGTH],*ps;
    	int     i;

    	ps=s; 
    	sprintf( log_buf, "Log %s: %s", ch->name, logline );
    	/* Make sure that was is displayed is what is typed */
    	for (i=0;log_buf[i];i++)
    	{ 
			*ps++=log_buf[i];  
			if (log_buf[i]=='$')
	    		*ps++='$';
			if (log_buf[i]=='{')
	    		*ps++='{';
    	}
    	*ps=0;
    	wiznet(s,ch,NULL,WIZ_SECURE,0,get_trust(ch));
    	log_string( log_buf );
	}

    if (ch->desc != NULL && ch->desc->snoop_by != NULL)
    {
        write_to_buffer (ch->desc->snoop_by, "% ", 2);
        write_to_buffer (ch->desc->snoop_by, logline, 0);
        write_to_buffer (ch->desc->snoop_by, "\n\r", 2);
    }

    if (!found)
    {
        /*
         * Look for command in socials table.
         */
        if (!check_social (ch, command, argument)
#ifdef IMC
		&& !imc_command_hook( ch, command, argument )
#endif
#ifdef I3
		&& !I3_command_hook(ch, command, argument)
#endif
		)
            send_to_char ("Huh?\n\r", ch);
        return;
    }

    /*
     * Character not in position for command?
     */
    if (ch->position < cmd_table[cmd].position)
    {
        switch (ch->position)
        {
            case POS_DEAD:
                send_to_char ("Lie still; you are DEAD.\n\r", ch);
                break;

            case POS_MORTAL:
            case POS_INCAP:
                send_to_char ("You are hurt far too bad for that.\n\r", ch);
                break;

            case POS_STUNNED:
                send_to_char ("You are too stunned to do that.\n\r", ch);
                break;

            case POS_SLEEPING:
                send_to_char ("In your dreams, or what?\n\r", ch);
                break;

            case POS_RESTING:
                send_to_char ("Nah... You feel too relaxed...\n\r", ch);
                break;

            case POS_SITTING:
                send_to_char ("Better stand up first.\n\r", ch);
                break;

            case POS_FIGHTING:
                send_to_char ("No way!  You are still fighting!\n\r", ch);
                break;

        }
        return;
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) (ch, argument);

    tail_chain ();
    return;
}

/* function to keep argument safe in all commands -- no static strings */
void do_function (CHAR_DATA * ch, DO_FUN * do_fun, char *argument)
{
    char *command_string;

    /* copy the string */
    command_string = str_dup (argument);

    /* dispatch the command */
    (*do_fun) (ch, command_string);

    /* free the string */
    free_string (command_string);
}

bool check_social (CHAR_DATA * ch, char *command, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found = FALSE;
    for (cmd = 0; social_table[cmd].name[0] != '\0'; cmd++)
    {
        if (command[0] == social_table[cmd].name[0]
            && !str_prefix (command, social_table[cmd].name))
        {
            found = TRUE;
            break;
        }
    }

    if (!found)
        return FALSE;

    if (!IS_NPC (ch) && IS_SET (ch->comm, COMM_NOEMOTE))
    {
        send_to_char ("You are anti-social!\n\r", ch);
        return TRUE;
    }

    switch (ch->position)
    {
        case POS_DEAD:
            send_to_char ("Lie still; you are DEAD.\n\r", ch);
            return TRUE;

        case POS_INCAP:
        case POS_MORTAL:
            send_to_char ("You are hurt far too bad for that.\n\r", ch);
            return TRUE;

        case POS_STUNNED:
            send_to_char ("You are too stunned to do that.\n\r", ch);
            return TRUE;

        case POS_SLEEPING:
            /*
             * I just know this is the path to a 12" 'if' statement.  :(
             * But two players asked for it already!  -- Furey
             */
            if (!str_cmp (social_table[cmd].name, "snore"))
                break;
            send_to_char ("In your dreams, or what?\n\r", ch);
            return TRUE;

    }

    one_argument (argument, arg);
    victim = NULL;
    if (arg[0] == '\0')
    {
        act (social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM);
        act (social_table[cmd].char_no_arg, ch, NULL, victim, TO_CHAR);
    }
    else if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
    }
    else if (victim == ch)
    {
        act (social_table[cmd].others_auto, ch, NULL, victim, TO_ROOM);
        act (social_table[cmd].char_auto, ch, NULL, victim, TO_CHAR);
    }
    else
    {
        act (social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT);
        act (social_table[cmd].char_found, ch, NULL, victim, TO_CHAR);
        act (social_table[cmd].vict_found, ch, NULL, victim, TO_VICT);

        if (!IS_NPC (ch) && IS_NPC (victim)
            && !IS_AFFECTED (victim, AFF_CHARM)
            && IS_AWAKE (victim) && victim->desc == NULL)
        {
            switch (number_bits (4))
            {
                case 0:

                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    act (social_table[cmd].others_found,
                         victim, NULL, ch, TO_NOTVICT);
                    act (social_table[cmd].char_found, victim, NULL, ch,
                         TO_CHAR);
                    act (social_table[cmd].vict_found, victim, NULL, ch,
                         TO_VICT);
                    break;

                case 9:
                case 10:
                case 11:
                case 12:
                    act ("$n slaps $N.", victim, NULL, ch, TO_NOTVICT);
                    act ("You slap $N.", victim, NULL, ch, TO_CHAR);
                    act ("$n slaps you.", victim, NULL, ch, TO_VICT);
                    break;
            }
        }
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number (char *arg)
{

    if (*arg == '\0')
        return FALSE;

    if (*arg == '+' || *arg == '-')
        arg++;

    for (; *arg != '\0'; arg++)
    {
        if (!isdigit (*arg))
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument (char *argument, char *arg)
{
    char *pdot;
    int number;

    for (pdot = argument; *pdot != '\0'; pdot++)
    {
        if (*pdot == '.')
        {
            *pdot = '\0';
            number = atoi (argument);
            *pdot = '.';
            strcpy (arg, pdot + 1);
            return number;
        }
    }

    strcpy (arg, argument);
    return 1;
}

/* 
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument (char *argument, char *arg)
{
    char *pdot;
    int number;

    for (pdot = argument; *pdot != '\0'; pdot++)
    {
        if (*pdot == '*')
        {
            *pdot = '\0';
            number = atoi (argument);
            *pdot = '*';
            strcpy (arg, pdot + 1);
            return number;
        }
    }

    strcpy (arg, argument);
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument (char *argument, char *arg_first)
{
    char cEnd;

    while (isspace (*argument))
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"')
        cEnd = *argument++;

    while (*argument != '\0')
    {
        if (*argument == cEnd)
        {
            argument++;
            break;
        }
        *arg_first = LOWER (*argument);
        arg_first++;
        argument++;
    }
    *arg_first = '\0';

    while (isspace (*argument))
        argument++;

    return argument;
}

/*
 * Contributed by Alander.
 */
void do_commands (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
    int category;

    col = 0;

    for( category = 0; category < MAX_CATEGORY; category++ )
    {
    	switch(category)
	{
	case CAT_IMM:
		if( get_trust(ch) >= LEVEL_HERO )
		    send_to_char("{g*** Immortal Commands ***{x\n\r", ch);
		break;
	case CAT_COMM:
		send_to_char("{g*** Communication Commands ***{x\n\r", ch);
		break;
	case CAT_INFO:
		send_to_char("{g*** Information Commands ***{x\n\r", ch);
		break;
	case CAT_CONFIG:
		send_to_char("{g*** Configuration Commands ***{x\n\r", ch);
		break;
	case CAT_MOVE:
		send_to_char("{g*** Movement Commands ***{x\n\r", ch);
		break;
	case CAT_OBJ:
		send_to_char("{g*** Object Manipulation Commands ***{x\n\r", ch);
		break;
	case CAT_FIGHT:
		send_to_char("{g*** Combat Commands ***{x\n\r", ch);
		break;
	case CAT_MISC:
		send_to_char("{g*** Miscellaneous Commands ***{x\n\r", ch);
		break;
	}
	
	for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if( cmd_table[cmd].cat == category && cmd_table[cmd].level <= get_trust(ch) && cmd_table[cmd].show )
		  {
		    sprintf( buf, "  %-11s", cmd_table[cmd].name );
		    send_to_char( buf, ch );
		    if( ++col %6 == 0 )
		       {
		  	send_to_char( "\n\r", ch);
		       }
		  }
	}
	
	if( col %6 != 0 )
	  {
	   send_to_char( "\n\r", ch);
	   col = 0;
	  }
    }

    if (col % 6 != 0)
        send_to_char ("\n\r", ch);
    return;
}

void do_wizhelp (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
    int clevel;

    col = 0;
    for( clevel = LEVEL_HERO; clevel < MAX_LEVEL + 1; clevel++ )
    {
	sprintf( buf, "{g*** Level %d ***{x\n\r", clevel );
	send_to_char( buf, ch );
	
	if( get_trust(ch) < clevel )
		return;
	
	for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
	    {
        	if (cmd_table[cmd].level == clevel && cmd_table[cmd].show)
        	{
            	sprintf (buf, "%-12s", cmd_table[cmd].name);
            	send_to_char (buf, ch);
            	if (++col % 6 == 0)
                	send_to_char ("\n\r", ch);
        	}
	    }

	if (col % 6 != 0)
	{
		send_to_char ("\n\r", ch);
		col = 0;
	}
    }
    
    if( col %6 != 0)
    	send_to_char("\n\r", ch);

    return;
}
