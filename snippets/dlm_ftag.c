/****************************************************************************
 * Automated Freeze Tag Code                                                *
 * Markanth : dlmud@dlmud.com                                               *
 * Devil's Lament : dlmud.com port 3778                                  *
 * Web Page : http://www.dlmud.com                                          *
 *                                                                          *
 * Provides automated freeze tag games in an area.                          *
 * Code orginally done by Nebseni of Clandestine MUD                        *
 * <clandestine.mudnet.net:9476>.                                           *
 *                                                                          *
 * All I ask in return is that you give me credit on your mud somewhere     *
 * or email me if you use it.                                               *
 ****************************************************************************/

>>> MAKEFILE <<<

1) Add to O_FILES:
    
    freeze.o

>>> COMM.C <<<

1) add to boot_db() after weather stuff:

  end_tag ();

>>> UPDATE.C <<<

1) add to update_handler() in pulse_point:

    tag_update ();

>>> HANDLER.C <<<

1) add to can_see() wherever appropriate:

  if (!IS_NPC (ch) && !IS_NPC (victim))
  {
    if (IS_SET (ch->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING)
	&& IS_SET (victim->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING))
      return TRUE;
  }

>>> ACT_INFO.C <<<

1) add to show_char_to_char_0() where victim is not an NPC:

    if (IS_SET (victim->pcdata->tag_flags, TAG_RED))
      strcat (buf, "{R[RED] {x");
    if (IS_SET (victim->pcdata->tag_flags, TAG_BLUE))
      strcat (buf, "{B[BLUE] {x");

2) add to do_who() in the formating sprintf() or appropriate:

    a) add 2 %s's and this:

    IS_SET (wch->pcdata->tag_flags, TAG_RED) ? "{R[RED] {x" : "",
    IS_SET (wch->pcdata->tag_flags, TAG_BLUE) ? "{B[BLUE] {x" : "",

>>> ACT_MOVE.C <<<

1) add to move_char() after the door is closed check:

  if (!IS_NPC (ch) && IS_SET (ch->pcdata->tag_flags, TAG_FROZEN)
      && IS_SET (ch->pcdata->tag_flags, TAG_PLAYING))
  {
    send_to_char ("You've been frozen, you can't move!\n\r", ch);
    return;
  }

>>> INTERP.C / INTERP.H <<<

1) add commands:

  {"ftag", do_ftag, POS_SLEEPING, 0, LOG_NORMAL, 1},
  {"tag", do_tag, POS_RESTING, 0, LOG_NORMAL, 1},
  {"red", do_red, POS_RESTING, 0, LOG_NORMAL, 1},
  {"blue", do_blue, POS_RESTING, 0, LOG_NORMAL, 1},

DECLARE_DO_FUN (do_ftag);
DECLARE_DO_FUN (do_tag);
DECLARE_DO_FUN (do_red);
DECLARE_DO_FUN (do_blue);

>>> MERC.H <<<

1) Define structure:

typedef struct tag_data TAG_DATA;

2) Add:

struct tag_data
{
  int status;
  int timer;
  int next;
  int playing;
};

#define TAG_OFF 0
#define TAG_ISWAIT 1
#define TAG_ISPLAY 2


3) Add room vnum defines (you'll have to create the rooms yourself and set the vnums here)

#define FTAG_WAIT_ROOM      24000
#define FTAG_MIN_VNUM		24001
#define FTAG_MAX_VNUM		24036

4) Add tag bits:

#define TAG_PLAYING		        (A)
#define TAG_FROZEN		        (B)
#define TAG_RED			        (C)
#define TAG_BLUE			    (D)
#define TAG_WAITING             (E)

5) add to pc_data struct:

  long tag_flags;

6) Add prototypes:

void tag_update args ((void));
void end_tag args ((void));
void start_tag args ((void));
void check_team_frozen args ((CHAR_DATA * ch));
void auto_tag args ((void));
bool is_tagging args ((CHAR_DATA * ch));
void tag_channel args((CHAR_DATA *ch, char *message));

