/* Automated quest code originally written by Vassago (Who i'm giving
 * credit to despite the fact he gives no credit to the people who's code he
 * copies (aka the rom consortium).  
 * Revamped by Kharas (mud@fading.tcimet.net)
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "merc.h"
#include "interp.h"

struct reward_type 
{
  char *name;
  char *keyword;
  int cost;
  bool object;
  int value;
  void *      where;
};

/* Descriptions of quest items go here:
Format is: "keywords", "Short description", "Long description" */
const struct quest_desc_type quest_desc[] =
{
	{"quest banana", 	"a {Rq{Yu{Ge{Cs{Bt {Ybanana{x", 
	"A {Rq{Yu{Ge{Cs{Bt {Ybanana{O lies here, waiting to be returned."},
	
	{"quest shoehorn", 	"a {Rq{Yu{Ge{Cs{Bt{x shoehorn", 
	"A {Rq{Yu{Ge{Cs{Bt{O shoehorn (the kind with teeth) is here."},
	
	{"quest zucchini", 	"a {Rq{Yu{Ge{Cs{Bt {gzucchini{x", 
	"A {Rq{Yu{Ge{Cs{Bt {gzucchini{O is here, waiting to be returned."},
	
	{"", "", ""}
};
#define MAX_QUESTOBJS	2

/* Local functions */
void generate_quest	args(( CHAR_DATA *ch, CHAR_DATA *questman ));
void quest_update	args(( void ));
bool quest_level_diff   args(( int clevel, int mlevel));

