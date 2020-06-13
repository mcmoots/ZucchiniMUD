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
 **************************************************************************/

/***************************************************************************
 *   ROM 2.4 is copyright 1993-1998 Russ Taylor                            *
 *   ROM has been brought to you by the ROM consortium                     *
 *       Russ Taylor (rtaylor@hypercube.org)                               *
 *       Gabrielle Taylor (gtaylor@hypercube.org)                          *
 *       Brian Moore (zump@rom.org)                                        *
 *   By using this code, you have agreed to follow the terms of the        *
 *   ROM license, in the file Rom24/doc/rom.license                        *
 **************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"


/* item type list */
const struct item_type item_table[] = {
    {ITEM_LIGHT, "light"},
    {ITEM_SCROLL, "scroll"},
    {ITEM_WAND, "wand"},
    {ITEM_STAFF, "staff"},
    {ITEM_WEAPON, "weapon"},
    {ITEM_TREASURE, "treasure"},
    {ITEM_ARMOR, "armor"},
    {ITEM_POTION, "potion"},
    {ITEM_CLOTHING, "clothing"},
    {ITEM_FURNITURE, "furniture"},
    {ITEM_TRASH, "trash"},
    {ITEM_BRAINS, "brains"},
    {ITEM_CONTAINER, "container"},
    {ITEM_DRINK_CON, "drink"},
    {ITEM_KEY, "key"},
    {ITEM_FOOD, "food"},
    {ITEM_MONEY, "money"},
    {ITEM_BOAT, "boat"},
    {ITEM_CORPSE_NPC, "npc_corpse"},
    {ITEM_CORPSE_PC, "pc_corpse"},
    {ITEM_FOUNTAIN, "fountain"},
    {ITEM_PILL, "pill"},
    {ITEM_PROTECT, "protect"},
    {ITEM_MAP, "map"},
    {ITEM_PORTAL, "portal"},
    {ITEM_WARP_STONE, "warp_stone"},
    {ITEM_ROOM_KEY, "room_key"},
    {ITEM_GEM, "gem"},
    {ITEM_JEWELRY, "jewelry"},
    {ITEM_JUKEBOX, "jukebox"},
    {0, NULL}
};

/* the god table */
const struct god_type god_table [] = 
{
  /* {name, title} */
  
  {"Nobody", "the Not There"},
  {"Santa", "Claus"},
  {"Money", "Money Money"},
  {"Allah", "the Compassionate, the Merciful"},
  {"Christ", "the Redeemer"},
  {"Amaterasu", "the {YSun Goddess{x"},
  {"Eris", "Goddess of Discord"},
  {"Sobek", "the Crocodile God"},
  {"Xena", "Warrior Princess"},
  {"", ""},
  {NULL, NULL}
};

/* weapon selection table */
const struct weapon_type weapon_table[] = {
    {"sword", WEAPON_SWORD, &gsn_sword},
    {"mace", WEAPON_MACE, &gsn_mace},
    {"dagger", WEAPON_DAGGER, &gsn_dagger},
    {"axe", WEAPON_AXE, &gsn_axe},
    {"staff", WEAPON_SPEAR, &gsn_spear},
    {"flail", WEAPON_FLAIL, &gsn_flail},
    {"whip", WEAPON_WHIP, &gsn_whip},
    {"polearm", WEAPON_POLEARM, &gsn_polearm},
    {"hand", WEAPON_HAND, &gsn_hand_to_hand},
    {NULL, 0, NULL}
};

/* vnums of newbie weapons */
const int newbie_weapon_table[] = { 3700, 3701, 3702 };
const int newbie_candy_table[] = { 3724, 3724, 3725, 3726, 3727, 3728, 3729, 3730, 3731 };

/* wiznet table and prototype for future flag setting */
const struct wiznet_type wiznet_table[] = {
    {"on", WIZ_ON, IM},
    {"prefix", WIZ_PREFIX, IM},
    {"ticks", WIZ_TICKS, IM},
    {"logins", WIZ_LOGINS, IM},
    {"sites", WIZ_SITES, L4},
    {"links", WIZ_LINKS, L7},
    {"newbies", WIZ_NEWBIE, IM},
    {"spam", WIZ_SPAM, L5},
    {"deaths", WIZ_DEATHS, IM},
    {"resets", WIZ_RESETS, L4},
    {"mobdeaths", WIZ_MOBDEATHS, L4},
    {"flags", WIZ_FLAGS, L5},
    {"penalties", WIZ_PENALTIES, L5},
    {"saccing", WIZ_SACCING, L5},
    {"levels", WIZ_LEVELS, IM},
    {"load", WIZ_LOAD, L2},
    {"restore", WIZ_RESTORE, L2},
    {"snoops", WIZ_SNOOPS, L2},
    {"switches", WIZ_SWITCHES, L2},
    {"secure", WIZ_SECURE, L1},
    {NULL, 0, 0}
};