7) Add external variable:

extern TAG_DATA tag_game;

>>> CHECK ADDITION  <<<

1) add to anywhere else you think is appropriate,
   ( do_quit() comes to mind ):

    if (IS_SET (ch->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING))
    {
      send_to_char ("You can't do that right now.\n\r", ch);
      return;
    }

>>> BEGIN FREEZE.C <<<

/****************************************************************************
 * Automated Freeze Tag Code                                                *
 * Markanth : dlmud@dlmud.com                                               *
 * Devil's Lament : dlmud.com port 3778                                     *
 * Web Page : http://www.dlmud.com                                          *
 *                                                                          *
 * Provides automated freeze tag games in an area.                          *
 * Code orginally done by Nebseni of Clandestine MUD                        *
 * <clandestine.mudnet.net:9476>.                                           *
 *                                                                          *
 * All I ask in return is that you give me credit on your mud somewhere     *
 * or email me if you use it.                                               *
 ****************************************************************************/

#include "include.h"

TAG_DATA tag_game;

void do_red(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC (ch))
    return;

  if (tag_game.status == TAG_OFF)
  {
    send_to_char ("There is no tag game playing.\n\r", ch);
    return;
  }

  if (argument[0] == '\0')
  {
    send_to_char ("Syntax:  red <message>\n\r", ch);
    return;
  }

  if (!IS_IMMORTAL (ch)
      && !IS_SET (ch->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING))
  {
    send_to_char ("You must be a freeze tag player to use this channel.\n\r", ch);
    return;
  }

  if (IS_SET (ch->pcdata->tag_flags, TAG_BLUE))
  {
    sprintf (buf, "{R{{RED}{B %s{x: %s\n\r", ch->name, argument);
    send_to_char (buf, ch);
  }
  else
  {
    sprintf (buf, "{R{{RED}{R %s{x: %s\n\r", ch->name, argument);
  }

  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->connected == CON_PLAYING && d->character && !IS_NPC(d->character)
	&& IS_SET (d->character->pcdata->tag_flags, TAG_RED))
    {
      send_to_char (buf, d->character);
    }
  }
}

void do_blue(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC (ch))
    return;

  if (tag_game.status == TAG_OFF)
  {
    send_to_char ("There is no tag game playing.\n\r", ch);
    return;
  }

  if (argument[0] == '\0')
  {
    send_to_char ("Syntax:  blue <message>\n\r", ch);
    return;
  }

  if (!IS_IMMORTAL (ch)
      && !IS_SET (ch->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING))
  {
    send_to_char ("You must be a freeze tag player to use this channel.\n\r", ch);
    return;
  }

  if (IS_SET (ch->pcdata->tag_flags, TAG_RED))
  {
    sprintf (buf, "{B{{BLUE}{R %s{x: %s\n\r", ch->name, argument);
    send_to_char (buf, ch);
  }
  else
  {
    sprintf (buf, "{B{{BLUE}{B %s{x: %s\n\r", ch->name, argument);
  }

  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->desc->connected == CON_PLAYING && d->character && !IS_NPC(d->character)
	&& IS_SET (d->character->pcdata->tag_flags, TAG_BLUE))
    {
      send_to_char (buf, d);
    }
  }
}

void end_tag (void)
{
  DESCRIPTOR_DATA *d;

  tag_game.status = TAG_OFF;
  tag_game.timer = -1;
  tag_game.next = number_range (30, 50);
  tag_game.playing = 0;

  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->desc->connected != CON_PLAYING || !d->character || IS_NPC(d->character))
      continue;

    if (IS_SET (d->character->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING))
    {
      char_from_room (d->character);
      char_to_room (d->character, get_room_index (ROOM_VNUM_TEMPLE));
      do_look (d->character, "auto");
      send_to_char ("Freeze tag has been stopped!\n\r", d->character);
    }
    d->character->pcdata->tag_flags = 0;
  }
}

