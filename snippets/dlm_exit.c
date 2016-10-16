
/********************************************************************************
 * Random Exit code copyright 1999-2001                                         *
 * Markanth : dlmud@dlmud.com                                                   *
 * Devil's Lament : dlmud.com port 3778                                      *
 * Web Page : http://www.dlmud.com                                              *
 *                                                                              *
 * This snippet uses OLC, and is set up for clan entrances but can easily be    *
 * be adapted for something else. It also requires a new exit flag, EX_TEMP     *
 * it is recomended exit flags use bitvectors.                                  *
 *                                                                              *
 * All I ask in return is that you give me credit on your mud somewhere         *
 * or email me if you use it.                                                   *
 ********************************************************************************/

>>>> DB2.C <<<<

1) Somewhere add these functions

int open_exit( ROOM_INDEX_DATA *toRoom, ROOM_INDEX_DATA *pRoom )
{
    int door;

        for(door = 0; door < MAX_DIR; door++)
        {
            if (!toRoom->exit[rev_dir[door]] && !pRoom->exit[door])
                return door;
        }
    return -1;
}

void clean_exits(ROOM_INDEX_DATA *pRoom)
{
    int door, rev;
    ROOM_INDEX_DATA *toRoom;
    EXIT_DATA *pExit, *toExit;

    for (door = 0; door < MAX_DIR; door++)
    {
        rev = rev_dir[door];

        if((pExit = pRoom->exit[door]) && IS_SET(pExit->rs_flags, EX_TEMP))
        {
            if(pExit->u1.in_room && (toExit = pExit->u1.in_room->exit[rev]) && IS_SET(toExit->rs_flags, EX_TEMP))
            {
                free_exit(toExit);
                toExit = NULL;
            }

            free_exit(pExit);
            pExit = NULL;
        }
    }
}

void add_random_exit(ROOM_INDEX_DATA *pRoom)
{
    EXIT_DATA *pExit, *toExit;
    ROOM_INDEX_DATA *pRoom, *toRoom;
    int door, rev;

    if(!pRoom)
    return;

    clean_exits(pRoom);

          for (;;)
          {
            toRoom = get_room_index (number_range (2, top_vnum /* or 32654 */ ));
            if (toRoom != NULL)
              if (!IS_SET (toRoom->room_flags, ROOM_PRIVATE)
              && !IS_SET (toRoom->room_flags, ROOM_SOLITARY)
              && !IS_SET (toRoom->room_flags, ROOM_SAFE)
              && !IS_SET (toRoom->room_flags, ROOM_NOWHERE)
              && !IS_SET (toRoom->room_flags, ROOM_NO_RECALL)
			  && !IS_SET (toRoom->room_flags, ROOM_IMP_ONLY)
			  && !IS_SET (toRoom->room_flags, ROOM_GODS_ONLY)
			  && !IS_SET (toRoom->room_flags, ROOM_HEROES_ONLY)
			  && !IS_SET (toRoom->room_flags, ROOM_NEWBIES_ONLY)
              && !IS_SET (toRoom->room_flags, ROOM_LAW)
              /* add more room stuff here */
              && toRoom->clan == 0
              && (door = open_exit(toRoom, pRoom)) != -1) /* check for open exits */
            break;
          }

	/* first room */
	pExit = new_exit();
	pExit->u1.to_room = toRoom;
	pExit->orig_door = door;
	SET_BIT(pExit->rs_flags, EX_TEMP);
	SET_BIT(pExit->exit_info, EX_TEMP);
	pRoom->exit[door] = pExit;

	/* random room */
	rev = rev_dir[door];
	toExit = new_exit();
	toExit->u1.to_room = pRoom;
	toExit->orig_door = rev;
	SET_BIT(toExit->rs_flags, EX_TEMP);
	SET_BIT(toExit->exit_info, EX_TEMP);
	toRoom->exit[rev] = toExit;
	
return;
}

void randomize_clans(void)
{
	ROOM_INDEX_DATA *pRoom;
	int clan;

	for(clan = 0; clan < MAX_CLAN; clan++)
    {
        if (clan_table[clan].independent ||  /* loner clan? */
            (pRoom = get_room_index(clan_table[clan].entrance)) == NULL)
            continue;

		add_random_exit(pRoom);
	}
}

>>>> DB.C <<<<

1) under "local booting procedures"

/* clan version */
void randomize_clans args ((void));

2) under boot_db() after loading clans add:

    logf ("Randomizing Clans.");
    randomize_clans();


>>>> MERC.H <<<<

1) Under "Exit Flags" add:

#define EX_TEMP                       (M)

>>>> TABLES.C <<<<

1) Under exit_flags[] table add:

  {"temp", EX_TEMP, TRUE},

>>>> OLC_SAVE.C <<<<