/* attack table  */
const struct attack_type attack_table[MAX_DAMAGE_MESSAGE] = {
    {"none", "hit", -1}, 

    {"digestion", "digestion", DAM_ACID},
    {"acbite", "acidic bite", DAM_ACID},
    {"slime", "slime", DAM_ACID},
    
    {"blast", "blast", DAM_BASH},
    {"pound", "pound", DAM_BASH},
    {"crush", "crush", DAM_BASH},
    {"suction", "suction", DAM_BASH},
    {"beating", "beating", DAM_BASH},
    {"charge", "charge", DAM_BASH},    
    {"slap", "slap", DAM_BASH},
    {"punch", "punch", DAM_BASH},
    {"smash", "smash", DAM_BASH},
    {"thwack", "thwack", DAM_BASH},
    
    {"chill", "chill", DAM_COLD},
    {"frbite", "freezing bite", DAM_COLD}, 
        
    {"wrath", "wrath", DAM_ENERGY},
    {"magic", "magic", DAM_ENERGY},
    {"edrain", "energy drain", DAM_ENERGY},
    {"woo", "New Age woo", DAM_ENERGY},

    {"flbite", "flaming bite", DAM_FIRE},
    {"flame", "flame", DAM_FIRE},

    {"divine", "divine power", DAM_HOLY},
	{"gkarma", "good karma", DAM_HOLY},
	{"pompous", "pomposity", DAM_HOLY},
	{"righteous", "self-righteousness", DAM_HOLY},
	
    {"razzle", "razzle", DAM_LIGHT},
    {"dazzle", "dazzle", DAM_LIGHT},
    {"laser", "laser beam", DAM_LIGHT},

    {"shbite", "shocking bite", DAM_LIGHTNING},
    {"shock", "shock", DAM_LIGHTNING},
    {"zap", "zap", DAM_LIGHTNING},
    
    {"psyblast", "psychic blast", DAM_MENTAL},
    {"mdrain", "mind drain", DAM_MENTAL},
    {"laugh", "laugh", DAM_MENTAL},
	{"drama", "drama circus", DAM_MENTAL},
            
    {"drain", "life drain", DAM_NEGATIVE},
    {"karma", "bad karma", DAM_NEGATIVE},
    {"bvibe", "bad vibe", DAM_NEGATIVE},
    {"lawsuit", "lawsuit", DAM_NEGATIVE},

    {"chomp", "chomp", DAM_PIERCE},
    {"stab", "stab", DAM_PIERCE},       
    {"bite", "bite", DAM_PIERCE},    
    {"pierce", "pierce", DAM_PIERCE},
    {"scratch", "scratch", DAM_PIERCE},
    {"peck", "peck", DAM_PIERCE},
    {"peckb", "peck", DAM_BASH},
    {"sting", "sting", DAM_PIERCE},
    {"thrust", "thrust", DAM_PIERCE},
    
    {"slice", "slice", DAM_SLASH},
    {"slash", "slash", DAM_SLASH},
    {"whip", "whip", DAM_SLASH},
    {"claw", "claw", DAM_SLASH},    
    {"grep", "grep", DAM_SLASH},    
    {"cleave", "cleave", DAM_SLASH},
    {"chop", "chop", DAM_SLASH},
    
    {"yodel", "yodel", DAM_SOUND},
    {"yell", "yell", DAM_SOUND},
    {"scream", "scream", DAM_SOUND},
    {"squeak","squeak", DAM_SOUND},
       

    {NULL, NULL, 0}
};

/* race table */
const struct race_type race_table[] = {
/*
    {
    name,        pc_race?,
    act bits,    aff_by bits,    off bits,
    imm,        res,        vuln,
    form,        parts 
    },
*/
    {"unique", FALSE, 0, 0, 0, 0, 0, 0, 0, 0},

    {
     "human", TRUE,
     0, 0, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "elf", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_CHARM, VULN_IRON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},
     
    {
     "half-elf", TRUE,
     0, 0, 0,
     0, RES_CHARM, VULN_IRON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "dwarf", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_POISON | RES_DISEASE, VULN_DROWNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "giant", TRUE,
     0, 0, 0,
     0, RES_FIRE | RES_COLD, VULN_MENTAL | VULN_LIGHTNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},
     
    {
     "half-giant", TRUE,
     0, 0, 0,
     0, RES_FIRE | RES_COLD, VULN_MENTAL | VULN_LIGHTNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},
     
    {
     "robot", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_SLASH, VULN_ACID | VULN_LIGHTNING,
     E | H | J | M | cc, A | B | C | E | G | H | I | J | K | X},
     
    {
     "pixie", TRUE,
     0, AFF_FLYING, OFF_FADE,
     0, RES_MAGIC, VULN_SUMMON | VULN_MENTAL,
     A | C | H | M | V, A | B | C | D | E | F | G | H | I | J | K | P},
     
    {
     "hooloovoo", TRUE,
     0, AFF_FLYING | AFF_PASS_DOOR, OFF_FADE,
     0, RES_WEAPON | RES_BASH, VULN_MENTAL | VULN_MAGIC | VULN_ENERGY | VULN_LIGHT,
     C | H | K | cc, E | F},

    {
     "bat", FALSE,
     0, AFF_FLYING | AFF_DARK_VISION, OFF_DODGE | OFF_FAST,
     0, 0, VULN_LIGHT,
     A | G | V, A | C | D | E | F | H | J | K | P},

    {
     "bear", FALSE,
     0, 0, OFF_CRUSH | OFF_DISARM | OFF_BERSERK,
     0, RES_BASH | RES_COLD, 0,
     A | G | V, A | B | C | D | E | F | H | J | K | U | V},

    {
     "cat", FALSE,
     0, AFF_DARK_VISION, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | Q | U | V},

    {
     "centipede", FALSE,
     0, AFF_DARK_VISION, 0,
     0, RES_PIERCE | RES_COLD, VULN_BASH,
     A | B | G | O, A | C | K},

    {
     "dog", FALSE,
     0, 0, OFF_FAST,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | U | V},

    {
     "doll", FALSE,
     0, 0, 0,
     IMM_COLD | IMM_POISON | IMM_HOLY | IMM_NEGATIVE | IMM_MENTAL |
     IMM_DISEASE | IMM_DROWNING, RES_BASH | RES_LIGHT,
     VULN_SLASH | VULN_FIRE | VULN_ACID | VULN_LIGHTNING | VULN_ENERGY,
     E | J | M | cc, A | B | C | G | H | K},

    {"dragon", FALSE,
     0, AFF_INFRARED | AFF_FLYING, 0,
     0, RES_FIRE | RES_BASH | RES_CHARM,
     VULN_PIERCE | VULN_COLD,
     A | H | Z, A | C | D | E | F | G | H | I | J | K | P | Q | U | V | X},

    {
     "fido", FALSE,
     0, 0, OFF_DODGE | ASSIST_RACE,
     0, 0, VULN_MAGIC,
     A | B | G | V, A | C | D | E | F | H | J | K | Q | V},

    {
     "fox", FALSE,
     0, AFF_DARK_VISION, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | Q | V},

    {
     "goblin", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_DISEASE, VULN_MAGIC,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "hobgoblin", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_DISEASE | RES_POISON, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K | Y},

    {
     "kobold", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_POISON, VULN_MAGIC,
     A | B | H | M | V, A | B | C | D | E | F | G | H | I | J | K | Q},

    {
     "lizard", FALSE,
     0, 0, 0,
     0, RES_POISON, VULN_COLD,
     A | G | X | cc, A | C | D | E | F | H | K | Q | V},

    {
     "modron", FALSE,
     0, AFF_INFRARED, ASSIST_RACE | ASSIST_ALIGN,
     IMM_CHARM | IMM_DISEASE | IMM_MENTAL | IMM_HOLY | IMM_NEGATIVE,
     RES_FIRE | RES_COLD | RES_ACID, 0,
     H, A | B | C | G | H | J | K},

    {
     "orc", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_DISEASE, VULN_LIGHT,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "pig", FALSE,
     0, 0, 0,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K},

    {
     "rabbit", FALSE,
     0, 0, OFF_DODGE | OFF_FAST,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K},

    {
     "school monster", FALSE,
     ACT_NOALIGN, 0, 0,
     IMM_CHARM | IMM_SUMMON, 0, VULN_MAGIC,
     A | M | V, A | B | C | D | E | F | H | J | K | Q | U},

    {
     "snake", FALSE,
     0, 0, 0,
     0, RES_POISON, VULN_COLD,
     A | G | X | Y | cc, A | D | E | F | K | L | Q | V | X},

    {
     "song bird", FALSE,
     0, AFF_FLYING, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | W, A | C | D | E | F | H | K | P},

    {
     "troll", FALSE,
     0, AFF_REGENERATION | AFF_INFRARED | AFF_DETECT_HIDDEN,
     OFF_BERSERK,
     0, RES_CHARM | RES_BASH, VULN_FIRE | VULN_ACID,
     A | B | H | M | V, A | B | C | D | E | F | G | H | I | J | K | U | V},

    {
     "water fowl", FALSE,
     0, AFF_SWIM | AFF_FLYING, 0,
     0, RES_DROWNING, 0,
     A | G | W, A | C | D | E | F | H | K | P},

    {
     "wolf", FALSE,
     0, AFF_DARK_VISION, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | J | K | Q | V},

    {
     "wyvern", FALSE,
     0, AFF_FLYING | AFF_DETECT_INVIS | AFF_DETECT_HIDDEN,
     OFF_BASH | OFF_FAST | OFF_DODGE,
     IMM_POISON, 0, VULN_LIGHT,
     A | B | G | Z, A | C | D | E | F | H | J | K | Q | V | X},

    {
     "unique", FALSE,
     0, 0, 0,
     0, 0, 0,
     0, 0},


    {
     NULL, 0, 0, 0, 0, 0, 0}
};