void start_tag (void)
{
  DESCRIPTOR_DATA *d;
  ROOM_INDEX_DATA *loc;
  int count = 0;
  char buf[MIL];

  tag_game.status = TAG_ISPLAY;
  tag_game.timer = 5 * tag_game.playing;
  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (ch->desc->connected != CON_PLAYING || !d->character || IS_NPC(d->character))
      continue;

    if (IS_SET (d->character->pcdata->tag_flags, TAG_WAITING))
    {
      count++;
      loc = get_room_index (number_range (FTAG_MIN_VNUM, FTAG_MAX_VNUM));
      REMOVE_BIT (d->character->pcdata->tag_flags, TAG_FROZEN);
      REMOVE_BIT (d->character->pcdata->tag_flags, TAG_WAITING);
      SET_BIT (d->character->pcdata->tag_flags, TAG_PLAYING);
      char_from_room (d->character);
      char_to_room (d->character, loc);
      do_look (d->character, "auto");
    }
  }
  sprinf(buf, "Freeze Tag has started! %d people playing.", count);
  tag_channel (NULL, buf);
}

bool fRed = FALSE;

void check_team_frozen (CHAR_DATA * ch)
{
  DESCRIPTOR_DATA *d;

  if (IS_NPC (ch) || !ch->pcdata)
    return;

  if (IS_SET (ch->pcdata->tag_flags, TAG_BLUE))
  {
    for (d = descriptor_list; d != NULL; d = d->next)
    {
      if (d->connected == CON_PLAYING 
      && d->character && !IS_NPC(d->character)
	  && IS_SET (tch->pcdata->tag_flags, TAG_PLAYING)
	  && IS_SET (tch->pcdata->tag_flags, TAG_BLUE)
	  && !IS_SET (tch->pcdata->tag_flags, TAG_FROZEN))
      {
	return;
      }
    }
    tag_channel (NULL, "The {RRED{x team has won FREEZE TAG!!!");
    end_tag ();
    return;
  }

  if (IS_SET (ch->pcdata->tag_flags, TAG_RED))
  {
    for (d = descriptor_list; d != NULL; d = d->next)
    {
      if (d->connected == CON_PLAYING
      && d->character && !IS_NPC(d->character)
	  && IS_SET (d->pcdata->tag_flags, TAG_PLAYING)
	  && IS_SET (d->pcdata->tag_flags, TAG_RED)
	  && !IS_SET (d->pcdata->tag_flags, TAG_FROZEN))
      {
	return;
      }
    }
    tag_channel (NULL, "The {BBLUE{x team has won FREEZE TAG!!!");
    end_tag ();
    return;
  }
}