1) Under save_rooms() find this:

	  if ((pExit = pRoomIndex->exit[door]) && pExit->u1.to_room)

   and change it to this:

    /* very important, don't want to save hundreds of unwanted exits */
	  if ((pExit = pRoomIndex->exit[door]) && pExit->u1.to_room
            && !IS_SET(pExit->rs_flags, EX_TEMP))


>>> POSSIBLE FUNCTION IN WIZ.C <<<

void do_randomize(CHAR_DATA *ch, char *argument)
{
    if(!str_cmp(argument, "clans"))
    {
        send_to_char("Randomizing Clans...\n\r", ch);
        randomize_clans();
        send_to_char("Done.\n\r", ch);
        return;
    }
    else if (!str_cmp(argument, "here"))
    {
        send_to_char("Adding Random exit here...\n\r", ch);
        add_random_exit(ch->in_room);
        send_to_char("Done.\n\r", ch);
        return;
    }
    else
      send_to_char("Syntax: randomize clan|here\n\r", ch);
    return;
}

---------------------
Bingo, your done.

OTHER NOTES:

If you can find a way to add an exit flag with the stock system, wtg, if not
heres a message from the rom list that explains how to use flags instead.

http://www.the-infinite.org/lists/romlist/1998/07/msg00529.html

If you find any problems here please email me at dlmud@dlmud.com     


/********************************************************************************
 * Pathway Skill copyright 1999-2001                                            *
 * Markanth : dlmud@dlmud.com                                                   *
 * Devil's Lament : dlmud.com port 3778                                      *
 * Web Page : http://www.dlmud.com                                              *
 *                                                                              *
 * This snippet requires Random Entrances code using temporary exits.           *
 *                                                                              *
 * All I ask in return is that you give me credit on your mud somewhere         *
 * or email me if you use it.                                                   *
 ********************************************************************************/


>>>> ACT_MOVE.C <<<<<<

1) Add functions.

int random_exit( ROOM_INDEX_DATA *pRoom )
{
    int door = number_range(0, MAX_DIR - 1);

    if (!pRoom->exit[door])
       return door;

    return -1;
}

void do_pathway(CHAR_DATA * ch, char *argument)
{
    ROOM_INDEX_DATA *pRoom. *vRoom;
    int door;
    CHAR_DATA *victim;


    if (get_skill(ch, gsn_pathway) == 0)
    {
	chsend("You have no clue how to do that...\n\r", ch);
	return;
    }
    if (argument[0] == '\0')
    {
	chsend("Open a pathway to who?\n\r", ch);
	return;
    }

    if ( number_percent() > (get_skill(ch, gsn_pathway) + 20)
    ||  ( victim = get_char_world( ch, argument ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   ch->in_room == NULL
    ||   !can_see_room(ch,victim->in_room) 
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= level + 3
    ||   (is_clan(victim) && !is_same_clan(ch,victim))
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */ 
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) )
    ||   (door = random_exit(ch->in_room)) == -1 )
    {
	chsend("You failed.\n\r", ch);
	return;
    }

    check_improve(ch, gsn_pathway, TRUE, 1);

    pRoom = ch->in_room;
    vRoom = victim->in_room;

    /* make one-way temporary exit */
	pRoom->exit[door] = new_exit();
	pRoom->exit[door]->u1.to_room = vRoom;
	pRoom->exit[door]->orig_door = door;
	free_string(pRoom->exit[door]->keyword);
	pRoom->exit[door]->description = str_dup("A magical pathway shimmers here.");
	free_string(pRoom->exit[door]->keyword);
	pRoom->exit[door]->keyword = str_dup("pathway");
	SET_BIT(pRoom->exit[door]->rs_flags, EX_TEMP);
	pRoom->exit[door]->exit_info = pRoom->exit[door]->rs_flags;
	printf_to_char(ch, "You have created a pathway to the %s!\n\r", dir_name[door]);
	return;
}

>>> MERC.H/DB.C <<<<

1) Decalre gsn's.

extern sh_int gsn_pathway;

int gsn_pathway;


>>> CONST.C <<<<

1) Add entry to skill table.

    { "pathway",
     {30, 50, ANGEL, ANGEL},
     {4, 5, 0, 0,},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_pathway, SLOT(0), 0, 0,
     "", "!Pathway!", "" },


>>>> INTERP.[CH] <<<<

1) Add command entries.

DECLARE_DO_FUN(do_pathway);

{"pathway", do_pathway, POS_STANDING, 0, LOG_NORMAL, 1},


------------
Done.
Players think they're that much closer to immhood when they use this skill.
------------
Markanth : dlmud@dlmud.com     
Devil's Lament : dlmud.com port 3778     
Web Page : http://dlmud.akbearsden.com
