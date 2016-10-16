/*
 * FILE:        ai.c
 * Purpose:     To create a basic AI sub-system to make
 *              AI battles more realistic..
 * Credits:     Zachery Crosswhite (Cyhawk/Thri)
 * Contact:     cyhawkx@gmail.com
 * Contact2:    AIM: CalibanL
 *
 * Install      Very basic, I used rom's previous ACT_MAGE
 * and info:    bit for this. Near the top of mobile_update in
 *              update.c, after the check for specs, and after
 *              my forced do_wear(ch, "all") (cough) I stuck..
 *
 *              if (IS_SET(ch->act, ACT_MAGE))
 *                  ai_wizard(ch);
 *
 *              I also changed MOBILE_PULSE to 20, due to
 *              the fact its normaly 8, and 99% of stock spells
 *              are 12 or higher.. so.. This is to prevent
 *              un-human-like casting ability. I would
 *              say make it 12 perhaps..
 *
 *              Idealy, when this is expanded, there will be
 *              a wrapper to determine class/AI, as well as
 *              choices for universal stuff (returning home,
 *              sleeping, eating, etc) before the specifics...
 *              As well as checks for how much 'wait' the mob
 *              has from spells (I have timed casting, but the
 *              idea is the same for Insta-cast/wait stock) This
 *              way, the mob casts as fast, or faster than a 
 *              good player.
 *
 * TODO:        Well, obviously this is just a basic piece of code...
 *              There needs to be ones for each 'class' of mobile
 *              you want to have, the 4 basics included in stock,
 *              as well as other special ones, (Group Healers,
 *              mobile leaders/'boss' mobs, maybe special mobs in
 *              here too, or 'super' mages..
 *
 *              Intelligence/Wisdom Checks, dumb wizards wont always
 *              cast, this also gives credence to giving mobs stat points
 *              too.
 *
 *              Checks for Mana, right now, they will always attempt to cast,
 *              I forsee some major spam-fests whe theyre out of mana.. ugh
 *              Maybe if theyre really hurt, they flee/find a healer..
 *              Ooo, I can see some intersting City AI here.
 *
 *              Debuf/Curse spells seprated. Lets face it, in PK, it goes in
 *              a specific order. Debuf->curse->fight (curse being any stat
 *              reduction or offensive enchantments). Mobiles should do this
 *              too.
 *
 *              Add function wrappers for ease of expandability and improvement
 *              of universal AI functions like going home, eating, etc.
 *
 * Licence:     This code falls under the Rom Licence, with the following
 *              additions:
 *              8) A helpfile accessable via the keyword '_thri' Must be
 *              viewable to all players, regardless of level, with the following
 *              Information:
 *              
 *              AI Sub-system created by Thri of Vandagard MUD.
 *              Email: cyhawkx@gmail.com
 *              AIM: CalibanL
 *
 *              Any previous snippets of mine used must also be included
 *              in this helpfile.
 *
 *              9) If you have a 'credits' command or credits helpfile,
 *              a small mention of my Name and the code snip should also
 *              be there.
 *
 *              10) You are also required to make contact with Thri via
 *              any means listed. (AIM/Email) Telling him you are going
 *              to use his code. (Email is best).
 *
 * Afterward:   Anywho, Enjoy the code, and make sure you expand it!
 *              Also, if you do expand it, send me what ya did, id love
 *              to incorporate this into the main sources, and maybe
 *              release a new stock codebase based around this, ROM 3.0?
 *              well, you know what i mean =)
 *
 *  -Cyhawk/Thri
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"

/**
 * AI_DEBUG: Turn on/off Chatting of what the AI is doing
 */
#define AI_DEBUG 1

/*
                    -> Cast
        -> Fight    -> Melee
Action              -> Debuf
                    -> Flee?

                    -> Heal Self
        -> Idle     -> Enchant Self
                    -> Idle Chit Chat
*/

/** ai_wizard()
 *
 * Primary AI Control for Wizard mobs, follows
 * a basic procedure, hopefuly this will work.
 *
 */
void ai_wizard(CHAR_DATA * mob)
{
    int _mob_hp = 100 * (mob->hit) / (mob->max_hit + 1);
    int _level  = mob->level;
    int control = -1;
    int chance  = 0;S
    
    /* Ok, Lets start with idle actions.. */
    if (mob->fighting == NULL)
    {
        /* ACTION_ Heal Start (75% or lower) */
        if (_mob_hp < 75)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "I need to heal myself");
#endif
            /* ACTION_ Heal: Lets see if he can hide first */
            if (!is_affected (mob, skill_lookup("invisibility"))
                && _level >= 8
                && !IS_AFFECTED(mob, AFF_INVISIBLE))
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Woha that was close, gotta hide!");
#endif
                do_function (mob, &do_cast, "'invisibility'");
                return;
            }
            
            if (_level >= 25)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "I'm over level 25, I'll use Heal");