void do_ftag(CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

  if (IS_NPC (ch))
    return;

  argument = one_argument (argument, arg1);

  if (arg1[0] == '\0')
  {
    send_to_char ("Syntax:  ftag join\n\r", ch);
    send_to_char ("         ftag info\n\r", ch);
    send_to_char ("         ftag start\n\r", ch);
    send_to_char ("To communicate use the 'blue' and 'red' channels.\n\r", ch);
    send_to_char("To tag someone once the game has started use the 'tag' comand.\n\r", ch);
    if (IS_IMMORTAL (ch))
    {
      send_to_char ("\n\rSyntax:  ftag reset\n\r", ch);
      send_to_char ("         ftag next\n\r", ch);
      send_to_char ("         ftag red <player>\n\r", ch);
      send_to_char ("         ftag blue <player>\n\r", ch);
    }
    return;
  }

  if (!str_cmp (arg1, "join"))
  {
    ROOM_INDEX_DATA *loc;

    if (tag_game.status != TAG_ISWAIT)
    {
      send_to_char ("There is no tag game to join.\n\r", ch);
      return;
    }

/* Add some checks here 
    if (ON_GQUEST (ch) || IS_QUESTOR (ch) || IS_SET (ch->act, PLR_WAR))
    {
      send_to_char ("You can't join right now.\n\r", ch);
      return;
    }
*/
    if (IS_SET (ch->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING))
    {
      send_to_char ("Your already playing.\n\r", ch);
      return;
    }

    if ((loc = get_room_index (FTAG_WAIT_ROOM)) == NULL)
    {
      send_to_char ("The freeze tag arena hasn't been finished yet.\n\r", ch);
      return;
    }
    send_to_char ("You join freeze tag.\n\r", ch);
    char_from_room (ch);
    char_to_room (ch, loc);
    tag_game.playing += 1;
    do_look (ch, "auto");
    if ((fRed = !fRed))
    {
      SET_BIT (ch->pcdata->tag_flags, TAG_WAITING);
      REMOVE_BIT (ch->pcdata->tag_flags, TAG_FROZEN);
      SET_BIT (ch->pcdata->tag_flags, TAG_RED);
      REMOVE_BIT (ch->pcdata->tag_flags, TAG_BLUE);
      send_to_char ("You are on the {RRED{x team!\n\r", ch);
      tag_channel (ch, "$n is on the {RRED{x team!");
    }
    else
    {
      SET_BIT (ch->pcdata->tag_flags, TAG_WAITING);
      REMOVE_BIT (ch->pcdata->tag_flags, TAG_FROZEN);
      SET_BIT (ch->pcdata->tag_flags, TAG_BLUE);
      REMOVE_BIT (ch->pcdata->tag_flags, TAG_RED);
      send_to_char ("You are on the {BBLUE{x team!\n\r", ch);
      tag_channel (ch, "$n is on the {BBLUE{x team!");
    }
    SET_BIT (ch->pcdata->tag_flags, TAG_WAITING);
    return;
  }

  if (!str_cmp (arg1, "info"))
  {
      if(tag_game.status == TAG_OFF)
      {
    sprintf (buf, "The next game of freeze tag will start in %d minute%s.\n\r",
	      tag_game.next, tag_game.next == 1 ? "" : "s");
    send_to_char(buf, ch);
      }
      else
          send_to_char("A tag game is currently playing.\n\r", ch);
    return;
  }

  if (!str_cmp (arg1, "start"))
  {
    if (tag_game.status != TAG_OFF)
    {
      send_to_char ("A game has already started.\n\r", ch);
      return;
    }

    /* add cost here */
    if (!IS_IMMORTAL (ch) /*&& ch->pcdata->trivia < 2*/)
    {
//      send_to_char ("It costs 2 trivia points to start a game.\n\r", ch);
        send_to_char("Only Immortals can start a tag game.\n\r", ch);
      return;
    }

    tag_channel (NULL, "A Freeze Tag Game has started! Type 'ftag join' to play.");
    tag_game.status = TAG_ISWAIT;
    tag_game.timer = 3;
    tag_game.playing = 0;
    tag_game.next = -1;
/*    if (!IS_IMMORTAL (ch))
      ch->pcdata->trivia -= 2; */
    return;
  }

  if (!IS_IMMORTAL (ch))
  {
    do_ftag (ch, "");
    return;
  }

  if (!str_cmp (arg1, "next"))
  {
    tag_game.next = atoi (argument);
    chprintf (ch, "Next freeze tag game will start in %d ticks.\n\r",
	      tag_game.next);
    return;
  }
  if (!str_cmp (arg1, "reset"))
  {
    end_tag ();
    send_to_char ("All players reset.\n\r", ch);
    return;
  }

  argument = one_argument (argument, arg2);
  if (arg2[0] == '\0' || (str_cmp (arg1, "red") && str_cmp (arg1, "blue")))
  {
    send_to_char ("Syntax:  ftag red <player>\n\r", ch);
    send_to_char ("         ftag blue <player>\n\r", ch);
    return;
  }

  if (tag_game.status == TAG_ISPLAY)
  {
    send_to_char ("The tag game has already started.\n\r", ch);
    return;
  }
  if ((victim = get_char_world (ch, arg2)) == NULL)
  {
    send_to_char ("They aren't here.\n\r", ch);
    return;
  }

  if (IS_NPC (victim))
  {
    send_to_char ("They can't play.\n\r", ch);
    return;
  }

  if (!str_cmp (arg1, "red"))
  {
    SET_BIT (victim->pcdata->tag_flags, TAG_WAITING);
    REMOVE_BIT (victim->pcdata->tag_flags, TAG_FROZEN);
    SET_BIT (victim->pcdata->tag_flags, TAG_RED);
    REMOVE_BIT (victim->pcdata->tag_flags, TAG_BLUE);
    act ("You are on the {RRED{x team!", ch, NULL, victim, TO_VICT);
    act ("$N is on the {RRED{x team!", ch, NULL, victim, TO_NOTVICT);
    act ("$N is on the {RRED{x team!", ch, NULL, victim, TO_CHAR);
  }
  else if (!str_cmp (arg1, "blue"))
  {
    SET_BIT (victim->pcdata->tag_flags, TAG_WAITING);
    REMOVE_BIT (victim->pcdata->tag_flags, TAG_FROZEN);
    SET_BIT (victim->pcdata->tag_flags, TAG_BLUE);
    REMOVE_BIT (victim->pcdata->tag_flags, TAG_RED);
    act ("You are on the {BBLUE{x team!", ch, NULL, victim, TO_VICT);
    act ("$N is on the {BBLUE{x team!", ch, NULL, victim, TO_NOTVICT);
    act ("$N is on the {BBLUE{x team!", ch, NULL, victim, TO_CHAR);
  }

  return;
}