/* The main quest function */
void do_quest(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *questman;
    OBJ_DATA *obj=NULL, *obj_next;
    OBJ_INDEX_DATA *questinfoobj;
    MOB_INDEX_DATA *questinfo;
    char buf [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    int i;

/* Add your rewards here.  Works as follows:
"Obj name shown when quest list is typed", "keywords for buying",
"Amount of quest points",  Does it load an object?,  IF it loads an
object, then the vnum, otherwise a value to set in the next thing,  This
is last field is a part of the char_data to be modified */ 

const struct reward_type reward_table[]= 
{
/*
  { "A Flask of Endless Water","flask water",   500, TRUE,  46,    0},
  { "Jug O' Moonshine",        "jug moonshine", 300, TRUE,  47,    0},
  { "Potion of Extra Healing", "extra healing", 300, TRUE,  4639,  0},
  { "Potion of Sanctuary",     "sanctuary",     150, TRUE,  3081,  0},
*/
  { "100,000 zlotys",          "100000 zlotys",	300, FALSE, 100000, &ch->cash},
  { "5 Practices",    	       "practices", 	500, FALSE, 4,      &ch->practice},
  { "a piece of rhubarb pie",  "rhubarb pie",   200, TRUE,  1200,   0},
  { NULL, NULL, 0, FALSE, 0, 0  } /* Never remove this!!! */
};


    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if(IS_NPC(ch))
	{ send_to_char("NPC's can't quest.\n\r",ch); return; }

	if(IS_IMMORTAL(ch) || ch->level >= LEVEL_IMMORTAL)
	{ send_to_char("Immortals should create their own quests.\n\r",ch); return; }

    if (arg1[0] == '\0')
    {
        send_to_char("Quest commands: Info, Time, Request, Complete, Quit, List, and Buy.\n\r",ch);
        send_to_char("For more information, type 'Help Quest'.\n\r",ch);
	return;
    }

	/* INFO */
    if (!strcmp(arg1, "info"))
    {
	if (IS_SET(ch->act, PLR_QUESTING))
	{
	    if (ch->pcdata->questmob == -1 && ch->pcdata->questgiver->short_descr != NULL)
		{	        
sprintf(buf,"Your quest is ALMOST complete!\n\rGet back to %s before your"
	   "time runs out!\n\r",ch->pcdata->questgiver->short_descr);
		send_to_char(buf,ch);
		}
	    else if (ch->pcdata->questobj > 0)
	    {
                questinfoobj = get_obj_index(ch->pcdata->questobj);
		if (questinfoobj != NULL)
		{
		    sprintf(buf,"You are on a quest to recover the"
				"fabled %s!\n\r",questinfoobj->name);		
		   send_to_char(buf,ch);
		}
		else send_to_char("You aren't currently on a quest.\n\r",ch);
		return;
	    }
	    else if (ch->pcdata->questmob > 0)
	    {
                questinfo = get_mob_index(ch->pcdata->questmob);
		if (questinfo != NULL)
		{
		    sprintf(buf,"You are on a quest to slay"
			       "%s!\n\r",questinfo->short_descr);
		    send_to_char(buf,ch);
		}
		else send_to_char("You aren't currently on a quest.\n\r",ch);
		return;
	    }
	}
	else
	    send_to_char("You aren't currently on a quest.\n\r",ch);
	return;
    }

	/* TIME */
    else if (!strcmp(arg1, "time"))
    {
	if (!IS_SET(ch->act,PLR_QUESTING))
	{
	    send_to_char("You aren't currently on a quest.\n\r",ch);
	    if (ch->pcdata->nextquest > 1)
		{
	        sprintf(buf,"There are %d minutes remaining until you can"
			" go on another quest.\n\r",ch->pcdata->nextquest);
	        send_to_char(buf,ch);
		}
	    else if (ch->pcdata->nextquest == 1)
		{
		sprintf(buf, "There is less than a minute remaining until"
			" you can go on another quest.\n\r");
		send_to_char(buf, ch);
		}
	}
        else if (ch->pcdata->countdown > 0)
        {
	    sprintf(buf, "Time left for current quest: %d\n\r",ch->pcdata->countdown);
	    send_to_char(buf, ch);
	}
	return;
    }

    for ( questman = ch->in_room->people; questman != NULL; questman = questman->next_in_room )
    {
	if (IS_NPC(questman) && IS_SET(questman->act, ACT_QUESTMASTER))
		break;
    }

    if (questman == NULL || !IS_SET(questman->act, ACT_QUESTMASTER))
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

    if ( questman->fighting != NULL)
    {
	send_to_char("Wait until the fighting stops.\n\r",ch);
        return;
    }

    ch->pcdata->questgiver = questman;

	/* LIST */
    if (!strcmp(arg1, "list"))
    {
        act("$n asks $N for a list of quest items.", ch, NULL, questman, TO_ROOM); 
	act ("You ask $N for a list of quest items.",ch, NULL, questman, TO_CHAR);
	send_to_char("Current Quest Items available for Purchase:\n\r", ch);
	if(reward_table[0].name == NULL)
	  send_to_char("  Nothing.\n\r",ch);
	else {
		send_to_char("  {g[{GCost{g]     [{GName{g]{x\n\r",ch);
		for(i=0;reward_table[i].name != NULL;i++)
	  	{	  
	 	sprintf(buf,"   {W%-4d{w       %s{x\n\r",
		        reward_table[i].cost, reward_table[i].name);
 		send_to_char(buf,ch);
 	     	}
		send_to_char("\n\rTo buy an item, type 'Quest buy <item>'.\n\r",ch);
	}
	return;
    }

	/* BUY */
    else if (!strcmp(arg1, "buy"))
    {
	bool found=FALSE;
	if (arg2[0] == '\0')
	{
	    send_to_char("To buy an item, type 'Quest buy <item>'.\n\r",ch);
	    return;
	}
	/* Use keywords rather than the name listed in qwest list */
	/* Do this to avoid problems with something like 'qwest buy the' */
	/* And people getting things they don't want... */
      for(i=0;reward_table[i].name != NULL;i++)
	if (is_name(arg2, reward_table[i].keyword))
	{ found = TRUE;
	    if (ch->pcdata->quest_curr >= reward_table[i].cost)
	    {
		ch->pcdata->quest_curr -= reward_table[i].cost;
		if(reward_table[i].object)
	            obj = create_object(get_obj_index(reward_table[i].value),ch->level);
		else
		   {
		   sprintf(buf,"In exchange for %d quest points, %s gives you %s.\n\r",
			reward_table[i].cost, questman->short_descr, reward_table[i].name );
		   send_to_char(buf,ch);
		   *(int *)reward_table[i].where += reward_table[i].value;
		   }
		break;
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_function( questman, &do_say, buf);
		return;
	    }
	}
	if(!found)
	{
	    sprintf(buf, "I don't have that item, %s.",ch->name);
	    do_say(questman, buf);
	}
	if (obj != NULL)
	{
	    sprintf(buf,"In exchange for %d quest points, %s gives you %s.\n\r",
		    reward_table[i].cost, questman->short_descr, obj->short_descr);
	    send_to_char(buf,ch);
	    obj_to_char(obj, ch);
	}
	return;
    }
    
    /* REQUEST */
    else if (!strcmp(arg1, "request"))
    {
        act("$n asks $N for a quest.", ch, NULL, questman, TO_ROOM); 
		act ("You ask $N for a quest.",ch, NULL, questman, TO_CHAR);
		if (IS_SET(ch->act,PLR_QUESTING))
		{
			sprintf(buf, "But you're already on a quest!");
			do_say(questman, buf);
			return;
		}
		if (ch->pcdata->nextquest > 0)
		{
			sprintf(buf, "You're very brave, %s, but let someone else have a chance.",ch->name);
			do_say(questman, buf);
			sprintf(buf, "Come back later.");
			do_say(questman, buf);
			return;
		}

		sprintf(buf, "Thank you, brave %s!",ch->name);
		
		if (!IS_SET(ch->act,PLR_QUESTING))
		do_say(questman, buf);
		
		ch->pcdata->questmob = 0;
		ch->pcdata->questobj = 0;
		
		generate_quest(ch, questman);

        if (ch->pcdata->questmob > 0 || ch->pcdata->questobj > 0)
		{
			ch->pcdata->countdown = number_range(10,30);
			SET_BIT(ch->act,PLR_QUESTING);
			sprintf(buf, "You have %d minutes to complete this quest.",ch->pcdata->countdown);
			do_say(questman, buf);
			sprintf(buf, "May the gods go with you!");
			do_say(questman, buf);
		}
		
		return;
    }

	/* COMPLETE */
    else if (!strcmp(arg1, "complete"))
    {
        act("$n informs $N $e has completed $S quest.", ch, NULL, questman, TO_ROOM); 
		act("You inform $N you have completed $S quest.",ch, NULL, questman, TO_CHAR);
		if (ch->pcdata->questgiver != questman)
		{
			sprintf(buf, "I never sent you on a quest! Perhaps you're thinking of someone else.");
			do_say(questman,buf);
			return;
		}
	
		if (IS_SET(ch->act,PLR_QUESTING))
		{
			bool obj_found = FALSE;
			if (ch->pcdata->questobj > 0 && ch->pcdata->countdown > 0)
			{
				for (obj = ch->carrying; obj != NULL; obj= obj->next_content)
				{
					obj_next = obj->next_content;
			
					if (obj != NULL && obj->pIndexData->vnum == ch->pcdata->questobj)
					{
					obj_found = TRUE;
								break;
					}
				}
			}
	
		 if ((ch->pcdata->questmob == -1 || (ch->pcdata->questobj && obj_found)) 
		&& ch->pcdata->countdown > 0)
		{
			int reward, pointreward;
			reward = number_range(50,1000);
			pointreward = number_range(20,50);
	
			sprintf(buf, "Congratulations on completing your quest!");
			do_say(questman,buf);
			sprintf(buf,"As a reward, I am giving you %d quest points, and %d zlotys.",pointreward,reward);
			do_say(questman,buf);

			REMOVE_BIT(ch->act,PLR_QUESTING);
			ch->pcdata->questgiver = NULL;
			ch->pcdata->countdown = 0;
			ch->pcdata->questmob = 0;
			ch->pcdata->questobj = 0;
			ch->pcdata->nextquest = 10;
			ch->cash += reward;
			ch->pcdata->quest_curr += pointreward;
			
			if(obj_found) extract_obj(obj);
			
			return;
		}
		 else if((ch->pcdata->questmob > 0 || ch->pcdata->questobj > 0)
			  && ch->pcdata->countdown > 0)
			{
			sprintf(buf, "You haven't completed the quest yet, but there is still time!");
			do_say(questman, buf);
			return;
			}
		 }
		if (ch->pcdata->nextquest > 0)
			  sprintf(buf,"But you didn't complete your quest in time!");
		else 
			  sprintf(buf, "You have to request a quest first, %s.",ch->name);
		do_say(questman, buf);
		return;
    }

	/* QUIT */
    else if (!strcmp(arg1, "quit"))
    {
        act("$n informs $N $e wishes to quit $S quest.", ch, NULL,questman, TO_ROOM); 
		act ("You inform $N you wish to quit $S quest.",ch, NULL, questman, TO_CHAR);
		if (ch->pcdata->questgiver != questman)
		{
			sprintf(buf, "I never sent you on a quest! Perhaps you're thinking of someone else.");
			do_say(questman,buf);
			return;
		}
	
		if (IS_SET(ch->act,PLR_QUESTING))
		{
				REMOVE_BIT(ch->act,PLR_QUESTING);
				ch->pcdata->questgiver = NULL;
				ch->pcdata->countdown = 0;
				ch->pcdata->questmob = 0;
			ch->pcdata->questobj = 0;
				ch->pcdata->nextquest = 30;
			sprintf(buf, "Your quest is over, but for your cowardly behavior, you may not quest again for 30 minutes.");
			do_say(questman,buf);
				return;
			}
		else
		{
		send_to_char("You aren't on a quest!",ch);
		return;
	} 
    }


    send_to_char("Quest commands: Info, Time, Request, Complete, Quit, List, and Buy.\n\r",ch);
    send_to_char("For more information, type 'Help Quest'.\n\r",ch);
    return;
}

void generate_quest(CHAR_DATA *ch, CHAR_DATA *questman)
{
    CHAR_DATA *victim;
    MOB_INDEX_DATA *vsearch;
    ROOM_INDEX_DATA *room;
    OBJ_DATA *questitem;
    char buf [MAX_STRING_LENGTH];
    long mcounter;
    int mob_vnum;

	for (mcounter = 0; mcounter < 1000; mcounter ++)
	{
		mob_vnum = number_range(100, 32600);

		if ( (vsearch = get_mob_index(mob_vnum) ) != NULL )
		{
			if (quest_level_diff(ch->level, vsearch->level) == TRUE
				&& vsearch->pShop == NULL
				&& !IS_SET(vsearch->act, ACT_TRAIN)
				&& !IS_SET(vsearch->act, ACT_PRACTICE)
				&& !IS_SET(vsearch->act, ACT_IS_HEALER)
				&& !IS_SET(vsearch->act, ACT_PET)
				&& !IS_SET(vsearch->affected_by, AFF_CHARM)
				&& number_percent() < 40) break;
			
			else vsearch = NULL;
		}
	}

	if ( vsearch == NULL || ( victim = get_char_world( ch, vsearch->player_name ) ) == NULL )
	{
		sprintf(buf, "I'm sorry, but I don't have any quests for you at this time.");
		do_say(questman, buf);
		sprintf(buf, "Try again later.");
		do_say(questman, buf);
		ch->pcdata->nextquest = 2;
		
		return;
	}

	if ( ( room = victim->in_room ) == NULL )
	{
		sprintf(buf, "I'm sorry, but I don't have any quests for you at this time.");
		do_say(questman, buf);
		sprintf(buf, "Try again later.");
		do_say(questman, buf);
		ch->pcdata->nextquest = 2;
		
		return;
	}
	
	if( IS_SET( room->room_flags, ROOM_PET_SHOP ) )
	{
		sprintf(buf, "I'm sorry, I don't have any quests for you right now.");
		do_say(questman, buf);
		sprintf(buf, "Try again later.");
		do_say(questman, buf);
		ch->pcdata->nextquest = 2;
		
		return;
	}
	
	if( number_percent() < 40 )
    {
    	do_say(questman, "You get an object quest!");
    	
		int descnum = 0;

		descnum = number_range(0,MAX_QUESTOBJS);
		questitem = create_object( get_obj_index(OBJ_VNUM_QUEST_OBJ), ch->level );
		
		if( quest_desc[descnum].name )
		{
			if(questitem->short_descr)
				free_string(questitem->short_descr);
			if(questitem->description)
				free_string(questitem->description);
			if(questitem->name)
				free_string(questitem->name);
	
			questitem->name        = str_dup(quest_desc[descnum].name);
			questitem->description = str_dup(quest_desc[descnum].description);
			questitem->short_descr = str_dup(quest_desc[descnum].short_descr);
		}
		
		do_say(questman, "Quest object created");
		
		obj_to_room(questitem, room);
		ch->pcdata->questobj = questitem->pIndexData->vnum;
	
		sprintf(buf, "Vile pilferers have stolen %s from the Cabaret Voltaire!",questitem->short_descr);
		do_say(questman, buf);
		do_say(questman, "My pet elephant, with her magic mirror, has pinpointed its location.");
	
		sprintf(buf, "Look in the general area of %s for %s!",room->area->name, room->name);
		do_say(questman, buf);
		return;
    }

    /* Quest to kill a mob */

    else 
    {
    switch(number_range(0,1))
    {
	case 0:
        sprintf(buf, "An enemy of mine, %s, is making vile threats against the art establishment.",victim->short_descr);
        do_say(questman, buf);
        sprintf(buf, "This threat must be eliminated!");
        do_say(questman, buf);
	break;

	case 1:
	sprintf(buf, "Zucchini's most heinous criminal, %s, has escaped from the dungeon!",victim->short_descr);
	do_say(questman, buf);
	sprintf(buf, "Since the escape, %s has murdered %d civillians!",victim->short_descr, number_range(2,20));
	do_say(questman, buf);
	do_say(questman,"The penalty for this crime is death, and you are to deliver the sentence!");
	break;
    }

    if (room->name != NULL)
    {
        sprintf(buf, "Seek %s out somewhere in the vicinity of %s!",victim->short_descr,room->name);
        do_say(questman, buf);

	sprintf(buf, "That location is in the general area of %s.",room->area->name);
	do_say(questman, buf);
    }
    ch->pcdata->questmob = victim->pIndexData->vnum;
    }

    return;
}

bool quest_level_diff(int clevel, int mlevel)
{
    if (clevel < 9 && mlevel < clevel + 2) return TRUE;
    else if (clevel <= 9 && mlevel < clevel + 3 
	  && mlevel > clevel - 5) return TRUE;
    else if (clevel <= 14 && mlevel < clevel + 4 
	  && mlevel > clevel - 5) return TRUE;
    else if (clevel <= 21 && mlevel < clevel + 5 
	  && mlevel > clevel - 4) return TRUE;
    else if (clevel <= 29 && mlevel < clevel + 6 
	  && mlevel > clevel - 3) return TRUE;
    else if (clevel <= 37 && mlevel < clevel + 7 
	  && mlevel > clevel - 2) return TRUE;
    else if (clevel <= 55 && mlevel < clevel + 8 
	  && mlevel > clevel - 1) return TRUE;
    else if(clevel > LEVEL_HERO) return TRUE; /* Imms can get anything :) */
    else return FALSE;
}
		
/* Called from update_handler() by pulse_area */

void quest_update(void)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch;
    char buf[MSL];
 
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->character != NULL && d->connected == CON_PLAYING)
        {
        ch = d->character;
	if(IS_NPC(ch))
	continue; 
        if (ch->pcdata->nextquest > 0)
        {
            ch->pcdata->nextquest--;
            if (ch->pcdata->nextquest == 0)
            {
                send_to_char("You may now quest again.\n\r",ch);
                return;
            }
        }
        else if (IS_SET(ch->act,PLR_QUESTING))
        {
            if (--ch->pcdata->countdown <= 0)
            {
                ch->pcdata->nextquest = 10;
                sprintf(buf,"You have run out of time for your"
"quest!\n\rYou may quest again in %d minutes.\n\r",ch->pcdata->nextquest);
		send_to_char(buf,ch);
                REMOVE_BIT(ch->act,PLR_QUESTING);
                ch->pcdata->questgiver = NULL;
                ch->pcdata->countdown = 0;
                ch->pcdata->questmob = 0;
                ch->pcdata->questobj = 0;
            }
            if (ch->pcdata->countdown > 0 && ch->pcdata->countdown < 6)
            {
                send_to_char("Better hurry, you're almost out of time for your quest!\n\r",ch);
                return;
            }
        }
        }
    }
    return;
}