const struct pc_race_type pc_race_table[] = {
    {"null race", "", 0, 
     {""}, {13, 13, 13, 13, 13}, {18, 18, 18, 18, 18}, 0},

/*
    {
    "race name",     short name,     points,
    { bonus skills },
    { base stats },        { max stats },        size 
    },
*/
    {"human", "{xHuman{x", 0, 
     {""},
     {13, 13, 13, 13, 13}, {18, 18, 18, 18, 18}, SIZE_MEDIUM},

    {"elf", "{x Elf {x", 5, 
     {"sneak", "hide"},
     {12, 14, 13, 15, 11}, {16, 20, 18, 21, 15}, SIZE_MEDIUM},
     
    {"half-elf", "{xHfElf{x", 2, 
     {"sneak"}, 
     {12, 14, 13, 14, 12}, {17, 19, 18, 20, 16}, SIZE_MEDIUM},

    {"dwarf", "{xDwarf{x", 8, 
     {"berserk"},
     {14, 12, 14, 10, 15}, {20, 16, 19, 14, 21}, SIZE_MEDIUM},

    {"giant", "{xGiant{x", 6, 
     {"bash", "fast healing"},
     {16, 11, 13, 11, 14}, {22, 15, 18, 15, 20}, SIZE_LARGE}, 
     
    {"half-giant", "{xHfGia{x", 3, 
     {"bash"},
     {15, 12, 13, 12, 14}, {20, 17, 18, 16, 19}, SIZE_LARGE},
     
    {"robot", "{DRobot{x", 4, 
     {""},
     {15, 15, 11, 12, 12}, {20, 22, 16, 18, 16}, SIZE_MEDIUM},
     
    {"pixie", "{mPixie{x", 2, 
     {""},
     {11, 15, 13, 15, 11}, {16, 20, 18, 21, 15}, SIZE_SMALL},
     
    {"hooloovoo", "{BHlvoo{x", 7, 
     {""},
     {10, 18, 13, 12, 11}, {15, 25, 18, 16, 16}, SIZE_SMALL},
};


/*
 * Titles.
 */
char *const title_table[MAX_LEVEL + 1][3] = {
     {"Man", "Woman", "Being"},

     {"Newbie", "Newbie", "Newbie"},
     {"{GBig Green Bug{x", "{BBig Blue Bug{x", "{YBig Yellow Bug{x"},
     {"Eater of Pies", "Eater of Cakes", "Eater of Noodle Casseroles"},
     {"Drinker of Beer", "Drinker of Coffee", "Drinker of {BWater{x"},
     {"Very Fast", "Very Fast", "Very Fast"},

     {"Lazy", "Lazy", "Lazy"},
     {"{MMagenta{x", "{GGreen{x", "{WWhite{x"},
     {"Righter of Lefts", "Righter of Lefts", "Lefter of Rights"},
     {"Injurious", "Splendid", "{DDrab{x"},
     {"{DDull{x", "Bored", "Fantabulous"},

     {"Silly", "Mistress of Camp", "Artificial"},
     {"Master of Vodka", "Intoxicated", "Drunk"},
     {"Hungry", "Hungry", "Hungry"},
     {"{DObscure{x", "{DObscure{x", "Sparkling"},
     {"Effervescent", "Shimmery", "{DObscure{x"},

     {"Exceptionally Silly", "Ludicrous", "Potato-Eater"},
     {"{J{YCheesemaker{x", "Corrupt", "Cowardly"},
     {"Butcher", "Baker", "{J{YCandlestick-maker{x"},
     {"Dog", "Cat", "Rat"},
     {"Aardvark", "Gazelle", "Three Toed Sloth"},

     {"Tolerable", "Pure", "Brave"},
     {"Able", "Adorable", "Awkward"},
     {"Bavarian", "Bumbler", "Backwards"},
     {"Crooked", "Corporate Shill", "Cantaloupe"},
     {"Devilish", "Daring", "Dunce"},

     {"{V{DElephant{x", "Elevated", "Elastic"},
     {"Fine", "Fantastic", "Fortunate"},
     {"Goopy", "Garrulous", "Good"},
     {"High", "Holy", "Helpful"},
     {"Innocent", "Itchy", "Incumbent"},

     {"Jumpy", "Jazz Artist", "Just"},
     {"{T{WKissable{x", "Kicking", "Korean"},
     {"Luscious", "Lamb", "Lumpy"},
     {"Miniscule", "Mitten-Wearer", "Majestic"},
     {"Noodle", "Nice", "Neutral"},

     {"Open", "Orange", "Opiate"},
     {"Pleasant", "{W{UPuddle-Stomper{x", "Perfect"},
     {"Quirky", "Queen of Apes", "Queer"},
     {"Riveting", "Righteous", "Rapid"},
     {"Sex God", "Sex Goddess", "Disturbingly Attractive"},

     {"Terrible", "Tantalizing", "Termite"},
     {"Umbrella", "Uncouth", "Underwear Thief"},
     {"Vast", "Void", "Villainous"},
     {"Wicked", "Wistful", "Wallflower"},
     {"{RX{x-{Ytreme{x", "{RX{x-{Ytreme{x", "{RX{x-{Ytreme{x"},

     {"Young", "{YYellow{x", "Yawning"},
     {"Zany", "Zesty", "Zippy"},
     {"Sore", "Heckled", "{mEggplant{x"},
     {"Weed", "{GKiwi{x", "Tub of {J{YMargarine{x"},
     {"Oolitic", "Crabby", "Soft"},

     {"Exceptional", "Exceptional", "Exceptional"},
     {"{YHappy Banana{x", "{gHappy Cucumber{x", "{BHappy Swimming Pool{x"},
     {"Default Title", "Default Title", "Default Title"},
     {"Default Title", "Default Title", "Default Title"},
     {"Default Title", "Default Title", "Default Title"},
     {"Default Title", "Default Title", "Default Title"},
     {"Default Title", "Default Title", "Default Title"},
     {"Default Title", "Default Title", "Default Title"},
     {"Default Title", "Default Title", "Default Title"},
     {"{Y{PGodawful Squid{x", "{Y{PSquishy Squid{x", "{Y{PSquirmy Squid{x"}

};