void do_tag(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

  if (IS_NPC (ch))
    return;

  if (tag_game.status == TAG_OFF)
  {
    send_to_char ("There is no tag game playing.\n\r", ch);
    return;
  }

  argument = one_argument (argument, arg);

  if (!IS_SET (ch->pcdata->tag_flags, TAG_PLAYING))
  {
    send_to_char ("You're not playing freeze tag.\n\r", ch);
    return;
  }

  if (arg[0] == '\0')
  {
    send_to_char ("Tag whom?\n\r", ch);
    return;
  }

  if ((victim = get_char_room (ch, NULL, arg)) == NULL)
  {
    send_to_char ("They aren't here.\n\r", ch);
    return;
  }

  if (victim == ch)
  {
    send_to_char ("You tag yourself.  How amusing.\n\r", ch);
    return;
  }

  if (IS_NPC (victim))
  {
    send_to_char ("You can't tag them.\n\r", ch);
    return;
  }

  if (!IS_SET (victim->pcdata->tag_flags, TAG_PLAYING))
  {
    send_to_char ("They're not playing freeze tag.\n\r", ch);
    return;
  }

  if (IS_SET (ch->pcdata->tag_flags, TAG_FROZEN))
  {
    send_to_char ("You can't tag, you're frozen!\n\r", ch);
    return;
  }

  act ("$n tags you.", ch, NULL, victim, TO_VICT);
  act ("$n tags $N.", ch, NULL, victim, TO_NOTVICT);
  act ("You tag $N.", ch, NULL, victim, TO_CHAR);

  if ((IS_SET (ch->pcdata->tag_flags, TAG_RED) &&
       IS_SET (victim->pcdata->tag_flags, TAG_RED))
      || (IS_SET (ch->pcdata->tag_flags, TAG_BLUE) &&
	  IS_SET (victim->pcdata->tag_flags, TAG_BLUE)))
  {
    if (IS_SET (victim->pcdata->tag_flags, TAG_FROZEN))
    {
      REMOVE_BIT (victim->pcdata->tag_flags, TAG_FROZEN);
      act ("You are no longer frozen!", ch, NULL, victim, TO_VICT);
      act ("$N is no longer frozen!", ch, NULL, victim, TO_NOTVICT);
      act ("$N is no longer frozen!", ch, NULL, victim, TO_CHAR);
    }
    else
    {
      act ("$N is not frozen!", ch, NULL, victim, TO_CHAR);
    }
  }
  else
  {
    if (IS_SET (victim->pcdata->tag_flags, TAG_FROZEN))
    {
      act ("$N is already frozen!", ch, NULL, victim, TO_CHAR);
    }
    else
    {
      SET_BIT (victim->pcdata->tag_flags, TAG_FROZEN);
      act ("You are frozen!", ch, NULL, victim, TO_VICT);
      act ("$N is frozen!", ch, NULL, victim, TO_NOTVICT);
      act ("$N is frozen!", ch, NULL, victim, TO_CHAR);
      check_team_frozen (victim);
    }
  }

  return;
}

