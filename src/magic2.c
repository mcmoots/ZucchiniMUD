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
 *  ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@hypercube.org)                                *
 *      Gabrielle Taylor (gtaylor@hypercube.org)                           *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Rom24/doc/rom.license                         *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"


extern char *target_name;

void spell_farsight (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    if (IS_AFFECTED (ch, AFF_BLIND))
    {
        send_to_char ("Maybe it would help if you could see?\n\r", ch);
        return;
    }

    do_function (ch, &do_scan, target_name);
}


void spell_portal (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone;

    if ((victim = get_char_world (ch, target_name)) == NULL
        || victim == ch
        || victim->in_room == NULL
        || !can_see_room (ch, victim->in_room)
        || IS_SET (victim->in_room->room_flags, ROOM_SAFE)
        || IS_SET (victim->in_room->room_flags, ROOM_PRIVATE)
        || IS_SET (victim->in_room->room_flags, ROOM_SOLITARY)
        || IS_SET (victim->in_room->room_flags, ROOM_NO_RECALL)
        || IS_SET (ch->in_room->room_flags, ROOM_NO_RECALL)
        || victim->level >= level + 3 || (!IS_NPC (victim) && victim->level >= LEVEL_HERO)    /* NOT trust */
        || (IS_NPC (victim) && IS_SET (victim->imm_flags, IMM_SUMMON))
        || (IS_NPC (victim) && saves_spell (level, victim, DAM_NONE))
        || (is_clan (victim) && !is_same_clan (ch, victim)))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }
    
    if( strstr( victim->in_room->area->builders, "Unlinked" ))
    {
    	send_to_char("You can't portal to areas that aren't linked yet!\n\r", ch);
	return;
    }

    stone = get_eq_char (ch, WEAR_HOLD);
    if (!IS_IMMORTAL (ch)
        && (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
        send_to_char ("You lack the proper component for this spell.\n\r",
                      ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
        act ("You draw upon the power of $p.", ch, stone, NULL, TO_CHAR);
        act ("It flares brightly and vanishes!", ch, stone, NULL, TO_CHAR);
        extract_obj (stone);
    }

    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 2 + level / 25;
    portal->value[3] = victim->in_room->vnum;

    obj_to_room (portal, ch->in_room);

    act ("$p rises up from the ground.", ch, portal, NULL, TO_ROOM);
    act ("$p rises up before you.", ch, portal, NULL, TO_CHAR);
}

void spell_nexus (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone;
    ROOM_INDEX_DATA *to_room, *from_room;

    from_room = ch->in_room;

    if ((victim = get_char_world (ch, target_name)) == NULL
        || victim == ch
        || (to_room = victim->in_room) == NULL
        || !can_see_room (ch, to_room) || !can_see_room (ch, from_room)
        || IS_SET (to_room->room_flags, ROOM_SAFE)
        || IS_SET (from_room->room_flags, ROOM_SAFE)
        || IS_SET (to_room->room_flags, ROOM_PRIVATE)
        || IS_SET (to_room->room_flags, ROOM_SOLITARY)
        || IS_SET (to_room->room_flags, ROOM_NO_RECALL)
        || IS_SET (from_room->room_flags, ROOM_NO_RECALL)
        || victim->level >= level + 3 || (!IS_NPC (victim) && victim->level >= LEVEL_HERO)    /* NOT trust */
        || (IS_NPC (victim) && IS_SET (victim->imm_flags, IMM_SUMMON))
        || (IS_NPC (victim) && saves_spell (level, victim, DAM_NONE))
        || (is_clan (victim) && !is_same_clan (ch, victim)))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }
    
    if( strstr( to_room->area->builders, "Unlinked" ))
    {
    	send_to_char("You can't use nexus to get to areas that aren't linked yet!\n\r", ch);
	return;
    }


    stone = get_eq_char (ch, WEAR_HOLD);
    if (!IS_IMMORTAL (ch)
        && (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
        send_to_char ("You lack the proper component for this spell.\n\r",
                      ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
        act ("You draw upon the power of $p.", ch, stone, NULL, TO_CHAR);
        act ("It flares brightly and vanishes!", ch, stone, NULL, TO_CHAR);
        extract_obj (stone);
    }

    /* portal one */
    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 1 + level / 10;
    portal->value[3] = to_room->vnum;

    obj_to_room (portal, from_room);

    act ("$p rises up from the ground.", ch, portal, NULL, TO_ROOM);
    act ("$p rises up before you.", ch, portal, NULL, TO_CHAR);

    /* no second portal if rooms are the same */
    if (to_room == from_room)
        return;

    /* portal two */
    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 1 + level / 10;
    portal->value[3] = from_room->vnum;

    obj_to_room (portal, to_room);

    if (to_room->people != NULL)
    {
        act ("$p rises up from the ground.", to_room->people, portal, NULL,
             TO_ROOM);
        act ("$p rises up from the ground.", to_room->people, portal, NULL,
             TO_CHAR);
    }
}

void spell_resurrect( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    OBJ_DATA *obj;
    CHAR_DATA *mob;
    int i;

    obj = get_obj_here( ch, target_name );

    if ( obj == NULL || !obj->item_type)
    {
        send_to_char( "Resurrect what?\n\r", ch );
        return;
    }

    /* Nothing but NPC corpses. */

    if( obj->item_type != ITEM_CORPSE_NPC )
    {
        if( obj->item_type == ITEM_CORPSE_PC )
            send_to_char( "You can't resurrect players.\n\r", ch );
        else
            send_to_char( "It would serve no purpose...\n\r", ch );
        return;
    }

    if( obj->level > (ch->level + 2) )
    {
        send_to_char( "You couldn't call forth such a great spirit.\n\r", ch );
        return;
    }

    if( ch->pet != NULL )
    {
        send_to_char( "You already have a pet.\n\r", ch );
        return;
    }

    /* Chew on the zombie a little bit, recalculate level-dependant stats */

    mob = create_mobile( get_mob_index( MOB_VNUM_ZOMBIE ) );
 
    mob->level                  = obj->level;
    mob->max_hit                = mob->level * 8 + number_range(
                                        mob->level * mob->level/4,
                                        mob->level * mob->level);

    mob->max_hit *= .9;
    mob->hit                    = mob->max_hit;
    mob->max_mana               = 100 + dice(mob->level,10);
    mob->mana                   = mob->max_mana;

    for (i = 0; i < 3; i++)
        mob->armor[i]           = interpolate(mob->level,100,-100);
    mob->armor[3]               = interpolate(mob->level,100,0);

    for (i = 0; i < MAX_STATS; i++)
        mob->perm_stat[i] = 11 + mob->level/4;

    /* You rang? */
    char_to_room( mob, ch->in_room );
    act( "$p springs to life as a hideous zombie!", ch, obj, NULL, TO_ROOM );
    act( "$p springs to life as a hideous zombie!", ch, obj, NULL, TO_CHAR );
logfile("sent act");
    extract_obj(obj);
logfile("extracted the corpse");
    /* Yessssss, massssssster... */
    SET_BIT(mob->affected_by, AFF_CHARM);
    SET_BIT(mob->act, ACT_PET);
    mob->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
    add_follower( mob, ch );
    mob->leader = ch;
    ch->pet = mob;
    /* For a little flavor... */
    do_say( mob, "Zhow may I zzerve you, mazhter?" );
    return;
}

void spell_munchies(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int amt = 0;
	OBJ_DATA *obj, *obj_next;
	
	/* */
	
	amt = -dice( level, 12 );
	if( saves_spell(level, victim, DAM_MENTAL))
		amt /= 2;
		
	gain_condition(victim, COND_HUNGER, amt);
	
	send_to_char("You have a sudden attack of the munchies.\n\r", victim);
	act("$n looks hungry.", victim, NULL, NULL, TO_ROOM);
	
	/* look for food in inventory */
	
	for( obj = victim->carrying; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next_content;
		
		if( obj->item_type == ITEM_FOOD )
		{
			do_function(victim, &do_eat, obj->name);
			break;
		}		
	}
	
	return;
}

void spell_earworm(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	
	if( saves_spell( level, victim, DAM_MENTAL ) ){
		act("$n hums a tuneless little tune.", victim, NULL, NULL, TO_ROOM);
		send_to_char("You suddenly think of a song, but can't remember the tune.\n\r", victim);
		return;
	}
	
	af.where = TO_AFFECTS;
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.location = APPLY_INT;
	af.modifier = -1;
	af.bitvector = 0;
	affect_to_char( victim, &af );
	send_to_char("You have the Smurf song stuck in your head.\n\r", victim);
	send_to_char("La la la la la la, laaaaa la la la laaaaa...\n\r", victim);
	
	if( ch != victim )
		act("$N looks unhappy.\n\r", ch, NULL, victim, TO_CHAR);
	return;
}