/*
 * Attribute bonus tables.
 */
const struct str_app_type str_app[26] = {
    {-5, -4, 0, 0},                /* 0  */
    {-5, -4, 3, 1},                /* 1  */
    {-3, -2, 3, 2},
    {-3, -1, 10, 3},            /* 3  */
    {-2, -1, 25, 4},
    {-2, -1, 55, 5},            /* 5  */
    {-1, 0, 80, 6},
    {-1, 0, 90, 7},
    {0, 0, 100, 8},
    {0, 0, 100, 9},
    {0, 0, 115, 10},            /* 10  */
    {0, 0, 115, 11},
    {0, 0, 130, 12},
    {0, 0, 130, 13},            /* 13  */
    {0, 1, 140, 14},
    {1, 1, 150, 15},            /* 15  */
    {1, 2, 165, 16},
    {2, 3, 180, 22},
    {2, 3, 200, 25},            /* 18  */
    {3, 4, 225, 30},
    {3, 5, 250, 35},            /* 20  */
    {4, 6, 300, 40},
    {4, 6, 350, 45},
    {5, 7, 400, 50},
    {5, 8, 450, 55},
    {6, 9, 500, 60}                /* 25   */
};



const struct int_app_type int_app[26] = {
    {3},                        /*  0 */
    {5},                        /*  1 */
    {7},
    {8},                        /*  3 */
    {9},
    {10},                        /*  5 */
    {11},
    {12},
    {13},
    {15},
    {17},                        /* 10 */
    {19},
    {22},
    {25},
    {28},
    {31},                        /* 15 */
    {34},
    {37},
    {40},                        /* 18 */
    {44},
    {49},                        /* 20 */
    {55},
    {60},
    {70},
    {80},
    {85}                        /* 25 */
};



const struct wis_app_type wis_app[26] = {
    {0},                        /*  0 */
    {0},                        /*  1 */
    {0},
    {0},                        /*  3 */
    {0},
    {1},                        /*  5 */
    {1},
    {1},
    {1},
    {1},
    {1},                        /* 10 */
    {1},
    {1},
    {1},
    {1},
    {2},                        /* 15 */
    {2},
    {2},
    {3},                        /* 18 */
    {3},
    {3},                        /* 20 */
    {3},
    {4},
    {4},
    {4},
    {5}                            /* 25 */
};



const struct dex_app_type dex_app[26] = {
    {60},                        /* 0 */
    {50},                        /* 1 */
    {50},
    {40},
    {30},
    {20},                        /* 5 */
    {10},
    {0},
    {0},
    {0},
    {0},                        /* 10 */
    {0},
    {0},
    {0},
    {0},
    {-10},                        /* 15 */
    {-15},
    {-20},
    {-30},
    {-40},
    {-50},                        /* 20 */
    {-60},
    {-75},
    {-90},
    {-105},
    {-120}                        /* 25 */
};