void auto_tag (void)
{
  DESCRIPTOR_DATA *d;
  int count = 0;

  for (d = descriptor_list; d != NULL; d = d->next)
    if (d->connected == CON_PLAYING)
      count++;

  if (count <= 2)
  {
    end_tag ();
    return;
  }

  tag_channel (NULL, "A Freeze Tag Game has started! Type 'ftag join' to play.");
  tag_game.status = TAG_ISWAIT;
  tag_game.timer = 3;
  tag_game.playing = 0;
  tag_game.next = -1;
}

void tag_update ( void )
{
    char buf[MSL];

    if ( tag_game.next > 0 && tag_game.status == TAG_OFF )
    {
        if ( --tag_game.next == 0 )
            auto_tag (  );
    }

    else if ( tag_game.status == TAG_ISWAIT )
    {

        tag_game.timer--;

        if ( tag_game.timer > 0 )
        {
            sprintf ( buf, "%d minute%s left to join freeze tag.", tag_game.timer,
                       tag_game.timer == 1 ? "" : "s" );
            tag_channel(NULL, buf);
        }
        else
        {
            if ( tag_game.playing < 3 )
            {
                end_tag (  );
                sprintf ( buf, "Not enough people for freeze tag, next game in %d minutes.",
                           tag_game.next );
                tag_channel(NULL, buf);
                return;
            }
            else
            {
                start_tag (  );
            }
        }
    }
    else if ( tag_game.status == TAG_ISPLAY )
    {
        if ( tag_game.playing == 0 )
        {
            end_tag (  );
            sprintf ( buf, "No one left in freeze tag, next game in %d minutes.",
                       tag_game.next );
            tag_channel(NULL, buf);
            return;
        }
        switch ( tag_game.timer )
        {
            case 0:
                end_tag (  );
                sprintf ( buf, "Time has run out for freeze tag, next game will start in %d minutes.",
                           tag_game.next );
                tag_channel(NULL, buf);
                return;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 10:
            case 15:
                sprintf ( buf, "%d minute%s remaining in freeze tag.",
                           tag_game.timer, tag_game.timer > 1 ? "s" : "" );
                tag_channel(NULL, buf);
            default:
                tag_game.timer--;
                break;
        }
    }
}


bool is_tagging (CHAR_DATA * ch)
{
  if (!ch || IS_NPC (ch))
    return FALSE;

  if (IS_SET (ch->pcdata->tag_flags, TAG_PLAYING | TAG_WAITING)
      && tag_game.status != TAG_OFF)
    return TRUE;

  return FALSE;
}

void tag_channel(CHAR_DATA *ch, char *message)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *dch;
    char buf[MAX_INPUT_LENGTH];

    for(d = descriptor_list; d != NULL; d = d->next)
    {
        if(d->connected != CON_PLAYING)
            continue;

        if((dch = d->character) == NULL || dch == ch)
            continue;

        sprintf(buf, "FREEZE TAG: %s");

        if(ch) /* don't use $N only $n in message */
            act(buf, ch, NULL, dch, TO_VICT);
        else
            send_to_char(buf, dch);
    }
}

>>>> END FREEZE.C <<<

Markanth : dlmud@dlmud.com     
Devil's Lament : dlmud.com port 3778     
Web Page : http://dlmud.akbearsden.com