#endif
                do_function (mob, &do_cast, "heal self");
                return;
            }
            else
            {
                if (mob->position == POS_RESTING)
                    return;
#if AI_DEBUG
                do_function (mob, &do_say, "I'm not over level 25, I need to rest");
#endif
                do_function (mob, &do_rest, "rest");
                return;
            }
        }
        else
        {
            if (mob->position != POS_STANDING)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Much Better!");
#endif
                do_function (mob, &do_stand, "");
            }
        }
                
        /* ACTION_ Heal End */
        
        /* ACTION_ Enchant */
        /* Enchant - Orcish Strength */
        if (!is_affected (mob, skill_lookup("orcish strength"))
           && _level >= 3)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "I don't have orcish strength on.");
#endif
            do_function (mob, &do_cast, "'orcish strength'");
            return;
        }
        /* Enchant - Invisibility */
        if (!is_affected (mob, skill_lookup("invisibility"))
            && _level >= 8
            && !IS_AFFECTED(mob, AFF_INVISIBLE))
        {
#if AI_DEBUG
            do_function (mob, &do_say, "I need to be invisible!");
#endif
            do_function (mob, &do_cast, "'invisibility'");
            return;
        }
        /* Enchant - Invisibility */
        if (!is_affected (mob, skill_lookup("stone skin"))
            && _level >= 12)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "I need to protect myself with Stone skin!");
#endif
            do_function (mob, &do_cast, "'stone skin'");
            return;
        }
        /* Enchant - Adept */
        if (!is_affected (mob, skill_lookup("adept"))
            && _level >= 20)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "What kind of Wizard isnt adept?");
#endif
            do_function (mob, &do_cast, "'adept'");
            return;
        }
        /* Enchant - Haste */
        if (!is_affected (mob, skill_lookup("haste"))
            && _level >= 30)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "I feel sluggish...");
#endif
            do_function (mob, &do_cast, "'haste'");
            return;
        }
        /* Enchant - Immolate */
        if (!is_affected (mob, skill_lookup("immolate"))
            && _level >= 31)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "I need to feel some POWAAAHH!!");
#endif
            do_function (mob, &do_cast, "'immolate'");
            return;
        }
        /* Enchant - Anti-magic Shell */
        if (!is_affected (mob, skill_lookup("anti-magic shell"))
            && _level >= 35)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "I feel too vulnerable..");
#endif
            do_function (mob, &do_cast, "'anti-magic shell'");
            return;
        }
        /* Enchant - Manashield */
        if (!is_affected (mob, skill_lookup("manashield"))
            && _level >= 45)
        {
#if AI_DEBUG
            do_function (mob, &do_say, "Dude? Wheres my Manashield?");
#endif
            do_function (mob, &do_cast, "'manashield'");
            return;
        }
    }
    
    /* Ok, Idle actions done, lets start the fighting stuff! */
    if (mob->fighting != NULL)
    {
        /* ACTION_ Fighting: Flee under 35% */
        if (_mob_hp <= 35)
        {
            /* 50% chance to flee every update */
            if (number_range(1, 100) > 50)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Holy bejezzus batman, Im out of here!");
#endif
                do_function (mob, &do_flee, "");
                return;
            }
        }
        /* Idealy, spells will be Highest Level to Lowest
         * So the AI will hit harder as theyre stronger..
         */
        /* Cut levels every 20 for ease of use */
        if (_level <= 999)
        {
            control = 8;
            chance = number_range(1, 8);

            if (chance == 1 && _level >= 1)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Eat Wizard Fire!");
#endif
                do_function (mob, &do_cast, "'wizard fire'");
                return;
            }
            if (chance == 2 && _level >= 1)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Suck on some Wizard Frost!");
#endif
                do_function (mob, &do_cast, "'wizard frost'");
                return;
            }
            if (chance == 3 && _level >= 1)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Feel the Powah of Wizard Spark!");
#endif
                do_function (mob, &do_cast, "'wizard spark'");
                return;
            }
            if (chance == 4 && _level >= 4)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Are my hands hot to you?");
#endif
                do_function (mob, &do_cast, "'burning hands'");
                return;
            }
            if (chance == 5 && _level >= 5)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Do you feel sick?");
#endif
                do_function (mob, &do_cast, "'weaken'");
                return;
            }
            if (chance == 6 && _level >= 6)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Ooo, purtty colours...");
#endif
                do_function (mob, &do_cast, "'colour spray'");
                return;
            }
            if (chance == 7 && _level >= 9)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Ooo, its chilly in here!");
#endif
                do_function (mob, &do_cast, "'chill touch'");
                return;
            }
            if (chance == 8 && _level >= 10)
            {
#if AI_DEBUG
                do_function (mob, &do_say, "Wow, your strong, can I have some?");
#endif
                do_function (mob, &do_cast, "'leech strength'");
                return;
            }
            return;
        }
    }   /* End fighting */
    
    return;
}
            