const struct con_app_type con_app[26] = {
    {-4, 20},                    /*  0 */
    {-3, 25},                    /*  1 */
    {-2, 30},
    {-2, 35},                    /*  3 */
    {-1, 40},
    {-1, 45},                    /*  5 */
    {-1, 50},
    {0, 55},
    {0, 60},
    {0, 65},
    {0, 70},                    /* 10 */
    {0, 75},
    {0, 80},
    {0, 85},
    {0, 88},
    {1, 90},                    /* 15 */
    {2, 95},
    {2, 97},
    {3, 99},                    /* 18 */
    {3, 99},
    {4, 99},                    /* 20 */
    {4, 99},
    {5, 99},
    {6, 99},
    {7, 99},
    {8, 99}                        /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const struct liq_type liq_table[] = {
/*    name            color    proof, full, thirst, food, ssize */
    {"water", "clear", {0, 1, 10, 0, 16}},
    {"beer", "amber", {12, 1, 8, 1, 12}},
    {"red wine", "burgundy", {30, 1, 8, 1, 5}},
    {"ale", "brown", {15, 1, 8, 1, 12}},
    {"dark ale", "dark", {16, 1, 8, 1, 12}},

    {"whisky", "golden", {120, 1, 5, 0, 2}},
    {"lemonade", "pink", {0, 1, 9, 2, 12}},
    {"firebreather", "boiling", {190, 0, 4, 0, 2}},
    {"local specialty", "clear", {151, 1, 3, 0, 2}},
    {"slime mold juice", "green", {0, 2, -8, 1, 2}},

    {"milk", "white", {0, 2, 9, 3, 12}},
    {"tea", "tan", {0, 1, 8, 0, 6}},
    {"coffee", "black", {0, 1, 8, 0, 6}},
    {"salt water", "clear", {0, 1, -2, 0, 1}},

    {"coke", "brown", {0, 2, 9, 2, 12}},
    {"root beer", "brown", {0, 2, 9, 2, 12}},
    {"red pop", "red", {0, 2, 9, 2, 12}},
    {"mountain dew", "yellow", {0, 2, 9, 2, 12}},
    
    {"elvish wine", "green", {35, 2, 8, 1, 5}},
    {"white wine", "golden", {28, 1, 8, 1, 5}},
    {"champagne", "golden", {32, 1, 8, 1, 5}},

    {"mead", "honey-colored", {34, 2, 8, 2, 12}},
    {"rose wine", "pink", {26, 1, 8, 1, 5}},
    {"benedictine wine", "burgundy", {40, 1, 8, 1, 5}},
    {"vodka", "clear", {130, 1, 5, 0, 2}},
    {"cranberry juice", "red", {0, 1, 9, 2, 12}},

    {"orange juice", "orange", {0, 2, 9, 3, 12}},
    {"absinthe", "green", {200, 1, 4, 0, 2}},
    {"brandy", "golden", {80, 1, 5, 0, 4}},
    {"aquavit", "clear", {140, 1, 5, 0, 2}},
    {"schnapps", "clear", {90, 1, 5, 0, 2}},
    
    {"icewine", "purple", {50, 2, 6, 1, 5}},
    {"amontillado", "burgundy", {35, 2, 8, 1, 5}},
    {"sherry", "red", {38, 2, 7, 1, 5}},
    {"framboise", "red", {50, 1, 7, 1, 5}},
    {"rum", "amber", {151, 1, 4, 0, 2}},
    {"cordial", "clear", {100, 1, 5, 0, 2}},
    
    {"blood", "red", {0, 2, -1, 2, 6}},
    {"sweat", "clear", {0, 1, -1, 0, 1}},
    {"tears", "clear", {0, 1, -1, 0, 1}},
    {"semen", "milky white", {0, 1, -2, 0, 1}},
    {"urine", "yellow", {0, 1, 0, 0, 1}},
    {"saliva",  "clear", {0, 1, 0, 0, 1}},

    {NULL, NULL, {0, 0, 0, 0, 0}}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)    n

const struct skill_type skill_table[MAX_SKILL] = {

/* { name, skill_level, rating, skill_adept, gain_cost
	 spell pointer, targets, position
	 gsn pointer, slot, min_mana, beats,
	 damage message, wear off message, wear off objects message}
*/

/*
 * Magic spells.
 */

    {"reserved", 99, 99, 100, 0,
     0, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (0), 0, 0,
     "", "", ""},

    {"armor", 5, 1, 75, 0, 								/* #3015 - an armadillo */
     spell_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (1), 5, 12,
     "", "You feel less armored.", ""},

    {"teleport", 13, 1, 75, 0,
     spell_teleport, TAR_CHAR_SELF, POS_FIGHTING,
     NULL, SLOT (2), 35, 12,
     "", "!Teleport!", ""},

    {"bless", 7, 1, 75, 0, 								/* #9542 - wandering prophet */
     spell_bless, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (3), 5, 12,
     "", "You feel less righteous.",
     "$p's holy aura fades."},

    {"blindness", 12, 1, 75, 0, 						/* #3403 - the wraith */
     spell_blindness, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_blindness, SLOT (4), 5, 12,
     "", "You can see again.", ""},

    {"burning hands", 8, 1, 75, 0,						/* #9215 - a flame */
     spell_burning_hands, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (5), 15, 12,
     "burning hands", "!Burning Hands!", ""},

    {"call lightning", 18, 1, 75, 0,
     spell_call_lightning, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (6), 15, 12,
     "lightning bolt", "!Call Lightning!", ""},

    {"charm person", 20, 1, 75, 0,						/* 8816 - Hig Hurtenflurst */
     spell_charm_person, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_charm_person, SLOT (7), 5, 12,
     "", "You feel more self-confident.", ""},

    {"chill touch", 4, 1, 75, 0, 					/* #9216 - a small snowflake */
     spell_chill_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (8), 15, 12,
     "chilling touch", "You feel less cold.", ""},

    {"color spray", 16, 1, 75, 0, 				/* #9235 - baby rainbow dragon */
     spell_colour_spray, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (10), 15, 12,
     "color spray", "!Color Spray!", ""},

    {"control weather", 15, 1, 75, 0,
     spell_control_weather, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (11), 25, 12,
     "", "!Control Weather!", ""},

    {"create food", 10, 1, 75, 0,						/* #20913 - a cook */
     spell_create_food, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (12), 5, 12,
     "", "!Create Food!", ""},

    {"create water", 3, 1, 75, 0, 						/* #9214 - a puddle */
     spell_create_water, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (13), 5, 12,
     "", "!Create Water!", ""},

    {"cure blindness", 6, 1, 75, 0,
     spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (14), 5, 12,
     "", "!Cure Blindness!", ""},

    {"cure critical", 13, 1, 75, 0,
     spell_cure_critical, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (15), 20, 12,
     "", "!Cure Critical!", ""},

    {"cure light", 2, 1, 75, 0,
     spell_cure_light, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (16), 10, 12,
     "", "!Cure Light!", ""},

    {"curse", 18, 1, 75, 0,						/* #21027 - Morgause */
     spell_curse, TAR_OBJ_CHAR_OFF, POS_FIGHTING,
     &gsn_curse, SLOT (17), 20, 12,
     "curse", "The curse wears off.",
     "$p is no longer impure."},

    {"detect evil", 4, 1, 75, 0, 				/* #3600 - Henry the Gardener*/
     spell_detect_evil, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (18), 5, 12,
     "", "The red in your vision disappears.", ""},

    {"detect invis", 11, 1, 75, 0,
     spell_detect_invis, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (19), 5, 12,
     "", "You no longer see invisible objects.",
     ""},

    {"detect magic", 2, 1, 75, 0,
     spell_detect_magic, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (20), 5, 12,
     "", "The detect magic wears off.", ""},

    {"detect poison", 7, 1, 75, 0, 					/* #6306 - drone spider */
     spell_detect_poison, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (21), 5, 12,
     "", "!Detect Poison!", ""},

    {"dispel evil", 15, 1, 75, 0, 						/* #3405 - the priest */
     spell_dispel_evil, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (22), 15, 12,
     "dispel evil", "!Dispel Evil!", ""},

    {"earthquake", 10, 1, 75, 0, 					/* #9229 - rock monster */
     spell_earthquake, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (23), 15, 12,
     "earthquake", "!Earthquake!", ""},

    {"enchant weapon", 17, 2, 75, 0,
     spell_enchant_weapon, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (24), 100, 24,
     "", "!Enchant Weapon!", ""},

    {"energy drain", 19, 1, 75, 0,
     spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (25), 35, 12,
     "energy drain", "!Energy Drain!", ""},

    {"fireball", 22, 1, 75, 0, 						/* #9226 - blue flame */
     spell_fireball, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (26), 15, 12,
     "fireball", "!Fireball!", ""},

    {"harm", 23, 1, 75, 0,						/* #29806 - a hermit */
     spell_harm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (27), 35, 12,
     "harm spell", "!Harm!,        " ""},

    {"heal", 21, 1, 75, 0,
     spell_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (28), 50, 12,
     "", "!Heal!", ""},

    {"invisibility", 5, 1, 75, 0,			/* #6115 - Shargugh */
     spell_invis, TAR_OBJ_CHAR_DEF, POS_STANDING,
     &gsn_invis, SLOT (29), 5, 12,
     "", "You are no longer invisible.",
     "$p fades into view."},

    {"lightning bolt", 13, 1, 75, 0,
     spell_lightning_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (30), 15, 12,
     "lightning bolt", "!Lightning Bolt!", ""},

    {"locate object", 9, 1, 75, 0,
     spell_locate_object, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (31), 20, 18,
     "", "!Locate Object!", ""},

    {"magic missile", 1, 1, 75, 0, 					/* #3710 - a bush lizard */
     spell_magic_missile, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (32), 15, 12,
     "magic missile", "!Magic Missile!", ""},

    {"poison", 12, 1, 75, 0,
     spell_poison, TAR_OBJ_CHAR_OFF, POS_FIGHTING,
     &gsn_poison, SLOT (33), 10, 12,
     "poison", "You feel less sick.",
     "The poison on $p dries up."},

    {"protection evil", 9, 1, 75, 0,
     spell_protection_evil, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (34), 5, 12,
     "", "You feel less protected.", ""},

    {"remove curse", 18, 1, 75, 0,
     spell_remove_curse, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (35), 5, 12,
     "", "!Remove Curse!", ""},

    {"sanctuary", 20, 1, 75, 0,
     spell_sanctuary, TAR_CHAR_DEFENSIVE, POS_STANDING,
     &gsn_sanctuary, SLOT (36), 75, 12,
     "", "The white aura around your body fades.",
     ""},

    {"sleep", 10, 1, 75, 0,							/* #20934 - Morgause */
     spell_sleep, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_sleep, SLOT (38), 15, 12,
     "", "You feel less tired.", ""},

    {"giant strength", 11, 1, 75, 0, 					/* #6506 - the giant */
     spell_giant_strength, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (39), 20, 12,
     "", "You feel weaker.", ""},

    {"summon", 12, 1, 75, 0,
     spell_summon, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (40), 50, 12,
     "", "!Summon!", ""},
	
    {"ventriloquate", 1, 1, 75, 0,				/* #4407 - the Illusionist */
     spell_ventriloquate, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (41), 5, 12,
     "", "!Ventriloquate!", ""},

    {"word of recall", 28, 1, 75, 0,
     spell_word_of_recall, TAR_CHAR_SELF, POS_RESTING,
     NULL, SLOT (42), 5, 12,
     "", "!Word of Recall!", ""}, 

    {"cure poison", 14, 1, 75, 0,
     spell_cure_poison, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (43), 5, 12,
     "", "!Cure Poison!", ""},

    {"detect hidden", 3, 1, 75, 0, 						/* #3402 - the ghoul */
     spell_detect_hidden, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (44), 5, 12,
     "", "You feel less aware of your surroundings.",
     ""},

    {"shocking grasp", 10, 1, 75, 0, 				/* #9218 - shocking grasp */
     spell_shocking_grasp, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (53), 15, 12,
     "shocking grasp", "!Shocking Grasp!", ""},

    {"identify", 13, 1, 75, 0,
     spell_identify, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (54), 12, 24,   /* slot changed from 53 to avoid duplicating shocking grasp */
     "", "!Identify!", ""},

    {"fly", 10, 1, 75, 0, 							/* #9578 - pegasus */
     spell_fly, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (56), 10, 18,
     "", "You slowly float to the ground.", ""},

    {"continual light", 6, 1, 75, 0,
     spell_continual_light, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (57), 7, 12,
     "", "!Continual Light!", ""},

    {"know alignment", 9, 1, 75, 0,
     spell_know_alignment, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (58), 9, 12,
     "", "!Know Alignment!", ""},

    {"dispel magic", 16, 1, 75, 0,
     spell_dispel_magic, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (59), 15, 12,
     "", "!Dispel Magic!", ""},

    {"cure serious", 8, 1, 75, 0, 					/* #6509 - dwarven doctor */
     spell_cure_serious, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (61), 15, 12,
     "", "!Cure Serious!", ""},

    {"cause light", 1, 1, 75, 0, 					/* #3400 - bony skeleton */
     spell_cause_light, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (62), 15, 12,
     "spell", "!Cause Light!", ""},

    {"cause critical", 13, 1, 75, 0, 				/* #9002 - pentadrone */
     spell_cause_critical, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (63), 20, 12,
     "spell", "!Cause Critical!", ""},

    {"cause serious", 7, 1, 75, 0, 						/* #9000 - tridrone */
     spell_cause_serious, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (64), 17, 12,
     "spell", "!Cause Serious!", ""},

    {"flamestrike", 20, 1, 75, 0,
     spell_flamestrike, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (65), 20, 12,
     "flamestrike", "!Flamestrike!", ""},

    {"stone skin", 25, 1, 75, 0, 					/* #5208 - stone golem */
     spell_stone_skin, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (66), 12, 18,
     "", "Your skin feels soft again.", ""},

    {"shield", 20, 1, 75, 0,					/* #20933 & #21014 - Merlin */
     spell_shield, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (67), 12, 18,
     "", "Your force shield shimmers then fades away.",
     ""},

    {"weaken", 11, 1, 75, 0,
     spell_weaken, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (68), 20, 12,
     "spell", "You feel stronger.", ""},

    {"mass invis", 22, 1, 75, 0,
     spell_mass_invis, TAR_IGNORE, POS_STANDING,
     &gsn_mass_invis, SLOT (69), 20, 24,
     "", "You are no longer invisible.", ""},

    {"acid blast", 30, 1, 75, 0,
     spell_acid_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (70), 20, 12,
     "acid blast", "!Acid Blast!"},

    {"faerie fire", 3, 1, 75, 0,			/* #21015 - a faerie lady */
     spell_faerie_fire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (72), 5, 12,
     "faerie fire", "The pink aura around you fades away.",
     ""},

    {"faerie fog", 14, 1, 75, 0,			/* #21029 - Morgaine */
     spell_faerie_fog, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (73), 12, 12,
     "faerie fog", "!Faerie Fog!", ""},

    {"pass door", 24, 1, 75, 0,
     spell_pass_door, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (74), 20, 12,
     "", "You feel solid again.", ""},

    {"infravision", 9, 1, 75, 0, 							/* #5201 - lamia */
     spell_infravision, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (77), 5, 18,
     "", "You no longer see in the dark.", ""},

	{"night vision", 10, 1, 75, 0,
	 spell_night_vision, TAR_CHAR_DEFENSIVE, POS_STANDING,
	 NULL, SLOT (78), 5, 18,
	 "", "You no longer see in the dark.", ""},

    {"create spring", 14, 1, 75, 0,				/* #29801 - a mystic */
     spell_create_spring, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (80), 20, 12,
     "", "!Create Spring!", ""},

    {"refresh", 5, 1, 75, 0,
     spell_refresh, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (81), 12, 18,
     "refresh", "!Refresh!", ""},

    {"change sex", 1, 1, 75, 0,				/* #4406 - Bearded Lady */
     spell_change_sex, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (82), 15, 12,
     "", "Your body feels familiar again.", ""},

    {"gate", 17, 1, 75, 0,
     spell_gate, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (83), 80, 12,
     "", "!Gate!", ""},

/*
 * Snippet spells
 */
    {"resurrect", 27, 1, 75, 0,
     spell_resurrect, TAR_IGNORE, POS_STANDING,
     NULL, SLOT(100), 100, 15,
     "", "!Resurrect!", ""},

/*
 * Dragon breath
 */
    {"acid breath", 31, 1, 75, 0,
     spell_acid_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (200), 100, 24,
     "blast of acid", "!Acid Breath!", ""},

    {"fire breath", 40, 1, 75, 0, 				/* #9209 - fire ruler */
     spell_fire_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (201), 200, 24,
     "blast of flame", "The smoke leaves your eyes.", ""},

    {"frost breath", 34, 1, 75, 0,
     spell_frost_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (202), 125, 24,
     "blast of frost", "!Frost Breath!", ""},

    {"gas breath", 39, 1, 75, 0,
     spell_gas_breath, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (203), 175, 24,
     "blast of gas", "!Gas Breath!", ""},

    {"lightning breath", 37, 1, 75, 0, 			/* #9211 - lightning ruler */
     spell_lightning_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (204), 150, 24,
     "blast of lightning", "!Lightning Breath!", ""}, 

/*
 * Original spells
 */
   {"munchies", 17, 1, 75, 0,				/* #131 - a stoner */
    spell_munchies, TAR_CHAR_OFFENSIVE, POS_SITTING,
    NULL, SLOT(300), 5, 9,
    "", "!munchies!", ""},
    
   {"earworm", 3, 1, 75, 0,					/* A smurf */
    spell_earworm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_earworm, SLOT(301), 5, 10, 
    "", "You no longer have a song stuck in your head.", ""},

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
    {"general purpose", 53, 0, 100, 0,
     spell_general_purpose, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (401), 0, 12,
     "general purpose ammo", "!General Purpose Ammo!", ""},

    {"high explosive", 53, 0, 100, 0,
     spell_high_explosive, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (402), 0, 12,
     "high explosive ammo", "!High Explosive Ammo!", ""},

/*
 * ROM spells (?)
 */

    {"chain lightning", 33, 1, 75, 0,
     spell_chain_lightning, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (500), 25, 12,
     "lightning", "!Chain Lightning!", ""},

    {"cure disease", 13, 1, 75, 0,
     spell_cure_disease, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (501), 20, 12,
     "", "!Cure Disease!", ""},

    {"haste", 21, 1, 75, 0,
     spell_haste, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (502), 30, 12,
     "", "You feel yourself slow down.", ""},

    {"plague", 17, 1, 75, 0,
     spell_plague, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_plague, SLOT (503), 20, 12,
     "sickness", "Your sores vanish.", ""},

    {"frenzy", 24, 1, 75, 0,			/* #12207 - an ROUS */
     spell_frenzy, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (504), 30, 24,
     "", "Your rage ebbs.", ""},

    {"demonfire", 34, 1, 75, 0, 			/* #10468 - greater fire demon */
     spell_demonfire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (505), 20, 12,
     "torments", "!Demonfire!", ""},

    {"holy word", 36, 2, 75, 0,
     spell_holy_word, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (506), 200, 24,
     "divine wrath", "!Holy Word!", ""},

    {"cancellation", 18, 1, 75, 0,
     spell_cancellation, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (507), 20, 12,
     "" "!cancellation!", ""},

    {"mass healing", 38, 2, 75, 0,
     spell_mass_healing, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (508), 100, 36,
     "", "!Mass Healing!", ""},

    {"calm", 16, 1, 75, 0,
     spell_calm, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (509), 30, 12,
     "", "You have lost your peace of mind.", ""},

    {"enchant armor", 16, 2, 75, 0,
     spell_enchant_armor, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (510), 100, 24,
     "", "!Enchant Armor!", ""},

    {"create rose", 1, 1, 75, 0,			/* #9585 - gardener */
     spell_create_rose, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (511), 30, 12,
     "", "!Create Rose!", ""},

    {"dispel good", 15, 1, 75, 0,
     spell_dispel_good, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (512), 15, 12,
     "dispel good", "!Dispel Good!", ""},

    {"detect good", 4, 1, 75, 0,
     spell_detect_good, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (513), 5, 12,
     "", "The gold in your vision disappears.", ""},

    {"protection good", 9, 1, 75, 0,
     spell_protection_good, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (514), 5, 12,
     "", "You feel less protected.", ""},

    {"slow", 23, 1, 75, 0,			/* #12810 - a crystal statue */
     spell_slow, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (515), 30, 12,
     "", "You feel yourself speed up.", ""},

    {"heat metal", 16, 1, 75, 0,
     spell_heat_metal, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (516), 25, 18,
     "spell", "!Heat Metal!", ""},

    {"recharge", 9, 1, 75, 0,
     spell_recharge, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (517), 60, 24,
     "", "!Recharge!", ""},

    {"ray of truth", 35, 1, 75, 0,
     spell_ray_of_truth, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (518), 20, 12,
     "ray of truth", "!Ray of Truth!", ""},

    {"portal", 30, 2, 75, 0,
     spell_portal, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (519), 100, 24,
     "", "!Portal!", ""},

    {"nexus", 35, 2, 75, 0,			/* #21031 - Viviane */
     spell_nexus, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (520), 150, 36,
     "", "!Nexus!", ""},

    {"farsight", 16, 1, 75, 0,		/* #6116 - the elder druid */
     spell_farsight, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (521), 36, 20,
     "farsight", "!Farsight!", ""},

    {"floating disc", 4, 1, 75, 0,
     spell_floating_disc, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (522), 40, 24,
     "", "!Floating disc!", ""},

    {"fireproof", 12, 1, 75, 0, 						/* #9220 - lava beast */
     spell_fireproof, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (523), 10, 12,
     "", "", "$p's protective aura fades."},


/* combat and weapons skills */

   {"axe", 1, 4, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_axe, SLOT (0), 0, 0,
     "", "!Axe!", ""},

    {"dagger", 1, 2, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_dagger, SLOT (0), 0, 0,
     "", "!Dagger!", ""},

    {"flail", 1, 3, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_flail, SLOT (0), 0, 0,
     "", "!Flail!", ""},

    {"mace", 1, 3, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_mace, SLOT (0), 0, 0,
     "", "!Mace!", ""},

    {"polearm", 1, 6, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_polearm, SLOT (0), 0, 0,
     "", "!Polearm!", ""},

    {"shield block", 1, 4, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_shield_block, SLOT (0), 0, 0,
     "", "!Shield!", ""},

    {"spear", 1, 4, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_spear, SLOT (0), 0, 0,
     "", "!Spear!", ""},

    {"sword", 1, 3, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_sword, SLOT (0), 0, 0,
     "", "!sword!", ""},

    {"whip", 1, 5, 75, 1000, 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_whip, SLOT (0), 0, 0,
     "", "!Whip!", ""},

    {"backstab", 2, 5, 75, 0,
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_backstab, SLOT (0), 0, 24,
     "backstab", "!Backstab!", ""},

    {"bash", 1, 4, 75, 2000,			/* #4409 Ivan the Strongman */
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_bash, SLOT (0), 0, 24,
     "bash", "!Bash!", ""},

    {"berserk", 18, 5, 80, 0,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_berserk, SLOT (0), 0, 24,
     "", "You feel your pulse slow down.", ""},

    {"dirt kicking", 3, 4, 75, 0,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_dirt, SLOT (0), 0, 24,
     "kicked dirt", "You rub the dirt out of your eyes.", ""},

    {"disarm", 11, 5, 75, 2000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_disarm, SLOT (0), 0, 24,
     "", "!Disarm!", ""},

    {"dodge", 1, 4, 80, 2000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_dodge, SLOT (0), 0, 0,
     "", "!Dodge!", ""},

    {"enhanced damage", 1, 4, 80, 2000,		/* Obj#3733 - Priest of Santa mprog */
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_enhanced_damage, SLOT (0), 0, 0,
     "", "!Enhanced Damage!", ""},

    {"envenom", 10, 4, 85, 0,
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_envenom, SLOT (0), 0, 36,
     "", "!Envenom!", ""},

    {"hand to hand", 1, 4, 75, 1000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_hand_to_hand, SLOT (0), 0, 0,
     "", "!Hand to Hand!", ""},

    {"kick", 8, 3, 75, 0,
     spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_kick, SLOT (0), 0, 12,
     "kick", "!Kick!", ""},

    {"parry", 1, 5, 75, 2000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_parry, SLOT (0), 0, 0,
     "", "!Parry!", ""},

    {"rescue", 1, 4, 75, 500,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_rescue, SLOT (0), 0, 12,
     "", "!Rescue!", ""},

    {"second attack", 5, 3, 75, 3000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_second_attack, SLOT (0), 0, 0,
     "", "!Second Attack!", ""},
     
    {"tackle", 20, 5, 75, 4000,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_tackle, SLOT(0), 0, 0, 
     "", "!Tackle!", ""},

    {"trip", 1, 5, 75, 0,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_trip, SLOT (0), 0, 24,
     "trip", "!Trip!", ""},
     
    {"third attack", 12, 4, 75, 0,			/* O#3141 - Zombie Timothy Leary's shop */
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_third_attack, SLOT (0), 0, 0,
     "", "!Third Attack!", ""}, 
     
    {"fourth attack", 30, 5, 70, 0,			/* #12201 - Inigo Montoya */
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_fourth_attack, SLOT (0), 0, 0,
     "", "!Fourth Attack!", ""},
     
    {"fifth attack", 40, 5, 65, 0,
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_fifth_attack, SLOT (0), 0, 0,
     "", "!Fifth Attack!", ""},

/* non-combat skills */

	{"butcher", 3, 1, 75, 0,
	spell_null, TAR_IGNORE, POS_RESTING,
	&gsn_butcher, SLOT(0), 0, 0,
	"", "!Butcher!", ""},

    {"fast healing", 6, 5, 75, 0,
     spell_null, TAR_IGNORE, POS_SLEEPING,
     &gsn_fast_healing, SLOT (0), 0, 0,
     "", "!Fast Healing!", ""},

    {"haggle", 1, 3, 75, 0, 					/* #9533 - nomad merchant */
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_haggle, SLOT (0), 0, 0,
     "", "!Haggle!", ""},

    {"hide", 1, 5, 75, 0,
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_hide, SLOT (0), 0, 12,
     "", "!Hide!", ""},

    {"lore", 6, 6, 85, 2000,
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_lore, SLOT (0), 0, 36,
     "", "!Lore!", ""},

    {"meditation", 6, 5, 80, 0,
     spell_null, TAR_IGNORE, POS_SLEEPING,
     &gsn_meditation, SLOT (0), 0, 0,
     "", "Meditation", ""},

    {"peek", 1, 3, 70, 0,
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_peek, SLOT (0), 0, 0,
     "", "!Peek!", ""},

    {"pick lock", 1, 4, 75, 0, 							/* #9535 - skinny kid */
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_pick_lock, SLOT (0), 0, 12,
     "", "!Pick!", ""},

    {"sneak", 4, 5, 75, 0,
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_sneak, SLOT (0), 0, 12,
     "", "You no longer feel stealthy.", ""},

    {"steal", 5, 4, 70, 0, 									/* #3005 - thief*/
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_steal, SLOT (0), 0, 24,
     "", "!Steal!", ""},

    {"scrolls", 1, 2, 75, 1000,
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_scrolls, SLOT (0), 0, 24,
     "", "!Scrolls!", ""},

    {"staves", 1, 2, 75, 1000,
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_staves, SLOT (0), 0, 12,
     "", "!Staves!", ""},
     
    {"vomit", 2, 1, 75, 1000,							/* #3064 - a drunk */
     spell_null, TAR_IGNORE, POS_SITTING,
     &gsn_vomit, SLOT (0), 0, 12,
     "", "!Vomit!", ""},

    {"wands", 1, 2, 75, 1000,
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_wands, SLOT (0), 0, 12,
     "", "!Wands!", ""},

    {"recall", 1, 2, 75, 1,
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_recall, SLOT (0), 0, 12,
     "", "!Recall!", ""} 
};

const struct group_type group_table[MAX_GROUP] = {

    {
     "rom basics", 0,
     {"recall"}
     }

};
